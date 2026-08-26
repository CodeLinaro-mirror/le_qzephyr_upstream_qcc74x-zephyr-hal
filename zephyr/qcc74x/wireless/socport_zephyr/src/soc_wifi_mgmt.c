

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <zephyr/kernel.h>
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include "soc_mem.h"

#if defined(CONFIG_WIFI_QCC75X)
#include <qcc75x_glb.h>
#else
#include <qcc74x_glb.h>
#endif

#include "rfparam_adapter.h"

#include <rtos_def.h>
#include <rtos_al.h>

#define WIFI_NODE DT_NODELABEL(wifi)
#define WIFI_IRQ  DT_IRQN(WIFI_NODE)

#ifndef CONFIG_QCC74X_PM_PDS15_LOG_LEVEL
#define CONFIG_QCC74X_PM_PDS15_LOG_LEVEL LOG_LEVEL_NONE
#endif

LOG_MODULE_REGISTER(qcc74x_soc_wifi, CONFIG_QCC74X_PM_PDS15_LOG_LEVEL);

#define QCC74X_PDS15_WIFI_LOG_DBG(...)                                  \
    do {                                                                \
        if (IS_ENABLED(CONFIG_QCC74X_PM_PDS15_DEBUG_LOG)) {             \
            LOG_DBG(__VA_ARGS__);                                       \
        }                                                               \
    } while (0)

extern void interrupt0_handler(void);

static void qc7xx_wifi_irq_bridge(const void *arg)
{
    (void)arg;
    interrupt0_handler();
}

static void qc7xx_wifi_irq_connect(void)
{
    /* Re-registering the same IRQ every PDS15 wake overflows
     * z_shared_isr_table_entry.clients[] under CONFIG_SHARED_INTERRUPTS,
     * corrupting adjacent kernel state. Connect once. */
    static bool connected;

    if (connected) {
        return;
    }
    connected = true;

#if defined(CONFIG_DYNAMIC_INTERRUPTS)
    (void)irq_connect_dynamic(WIFI_IRQ, 0, qc7xx_wifi_irq_bridge, NULL, 0);
#else
    IRQ_CONNECT(WIFI_IRQ, 0, qc7xx_wifi_irq_bridge, NULL, 0);
#endif
}

static void __wifi_interrupt_init(void)
{
    qc7xx_wifi_irq_connect();
    irq_enable(WIFI_IRQ);
}

void qc7xx_wifi_irq_resume_enable(void)
{
    qc7xx_wifi_irq_connect();
    irq_enable(WIFI_IRQ);
    QCC74X_PDS15_WIFI_LOG_DBG("qcc74x_pds15_resume: wifi irq reconnected irq=%d",
                              WIFI_IRQ);
}

int qc7xx_wifi_rf_resume_restore(void)
{
#if defined(CONFIG_WIFI_QCC75X)
    return 0;
#else
    struct wl_cfg_t *wl_cfg;
    uint32_t xtal_value = 0;
    int ret;

    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_IP_WIFI_PHY |
                         GLB_AHB_CLOCK_IP_WIFI_MAC_PHY |
                         GLB_AHB_CLOCK_IP_WIFI_PLATFORM);

    wl_cfg = rfparam_cfg_get();
    if (wl_cfg == NULL) {
        LOG_ERR("qcc74x_pds15_resume: wifi rf restore missing rf cfg");
        return -ENODEV;
    }

    HBN_Get_Xtal_Value(&xtal_value);

    /*
     * Mirror Bouffalo SDK board_rf_ctl(BRD_CTL_RF_RESET_DEFAULT, 0).
     * This restores RF state after PDS15 without forcing a full RF
     * parameter reload/calibration.
     */
    wl_cfg->en_param_load = 0;
    wl_cfg->en_full_cal = 0;
    wl_cfg->mode = WL_API_MODE_ALL;
    wl_cfg->param.xtalfreq_hz = xtal_value;
    wl_cfg->capcode_set = rfparam_set_capcode;
    wl_cfg->capcode_get = rfparam_get_capcode;
    wl_cfg->param_load = rfparam_load;

    ret = wl_init();
    if (ret != WL_API_STATUS_OK) {
        LOG_ERR("qcc74x_pds15_resume: wifi rf restore failed ret=%d", ret);
        return -EIO;
    }

    wl_cfg->en_param_load = 0;
    wl_cfg->en_full_cal = 0;

    QCC74X_PDS15_WIFI_LOG_DBG("qcc74x_pds15_resume: wifi rf restore done xtal=%u",
                              xtal_value);

    return 0;
#endif
}

K_THREAD_STACK_DEFINE(wifi_stack, 8192);
static struct k_thread wifi_thread_data;

void wifi_main(void *param);
static RTOS_TASK_FCT(wifi_firmware_main)
{
    wifi_main(NULL);

    printf("%s, line:%d\r\n", __FUNCTION__, __LINE__);
    rtos_task_delete(CURRENT_TASK_HANDLE);
}
void cmd_wifi_main(void)
{
    rtos_task_create(wifi_firmware_main, "wifi_main", 0, 8192, NULL, K_PRIO_PREEMPT(1), NULL);
}

int soc_wifi_init(void)
{
    /* em config */
#if defined(CONFIG_WIFI_QCC75X)
#else
    extern int qc7xx_sys_em_config(void);
    qc7xx_sys_em_config();
#endif

    /* RF param init */
    if (0 != rfparam_init(0, NULL, 0)) {
        printf("PHY RF init failed!\r\n");
        return 0;
    }

    /* enable wifi clock */
    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_IP_WIFI_PHY | GLB_AHB_CLOCK_IP_WIFI_MAC_PHY | GLB_AHB_CLOCK_IP_WIFI_PLATFORM);
#if defined(CONFIG_SOC_QCC75X)
#else
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_WIFI);
#endif

    __wifi_interrupt_init();

    cmd_wifi_main();

    return 0;
}
