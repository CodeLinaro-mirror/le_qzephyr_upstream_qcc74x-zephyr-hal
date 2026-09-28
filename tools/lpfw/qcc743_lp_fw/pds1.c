/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qcc74x_pm.h"
#include "qcc74x_glb.h"
#include "qcc74x_hbn.h"
#include "qcc74x_pds.h"
#include "qc7xx_irq.h"
#include "lp_wdt.h"
#include "qcc74x_lp.h"

/* SDK default level-1 table, unchanged. Retention is applied at runtime below. */
static PDS_DEFAULT_LV_CFG_Type ATTR_TCM_CONST_SECTION pdsCfgLevel1 = {
    .pdsCtl = {
        .pdsStart = 1,
        .sleepForever = 0,
        .xtalForceOff = 0,
        .saveWiFiState = 0,
        .ldo11Off = 1,
        .bgSysOff = 1,
        .ctrlGpioIePuPd = 0,
        .dcdc18Off = 1,
        .clkOff = 1,
        .memStby = 1,
        .glbRstProtect = 0,
        .isolation = 1,
        .waitXtalRdy = 0,
        .pdsPwrOff = 1,
        .xtalOff = 1,
        .socEnbForceOn = 0,
        .pdsRstSocEn = 0,
        .pdsRC32mOn = 0,
        .pdsDcdc11VselEn = 0,
        .usbpllOff = 1,
        .aupllOff = 1,
        .wifipllOff = 1,
        .pdsDcdc11Vsel = 0x8,
        .pdsCtlRfSel = 3,
        .pdsUseTbttSlp = 0,
        .pdsGpioIsoMod = 0,
    },
    .pdsCtl2 = {
        .forceCpuPwrOff = 0,
        .forceWbPwrOff = 0,
        .forceUsbPwrOff = 0,
        .forceCpuIso = 0,
        .forceWbIso = 0,
        .forceUsbIso = 0,
        .forceCpuPdsRst = 0,
        .forceWbPdsRst = 0,
        .forceUsbPdsRst = 0,
        .forceCpuMemStby = 0,
        .forceWbMemStby = 0,
        .forceUsbMemStby = 0,
        .forceCpuGateClk = 0,
        .forceWbGateClk = 0,
        .forceUsbGateClk = 0,
    },
    .pdsCtl3 = {
        .forceMiscPwrOff = 0,
        .forceMiscIsoEn = 0,
        .forceMiscPdsRst = 0,
        .forceMiscMemStby = 0,
        .forceMiscGateClk = 0,
        .cpuIsoEn = 0,
        .wbIsoEn = 1,
        .usbIsoEn = 0,
        .miscIsoEn = 0,
    },
    .pdsCtl4 = {
        .cpuPwrOff = 0,
        .cpuRst = 0,
        .cpuMemStby = 0,
        .cpuGateClk = 0,
        .wbPwrOff = 1,
        .wbRst = 1,
        .wbMemStby = 0,
        .wbGateClk = 0,
        .usbPwrOff = 0,
        .usbRst = 0,
        .usbMemStby = 0,
        .usbGateClk = 0,
        .miscPwrOff = 0,
        .miscRst = 0,
        .miscMemStby = 0,
        .miscGateClk = 0,
    },
    .pdsCtl5 = {
        .cpuWfiMask = 0,
        .pdsPadOdEn = 0,
        .ctrlUsb33 = 0,
        .ldo18ioOff = 1,
        .pdsGpioKeep = 0,
    }
};

extern QC7XX_Err_Type AON_Set_LDO11_SOC_Sstart_Delay(uint8_t delay);

/* Effective LPFW SDK sequence: FLASH/PLL_POWER_OFF=0, RF_POWER_OFF=1,
 * GPIO_KEEP_EN/LDO18IO_POWER_DOWN=0. Do not inherit APP compile-time policy.
 * The caller has already switched the MCU clock to RC32M. */
static void ATTR_TCM_SECTION lpfw_pds1_sleep(uint32_t sleep_time)
{
    PDS_DEFAULT_LV_CFG_Type *pPdsCfg = &pdsCfgLevel1;
    uint32_t tmpVal;
    uint32_t pds_ram[4];
    PDS_RAM_CFG_Type pds_ram_ctrl = {
        (PDS_CTRL_RAM1_Type *)&pds_ram[0], (PDS_CTRL_RAM2_Type *)&pds_ram[1],
        (PDS_CTRL_RAM3_Type *)&pds_ram[2], (PDS_CTRL_RAM4_Type *)&pds_ram[3]
    };
    uint8_t xtal_type = 0;

    if ((sleep_time != 0) && (sleep_time <= PDS_WARMUP_LATENCY_CNT)) {
        return;
    }
    uintptr_t irq_flag = qc7xx_irq_save();

    tmpVal = QC7XX_RD_REG(AON_BASE, AON_GPADC_REG_CMD);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, AON_GPADC_GLOBAL_EN);
    QC7XX_WR_REG(AON_BASE, AON_GPADC_REG_CMD, tmpVal);
    HBN_Get_Xtal_Type(&xtal_type);

    tmpVal = QC7XX_RD_REG(PDS_BASE, PDS_INT);
    if (sleep_time) {
        tmpVal |= (1 << 10);
    }
    QC7XX_WR_REG(PDS_BASE, PDS_INT, tmpVal);

    /* Same GPIO clear-latch release and PDS status clear as pm_pds_intc_clr. */
    tmpVal = QC7XX_RD_REG(PDS_BASE, PDS_GPIO_INT);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, PDS_GPIO_SET1_INT_CLR);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, PDS_GPIO_SET2_INT_CLR);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, PDS_GPIO_SET3_INT_CLR);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, PDS_GPIO_SET4_INT_CLR);
    QC7XX_WR_REG(PDS_BASE, PDS_GPIO_INT, tmpVal);
    PDS_IntClear();

    /* This entry is selected only for runtime PDS1. Match APP retention. */
    pPdsCfg->pdsCtl.bgSysOff = 0;
    pPdsCfg->pdsCtl5.ldo18ioOff = 0;

    if (0 != QC7XX_RD_REG(PDS_BASE, PDS_GPIO_I_SET)) {
        pPdsCfg->pdsCtl.ctrlGpioIePuPd = 1;
    }
    tmpVal = QC7XX_RD_REG(PDS_BASE, PDS_INT);
    if (QC7XX_IS_REG_BIT_SET(tmpVal, PDS_WAKEUP_BY_IRRX_EN)) {
        GLB_Set_MCU_System_CLK(GLB_MCU_SYS_CLK_XTAL);
        pPdsCfg->pdsCtl.xtalOff = 0;
    }
    AON_Set_LDO11_SOC_Sstart_Delay(0x2);

    /* Preserve existing RAM control fields; change only the SDK PDS1 fields.
     * Like the SDK, these registers are NOT restored after WFI: no PDS15
     * WRAM/OCRAM retention bits are injected into this CPU-retained path. */
    pds_ram[0] = QC7XX_RD_REG(PDS_BASE, PDS_RAM1);
    pds_ram[1] = QC7XX_RD_REG(PDS_BASE, PDS_RAM2);
    pds_ram[2] = QC7XX_RD_REG(PDS_BASE, PDS_RAM3);
    pds_ram[3] = QC7XX_RD_REG(PDS_BASE, PDS_RAM4);
    pds_ram_ctrl.pds_ram1->ctlRamClk = 1;
    pds_ram_ctrl.pds_ram2->wramSlp &= ~0x3ff;
    pds_ram_ctrl.pds_ram4->cr_ocram_slp &= ~0xfffff;
    PDS_RAM_Config(pds_ram_ctrl.pds_ram1, pds_ram_ctrl.pds_ram2,
                   pds_ram_ctrl.pds_ram3, pds_ram_ctrl.pds_ram4);
    PDS_Default_Level_Config(pPdsCfg, sleep_time);
    __WFI();
    qc7xx_irq_restore(irq_flag);
}

/* Thread context only: preserve the existing IRQ setup and CPU-reset bridge. */
void ATTR_TCM_SECTION lpfw_pds1_mode_enter(uint32_t sleep_time)
{
    /* Reject invalid finite sleeps without IRQ changes or a false reset wake.
     * Zero retains the SDK's indefinite-sleep semantics. */
    if ((sleep_time != 0) && (sleep_time <= PDS_WARMUP_LATENCY_CNT)) {
        return;
    }
    /* This valid entry never returns: keep MIE masked through WFI and reset.
     * The inner SDK-equivalent save/restore therefore restores masked MIE. */
    lp_wdt_disable();
    (void)qc7xx_irq_save();
    qc7xx_l1c_dcache_clean_all();
    /* Quiesce only: the effective PDS status clear is lpfw_pds1_sleep's, which
     * runs after the GPIO wake latch has been released. */
    pm_pds_wakeup_irq_quiesce();
    PDS_IntMask(PDS_INT_WAKEUP, UNMASK);
    qc7xx_irq_enable(PDS_WAKEUP_IRQn);
    lpfw_pds1_sleep(sleep_time);
    int reset_rc = pm_reset_cpu_to_lpfw();
    if (reset_rc != 0) {
        iot2lp_para->handoff_failure = (uint32_t)-reset_rc;
        /* The reset never took effect, so pm_reset_cpu_to_lpfw() did not get to
         * quiesce: do it here, or the WFI trap below spins on an armed source. */
        pm_pds_wakeup_irq_quiesce();
    }
    /* No fallback to PDS15 if reset fails. */
    __asm volatile("csrc mstatus, 8" ::: "memory");
    while (1) {
        __WFI();
    }
}
