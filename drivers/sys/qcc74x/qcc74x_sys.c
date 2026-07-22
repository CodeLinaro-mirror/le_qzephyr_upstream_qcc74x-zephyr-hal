/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_lp.h"
#include "qcc74x_glb.h"
#include "qcc74x_hbn.h"
#include "qcc74x_psram.h"
#include "qc7xx_sys.h"

#define REASON_SOFTWARE    (4)   // software        soft
#define REASON_SWWDT       (3)   // watchdog reboot wdog
#define REASON_EXCEPTION   (2)   // software        soft
#define REASON_HWWDT       (1)   // watchdog reboot wdog
#define REASON_POWEROFF    (0x0) // software        soft

#define RST_REASON         HBN_SYS_RESET_REASON     // use 4 Bytes
#define RST_REASON_CHK     HBN_SYS_RESET_REASON_CHK // use 4 Bytes
#define RST_REASON_CHK_VAL (0xBF1BA55A)

static QC7XX_RST_REASON_E s_rst_reason = QC7XX_RST_POWER_OFF;

static char *RST_REASON_ARRAY[] = {
    "QC7XX_RST_POWER_OFF",
    "QC7XX_RST_HARDWARE_WATCHDOG",
    "QC7XX_RST_FATAL_EXCEPTION",
    "QC7XX_RST_SOFTWARE_WATCHDOG",
    "QC7XX_RST_SOFTWARE",
    "QC7XX_RST_HBN_WAKEUP",
    "QC7XX_RST_BOD"
};

QC7XX_RST_REASON_E qc7xx_sys_rstinfo_get(void)
{
    return s_rst_reason;
}

char *qc7xx_sys_rstinfo_getstring(void)
{
    return RST_REASON_ARRAY[s_rst_reason];
}

int qc7xx_sys_rstinfo_set(QC7XX_RST_REASON_E val)
{
    RST_REASON = (uint32_t)val;
    RST_REASON_CHK = (uint32_t)val ^ RST_REASON_CHK_VAL;

    return 0;
}

void qc7xx_sys_rstinfo_init(void)
{
    uint32_t reason = RST_REASON;
    uint32_t reason_chk = RST_REASON_CHK;

    uint8_t reset_evt = 0;

    HBN_Get_Reset_Event(&reset_evt);

    if (0x1 & (reset_evt >> 4)) {
        s_rst_reason = QC7XX_RST_BOD;
    } else {
        if ((reason ^ RST_REASON_CHK_VAL) == reason_chk) {
            s_rst_reason = reason;
        }
    }

    HBN_Clr_Reset_Event();

    RST_REASON = REASON_HWWDT;
    RST_REASON_CHK = REASON_HWWDT ^ RST_REASON_CHK_VAL;
}

int qc7xx_sys_rstinfo_getsting(char *info)
{
    memcpy(info, (char *)RST_REASON_ARRAY[s_rst_reason], strlen(RST_REASON_ARRAY[s_rst_reason]));
    *(info + strlen(RST_REASON_ARRAY[s_rst_reason])) = '\0';
    return 0;
}

int qc7xx_sys_isxipaddr(uint32_t addr)
{
    if (((addr & 0xFF000000) == 0xA0000000) || ((addr & 0xFF000000) == 0xA1000000) ||
        ((addr & 0xFF000000) == 0xA2000000) || ((addr & 0xFF000000) == 0xA3000000) ||
        ((addr & 0xFF000000) == 0xA4000000) || ((addr & 0xFF000000) == 0xA5000000) ||
        ((addr & 0xFF000000) == 0xA6000000) || ((addr & 0xFF000000) == 0xA7000000)) {
        return 1;
    }
    return 0;
}

int qc7xx_sys_em_config(void)
{
    extern uint8_t __LD_CONFIG_EM_SEL;
    volatile uint32_t em_size;

    em_size = (uint32_t)&__LD_CONFIG_EM_SEL;

    if (em_size == 0) {
        GLB_Set_EM_Sel(GLB_WRAM160KB_EM0KB);
    } else if (em_size == 32 * 1024) {
        GLB_Set_EM_Sel(GLB_WRAM128KB_EM32KB);
    } else if (em_size == 64 * 1024) {
        GLB_Set_EM_Sel(GLB_WRAM96KB_EM64KB);
    } else {
        GLB_Set_EM_Sel(GLB_WRAM96KB_EM64KB);
    }

    return 0;
}

int qc7xx_sys_reset_por(void)
{
    HBN_Power_Off_Xtal_32K();
    HBN_32K_Sel(0); // f32k select rc32k
    qc7xx_sys_rstinfo_set(QC7XX_RST_SOFTWARE);
    __disable_irq();

    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_WIFI);
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_BTDM);
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_ZIGBEE);
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_BLE2);
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_ZIGBEE2);

    arch_delay_ms(10);

    GLB_SW_POR_Reset();
    while (1) {
        /*empty dead loop*/
    }

    return 0;
}

void qc7xx_sys_reset_system(void)
{
    qc7xx_sys_rstinfo_set(QC7XX_RST_SOFTWARE);
    __disable_irq();
    GLB_SW_System_Reset();
    while (1) {
        /*empty dead loop*/
    }
}

void qc7xx_sys_reset_system_from_interface(void)
{
    __disable_irq();
    HBN_Set_User_Boot_Config(1);
    qc7xx_sys_reset_por();
    while (1) {
        /*empty dead loop*/
    }
}

#define SYSMAP_BASE_OFFSET       (12)
#define SYSMAP_ATTR_STRONG_ORDER (1 << 4)
#define SYSMAP_ATTR_CACHE_ABLE   (1 << 3)
#define SYSMAP_ATTR_BUFFER_ABLE  (1 << 2)

void qc7xx_cpu_sysmap_init(bool dcache_preload_en, bool dcache_amr_en)
{
    extern uint8_t __LD_CONFIG_EM_SEL;
    uint32_t sysmap_base = SYSMAP_BASE;
    uint32_t em_size = (uint32_t)&__LD_CONFIG_EM_SEL;
    uint32_t wram_cacheable_end = 0x63038000 - em_size;

#ifdef CONFIG_LPAPP
    uint32_t lpfw_share_end = (uint32_t)&__lpfw_share_end__;

    /*
     * LPAPP keeps a reserved LPFW share window at the front of WRAM.
     * Repeated LPFW copy/verify after wake still uses the 0x63xxxxxx
     * cacheable alias of that window, so make sure the sysmap keeps the
     * share range visible even if regular application WRAM usage ends
     * earlier.
     */
    if (wram_cacheable_end < lpfw_share_end) {
        wram_cacheable_end = lpfw_share_end;
    }
#endif

    /* 1. 0x00000000~0x62FC0000: Strong-Order, Non-Cacheable, Non-Bufferable */
    putreg32(0x62FC0000 >> SYSMAP_BASE_OFFSET, (sysmap_base + 0x00));
    putreg32(SYSMAP_ATTR_STRONG_ORDER, (sysmap_base + 0x04));
    sysmap_base += 0x08;

    /* 2. ocram 0x62FC0000~0x63010000: Weak-Order, Cacheable, Bufferable */
    putreg32(0x63010000 >> SYSMAP_BASE_OFFSET, (sysmap_base + 0x00));
    putreg32(SYSMAP_ATTR_CACHE_ABLE | SYSMAP_ATTR_BUFFER_ABLE, (sysmap_base + 0x04));
    sysmap_base += 0x08;

    /* 3. WRAM including the reserved LPFW share window: Cacheable/Bufferable. */
    putreg32(wram_cacheable_end >> SYSMAP_BASE_OFFSET, (sysmap_base + 0x00));
    putreg32(SYSMAP_ATTR_CACHE_ABLE | SYSMAP_ATTR_BUFFER_ABLE, (sysmap_base + 0x04));
    sysmap_base += 0x08;

    /* 4. Remaining remapped RAM/empty space up to flash: Strong-Order. */
    putreg32(0xA0000000 >> SYSMAP_BASE_OFFSET, (sysmap_base + 0x00));
    putreg32(SYSMAP_ATTR_STRONG_ORDER, (sysmap_base + 0x04));
    sysmap_base += 0x08;

    /* 5. flash(32M) 0xA0000000~0xA2000000: Weak-Order, Cacheable, Non-Bufferable */
    putreg32(0xA2000000 >> SYSMAP_BASE_OFFSET, (sysmap_base + 0x00));
    putreg32(SYSMAP_ATTR_CACHE_ABLE, (sysmap_base + 0x04));
    sysmap_base += 0x08;

    /* 6. empty 0xA2000000~0xA8000000: Strong-Order, Non-Cacheable, Non-Bufferable */
    putreg32(0xA8000000 >> SYSMAP_BASE_OFFSET, (sysmap_base + 0x00));
    putreg32(SYSMAP_ATTR_STRONG_ORDER, (sysmap_base + 0x04));
    sysmap_base += 0x08;

    /* 7. psram(4M) 0xA8000000~0xA8400000: Weak-Order, Cacheable, Bufferable */
    if (QCC74X_PSRAM_INIT_DONE) {
        putreg32(0xA8400000 >> SYSMAP_BASE_OFFSET, (sysmap_base + 0x00));
        putreg32(SYSMAP_ATTR_CACHE_ABLE | SYSMAP_ATTR_BUFFER_ABLE, (sysmap_base + 0x04));
        sysmap_base += 0x08;
    }

    /* 8. others: Strong-Order, Non-Cacheable, Non-Bufferable */
    putreg32(0xFFFFF000 >> SYSMAP_BASE_OFFSET, (sysmap_base + 0x00));
    putreg32(SYSMAP_ATTR_STRONG_ORDER, (sysmap_base + 0x04));
    sysmap_base += 0x08;

    uint32_t mhint = __get_MHINT();

    if (dcache_preload_en) {
        /* enable Dcache preload. */
        mhint |= (1 << 2);
        /* set D-cache preload size: 2-line */
        mhint &= ~(3 << 13);
        mhint |= (0 << 13);
    } else {
        mhint &= ~(1 << 2);
    }

    if (dcache_amr_en) {
        /* set AMR(Adaptive Miss Handling for Writes) size: 3-line */
        mhint |= (1 << 3);
    } else {
        mhint &= ~(3 << 3);
    }
    __set_MHINT(mhint);
}
