/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "qcc74x_lp.h"

#include "qcc74x_pm.h"

#include "stdint.h"
#include "tzc_sec_reg.h"
#include "hbn_reg.h"
#include "pds_reg.h"
#include "glb_reg.h"
#include "qcc74x_glb.h"
#include "qcc74x_glb_gpio.h"
#include "qcc74x_aon.h"
#include "lpfw_printf.h"

#include "shell.h"
// #include "common.h"
#include "lp_wdt.h"
#include "lp_gpio.h"
#include "qc7xx_irq.h"

#define DISABLE_PART_LOG 1

extern uint32_t __RAM_WIFI_START_ADD;

extern void lpfw_pds15_mode_enter(uint32_t sleep_time);
extern void lpfw_pds1_mode_enter(uint32_t sleep_time);
extern int lpfw_mac_reset_for_app(void);

static uint8_t lpfw_rx_started;

static __attribute__((noreturn)) void lpfw_handoff_stop(uint32_t reason)
{
    __asm volatile("csrci mstatus, 8" ::: "memory");
    iot2lp_para->handoff_failure = reason;
    pm_pds_wakeup_irq_quiesce();
    lp_wdt_disable();
    for (;;) {
        __WFI();
    }
}

#define LP_FW_PRE_JUMP_ADDR     0x20010000

#define ATTR_ROM_WIFI_SECTION   //__attribute__((section(".romwifi_code." ATTR_UNI_SYMBOL)))

#define SLEEP_IDLE_OPT_US       (150)

#define SLEEP_UNKOWN_CORRECT_US (0)

#define TIM_IE_ID               5

#define DUMMY_WAIT \
    {              \
        __NOP();   \
    }

#define DELAY_US(N)                                                               \
    {                                                                             \
        for (uint8_t delay_us_i = 0; delay_us_i < N; delay_us_i++) {              \
            for (uint8_t delay_us_temp = 0; delay_us_temp < 4; delay_us_temp++) { \
                DUMMY_WAIT;                                                       \
            }                                                                     \
        }                                                                         \
    }

typedef struct __attribute__((packed)) {
    uint32_t addr;
    uint32_t stack;
} app_jump_para_t;

typedef struct {
    volatile uint64_t beacon_time_stamp;
    volatile uint16_t beacon_interval;
    volatile uint16_t capabilities;
} lp_fw_fixed_param_t;

typedef struct {
    volatile uint8_t ie_number;
    volatile uint8_t ie_length;
    volatile uint8_t ie_data[0];
} lp_fw_ie_t;

typedef struct {
    volatile uint64_t beacon_timestamp_us;
    volatile uint64_t rtc_timestamp_us;
} lp_fw_timestamp_t;

extern uint8_t lpfw_get_gpio_value(uint8_t gpioPin);
extern void lpfw_beacon_status_clear(void);
extern int lpfw_check_beacon_interval(void);
extern int lpfw_check_beacon_and_bssid(void);
extern void lpfw_wifi_init(uint32_t *wl_rmem_addr, volatile iot2lp_para_t *p_iot2lp_parameter);
extern void lpfw_wifi_clk_gate(void);
extern int lpfw_bcn_timestamp_check(uint64_t beacon_timestamp_now_us, uint64_t rtc_timestamp_now_us);
extern volatile lp_fw_ie_t *lpfw_find_tim(uint32_t interval_start_mtimer, uint8_t beacon_rate_code);
extern int lpfw_recal_rc32k(uint64_t beacon_timestamp_now_us, uint64_t rtc_timestamp_now_us, uint32_t mode, int clock_ready_check);
extern int32_t lpfw_calculate_beacon_delay(uint64_t beacon_timestamp_us, uint64_t rtc_timestamp_us, uint32_t mode);
extern int32_t lpfw_beacon_delay_sliding_win_get_average();
extern int qc7xx_lp_beacon_tim_update(uint8_t *tim, uint8_t mode);
extern void mtimer_init(uint32_t timeout_us);
extern void lpfw_static_val_addr_init(uint32_t *addr);
extern void lpfw_wifi_rx_start(void);
extern int lpfw_wifi_rx_broadcast(uint32_t buf_addr);

#if LPFW_LOG_EN
extern int lpfw_uart_tx_isbusy(uint8_t uart_id);
#endif

void hbn_rtc_timeout_handler(void);
void app_jump_prepare(uint32_t wakeup_cause, uint32_t machw_reset);

extern uint32_t __hbn_load_addr;
extern uint32_t __hbn_ram_start__;
extern uint32_t __hbn_ram_end__;
extern uint32_t _sp_main;

ATTR_HBN_DATA_SECTION app_jump_para_t app_jump_para;

// ocram code
lp_fw_static_variable_t lpfw_ram_static_val;

/* wifi rx buff */
// __attribute__((section(".wifi_rx_buffer"))) uint32_t wifi_rx_buff[1024 / 4];
// #define LPFW_WIFI_RX_BUFF ((uint32_t)wifi_rx_buff & 0x2FFFFFFF)

/* Go back to sleep (PDS15) */
int blfw_pds_mode_enter(uint32_t pds_sleep_us, volatile iot2lp_para_t *p_iot2lp_para)
{
    uint32_t pds_sleep_cnt;

    /* Keep MAC accessible until the PDS1 APP-return decision is made. */
    if (p_iot2lp_para->pds_level != 1) {
        lpfw_wifi_clk_gate();
    }

    /* set bootrom startup addr */
    pm_set_wakeup_callback((void (*)(void))LP_FW_PRE_JUMP_ADDR /*lp_fw_pre*/);

    /* us to pds_cnt */
    pds_sleep_cnt = QC7XX_US_TO_PDS_CNT(pds_sleep_us);

    AON_Output_Float_LDO15_RF();

    /* Minimum limit */
    if (pds_sleep_cnt < (PDS_WAKEUP_MINI_LIMIT_CNT + PDS_WAKEUP_DELAY_CNT)) {
        pds_sleep_cnt = PDS_WAKEUP_MINI_LIMIT_CNT;
    } else {
        pds_sleep_cnt -= PDS_WAKEUP_DELAY_CNT;
    }

    if (p_iot2lp_para->rtc_wakeup_en) {
        uint64_t rtc_current_cnt, rtc_wakeup_cnt;
        uint32_t rtc_sleep_cnt;

        rtc_wakeup_cnt = p_iot2lp_para->rtc_wakeup_cnt;

        /* get pds cnt */
        HBN_Get_RTC_Timer_Val((uint32_t *)&rtc_current_cnt, (uint32_t *)&rtc_current_cnt + 1);

        /* rtc timeout wakeup */
        if (rtc_current_cnt + PDS_WAKEUP_MINI_LIMIT_CNT >= rtc_wakeup_cnt) {
            app_jump_prepare(LPFW_WAKEUP_TIME_OUT, 1);
        }

        /* get rtc Remaining cnt */
        rtc_sleep_cnt = rtc_wakeup_cnt - rtc_current_cnt;

        /* rtc and pds clashed in time */
        if ((rtc_sleep_cnt >= pds_sleep_cnt && ((rtc_sleep_cnt - pds_sleep_cnt) <= QC7XX_MS_TO_PDS_CNT(PROTECT_BF_MS))) ||
            (pds_sleep_cnt >= rtc_sleep_cnt && ((pds_sleep_cnt - rtc_sleep_cnt) <= QC7XX_MS_TO_PDS_CNT(PROTECT_AF_MS)))) {
#ifndef DISABLE_PART_LOG
            lpfw_printf_debug("lpfw rtc advance %d ", QC7XX_PDS_CNT_TO_US(rtc_sleep_cnt));
#endif

            if (pds_sleep_cnt > QC7XX_MS_TO_PDS_CNT(PROTECT_AF_MS)) {
                /* Rewrite the rtc sleep cnt */
                rtc_sleep_cnt = pds_sleep_cnt - QC7XX_MS_TO_PDS_CNT(PROTECT_AF_MS);
            } else {
                rtc_sleep_cnt = 0;
            }

            if (rtc_sleep_cnt < (PDS_WAKEUP_MINI_LIMIT_CNT + PDS_WAKEUP_DELAY_CNT)) {
                /* minimum time limit */
                app_jump_prepare(LPFW_WAKEUP_TIME_OUT, 1);
            } else {
                rtc_sleep_cnt -= PDS_WAKEUP_DELAY_CNT;
            }

            /* update rtc wakeup cnt */
            rtc_wakeup_cnt = rtc_current_cnt + rtc_sleep_cnt;
            p_iot2lp_para->rtc_wakeup_cnt = rtc_wakeup_cnt;

            /* set rtc compare */
            QC7XX_WR_REG(HBN_BASE, HBN_TIME_H, (uint32_t)(rtc_wakeup_cnt >> 32));
            QC7XX_WR_REG(HBN_BASE, HBN_TIME_L, (uint32_t)rtc_wakeup_cnt);

            lpfw_printf_debug("--> %d\r\n", QC7XX_PDS_CNT_TO_US(rtc_sleep_cnt));
        }
    }

    if (p_iot2lp_para->pds_level == 1) {
        lpfw_wifi_clk_gate();
    }
    lpfw_printf_debug("pds_sleep_us %d\r\n", pds_sleep_us);

    /* Wait for the character to finish sending */
#if LPFW_LOG_EN
    lpfw_printf_debug("\r\n\r\n");
    while (lpfw_uart_tx_isbusy(0)) {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
#endif

    /* MCU CLK SELECT RC32M */
    GLB_Set_MCU_System_CLK(GLB_MCU_SYS_CLK_RC32M);
    /* Power Off WifiPLL */
    // GLB_Power_Off_WIFIPLL();

    /* limit */
    if (pds_sleep_cnt > 32768 * 3) {
        pds_sleep_cnt = 32768 * 3;
    }

    /* update active_lpfw time */
    qc7xx_lp_time_info_update_lpfw();

    /* pds mode enter */
    if (p_iot2lp_para->pds_level == 1) {
        lpfw_pds1_mode_enter(pds_sleep_cnt);
        lpfw_handoff_stop(4);
    } else if (p_iot2lp_para->pds_level == 15) {
        lpfw_pds15_mode_enter(pds_sleep_cnt);
    } else {
        lpfw_handoff_stop(5);
    }

    return 0;
}

/* jump to app (wakeup) */
void app_jump_prepare(uint32_t wakeup_cause, uint32_t machw_reset)
{
    if (iot2lp_para->pds_level == 1) {
        __asm volatile("csrci mstatus, 8" ::: "memory");
        pm_pds_wakeup_irq_quiesce();
        qc7xx_irq_disable(GPIO_INT0_IRQn);
        qc7xx_irq_disable(HBN_OUT0_IRQn);
        qc7xx_irq_disable(HBN_OUT1_IRQn);
        qc7xx_irq_disable(WIFI_IRQn);
        qc7xx_irq_disable(MTIME_IRQn);
        if (lpfw_rx_started && lpfw_mac_reset_for_app() != 0) {
            lpfw_handoff_stop(1);
        }
        qc7xx_irq_clear_pending(WIFI_IRQn);
        qc7xx_irq_clear_pending(MTIME_IRQn);
    }
    /* gate wifi clk */
    lpfw_wifi_clk_gate();

    /* disable and clear rtc int */
    QC7XX_WR_REG(HBN_BASE, HBN_CTL, QC7XX_RD_REG(HBN_BASE, HBN_CTL) & 0xfffffff1);
    HBN_Clear_RTC_INT();

    /* Recording the wake up Cause */
    iot2lp_para->wakeup_reason = wakeup_cause;

    iot2lp_para->do_xip_recovery = 1;

    pm_set_wakeup_callback((void (*)(void))LP_FW_PRE_JUMP_ADDR /*lp_fw_pre*/);

    /* Wait for the character to finish sending */
#if LPFW_LOG_EN
    while (lpfw_uart_tx_isbusy(0)) {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
#endif

    /* MCU CLK SELECT RC32M */
    GLB_Set_MCU_System_CLK(GLB_MCU_SYS_CLK_RC32M);
    /* Power Off WifiPLL */
    // GLB_Power_Off_WIFIPLL();

    /* update active_lpfw time */
    qc7xx_lp_time_info_update_lpfw();

    if (iot2lp_para->pds_level == 1) {
        (void)pm_reset_cpu_to_lpfw();
        lpfw_handoff_stop(2);
    }
    if (iot2lp_para->pds_level != 15) {
        lpfw_handoff_stop(5);
    }
    /* Preserve the PDS15 APP return bridge only for PDS15. */
    lpfw_pds15_mode_enter(40);
}

static uint8_t get_broadcast_bit(uint8_t data)
{
    return data & 0x01;
}

static uint8_t get_bitmap_offset(uint8_t data)
{
    return data & 0xfe;
}

static uint8_t get_bitmap(uint16_t aid, uint8_t *data, uint16_t len)
{
    uint8_t bitmap_control = data[0];
    uint8_t *pvb_addr = (uint8_t *)(data + 1);
    uint8_t bitmap_offset = 0;
    uint16_t temp_aid;

    bitmap_offset = get_bitmap_offset(bitmap_control);

    if ((aid / 8) < bitmap_offset) {
        return 0;
    } else {
        temp_aid = aid - (uint16_t)bitmap_offset * 8;
        if ((temp_aid / 8) > (len - 2)) {
            return 0;
        } else {
            return (pvb_addr[temp_aid >> 3] >> (temp_aid & 0x07)) & 0x1;
        }
    }
}

/* parse tim and go to sleep/wakeup */
static int lp_fw_parse_tim(volatile lp_fw_ie_t *p_ie_tim, volatile iot2lp_para_t *p_iot2lp_para, lp_fw_timestamp_t *p_time_stamp)
{
    int ret;
    uint32_t b_beacon_delay_us = 0;
    uint8_t dtim_cnt, broadcast_bit;
    uint32_t do_app_jump = 0;

    if (p_ie_tim->ie_number != TIM_IE_ID) {
        return -1;
    }

    dtim_cnt = p_ie_tim->ie_data[0];

    /* check bcn timestamp */
    ret = lpfw_bcn_timestamp_check(p_time_stamp->beacon_timestamp_us, p_time_stamp->rtc_timestamp_us);
    if (ret < 0) {
#ifndef DISABLE_PART_LOG
        lpfw_printf_debug("bcn_timestamp_check error %d\r\n", ret);
#endif
        return -1;
    }

    /* recal_rc32k */
    ret = lpfw_recal_rc32k(p_time_stamp->beacon_timestamp_us, p_time_stamp->rtc_timestamp_us, BEACON_STAMP_LPFW, 0);
    if (ret < 0) {
#ifndef DISABLE_PART_LOG
        lpfw_printf_debug("lpfw_recal_rc32k error %d\r\n", ret);
#endif
        return -2;
    }

#ifndef DISABLE_PART_LOG
    lpfw_printf_debug("rc32k code %d ppm %d\r\n", p_iot2lp_para->rc32k_fr_ext, p_iot2lp_para->rtc32k_error_ppm);
#endif

/* updata beacon delay */
#ifdef SHARED_FUNC_EN
    b_beacon_delay_us = shared_lpfw_calculate_beacon_delay(p_time_stamp->beacon_timestamp_us, p_time_stamp->rtc_timestamp_us, BEACON_STAMP_LPFW);
#else
    b_beacon_delay_us = lpfw_calculate_beacon_delay(p_time_stamp->beacon_timestamp_us, p_time_stamp->rtc_timestamp_us, BEACON_STAMP_LPFW);
#endif

    /* update beacon tim */
    qc7xx_lp_beacon_tim_update((uint8_t *)p_ie_tim, BEACON_STAMP_LPFW);

#ifndef DISABLE_PART_LOG
    lpfw_printf_debug("beacon_delay_us %d, offset %d\r\n", b_beacon_delay_us, p_iot2lp_para->bcn_delay_offset);

    lpfw_printf_debug("beacon_delay_ave_us %d\r\n", BCN_DELAY_AVE);
    lpfw_printf_debug("mtimer_timeout %d\r\n", MTIMER_TIMEOUT);
#endif

    if (dtim_cnt == 0) {
        broadcast_bit = get_broadcast_bit(p_ie_tim->ie_data[2]);
        if (broadcast_bit) {
            do_app_jump |= LPFW_WAKEUP_WIFI_BROADCAST;
        }
    }

    if (get_bitmap(p_iot2lp_para->aid, (uint8_t *)&p_ie_tim->ie_data[2], p_ie_tim->ie_length - 2)) {
        do_app_jump |= LPFW_WAKEUP_WIFI;
    }

#ifndef DISABLE_PART_LOG
    lpfw_printf_debug("g_wifi_rx_idle_mtimer %d\r\n", WIFI_RX_START_MTIMER - WIFI_RX_READY_MTIMER);
    lpfw_printf_debug("g_ready_mtimer %d\r\n", WIFI_RX_READY_MTIMER);
    lpfw_printf_debug("tim bit set: %d\r\n", do_app_jump);
#endif
    return do_app_jump;
}

static void lpfw_sleep_ml(volatile iot2lp_para_t *p_iot2lp_para)
{
    uint32_t b_beacon_delay_us = 0;
    uint32_t pds_sleep_us = 0, dtim_period_us, beacon_interval_us, total_error_us;
    uint32_t dtim_num;

    /* beacon interval us, TU * 1024 */
    beacon_interval_us = iot2lp_para->beacon_interval_tu * 1024;

    if (p_iot2lp_para->bcmc_dtim_mode) {
        dtim_num = p_iot2lp_para->beacon_dtim_period * p_iot2lp_para->dtim_num;

        if (p_iot2lp_para->last_beacon_dtim_count) {
            dtim_num = dtim_num + p_iot2lp_para->last_beacon_dtim_count - p_iot2lp_para->beacon_dtim_period;
        }

    } else {
        dtim_num = p_iot2lp_para->dtim_num;
    }

    b_beacon_delay_us = p_iot2lp_para->last_beacon_delay_us;
    dtim_period_us = dtim_num * beacon_interval_us;

#ifndef DISABLE_PART_LOG
    lpfw_printf_debug("dtim num %d, dtim_period %d\r\n", dtim_num, dtim_period_us);
#endif

/* Get the time sleep*/
#ifdef SHARED_FUNC_EN
    LPFW_SLEEP_MTIMER = shared_cpu_get_mtimer_counter();
#else
    LPFW_SLEEP_MTIMER = CPU_Get_MTimer_Counter();
#endif

    /* dtim period */
    pds_sleep_us = dtim_period_us;

    /* now -> next bcn */
    pds_sleep_us = pds_sleep_us - ((LPFW_SLEEP_MTIMER - WIFI_RX_START_MTIMER) + WIFI_RX_READY_MTIMER) % beacon_interval_us;

    /* beacon delay and Keep idle time */
    pds_sleep_us = pds_sleep_us - b_beacon_delay_us - SLEEP_IDLE_OPT_US - SLEEP_UNKOWN_CORRECT_US;

    /* rc32k error compensation (now -> next bcn) */
    pds_sleep_us += (int32_t)((int64_t)pds_sleep_us * p_iot2lp_para->rtc32k_error_ppm / (1000 * 1000));

    /* Jitter total error form (now -> next bcn), jitter error compensation (ppm) */
    total_error_us = (int64_t)pds_sleep_us * p_iot2lp_para->rtc32k_jitter_error_ppm / (1000 * 1000);
    if (total_error_us > 2 * 1000) {
        total_error_us = 2 * 1000;
    }
    pds_sleep_us = pds_sleep_us - total_error_us;

    /* Manual compensation value */
    pds_sleep_us = pds_sleep_us - p_iot2lp_para->tpre;

    lpfw_printf_debug("pds_sleep_us %d, dtim_cnt:%d\r\n", pds_sleep_us, p_iot2lp_para->last_beacon_dtim_count);

    /* Recorded the error value, for mtimer timeout */
    iot2lp_para->last_sleep_error_us = total_error_us;

    blfw_pds_mode_enter(pds_sleep_us, p_iot2lp_para);
}

#if LPFW_SHELL_EN
int uart_rx_char(void);

#if LPFW_MFG_EN
static int shell_exit_lpfw(int argc, char **argv)
{
#ifndef DISABLE_PART_LOG
    lpfw_printf("will exit LPFW...\r\n");
#endif
    /* gate wifi clk */
    lpfw_wifi_clk_gate();

    /* MCU CLK SELECT RC32M */
    GLB_Set_MCU_System_CLK(GLB_MCU_SYS_CLK_RC32M);
    /* Power Off WifiPLL */
    // GLB_Power_Off_WIFIPLL();
    /* Clear HBN Jump Addr */
    pm_set_wakeup_callback((void (*)(void))0xFFFFFFFF);

    QC7XX_WR_REG(HBN_BASE, HBN_RSV0, 0);

    //pm_pds_mode_enter(PM_PDS_LEVEL_15, 40);
    GLB_SW_POR_Reset();
    return 0;
}
SHELL_CMD_EXPORT_ALIAS(shell_exit_lpfw, exit_lpfw, shell exit_lpfw.);

#else

#ifndef DISABLE_PART_LOG
static int shell_test(int argc, char **argv)
{
    lpfw_printf("shell ok!\r\n");

    for (uint16_t i = 0; i < argc; i++) {
        lpfw_printf("arg%d: %s\r\n", i, argv[i]);
    }
    lpfw_printf("\r\n");
    return 0;
}
SHELL_CMD_EXPORT_ALIAS(shell_test, test, shell test.);

static int shell_mem_read(int argc, char **argv)
{
    uint32_t *word_addr = NULL;
    uint32_t word_num = 0;

    if (argc < 2) {
        lpfw_printf("Too few parameters\r\n");
        lpfw_printf("mem_read <start addr> <word num>\r\n");
        return 0;
    }

    word_addr = (uint32_t *)strtoll(argv[1], NULL, 16);
    if ((uint32_t)word_addr % 4) {
        lpfw_printf("Addresses are not aligned\r\n");
        return 0;
    }

    if (argc > 2) {
        word_num = strtoll(argv[2], NULL, 16);
    } else {
        word_num = 1;
    }

    for (uint16_t i = 0; i < word_num; i++) {
        lpfw_printf("0x%08X : 0x%08X\r\n", (uint32_t)word_addr, *word_addr);
        word_addr++;
    }
    lpfw_printf("\r\n");
    return 0;
}
SHELL_CMD_EXPORT_ALIAS(shell_mem_read, mem_read, memory read.);
#endif

#endif

#endif

ATTR_ROM_WIFI_SECTION uint32_t lpfw_calculate_rx_timeout(int32_t *bcn_delay_ave)
{
    uint32_t mtimer_timeout, total_error_us;

    if (iot2lp_para->last_beacon_stamp_rtc_valid == 0 || iot2lp_para->continuous_loss_cnt > iot2lp_para->bcn_loss_level_max) {
        return (110 * 1000);
    }

    /* Total error */
    total_error_us = iot2lp_para->last_sleep_error_us;

/* get mini and ave */
#ifdef SHARED_FUNC_EN
    *bcn_delay_ave = shared_lpfw_beacon_delay_sliding_win_get_average();
#else
    *bcn_delay_ave = lpfw_beacon_delay_sliding_win_get_average();
#endif

    /* The bcn_delay average  */
    mtimer_timeout = *bcn_delay_ave * 5 / 2 + 2000;

    /* Forward and backward error */
    mtimer_timeout = mtimer_timeout + total_error_us * 2;

    mtimer_timeout = mtimer_timeout + SLEEP_IDLE_OPT_US;

    /* limit */
    if (mtimer_timeout < iot2lp_para->mtimer_timeout_mini_us) {
        mtimer_timeout = iot2lp_para->mtimer_timeout_mini_us;
    } else if (mtimer_timeout > iot2lp_para->mtimer_timeout_max_us) {
        mtimer_timeout = iot2lp_para->mtimer_timeout_max_us;
    }

    if (iot2lp_para->lpfw_wakeup_cnt <= 1) {
        if (mtimer_timeout < 6000) {
            mtimer_timeout = 6000;
        }
    }

    if (mtimer_timeout > 120000) {
        mtimer_timeout = 120000;
    }

    if (iot2lp_para->continuous_loss_cnt) {
        uint32_t win_extend_duration_us = iot2lp_para->bcn_loss_cfg_table[iot2lp_para->bcn_loss_level].win_extend_duration_us;
        if (mtimer_timeout < win_extend_duration_us) {
            mtimer_timeout = win_extend_duration_us;
        }
    }

    return mtimer_timeout;
}

/* set bus and cpu clock */
static void lpfw_set_hclk_div(uint32_t div)
{
    uint32_t tmpVal;

    /* config hclk_div=div */
    tmpVal = QC7XX_RD_REG(GLB_BASE, GLB_SYS_CFG0);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_HCLK_DIV, div);
    QC7XX_WR_REG(GLB_BASE, GLB_SYS_CFG0, tmpVal);
}

/* mtimer interrupt */
void mtimer_irq_handler(void)
{
#if (LPFW_GPIO_DEBUG == 1)
    /* pull gpio12 high */
    (*(volatile uint32_t *)(GLB_BASE + 0x8F4)) = 0x1000B40;

#ifdef SHARED_FUNC_EN
    shared_arch_delay_ms(1);
#else
    arch_delay_ms(1);
#endif
#endif

    lp_fw_bcn_loss_level_t *bcn_loss_cfg = NULL;
    uint8_t bcn_loss_level = 0;
    uint8_t wakeup_flag = 0;

    /* config hclk_div=3, Cpu Clock = 80M */
    lpfw_set_hclk_div(3);

    /* stat rate of packet loss*/
    iot2lp_para->lp_info->bcn_lpfw_loss_cnt++;

    /*  */
    iot2lp_para->continuous_loss_cnt++;

    /* stat number of continous packet loss*/
    if ((iot2lp_para->continuous_loss_cnt < iot2lp_para->continuous_loss_cnt_max) &&
        (iot2lp_para->bcn_loss_level < iot2lp_para->bcn_loss_level_max)) {
        bcn_loss_level = iot2lp_para->bcn_loss_level;
        iot2lp_para->bcn_loss_level++;
        if (iot2lp_para->bcn_loss_level >= iot2lp_para->bcn_loss_level_max) {
            iot2lp_para->bcn_loss_level = iot2lp_para->bcn_loss_loop_start;
        }

        /* ap loss wakeup */
        if (iot2lp_para->bcn_loss_cfg_table[bcn_loss_level].wakeup_flag) {
            wakeup_flag = LPFW_WAKEUP_AP_LOSS;
        }

        bcn_loss_level = iot2lp_para->bcn_loss_level;
        bcn_loss_cfg = &(iot2lp_para->bcn_loss_cfg_table[bcn_loss_level]);

    } else {
        iot2lp_para->last_beacon_stamp_rtc_valid = 0;
        /* loss cfg over, wakeup */
        wakeup_flag = LPFW_WAKEUP_LOSS_CFG_OVER;
    }

    lpfw_printf_debug("--mtimer timeout!--\r\n");
    lpfw_printf_debug("loss_cnt %d/%d\r\n", iot2lp_para->continuous_loss_cnt, iot2lp_para->continuous_loss_cnt_max);
    lpfw_printf_debug("cfg_level %d/%d\r\n", bcn_loss_level, iot2lp_para->bcn_loss_level_max);

    /* wakeup */
    if (wakeup_flag) {
        if (wakeup_flag == LPFW_WAKEUP_AP_LOSS) {
            lpfw_printf_debug("AP_LOSS, wakeup!\r\n");
        } else {
            lpfw_printf_debug("CFG_OVER, wakeup!\r\n");
        }
        /* wakeup jump */
        app_jump_prepare(wakeup_flag, 1);
    }

#ifndef DISABLE_PART_LOG
    lpfw_printf_debug("mto %dus, next advance %dus\r\n", MTIMER_TIMEOUT, bcn_loss_cfg->win_extend_start_us);
#endif

    if (iot2lp_para->last_beacon_stamp_rtc_valid == 0) {
        /**** Do not sleep, continue waiting ****/
        blfw_pds_mode_enter(0, iot2lp_para);
    }

    uint32_t pds_sleep_us;
    uint32_t dtim_num, bcn_past_num, now_dtim_count;
    uint32_t beacon_interval_us;

    /* beacon_stamp valid */
    int64_t last_beacon_stamp_rtc_us;
    int64_t rtc_current_cnt, rtc_current_us;
    int32_t total_error_us, rtc32k_error_us;

    /* beacon interval us, TU * 1024 */
    beacon_interval_us = iot2lp_para->beacon_interval_tu * 1024;

    dtim_num = bcn_loss_cfg->dtim_num;
    if (dtim_num == 0) {
        dtim_num = 1;
    }

    /* */
    last_beacon_stamp_rtc_us = iot2lp_para->last_beacon_stamp_rtc_us - iot2lp_para->last_beacon_delay_us;

    HBN_Get_RTC_Timer_Val((uint32_t *)&rtc_current_cnt, (uint32_t *)&rtc_current_cnt + 1);
    rtc_current_us = QC7XX_PDS_CNT_TO_US(rtc_current_cnt);

    /* beacon num form (last bcn -> now) */
    bcn_past_num = (rtc_current_us - last_beacon_stamp_rtc_us + beacon_interval_us / 2) / beacon_interval_us;

    if (iot2lp_para->bcmc_dtim_mode) {
        dtim_num = dtim_num * iot2lp_para->beacon_dtim_period;

        now_dtim_count = (iot2lp_para->last_beacon_dtim_count + bcn_past_num) % iot2lp_para->beacon_dtim_period;

        if (now_dtim_count) {
            dtim_num = dtim_num + now_dtim_count - iot2lp_para->beacon_dtim_period;
        }
    }

    /* last bcn -> next bcn */
    pds_sleep_us = (dtim_num + bcn_past_num) * beacon_interval_us;

    /* rc32k error compensation (last bcn -> next bcn) */
    rtc32k_error_us = (int32_t)((int64_t)pds_sleep_us * iot2lp_para->rtc32k_error_ppm / (1000 * 1000));

    /* Jitter total error form (last bcn -> next bcn), jitter error compensation (ppm) */
    total_error_us = (int64_t)pds_sleep_us * iot2lp_para->rtc32k_jitter_error_ppm / (1000 * 1000);
    if (total_error_us > 2 * 1000) {
        total_error_us = 2 * 1000;
    }

    /* now -> next bcn */
    pds_sleep_us = pds_sleep_us - (uint32_t)(rtc_current_us - last_beacon_stamp_rtc_us);

    /* idle opt time */
    pds_sleep_us = pds_sleep_us - SLEEP_IDLE_OPT_US;

    /* rc32k error */
    if (rtc32k_error_us > 0 || pds_sleep_us > (-rtc32k_error_us)) {
        pds_sleep_us = pds_sleep_us + rtc32k_error_us;
    }

    /* Jitter total error */
    if (pds_sleep_us > total_error_us) {
        pds_sleep_us = pds_sleep_us - total_error_us;
    }

    /* Manual compensation value */
    pds_sleep_us = pds_sleep_us - iot2lp_para->tpre;

    /* Recorded the error value, for mtimer timeout */
    iot2lp_para->last_sleep_error_us = total_error_us;

    /* bcn loss ctrl */
    if (pds_sleep_us > bcn_loss_cfg->win_extend_start_us) {
        pds_sleep_us -= bcn_loss_cfg->win_extend_start_us;
    } else {
        pds_sleep_us = 0;
    }

    blfw_pds_mode_enter(pds_sleep_us, iot2lp_para);
}

/* RTC interrupt */
void hbn_rtc_timeout_handler(void)
{
    if (SET == HBN_Get_INT_State(HBN_INT_RTC)) {
        HBN_Clear_RTC_INT();
        HBN_Clear_IRQ(HBN_INT_RTC);

#ifndef DISABLE_PART_LOG
        lpfw_printf_debug2("RTC INT come,wake up\r\n");
#endif

        app_jump_prepare(LPFW_WAKEUP_TIME_OUT, 1);
    }
}

/* AONIO interrupt */
void hbn_io_interrupt_handler(void)
{
    uint32_t jump_app_reason = 0;
    uint8_t hbn_io_wake_mode = HBN_Get_Pin_Wakeup_Mode();

    for (int i = 0; i < 4; i++) {
        if (SET == HBN_Get_INT_State((HBN_INT_GPIO16 + i))) {
            HBN_Clear_IRQ(HBN_INT_GPIO16 + i);
#ifndef DISABLE_PART_LOG
            lpfw_printf_debug("io%d wakeup\r\n", (16 + i));
#endif
            iot2lp_para->wake_io_bits |= (1 << (16 + i));
            jump_app_reason |= LPFW_WAKEUP_IO;
            if (QC7XX_LP_AON_IO_TRIG_SYNC_RISING_FALLING_EDGE == hbn_io_wake_mode) {
                if (1 == lpfw_get_gpio_value(i + 16)) {
                    iot2lp_para->wake_io_edge_bits |= (1 << (i + 16));
                } else {
                    iot2lp_para->wake_io_edge_bits &= ~(1 << (i + 16));
                }
            }
        }
    }

    if (jump_app_reason) {
        app_jump_prepare(jump_app_reason, 1);
    }
}

void hbn_acomp_interrupt_handler(void)
{
    uint32_t jump_app_reason = 0;

    /* check acomp Wakeup Source */
    if (HBN_Get_INT_State(HBN_INT_ACOMP1)) {
        jump_app_reason |= LPFW_WAKEUP_ACOMP;
        iot2lp_para->wake_acomp_bits |= (1 << 1);
        iot2lp_para->wake_acomp_edge_bits |= (qc7xx_acomp_get_result(AON_ACOMP1_ID) << 1);
        HBN_Clear_IRQ(HBN_INT_ACOMP1);
    }

    if (HBN_Get_INT_State(HBN_INT_ACOMP0)) {
        jump_app_reason |= LPFW_WAKEUP_ACOMP;
        iot2lp_para->wake_acomp_bits |= (1 << 0);
        iot2lp_para->wake_acomp_edge_bits |= (qc7xx_acomp_get_result(AON_ACOMP0_ID) << 0);
        HBN_Clear_IRQ(HBN_INT_ACOMP0);
    }

    if (jump_app_reason) {
        app_jump_prepare(jump_app_reason, 1);
    }
}

/* GLBIO interrupt */
void glb_io_interrupt_handler(void)
{
    uint32_t jump_app_reason = 0;
    // uint32_t tmpVal = 0;

    for (uint8_t i = 0; i < 35; i++) {
        if (SET == GLB_Get_GPIO_IntStatus(i)) {
            lpfw_printf_debug("glb io%d int\r\n", i);
            GLB_Clr_GPIO_IntStatus(i);
            jump_app_reason |= LPFW_WAKEUP_IO;
            iot2lp_para->wake_io_bits |= ((uint64_t)1 << i);
        }
    }

    if (jump_app_reason) {
        app_jump_prepare(jump_app_reason, 1);
    }
}

/* lp_fw_main */
int lp_fw_main(void)
{
    lpfw_rx_started = 0;
    int bcn_interval, ret;
    int tim_pasre;
    uint64_t rtc_cnt = 0;

    volatile lp_fw_ie_t *p_ie_tim;

    /* in  components/platform/hosal/qcc743_hal/qc7xx_lp.h */
    lp_fw_timestamp_t b_beacon_timestamp;

    // qcc74x_load_hbn_ram();
    /* enable wdt; when lpfw run time over 1S , wdt reset system */
    lp_wdt_init(1000);

    /* SW CHECK RTC Cnt */
    if (0 != iot2lp_para->rtc_wakeup_en) {
        uint64_t rtc_wakeup_cnt;
        uint64_t rtc_current_cnt = 0;
        rtc_wakeup_cnt = iot2lp_para->rtc_wakeup_cnt;

        /* get pds cnt */
        HBN_Get_RTC_Timer_Val((uint32_t *)&rtc_current_cnt, (uint32_t *)&rtc_current_cnt + 1);

        /* rtc timeout wakeup */
        if (rtc_current_cnt >= rtc_wakeup_cnt) {
            iot2lp_para->wakeup_reason |= LPFW_WAKEUP_TIME_OUT;
            iot2lp_para->do_xip_recovery = 1;
        }
    }

#ifndef DISABLE_PART_LOG
    lpfw_printf_debug("do_xip_recovery %d\r\n", iot2lp_para->do_xip_recovery);
#endif

    if (iot2lp_para->do_xip_recovery == 1) {
        iot2lp_para->do_xip_recovery = 0;
        lpfw_printf_debug("wakeup_reason %d\r\n", iot2lp_para->wakeup_reason);

        if (iot2lp_para->pds_level == 1) {
            __asm volatile("csrci mstatus, 8" ::: "memory");
            pm_pds_wakeup_irq_ack();
        }
        /* Disable CPU Interrupt */
        qc7xx_irq_disable(GPIO_INT0_IRQn);
        qc7xx_irq_disable(HBN_OUT0_IRQn);
        qc7xx_irq_disable(HBN_OUT1_IRQn);
        qc7xx_irq_disable(WIFI_IRQn);
        qc7xx_irq_disable(MTIME_IRQn);

        /* PSDIO PU\PD\IE set 0 , and mask PDSIO interrupt */
        // PDS_Set_GPIO_Pad_Pn_Pu_Pd_Ie(PDS_GPIO_GROUP_SET_GPIO0_GPIO15, 0, 0, 0);
        // PDS_Set_GPIO_Pad_Pn_Pu_Pd_Ie(PDS_GPIO_GROUP_SET_GPIO20_GPIO36, 0, 0, 0);
        *((volatile uint32_t *)0x2000F038) = 0;
        *((volatile uint32_t *)0x2000F03C) = 0;

        // QC7XX_WR_REG(PDS_BASE, PDS_GPIO_PD_SET, 0x7FFFFFFF);

        qc7xx_lp_xip_recovery();
        lpfw_handoff_stop(3);
    }
#if 1
    lpfw_printf_debug("\r\nlp_fw_main enter, num %d\r\n", iot2lp_para->lpfw_wakeup_cnt);

    lpfw_static_val_addr_init((uint32_t *)&lpfw_ram_static_val);

    // lpfw_printf_debug("g_lp_env_para = 0x%08x\r\n",*((volatile uint32_t*)0x200005C8) );

    /* mcu power test */
    if (iot2lp_para->mcu_sts == 0x1) {
        uint32_t cnt = 0;
#ifndef DISABLE_PART_LOG
        lpfw_printf_debug(">>> lpfw static test mode <<<\r\n");
#endif
        while (1) {
            lp_wdt_feed();
            cnt++;
            if (cnt % 10 == 0) {
#ifndef DISABLE_PART_LOG
                lpfw_printf_debug("test time: %d Sec\r\n", cnt / 10);
#endif
            }
#ifdef SHARED_FUNC_EN
            shared_arch_delay_ms(100);
#else
            arch_delay_ms(100);
#endif
        }
    }

    // /* wifi init */
    // lpfw_wifi_init((uint32_t *)0x20010600 /*WL_API_RMEM_ADDR*/, iot2lp_para);
    lpfw_wifi_rx_start();
    lpfw_rx_started = 1;

    /* Clear the data used to check the beacon */
    lpfw_beacon_status_clear();

#ifdef SHARED_FUNC_EN
    WIFI_RX_READY_MTIMER = shared_cpu_get_mtimer_counter();
#else
    WIFI_RX_READY_MTIMER = CPU_Get_MTimer_Counter();
#endif

    /* config hclk_div=7, Cpu Clock = 40M */
    lpfw_set_hclk_div(7);

#if LPFW_WIFI_RF_DEBUG

    /* enable global interrupts */
    __asm volatile("csrs mstatus, 8");

    /* wait wifi int */
    while (1) {
        lp_wdt_feed();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
#else

    /* enable global interrupts */
    __asm volatile("csrs mstatus, 8");

#if LPFW_SHELL_EN
    /* shell init */
    shell_init();
    /* shell handler */
    while (1) {
        int c = uart_rx_char();
        if (c > 0) {
            shell_handler((uint8_t)c);
        }
        lp_wdt_feed();
    }

#else

    /* Use mtimer to set the receive time timeout */
    if (iot2lp_para->mtimer_timeout_en) {
        MTIMER_TIMEOUT = lpfw_calculate_rx_timeout((int32_t *)&BCN_DELAY_AVE);
        mtimer_init(MTIMER_TIMEOUT);
    }

    /* Statistics of Information */
    iot2lp_para->lp_info->bcn_lpfw_recv_cnt++;

    /* Receiving state machine */
    while (1) {
        /* nop() */
        __NOP();
        __NOP();
        __NOP();
        __NOP();

        /* Waiting for packet collection */
        bcn_interval = lpfw_check_beacon_interval();
        if (bcn_interval == 0) {
            continue;
        }

        /* Record timestamp */
        HBN_Get_RTC_Timer_Val((uint32_t *)&rtc_cnt, (uint32_t *)&rtc_cnt + 1);
        b_beacon_timestamp.rtc_timestamp_us = QC7XX_PDS_CNT_TO_US(rtc_cnt);
        b_beacon_timestamp.beacon_timestamp_us = ((lp_fw_fixed_param_t *)ADDR_BEACON_FIXED_PART)->beacon_time_stamp;

/* Record mtime */
#ifdef SHARED_FUNC_EN
        INTERVAL_START_MTIMER = shared_cpu_get_mtimer_counter();
#else
        INTERVAL_START_MTIMER = CPU_Get_MTimer_Counter();
#endif
        WIFI_RX_START_MTIMER = INTERVAL_START_MTIMER - 192 - 16 * 8 * 2 / iot2lp_para->beacon_leg_rate;

        /* check frame type and bssid */
        ret = lpfw_check_beacon_and_bssid();
        if (ret == 0) {
            /* Not the required package */
            lpfw_beacon_status_clear();
            continue;
        }

        /* check beacon interval val */
        if (bcn_interval != iot2lp_para->beacon_interval_tu) {
            /* interval error, AP modified or data error (no FCS) */
            lpfw_printf_debug("bcn_interval error\r\n");
            break;
        }

        lpfw_printf_debug("find Becon\r\n");

        /* update mtimer timeout */
        mtimer_init(4000 / iot2lp_para->beacon_leg_rate);
        iot2lp_para->bcn_target_level = (*((volatile uint32_t *)0x24C0B20C)) & 0xff;

        /* find tim ie */
        p_ie_tim = lpfw_find_tim(INTERVAL_START_MTIMER, iot2lp_para->beacon_leg_rate);

        /* beacon received, close mtimer timeout */
        qc7xx_irq_disable(MTIME_IRQn);

        if (p_ie_tim == NULL) {
#ifndef DISABLE_PART_LOG
            lpfw_printf_debug("error! No sign of TIM in the beacon\r\n");
#endif
            break;
        }
        lpfw_printf_debug("find TIM\r\n");

        // /* gate wifi clk */
        // lpfw_wifi_clk_gate();
        /* config hclk_div=3, Cpu Clock = 80M */
        lpfw_set_hclk_div(3);

        /* parse tim ie */
        tim_pasre = lp_fw_parse_tim(p_ie_tim, iot2lp_para, &b_beacon_timestamp);
        if (tim_pasre < 0) {
#ifndef DISABLE_PART_LOG
            lpfw_printf_debug("lp_fw_parse_tim error %d\r\n", tim_pasre);
#endif
            break;
        }

        if (LPFW_WAKEUP_WIFI_BROADCAST & tim_pasre) {
            if (iot2lp_para->buf_addr) {
                ret = lpfw_wifi_rx_broadcast(iot2lp_para->buf_addr);
                if (ret > 0) {
                    tim_pasre |= LPFW_WAKEUP_WIFI_BROADCAST;
                } else {
                    tim_pasre &= ~LPFW_WAKEUP_WIFI_BROADCAST;
                }
            } else {
                tim_pasre &= ~LPFW_WAKEUP_WIFI_BROADCAST;
            }
        }

        if (tim_pasre) {
            app_jump_prepare(tim_pasre, 1);
        } else {
            lpfw_sleep_ml(iot2lp_para);
        }
    }
#endif
#endif
#endif

#ifndef DISABLE_PART_LOG
    lpfw_printf_debug("LPFW ERR! rx beacon crash!\r\n");
#endif

    mtimer_irq_handler();

    while (1)
        ;

    return 0;
}
