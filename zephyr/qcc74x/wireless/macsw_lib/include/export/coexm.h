/**
 ****************************************************************************************
 *
 * @file coexm.h
 *
 * @brief QCC74X Wi-Fi/BLE coexistence runtime interface.
 *
 ****************************************************************************************
 */

#ifndef _EXPORT_COEXM_H_
#define _EXPORT_COEXM_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QCC74X_COEX_DUTY_MIN_MS      10U
#define QCC74X_COEX_DUTY_MAX_MS      90U
#define QCC74X_COEX_DUTY_DEFAULT_MS  10U

enum pta_role {
	PTA_ROLE_BT = 0,
	PTA_ROLE_WIFI,
	PTA_ROLE_WIFI_AND_BT_DEFAULT,
	PTA_ROLE_THREAD,
	PTA_ROLE_PTI,
	PTA_ROLE_MAX,
};

#define PM_COEX_SLEEP_FAIL_GUARD         (1U << 0)
#define PM_COEX_SLEEP_FAIL_PS_OFF        (1U << 1)
#define PM_COEX_SLEEP_FAIL_PREVENT_SLEEP (1U << 2)
#define PM_COEX_SLEEP_FAIL_TX_INFLIGHT   (1U << 3)
#define PM_COEX_SLEEP_FAIL_HW_TIMER      (1U << 4)
#define PM_COEX_SLEEP_FAIL_KE_MSG        (1U << 5)
#define PM_COEX_SLEEP_FAIL_CPU           (1U << 6)
#define PM_COEX_SLEEP_FAIL_SLEEP_CTL     (1U << 7)

int pm_coex_init(void);
int pm_coex_deinit(void);
int pm_coex_sleep(void);
int pm_coex_wakeup(void);
int pm_coex_pause(void);
int pm_coex_resume(void);

bool ps_is_coex_mode(void);
bool coex_coord_is_enabled(void);
bool coex_coord_is_active(void);

void ps_coex_mode_enable(void);
void ps_coex_mode_disable(void);
void ps_coex_runtime_pause(void);
void ps_coex_runtime_resume(void);

void coex_coord_on_tbtt(uint32_t tbtt_time, uint8_t vif_index);
bool coex_coord_on_wifi_suspend_enter(void);
void coex_coord_on_wifi_wake(bool slept_committed);
void coex_coord_on_enable(void);
void coex_coord_on_disable(void);
void coex_coord_on_runtime_pause(void);
void coex_coord_on_runtime_resume(void);

bool pm_coex_is_wifi_active_window(void);
int pm_coex_set_wifi_active_window(bool is_active);
int pm_coex_set_wifi_connecting(bool is_connecting);
void pm_coex_force_wifi_role(void);
void pm_coex_force_ble_and_thread(void);
void pm_coex_set_wifi_ack_ba_autoresp_block(bool disable);

struct pm_coex_status {
	uint8_t ps_coex_state;
	uint8_t pm_state;
	uint8_t pta_current_role;
	bool wifi_active_window;
	bool wifi_connecting;
	uint32_t wifi_duty_ms;
};

int pm_coex_get_status(struct pm_coex_status *out);

#ifdef __cplusplus
}
#endif

#endif /* _EXPORT_COEXM_H_ */
