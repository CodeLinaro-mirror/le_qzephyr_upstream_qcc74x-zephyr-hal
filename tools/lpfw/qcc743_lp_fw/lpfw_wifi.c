/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hbn_reg.h"
#include "pds_reg.h"
#include "glb_reg.h"
#include "qcc74x_glb.h"
#include "qcc74x_aon.h"

// #include "export/rwnx.h"
// #include "plf/refip/src/driver/rwnx_platform.h"
// #include "ip/lmac/src/rx/rxl/rxl_hwdesc.h"
// STA Info table
// #include "ip/lmac/src/mm/mm.h"

//#include "rwnx_platform.h"

#include "qcc74x_lp.h"

// #include "lpfw_printf.h"
#define ATTR_ROM_WIFI_SECTION __attribute__((section(".romwifi_code." ATTR_UNI_SYMBOL)))

#define WL_API_RMEM_ADDR      0x20010600

#define BEACON_SIZE_TH        (0x400 - 0xA0)

#define BEACON_SUBTYPE        0x80
#define SIZE_BEACON_SUBTYPE   1

#define TIM_IE_ID             5

/*  */
#define LP_ABS(a)       (((a) > 0)? (a) : (-(a)))
#define LP_DIFF(a, b)   (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

typedef struct {
    volatile uint8_t ie_number;
    volatile uint8_t ie_length;
    volatile uint8_t ie_data[0];
} lp_fw_ie_t;


ATTR_ROM_WIFI_SECTION void lpfw_static_val_addr_init(uint32_t* addr)
{
    ((iot2lp_para_t *)IOT2LP_PARA_ADDR)->lpfw_static_val = (lp_fw_static_variable_t*) addr;
}

ATTR_ROM_WIFI_SECTION void lpfw_wifi_addr_init(   uint32_t buff_start_addr,
                            uint32_t beacon_start_addr,
                            uint32_t beacon_bssid_addr,
                            uint32_t beacon_fixed_part_addr,
                            uint32_t beacon_interval_part_addr,
                            uint32_t beacon_variable_part_addr)
{
    RAM_WIFI_START_ADD = buff_start_addr;
    ADDR_BEACON_START = beacon_start_addr;
    ADDR_BEACON_BSSID = beacon_bssid_addr;
    ADDR_BEACON_FIXED_PART = beacon_fixed_part_addr;
    ADDR_BEACON_INTERVAL_PART = beacon_interval_part_addr;
    ADDR_BEACON_VARIABLE_PART = beacon_variable_part_addr;
}

ATTR_ROM_WIFI_SECTION void lpfw_beacon_status_clear(void)
{
    /* Avoid simultaneous operation with MAC DMA */

    /* clear frame type */
    *((volatile uint32_t *)ADDR_BEACON_SUBTYPE) = 0;

    /* clear BSSID */
    *((volatile uint32_t *)ADDR_BEACON_BSSID) = 0;
    *((volatile uint32_t *)ADDR_BEACON_BSSID + 1) = 0;

    /* clear beacon interval */
    *(volatile uint16_t *)ADDR_BEACON_INTERVAL_PART = 0;
    __DSB();
}

ATTR_ROM_WIFI_SECTION int lpfw_check_beacon_interval(void)
{
    int bcn = *(volatile uint16_t *)ADDR_BEACON_INTERVAL_PART;

    /* if g_beacon_interval not equl 0, p_time_stamp is exactly ready */

    return bcn;
}

ATTR_ROM_WIFI_SECTION int lpfw_check_beacon_and_bssid(void)
{
    /* check beacon */
    if (*((volatile uint8_t *)ADDR_BEACON_SUBTYPE) != BEACON_SUBTYPE) {
        return 0;
    }

    /* check bssid */
    for (uint8_t i = 0; i < 6; i++) {
        if (iot2lp_para->bssid[i] != *(volatile uint8_t *)(ADDR_BEACON_BSSID + i)) {
            return 0;
        }
    }

    return 1;
}

ATTR_ROM_WIFI_SECTION volatile lp_fw_ie_t *lpfw_find_tim(uint32_t interval_start_mtimer, uint8_t beacon_rate_code)
{
    volatile lp_fw_ie_t *p_ie = (volatile lp_fw_ie_t *)ADDR_BEACON_VARIABLE_PART;
    uint32_t mtimer_now;
    uint32_t wait_dtim_us=0;

    while ((uint32_t)p_ie - ADDR_BEACON_START < BEACON_SIZE_TH) {
        /* nop() */
        __NOP();
        __NOP();
        __NOP();
        __NOP();

        /* Use time to get the amount of data that has been received */
        #ifdef SHARED_FUNC_EN
        mtimer_now = shared_cpu_get_mtimer_counter();
        #else
        mtimer_now = CPU_Get_MTimer_Counter();
        #endif
        if (mtimer_now - interval_start_mtimer < ((uint32_t)p_ie - ADDR_BEACON_FIXED_PART + 4) * 8 * 2 /  beacon_rate_code) {
            /* The data has not been refreshed before the time is up */
            continue;
        }

        if (p_ie->ie_length == 0) {
            if (p_ie->ie_number == 0 && (mtimer_now - interval_start_mtimer > ((uint32_t)p_ie - ADDR_BEACON_FIXED_PART + 16) * 8 * 2 / beacon_rate_code)) {
                /* When the BSSID is not broadcasted, the length of the BSSID can be considered as 0 */
            } else {
                continue;
            }
        }

        /* Move to the next ie */
        if (p_ie->ie_number != TIM_IE_ID) {
            p_ie = (void *)p_ie + p_ie->ie_length + 2;
            continue;
        }

        wait_dtim_us = (p_ie->ie_length * 8 * 2 /  beacon_rate_code);

        if(wait_dtim_us){
            /** for example, ie_length = 4
            * 1M    arch_delay_us(32) : 40us
            * 2M    arch_delay_us(16) : 24us
            * 5.5M  arch_delay_us(5) :  13us
            * 11M   arch_delay_us(3) :  10us
            */
            #ifdef SHARED_FUNC_EN
            shared_arch_delay_us(wait_dtim_us);
            #else
            arch_delay_us(wait_dtim_us);
            #endif
        } else {
            /* delay 1 us*/
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        }

        /* return offset */
        return p_ie;
    }

    /* error */
    return NULL;
}

/* wifi gate */
ATTR_ROM_WIFI_SECTION void lpfw_wifi_clk_gate(void)
{
    uint32_t tmpVal = 0;
    tmpVal = QC7XX_RD_WORD(GLB_BASE + 0x830);
    tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_WIFIPLL_EN_DIV3);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_WIFIPLL_EN_DIV4);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_WIFIPLL_EN_DIV5);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_WIFIPLL_EN_DIV6);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_WIFIPLL_EN_DIV8);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_WIFIPLL_EN_DIV10);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_WIFIPLL_EN_DIV12);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_WIFIPLL_EN_DIV20);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_WIFIPLL_EN_DIV30);
    QC7XX_WR_WORD(GLB_BASE + 0x830, tmpVal);
}

ATTR_ROM_WIFI_SECTION int lpfw_bcn_timestamp_check(uint64_t beacon_timestamp_now_us, uint64_t rtc_timestamp_now_us)
{
    uint64_t rtc_timestamp_last_us, beacon_timestamp_last_us;
    int64_t rtc_us, beacon_us;

    /* get last timestamp */
    rtc_timestamp_last_us = iot2lp_para->last_beacon_stamp_rtc_us;
    beacon_timestamp_last_us = iot2lp_para->last_beacon_stamp_beacon_us;

    /*  */
    if(iot2lp_para->last_beacon_stamp_rtc_valid == 0){
        return 1;
    }

    if (beacon_timestamp_now_us < beacon_timestamp_last_us) {
        /* The timestamp is abnormal. Abort */
        return -1;
    }

    /* Obtaining Duration */
    rtc_us = rtc_timestamp_now_us - rtc_timestamp_last_us;
    beacon_us = beacon_timestamp_now_us - beacon_timestamp_last_us;

    if ( beacon_us > 3 * 60 * 1000 * 1000 || rtc_us > 3 * 60 * 1000 * 1000) {
        /* The time span is too large. Abort */
        return -2;
    }

    return 0;
}

/* Calibrate RC32K and update the beacon timestamp */
ATTR_ROM_WIFI_SECTION int lpfw_recal_rc32k(uint64_t beacon_timestamp_now_us, uint64_t rtc_timestamp_now_us, uint32_t mode, int clock_ready_check)
{
    uint64_t rtc_timestamp_last_us, beacon_timestamp_last_us;
    int64_t rtc_us, beacon_us;
    int diff_us, diff_ppm;
    int ret = 0;

    /* get last timestamp */
    rtc_timestamp_last_us = iot2lp_para->last_rc32trim_stamp_rtc_us;
    beacon_timestamp_last_us = iot2lp_para->last_rc32trim_stamp_beacon_us;

    if(clock_ready_check && qc7xx_lp_get_32k_clock_ready() == 0) {
        /* wait 32k_clock ready */
        ret = -2;
        iot2lp_para->last_rc32trim_stamp_valid = 0;
        qc7xx_lp_set_32k_trim_ready(0);
        return ret;
    }

    if (iot2lp_para->last_rc32trim_stamp_valid == 0) {
        ret = -3;
        /* update timestamp */
        goto update_tsf;
    }

    if (iot2lp_para->rc32k_auto_cal_en == 0) {
        ret = -4;
        /* update timestamp */
        goto update_tsf;
    }

    if (rtc_timestamp_now_us < rtc_timestamp_last_us || beacon_timestamp_now_us < beacon_timestamp_last_us) {
        /* The timestamp is abnormal. Abort calibration */
        ret = -5;
        /* update timestamp */
        if(mode == BEACON_STAMP_APP){
            goto update_tsf;
        }else{
            return ret;
        }
    }

    /* Obtaining Duration */
    rtc_us = rtc_timestamp_now_us - rtc_timestamp_last_us;
    beacon_us = beacon_timestamp_now_us - beacon_timestamp_last_us;

    if (rtc_us > 5 * 1000 * 1000 || beacon_us > 5 * 1000 * 1000) {
        /* The time span is too large. Abort calibration */
        ret = 6;
        /* update timestamp */
        goto update_tsf;
    }

    if (rtc_us < 250 * 1000 || beacon_us < 250 * 1000) {
        /* Time is too short, accuracy is too low, abandon calibration */
        ret = 1;
        /* not update timestamp !!! */
        return ret;
    }

    /* Time difference */
    diff_us = rtc_us - beacon_us;
    diff_ppm = (int64_t)diff_us * 1000 * 1000 / beacon_us;

    if (LP_ABS(diff_ppm) > 10 * 1000) {
        /* The error is too large. Abort calibration */
        ret = -7;
        if(mode == BEACON_STAMP_APP){
            goto update_tsf;
        }else{
            return ret;
        }
    }

    if(clock_ready_check && qc7xx_lp_get_32k_trim_ready() == 0){
        /*  */
        ret = 2;
        if(LP_DIFF(iot2lp_para->rtc32k_error_ppm, diff_ppm) < 1500){
            /* error ppm diff < 1500, trim ready, (allow dtim sleep) */
            qc7xx_lp_set_32k_trim_ready(1);
            ret = 3;
        }
    }

    if (QC7XX_GET_REG_BITS_VAL(QC7XX_RD_REG(HBN_BASE, HBN_GLB), HBN_F32K_SEL)) {
        /* rtc use xtal32k, only soft recal */
        iot2lp_para->rtc32k_error_ppm = (diff_ppm * 1 + iot2lp_para->rtc32k_error_ppm * 7) / 8;

        /* update timestamp */
        goto update_tsf;

    } else {
        /* rtc32k error (ppm) */
        iot2lp_para->rtc32k_error_ppm = (diff_ppm * 2 + iot2lp_para->rtc32k_error_ppm * 6) / 8;

        if(iot2lp_para->rtc32k_error_ppm > 3000 || iot2lp_para->rtc32k_error_ppm < -3000){

            /* get rc32k recal code */
            uint32_t rc32k_reg = *((volatile uint32_t *)0x2000F200);
            uint32_t rc32k_code = rc32k_reg >> 22;

            if(iot2lp_para->rtc32k_error_ppm > 3000) {
                rc32k_code += 1;
                iot2lp_para->rtc32k_error_ppm -= 1800;

            }else if (iot2lp_para->rtc32k_error_ppm < -3000) {
                rc32k_code -= 1;
                iot2lp_para->rtc32k_error_ppm += 1800;
            }

            /* set rc32k code */
            rc32k_reg &= ~0xffc00000;
            rc32k_reg |= rc32k_code << 22;
            *((volatile uint32_t *)0x2000F200) = rc32k_reg;

            /* save the code */
            iot2lp_para->rc32k_fr_ext = rc32k_code;
        }

        /* update timestamp */
        goto update_tsf;
    }

update_tsf:
    /* update timestamp */
    iot2lp_para->last_rc32trim_stamp_rtc_us = rtc_timestamp_now_us;
    iot2lp_para->last_rc32trim_stamp_beacon_us = beacon_timestamp_now_us;
    iot2lp_para->last_rc32trim_stamp_valid = mode;
    return ret;
}
