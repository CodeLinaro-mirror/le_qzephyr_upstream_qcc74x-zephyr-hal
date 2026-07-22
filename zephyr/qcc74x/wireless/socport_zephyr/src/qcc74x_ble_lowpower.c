/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stdint.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/soc/qualcomm/qcc74x_ble_lowpower.h>
#include <zephyr/soc/qualcomm/qcc74x_pm_pds15_diag.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/util.h>

#include "btble_inc/btble_lib_api.h"
#include "qc7xx_lp.h"
#include "qcc74x_glb.h"

#ifndef CONFIG_QCC74X_PM_PDS15_BLE_CRASH_TOLERANCE_US
#define CONFIG_QCC74X_PM_PDS15_BLE_CRASH_TOLERANCE_US 50000
#endif

#ifndef CONFIG_QCC74X_PM_PDS15_LOG_LEVEL
#define CONFIG_QCC74X_PM_PDS15_LOG_LEVEL LOG_LEVEL_NONE
#endif

LOG_MODULE_REGISTER(qcc74x_ble_lp, CONFIG_QCC74X_PM_PDS15_LOG_LEVEL);

extern void rwip_restore_ble_reg(void);
extern void rwip_prevent_sleep_clear(uint16_t prv_slp_bit);

static struct k_spinlock qcc74x_ble_lp_diag_lock;
static struct qcc74x_pm_pds15_ble_diag_status qcc74x_ble_lp_diag;
/* When set, force the next crash_check() to take the recovery path
 * regardless of elapsed_us. Cleared after one trigger by atomic_cas.
 */
static atomic_t qcc74x_ble_lp_inject_crash_flag;

void qcc74x_pm_pds15_ble_diag_get(struct qcc74x_pm_pds15_ble_diag_status *status)
{
	if (status == NULL) {
		return;
	}

	k_spinlock_key_t key = k_spin_lock(&qcc74x_ble_lp_diag_lock);

	*status = qcc74x_ble_lp_diag;

	k_spin_unlock(&qcc74x_ble_lp_diag_lock, key);
}

void qcc74x_pm_pds15_ble_diag_reset(void)
{
	k_spinlock_key_t key = k_spin_lock(&qcc74x_ble_lp_diag_lock);

	qcc74x_ble_lp_diag = (struct qcc74x_pm_pds15_ble_diag_status){ 0 };

	k_spin_unlock(&qcc74x_ble_lp_diag_lock, key);
}

void qcc74x_pm_pds15_ble_inject_crash(void)
{
	atomic_set(&qcc74x_ble_lp_inject_crash_flag, 1);
}

int qcc74x_ble_lowpower_prepare(uint64_t *timeout_us,
				struct qcc74x_ble_lowpower_session *session)
{
	struct qcc74x_ble_lowpower_session sess = { 0 };
	int32_t ble_sleep_rtc = 0;
	uint64_t ble_us = 0U;
	bool capped = false;
	bool bt_ready;
	int rc = 0;

	if (timeout_us == NULL || session == NULL) {
		return -EINVAL;
	}

	bt_ready = bt_is_ready();
	if (bt_ready) {
		/* Host stack ready — query BLE controller for its sleep horizon. */
		ble_sleep_rtc = btble_controller_sleep(0);

		if (ble_sleep_rtc < 0) {
			LOG_DBG("ble_prepare: ble active rc=%d, abort pds15",
				ble_sleep_rtc);
			rc = -EBUSY;
		} else if (ble_sleep_rtc > 0) {
			ble_us = QC7XX_PDS_CNT_TO_US(
				(uint64_t)(uint32_t)ble_sleep_rtc);
			sess.expected_us = ble_us;
			if (ble_us < *timeout_us) {
				*timeout_us = ble_us;
				sess.capping = true;
				capped = true;
				LOG_DBG("ble_prepare: cap timeout to ble=%llu us",
					(unsigned long long)ble_us);
			}
		}
	}

	/* Update diagnostics (one spinlock region). */
	{
		k_spinlock_key_t key = k_spin_lock(&qcc74x_ble_lp_diag_lock);

		qcc74x_ble_lp_diag.sampled = true;
		qcc74x_ble_lp_diag.bt_ready = bt_ready;
		qcc74x_ble_lp_diag.sample_count++;
		qcc74x_ble_lp_diag.last_sleep_rtc = ble_sleep_rtc;

		if (rc == -EBUSY) {
			qcc74x_ble_lp_diag.blocked_count++;
		} else if (capped) {
			qcc74x_ble_lp_diag.capped_count++;
			qcc74x_ble_lp_diag.last_capped_us = ble_us;
		} else {
			qcc74x_ble_lp_diag.allowed_count++;
		}

		k_spin_unlock(&qcc74x_ble_lp_diag_lock, key);
	}

	*session = sess;
	return rc;
}

void qcc74x_ble_lowpower_crash_check(
	const struct qcc74x_ble_lowpower_session *session,
	uint64_t elapsed_us)
{
	bool inject = atomic_cas(&qcc74x_ble_lp_inject_crash_flag, 1, 0);

	if (!inject) {
		if (!session->capping || session->expected_us == 0U) {
			return;
		}
		if (elapsed_us <= session->expected_us +
				  CONFIG_QCC74X_PM_PDS15_BLE_CRASH_TOLERANCE_US) {
			return;
		}
	}

	LOG_ERR(
		"ble_crash: elapsed=%llu us expected=%llu us inject=%d — GLB reset BTDM",
		(unsigned long long)elapsed_us,
		(unsigned long long)session->expected_us, inject ? 1 : 0);

	GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_PDS);
	GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_BTDM);
	rwip_restore_ble_reg();
	rwip_prevent_sleep_clear(0xffffU);

	{
		k_spinlock_key_t key = k_spin_lock(&qcc74x_ble_lp_diag_lock);

		qcc74x_ble_lp_diag.crash_recovery_count++;
		qcc74x_ble_lp_diag.last_crash_elapsed_us = elapsed_us;
		qcc74x_ble_lp_diag.last_crash_expected_us = session->expected_us;

		k_spin_unlock(&qcc74x_ble_lp_diag_lock, key);
	}
}
