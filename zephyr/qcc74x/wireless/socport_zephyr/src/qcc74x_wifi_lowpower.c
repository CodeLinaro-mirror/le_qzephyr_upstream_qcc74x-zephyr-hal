/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/soc/qualcomm/qcc74x_wifi_lowpower.h>

#include "rwnx.h"
#include "wifi_mgmr_ext.h"

extern void qc7xx_wifi_irq_resume_enable(void);
extern uint32_t wifi_get_next_wakeup_timer_time(void);

static struct k_spinlock qcc74x_wifi_lowpower_lock;
static struct qcc74x_wifi_lowpower_status qcc74x_wifi_lowpower_last_status;

#define QCC74X_WIFI_LOWPOWER_RAW_STATE_DISCONNECTED       0
#define QCC74X_WIFI_LOWPOWER_RAW_STATE_INACTIVE           1
#define QCC74X_WIFI_LOWPOWER_RAW_STATE_INTERFACE_DISABLED 2
#define QCC74X_WIFI_LOWPOWER_DEFAULT_DTIM_ORIGIN          1U

static void qcc74x_wifi_lowpower_status_defaults(
	struct qcc74x_wifi_lowpower_status *status)
{
	*status = (struct qcc74x_wifi_lowpower_status){
		.sampled = true,
		.wifi_config_enabled = IS_ENABLED(CONFIG_WIFI_QCC74X),
		.wifi_ps_compiled = IS_ENABLED(CONFIG_QCC74X_WIFI_LOW_POWER),
		.bssid_rc = QCC74X_WIFI_LOWPOWER_NOT_RUN,
		.channel_rc = QCC74X_WIFI_LOWPOWER_NOT_RUN,
		.sta_raw_state = QCC74X_WIFI_LOWPOWER_NOT_RUN,
		.aid = QCC74X_WIFI_LOWPOWER_NOT_RUN,
		.channel = QCC74X_WIFI_LOWPOWER_NOT_RUN,
		.twt_flow_count = QCC74X_WIFI_LOWPOWER_NOT_RUN,
		.ps_sleep_check_rc = QCC74X_WIFI_LOWPOWER_NOT_RUN,
		.lpfw_check_allow = QCC74X_WIFI_LOWPOWER_NOT_RUN,
	};
}

static enum qcc74x_wifi_lowpower_reason
qcc74x_wifi_lowpower_classify(
	const struct qcc74x_wifi_lowpower_status *status)
{
	if (!status->wifi_config_enabled) {
		return QCC74X_WIFI_LOWPOWER_REASON_WIFI_DISABLED;
	}

	if (!status->sta_connected) {
		if ((status->sta_raw_state != QCC74X_WIFI_LOWPOWER_NOT_RUN) &&
		    (status->sta_raw_state !=
		     QCC74X_WIFI_LOWPOWER_RAW_STATE_DISCONNECTED) &&
		    (status->sta_raw_state !=
		     QCC74X_WIFI_LOWPOWER_RAW_STATE_INACTIVE) &&
		    (status->sta_raw_state !=
		     QCC74X_WIFI_LOWPOWER_RAW_STATE_INTERFACE_DISABLED)) {
			return QCC74X_WIFI_LOWPOWER_REASON_STA_BUSY;
		}

		return QCC74X_WIFI_LOWPOWER_REASON_NOT_CONNECTED;
	}

	if (!status->wifi_ps_compiled) {
		return QCC74X_WIFI_LOWPOWER_REASON_PS_NOT_COMPILED;
	}

	if ((status->ps_sleep_check_rc != QCC74X_WIFI_LOWPOWER_NOT_RUN) &&
	    (status->ps_sleep_check_rc != 0)) {
		return QCC74X_WIFI_LOWPOWER_REASON_PS_SLEEP_CHECK_FAILED;
	}

	if (status->lpfw_check_allow == 0) {
		return QCC74X_WIFI_LOWPOWER_REASON_LPFW_BLOCKED;
	}

	if (status->next_wakeup_timer_valid &&
	    (status->next_wakeup_timer_us <
	     QCC74X_WIFI_LOWPOWER_WAKEUP_AHEAD_US)) {
		return QCC74X_WIFI_LOWPOWER_REASON_NEXT_WAKE_TOO_CLOSE;
	}

	if ((status->twt_flow_count > 0) &&
	    !status->next_wakeup_timer_valid) {
		return QCC74X_WIFI_LOWPOWER_REASON_TWT_NEXT_WAKE_INVALID;
	}

	if (!status->active_check_ran) {
		return QCC74X_WIFI_LOWPOWER_REASON_ACTIVE_CHECK_NOT_RUN;
	}

	return QCC74X_WIFI_LOWPOWER_REASON_READY;
}

static void qcc74x_wifi_lowpower_store_status(
	const struct qcc74x_wifi_lowpower_status *status)
{
	k_spinlock_key_t key = k_spin_lock(&qcc74x_wifi_lowpower_lock);

	qcc74x_wifi_lowpower_last_status = *status;

	k_spin_unlock(&qcc74x_wifi_lowpower_lock, key);
}

void qcc74x_wifi_lowpower_sample(bool active_check,
				 struct qcc74x_wifi_lowpower_status *status)
{
	struct qcc74x_wifi_lowpower_status sample;
	int channel = 0;

	qcc74x_wifi_lowpower_status_defaults(&sample);

	if (sample.wifi_config_enabled) {
		sample.sta_raw_state = wifi_mgmr_sta_state_get_ext();
		sample.bssid_rc = wifi_mgmr_sta_get_bssid(sample.bssid);
		sample.sta_connected = (sample.bssid_rc == 0);

		if (sample.sta_connected) {
			sample.channel_rc =
				wifi_mgmr_sta_channel_get(&channel);
			if (sample.channel_rc == 0) {
				sample.channel = channel;
			}
			sample.aid = wifi_mgmr_sta_aid_get();
			sample.twt_flow_count =
				wifi_mgmr_sta_twt_flow_get();
			sample.lpfw_check_allow =
				qc7xx_lp_fw_enter_check_allow();
			sample.next_wakeup_timer_us =
				wifi_get_next_wakeup_timer_time();
			sample.next_wakeup_timer_valid =
				(sample.next_wakeup_timer_us != UINT32_MAX);

			if (active_check) {
				struct rwnxl_sleep_check_result ps_result;

				sample.active_check_ran = true;
				sample.ps_sleep_check_rc =
					rwnxl_ps_sleep_check_quiet(
						&ps_result);
				sample.ps_detail.cpu_can_sleep =
					ps_result.cpu_can_sleep;
				sample.ps_detail.queue_sent_empty =
					ps_result.queue_sent_empty;
				sample.ps_detail.queue_saved_empty =
					ps_result.queue_saved_empty;
				sample.ps_detail.ps_allowed =
					ps_result.ps_allowed;
				sample.ps_detail.tx_allowed =
					ps_result.tx_allowed;
				sample.ps_detail.machw_allowed =
					ps_result.machw_allowed;
			}
		}
	}

	sample.reason = qcc74x_wifi_lowpower_classify(&sample);
	qcc74x_wifi_lowpower_store_status(&sample);

	if (status != NULL) {
		*status = sample;
	}
}

int qcc74x_wifi_lowpower_can_enter(uint64_t timeout_us,
				   struct qcc74x_wifi_lowpower_status *status)
{
	struct qcc74x_wifi_lowpower_status sample;

	ARG_UNUSED(timeout_us);

	qcc74x_wifi_lowpower_sample(true, &sample);
	if (status != NULL) {
		*status = sample;
	}

	switch (sample.reason) {
	case QCC74X_WIFI_LOWPOWER_REASON_READY:
	case QCC74X_WIFI_LOWPOWER_REASON_NOT_CONNECTED:
	case QCC74X_WIFI_LOWPOWER_REASON_WIFI_DISABLED:
		return 0;
	default:
		return -EAGAIN;
	}
}

int qcc74x_wifi_lowpower_prepare(qc7xx_lp_fw_cfg_t *cfg,
				 uint64_t *timeout_us,
				 struct qcc74x_wifi_lowpower_session *session)
{
	struct qcc74x_wifi_lowpower_status status;
	uint32_t next_wakeup_us;
	uint64_t requested_timeout_us;
	bool beacon_stamp_ready;

	if ((cfg == NULL) || (timeout_us == NULL) || (session == NULL)) {
		return -EINVAL;
	}

	requested_timeout_us = *timeout_us;
	*session = (struct qcc74x_wifi_lowpower_session){ 0 };

	qcc74x_wifi_lowpower_sample(true, &status);
	session->status = status;

	if (!status.wifi_config_enabled) {
		return 0;
	}

	if (!status.sta_connected) {
		if (status.reason != QCC74X_WIFI_LOWPOWER_REASON_NOT_CONNECTED) {
			return -EAGAIN;
		}

		rwnxl_regs_save_ops();
		session->resume_needed = true;
		return 0;
	}

	if (status.reason != QCC74X_WIFI_LOWPOWER_REASON_READY) {
		return -EAGAIN;
	}

	cfg->bcmc_dtim_mode = 0U;
	cfg->dtim_origin = QCC74X_WIFI_LOWPOWER_DEFAULT_DTIM_ORIGIN;
	beacon_stamp_ready =
		(iot2lp_para->last_beacon_stamp_rtc_valid != 0U) &&
		(iot2lp_para->last_beacon_stamp_rtc_us != 0U) &&
		(iot2lp_para->beacon_dtim_period != 0U);
	cfg->tim_wakeup_en =
		((status.twt_flow_count == 0) && beacon_stamp_ready) ? 1U : 0U;
	cfg->dtim_num = cfg->dtim_origin;
	cfg->mtimer_timeout_mini_us = 4500U;
	cfg->mtimer_timeout_max_us = 12000U;

	status.beacon_ready = beacon_stamp_ready;
	status.tim_wakeup_en = (cfg->tim_wakeup_en != 0U);
	session->status = status;
	session->tim_wakeup_en = status.tim_wakeup_en;
	qcc74x_wifi_lowpower_store_status(&status);

	qc7xx_lp_fw_bcn_loss_cfg_dtim_default(cfg->dtim_origin);

	if (rwnxl_connected_enter_ops() != 0) {
		status.reason = QCC74X_WIFI_LOWPOWER_REASON_ENTER_OPS_FAILED;
		session->status = status;
		qcc74x_wifi_lowpower_store_status(&status);
		return -EIO;
	}

	if (rwnxl_pds_wifi_config(cfg) != 0) {
		status.reason = QCC74X_WIFI_LOWPOWER_REASON_CONFIG_FAILED;
		session->status = status;
		qcc74x_wifi_lowpower_store_status(&status);
		return -EIO;
	}

	if (qc7xx_lp_fw_enter_check_allow() == 0) {
		status.reason = QCC74X_WIFI_LOWPOWER_REASON_LPFW_BLOCKED;
		session->status = status;
		qcc74x_wifi_lowpower_store_status(&status);
		return -EAGAIN;
	}

	next_wakeup_us = wifi_get_next_wakeup_timer_time();
	if (next_wakeup_us != UINT32_MAX) {
		if (next_wakeup_us < QCC74X_WIFI_LOWPOWER_WAKEUP_AHEAD_US) {
			status.reason =
				QCC74X_WIFI_LOWPOWER_REASON_NEXT_WAKE_TOO_CLOSE;
			status.next_wakeup_timer_us = next_wakeup_us;
			status.next_wakeup_timer_valid = true;
			session->status = status;
			qcc74x_wifi_lowpower_store_status(&status);
			return -EAGAIN;
		}

		if ((uint64_t)next_wakeup_us < *timeout_us) {
			*timeout_us = next_wakeup_us;
		}
	} else if (status.twt_flow_count > 0) {
		status.reason =
			QCC74X_WIFI_LOWPOWER_REASON_TWT_NEXT_WAKE_INVALID;
		status.next_wakeup_timer_us = next_wakeup_us;
		status.next_wakeup_timer_valid = false;
		session->status = status;
		qcc74x_wifi_lowpower_store_status(&status);
		return -EAGAIN;
	}

	ARG_UNUSED(requested_timeout_us);

	rwnxl_regs_save_ops();
	session->resume_needed = true;

	return 0;
}

void qcc74x_wifi_lowpower_resume(
	const struct qcc74x_wifi_lowpower_session *session,
	qc7xx_lp_fw_cfg_t *cfg)
{
	if ((session == NULL) || (cfg == NULL) || !session->resume_needed) {
		return;
	}

	rwnxl_resume_wifi_not_isr(cfg);
	qc7xx_wifi_irq_resume_enable();
}

void qcc74x_wifi_lowpower_get_status(
	struct qcc74x_wifi_lowpower_status *status)
{
	k_spinlock_key_t key;

	if (status == NULL) {
		return;
	}

	key = k_spin_lock(&qcc74x_wifi_lowpower_lock);
	*status = qcc74x_wifi_lowpower_last_status;
	k_spin_unlock(&qcc74x_wifi_lowpower_lock, key);
}
