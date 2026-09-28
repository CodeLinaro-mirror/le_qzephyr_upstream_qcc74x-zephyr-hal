/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qcc74x_pm.h"
// #include "hal_clock.h"
#include "qc7xx_rtc.h"
#include "qcc74x_glb.h"
// #include "qcc74x_adc.h"
#include "qcc74x_hbn.h"
#include "qcc74x_pds.h"
#include "qcc74x_lp.h"

#ifndef PM_PDS_LDO_LEVEL_DEFAULT
#define PM_PDS_LDO_LEVEL_DEFAULT HBN_LDO_LEVEL_1P10V
#endif
static PDS_DEFAULT_LV_CFG_Type ATTR_TCM_CONST_SECTION pdsCfgLevel15 = {
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
        .waitXtalRdy = 1,
        .pdsPwrOff = 1,
        .xtalOff = 1,
        .socEnbForceOn = 1,
        .pdsRstSocEn = 1,
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
        .cpuIsoEn = 1,
        .wbIsoEn = 1,
        .usbIsoEn = 1,
        .miscIsoEn = 1,
    },
    .pdsCtl4 = {
        .cpuPwrOff = 1,
        .cpuRst = 1,
        .cpuMemStby = 0,
        .cpuGateClk = 0,
        .wbPwrOff = 1,
        .wbRst = 1,
        .wbMemStby = 0,
        .wbGateClk = 0,
        .usbPwrOff = 1,
        .usbRst = 1,
        .usbMemStby = 0,
        .usbGateClk = 0,
        .miscPwrOff = 1,
        .miscRst = 1,
        .miscMemStby = 0,
        .miscGateClk = 0,
    },
    .pdsCtl5 = {
        .cpuWfiMask = 0,
        .pdsPadOdEn = 0,
        .ctrlUsb33 = 1,
        .ldo18ioOff = 1,
        .pdsGpioKeep = 0,
    }
};
extern QC7XX_Err_Type AON_Set_LDO11_SOC_Sstart_Delay(uint8_t delay);
#if 1
static void pm_pds_intc_clr(void)
{
    // pm_clr_pds_gpio_int();
    PDS_IntClear();
}
void ATTR_TCM_SECTION lpfw_pds15_mode_enter(uint32_t sleep_time)
{
    PDS_DEFAULT_LV_CFG_Type *pPdsCfg = NULL;
    uint32_t tmpVal;

    if(( sleep_time!=0 ) && ( sleep_time <= PDS_WARMUP_LATENCY_CNT )) {
        return;
    }

    /* MCU CLK SELECT RC32M */
    GLB_Set_MCU_System_CLK(GLB_MCU_SYS_CLK_RC32M);
    /* Power Off WifiPLL */
    // GLB_Power_Off_WIFIPLL();

    /* To make it simple and safe*/
    // cpu_global_irq_disable();
    __ASM volatile("csrc mstatus, 8");

    /* lpfw_bin not change ldo's value */
    // HBN_Set_Ldo11_Rt_Vout(PM_PDS_LDO_LEVEL_DEFAULT);

    /* if HBN irq isn't 0 and not clear HBN irq, will wakeup at once */
    /* clear HBN irq*/
    // QC7XX_WR_REG(HBN_BASE, HBN_IRQ_CLR, 0xffffffff);
    // QC7XX_WR_REG(HBN_BASE, HBN_IRQ_CLR, 0);

    tmpVal = QC7XX_RD_REG(PDS_BASE, PDS_INT);

    if (sleep_time)
        tmpVal |= (1 << 10); // unmask pds sleep time wakeup
    QC7XX_WR_REG(PDS_BASE, PDS_INT, tmpVal);

    /* clear all interrupt status */
    pm_pds_intc_clr();


    /* enable PDS interrupt to wakeup CPU (PDS1:CPU not powerdown, CPU __WFI)*/
    /* if enter pds1, please User call pm_pds_irq_register function before pm_pds15_mode_enter */
    // pm_pds_irq_register();

    /***********************************************************/

    pPdsCfg = &pdsCfgLevel15;

    PDS_Pu_PLL_Enable();

    if (0 != QC7XX_RD_REG(PDS_BASE, PDS_GPIO_I_SET)) {
        pPdsCfg->pdsCtl.ctrlGpioIePuPd = 1;
    }

    /* config  ldo11soc_sstart_delay_aon =2 , cr_pds_pd_ldo11=0 to speedup
   * ldo11soc_rdy_aon */
    #ifdef SHARED_FUNC_EN
    shared_aon_set_ldo11_soc_sstart_delay(0x2);
    #else
    AON_Set_LDO11_SOC_Sstart_Delay(0x2);
    #endif

    /* config ram */
    tmpVal = QC7XX_RD_REG(PDS_BASE, PDS_RAM1);
    tmpVal |= (1<<31);
    QC7XX_WR_REG(PDS_BASE, PDS_RAM1, tmpVal);
    QC7XX_WR_REG(PDS_BASE, PDS_RAM2, (0x3ff<<10));
    QC7XX_WR_REG(PDS_BASE, PDS_RAM3, 0x000fffff );
    QC7XX_WR_REG(PDS_BASE, PDS_RAM4, 0);
    // uint32_t pds_ram[4];
    // pds_ram[0]=QC7XX_RD_REG(PDS_BASE, PDS_RAM1);
    // pds_ram[0] |= (1<<31);

    // pds_ram[1]=(0x3ff<<10);
    // pds_ram[2]= 0x000fffff;
    // pds_ram[3]= 0;

    // PDS_RAM_Config(
    //     (PDS_CTRL_RAM1_Type *)pds_ram, (PDS_CTRL_RAM2_Type *)(pds_ram+1),
    //     (PDS_CTRL_RAM3_Type *)(pds_ram+2), (PDS_CTRL_RAM4_Type *)(pds_ram+3));


    #ifdef SHARED_FUNC_EN
    shared_pds_default_level_config((uint32_t*)pPdsCfg, sleep_time);
    #else
    PDS_Default_Level_Config(pPdsCfg, sleep_time);
    #endif


    __WFI();

    /* enable global interrupts */
    __asm volatile("csrs mstatus, 8");
    /*************************************************************************/
}
#endif