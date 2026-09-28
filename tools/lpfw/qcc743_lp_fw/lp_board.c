/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "qc7xx_irq.h"
#include "lp_uart.h"
#include "qcc74x_common.h"
#include "qcc74x_aon.h"

#include "hbn_reg.h"
#include "pds_reg.h"
#include "glb_reg.h"
#include "qcc74x_pm.h"
#include "lpfw_printf.h"
#include "qc7xx_acomp.h"
#include "qcc74x_glb_gpio.h"
#include "qcc74x_lp.h"
#include "csi_core.h"

#define ATTR_ROM_WIFI_SECTION //__attribute__((section(".romwifi_code." ATTR_UNI_SYMBOL)))

void trap_c(uintptr_t *regs)
{
    puts("trap_c enter\r\n");
    while (1)
        ;
}
#if 0
typedef void (*pFunc)(void);
extern void Default_IRQHandler(void);
extern void Default_Handler(void);
extern void Mtspend_Handler();
extern void Mtimer_IRQHandler();
const pFunc __Vectors[] __attribute__((section(".init"), aligned(64))) = {
    Default_Handler, /*         */
    Default_Handler, /*         */
    Default_Handler, /*         */
#if defined(__OS_FREERTOS__)
    Mtspend_Handler, /*  3: M-mode Soft IRQ   */
#else
    Default_IRQHandler, /*  3: M-mode Soft IRQ   */
#endif
    Default_Handler,   /*         */
    Default_Handler,   /*         */
    Default_Handler,   /*         */
    Mtimer_IRQHandler, /*  7: M-mode Timer IRQ  */
    Default_Handler,   /*         */
    Default_Handler,   /*         */
    Default_Handler,   /*         */
    Default_Handler,   /*         */
    Default_Handler,   /*         */
    Default_Handler,   /*         */
    Default_Handler,   /*         */
    Default_Handler,   /*         */

    Default_IRQHandler, //BMX_ERR_IRQHandler_Wrapper,              /* 16 +  0 */
    Default_IRQHandler, //BMX_TO_IRQHandler_Wrapper,               /* 16 +  1 */
    Default_IRQHandler, //L1C_BMX_ERR_IRQHandler_Wrapper,          /* 16 +  2 */
    Default_IRQHandler, //L1C_BMX_TO_IRQHandler_Wrapper,           /* 16 +  3 */
    Default_IRQHandler, //SEC_BMX_ERR_IRQHandler_Wrapper,          /* 16 +  4 */
    Default_IRQHandler, //RF_TOP_INT0_IRQHandler_Wrapper,          /* 16 +  5 */
    Default_IRQHandler, //RF_TOP_INT1_IRQHandler_Wrapper,          /* 16 +  6 */
    Default_IRQHandler, //SDIO_IRQHandler_Wrapper,                 /* 16 +  7 */
    Default_IRQHandler, //DMA_BMX_ERR_IRQHandler_Wrapper,          /* 16 +  8 */
    Default_IRQHandler, //SEC_GMAC_IRQHandler_Wrapper,             /* 16 +  9 */
    Default_IRQHandler, //SEC_CDET_IRQHandler_Wrapper,             /* 16 + 10 */
    Default_IRQHandler, //SEC_PKA_IRQHandler_Wrapper,              /* 16 + 11 */
    Default_IRQHandler, //SEC_TRNG_IRQHandler_Wrapper,             /* 16 + 12 */
    Default_IRQHandler, //SEC_AES_IRQHandler_Wrapper,              /* 16 + 13 */
    Default_IRQHandler, //SEC_SHA_IRQHandler_Wrapper,              /* 16 + 14 */
    Default_IRQHandler, //DMA_ALL_IRQHandler_Wrapper,              /* 16 + 15 */
    Default_IRQHandler, //0,                                       /* 16 + 16 */
    Default_IRQHandler, //0,                                       /* 16 + 17 */
    Default_IRQHandler, //0,                                       /* 16 + 18 */
    Default_IRQHandler, //IRTX_IRQHandler_Wrapper,                 /* 16 + 19 */
    Default_IRQHandler, //IRRX_IRQHandler_Wrapper,                 /* 16 + 20 */
    Default_IRQHandler, //0,                                       /* 16 + 21 */
    Default_IRQHandler, //0,                                       /* 16 + 22 */
    Default_IRQHandler, //SF_CTRL_IRQHandler_Wrapper,              /* 16 + 23 */
    Default_IRQHandler, //0,                                       /* 16 + 24 */
    Default_IRQHandler, //GPADC_DMA_IRQHandler_Wrapper,            /* 16 + 25 */
    Default_IRQHandler, //EFUSE_IRQHandler_Wrapper,                /* 16 + 26 */
    Default_IRQHandler, //SPI_IRQHandler_Wrapper,                  /* 16 + 27 */
    Default_IRQHandler, //0,                                       /* 16 + 28 */
    Default_IRQHandler, //UART0_IRQHandler_Wrapper,                /* 16 + 29 */
    Default_IRQHandler, //UART1_IRQHandler_Wrapper,                /* 16 + 30 */
    Default_IRQHandler, //0,                                       /* 16 + 31 */
    Default_IRQHandler, //I2C_IRQHandler_Wrapper,                  /* 16 + 32 */
    Default_IRQHandler, //0,                                       /* 16 + 33 */
    Default_IRQHandler, //PWM_IRQHandler_Wrapper,                  /* 16 + 34 */
    Default_IRQHandler, //0,                                       /* 16 + 35 */
    Default_IRQHandler, //TIMER_CH0_IRQHandler_Wrapper,            /* 16 + 36 */
    Default_IRQHandler, //TIMER_CH1_IRQHandler_Wrapper,            /* 16 + 37 */
    Default_IRQHandler, //TIMER_WDT_IRQHandler_Wrapper,            /* 16 + 38 */
    Default_IRQHandler, //0,                                       /* 16 + 39 */
    Default_IRQHandler, //0,                                       /* 16 + 40 */
    Default_IRQHandler, //0,                                       /* 16 + 41 */
    Default_IRQHandler, //0,                                       /* 16 + 42 */
    Default_IRQHandler, //0,                                       /* 16 + 43 */
    Default_IRQHandler, //GPIO_INT0_IRQHandler_Wrapper,            /* 16 + 44 */
    Default_IRQHandler, //0,                                       /* 16 + 45 */
    Default_IRQHandler, //0,                                       /* 16 + 46 */
    Default_IRQHandler, //0,                                       /* 16 + 47 */
    Default_IRQHandler, //0,                                       /* 16 + 48 */
    Default_IRQHandler, //0,                                       /* 16 + 49 */
    Default_IRQHandler, //PDS_WAKEUP_IRQHandler_Wrapper,           /* 16 + 50 */
    Default_IRQHandler, //HBN_OUT0_IRQHandler_Wrapper,             /* 16 + 51 */
    Default_IRQHandler, //HBN_OUT1_IRQHandler_Wrapper,             /* 16 + 52 */
    Default_IRQHandler, //BOR_IRQHandler_Wrapper,                  /* 16 + 53 */
    Default_IRQHandler, //WIFI_IRQHandler_Wrapper,                 /* 16 + 54 */
    Default_IRQHandler, //BZ_PHY_IRQHandler_Wrapper,               /* 16 + 55 */
    Default_IRQHandler, //BLE_IRQHandler_Wrapper,                  /* 16 + 56 */
    Default_IRQHandler, //MAC_TXRX_TIMER_IRQHandler_Wrapper,       /* 16 + 57 */
    Default_IRQHandler, //MAC_TXRX_MISC_IRQHandler_Wrapper,        /* 16 + 58 */
    Default_IRQHandler, //MAC_RX_TRG_IRQHandler_Wrapper,           /* 16 + 59 */
    Default_IRQHandler, //MAC_TX_TRG_IRQHandler_Wrapper,           /* 16 + 60 */
    Default_IRQHandler, //MAC_GEN_IRQHandler_Wrapper,              /* 16 + 61 */
    Default_IRQHandler, //MAC_PORT_TRG_IRQHandler_Wrapper,         /* 16 + 62 */
    Default_IRQHandler, //WIFI_IPC_PUBLIC_IRQHandler_Wrapper,      /* 16 + 63 */
};
#endif

void board_init(void);

void interrupt0_handler(void);
int lpfw_uart_init(void);

ATTR_ROM_WIFI_SECTION void lpfw_gpio_int_set(uint32_t gpioPin, uint32_t trig)
{
    uint32_t gpioCfgAddress = 0;
    uint32_t tmpVal = 0;
    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);
    // tmpVal = 0; //QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_IE, 1);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_FUNC_SEL, 0xB);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_INT_MODE_SET, trig);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_INT_MASK, 0);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);
}

ATTR_ROM_WIFI_SECTION QC7XX_Mask_Type lpfw_get_pdsio_mask(uint32_t pin)
{
    uint32_t tmpVal = 0;
    uint32_t bit = pin;
    tmpVal = QC7XX_RD_REG(PDS_BASE, PDS_GPIO_PD_SET);

    if ((pin > 34) || ((pin > 15) && (pin < 20))) {
        return MASK;
    }

    if (bit > 15) {
        bit -= 4;
    }

    if ((tmpVal >> bit) & 0x1) {
        return MASK;
    } else {
        return UNMASK;
    }
}

ATTR_ROM_WIFI_SECTION QC7XX_Sts_Type lpfw_get_pdsio_trig(uint32_t pin, uint32_t *trig)
{
    uint32_t tmpVal = 0;
    uint32_t bit = pin;
    tmpVal = QC7XX_RD_REG(PDS_BASE, PDS_GPIO_INT);

    if ((pin > 34) || ((pin > 15) && (pin < 20))) {
        return RESET;
    }

    if (bit > 15) {
        bit -= 4;
    }

    bit = (bit / 8 * 8) + 4;

    *trig = (tmpVal >> bit) & 0xF;

    return SET;
}

ATTR_ROM_WIFI_SECTION QC7XX_Sts_Type lpfw_set_glbio_int(void)
{
    uint32_t ret = RESET;
    uint32_t trig = 0;
    uint32_t tmpVal = 0;
    uint32_t tmpPin = 0;

    tmpVal = QC7XX_RD_REG(PDS_BASE, PDS_GPIO_PD_SET);

    uint8_t grp = 0;

    for (; grp < 4; grp++) {
        tmpPin = 8 * grp;
        if ((tmpVal >> tmpPin) & 0xFF) {
            if (tmpPin >= 16) {
                tmpPin += 4;
            }

            lpfw_get_pdsio_trig(tmpPin, &trig);

            for (uint8_t i = 0; i < 8; i++) {
                if (UNMASK == lpfw_get_pdsio_mask(tmpPin + i)) {
                    // lpfw_printf_debug("set io%d\r\n", (tmpPin + i));
                    lpfw_gpio_int_set(tmpPin + i, trig);
                    ret = SET;
                }
            }
        }
    }

    return ret;
}

int count_bits(uint32_t value)
{
    uint32_t temp = value;
    temp = temp - ((temp >> 1) & 0x55555555);
    temp = (temp & 0x33333333) + ((temp >> 2) & 0x33333333);
    temp = (temp + (temp >> 4)) & 0x0f0f0f0f;
    temp = temp + (temp >> 8);
    return (temp + (temp >> 16)) & 0xff;
}

ATTR_ROM_WIFI_SECTION uint8_t PDS_Get_GPIO_Pad_IntMode(uint8_t set)
{
    uint32_t tmpVal = 0;
    uint8_t trig;

    tmpVal = QC7XX_RD_REG(PDS_BASE, PDS_GPIO_INT);

    trig = ((tmpVal >> (set * 8 + 4)) & 0xF);

    return trig;
}

uint8_t lpfw_get_gpio_value(uint8_t gpioPin)
{
    uint32_t gpioCfgAddress = 0;
    uint8_t gpioVal = 0;

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);

    /* GPIO Function Select SWGPIO */
    *((volatile uint32_t *)gpioCfgAddress) &= ~(0x1f << 8);
    *((volatile uint32_t *)gpioCfgAddress) |= (0xb << 8);

    /* GPIO Output Disable */
    *((volatile uint32_t *)gpioCfgAddress) &= ~(1 << 6);

    /* GPIO Input Enable */
    *((volatile uint32_t *)gpioCfgAddress) |= (1 << 0);

    /* get GPIO value */
    gpioVal = (*((volatile uint32_t *)gpioCfgAddress) >> 28) & 0x1;

    return gpioVal;
}

void lpfw_get_pds_pad_stat(uint8_t pds_gpio_stat, uint8_t io_grp)
{
    uint8_t io_bit = 0;

    if (io_grp < PDS_GPIO_INT_SET_3_GPIO20_GPIO27) {
        io_bit = 8 * io_grp;
    } else {
        io_bit = 8 * io_grp + 4;
    }

    if (pds_gpio_stat) {
        uint8_t pds_gpio_trig = PDS_Get_GPIO_Pad_IntMode(io_grp);
        if ((pds_gpio_trig == QC7XX_LP_PDS_IO_TRIG_SYNC_FALLING_EDGE) || (pds_gpio_trig == QC7XX_LP_PDS_IO_TRIG_ASYNC_FALLING_EDGE)) {
            for (uint8_t i = 0; i < 8; i++) {
                if ((pds_gpio_stat >> i) & 0x1) {
                    if (0 == lpfw_get_gpio_value(i + io_bit)) {
                        iot2lp_para->wake_io_bits |= ((uint64_t)1 << (i + io_bit));
                    }
                }
            }
        } else {
            iot2lp_para->wake_io_bits |= ((uint64_t)pds_gpio_stat << io_bit);
        }
    }
}

void clear_pds_gpio(void)
{
    PDS_Set_GPIO_Pad_IntClr(PDS_GPIO_INT_SET_1_GPIO0_GPIO7);
    PDS_Set_GPIO_Pad_IntClr(PDS_GPIO_INT_SET_2_GPIO8_GPIO15);
    PDS_Set_GPIO_Pad_IntClr(PDS_GPIO_INT_SET_3_GPIO20_GPIO27);
    PDS_Set_GPIO_Pad_IntClr(PDS_GPIO_INT_SET_4_GPIO28_GPIO34);
}

ATTR_ROM_WIFI_SECTION uint8_t lpfw_wake_bits_get(void)
{
    uint8_t wake_flag = 0;
    uint32_t tmpVal = 0;

    /* get source of wakeup pds mode*/
    tmpVal = QC7XX_RD_REG(PDS_BASE, PDS_INT);
    tmpVal = QC7XX_GET_REG_BITS_VAL(tmpVal, PDS_RO_PDS_WAKEUP_EVENT);

    if ((tmpVal & 0x7FE)) {
        /* will jump APP, if wakeup source is not pds_timer */
    } else {
        clear_pds_gpio();
        return wake_flag;
    }

    /* check PDS_IO Wakeup Source */
    if (pm_pds_get_wakeup_src(PDS_WAKEUP_BY_PDS_GPIO_IRQ_EN_POS)) {
        iot2lp_para->wakeup_reason |= LPFW_WAKEUP_IO;

        uint32_t pds_gpio_stat = QC7XX_RD_REG(PDS_BASE, PDS_GPIO_STAT);

        uint8_t pds_wake_io_count = count_bits(pds_gpio_stat);
        if (pds_wake_io_count == 0) {
            /* err*/

        } else if (pds_wake_io_count == 1) {
            iot2lp_para->wake_io_bits |= (pds_gpio_stat & 0xFFFF);
            iot2lp_para->wake_io_bits |= (((((uint64_t)pds_gpio_stat) >> 16) & 0x7FFF) << 20);
        } else {
            uint8_t pds_gpio_0_7_stat = pds_gpio_stat & 0xFF;
            uint8_t pds_gpio_8_15_stat = (pds_gpio_stat >> 8) & 0xFF;
            uint8_t pds_gpio_20_27_stat = (pds_gpio_stat >> 16) & 0xFF;
            uint8_t pds_gpio_28_34_stat = (pds_gpio_stat >> 24) & 0x7F;

            lpfw_get_pds_pad_stat(pds_gpio_0_7_stat, PDS_GPIO_INT_SET_1_GPIO0_GPIO7);
            lpfw_get_pds_pad_stat(pds_gpio_8_15_stat, PDS_GPIO_INT_SET_2_GPIO8_GPIO15);
            lpfw_get_pds_pad_stat(pds_gpio_20_27_stat, PDS_GPIO_INT_SET_3_GPIO20_GPIO27);
            lpfw_get_pds_pad_stat(pds_gpio_28_34_stat, PDS_GPIO_INT_SET_4_GPIO28_GPIO34);
        }
        clear_pds_gpio();

        if ((iot2lp_para->wake_io_bits & 0xFFFFFFFFF) != 0) {
            wake_flag = 1;
        }
    }

    if (pm_pds_get_wakeup_src(PDS_WAKEUP_BY_HBN_IRQ_OUT_EN_POS)) {
        wake_flag = 1;
        uint8_t hbn_io_wake_mode = HBN_Get_Pin_Wakeup_Mode();
        /* check aonio Wakeup Source */
        for (int i = 0; i < 4; i++) {
            if (HBN_Get_INT_State((HBN_INT_GPIO16 + i))) {
                if (QC7XX_LP_AON_IO_TRIG_SYNC_RISING_FALLING_EDGE == hbn_io_wake_mode) {
                    if (1 == lpfw_get_gpio_value(i + 16)) {
                        iot2lp_para->wake_io_edge_bits |= (1 << (i + 16));
                    } else {
                        iot2lp_para->wake_io_edge_bits &= ~(1 << (i + 16));
                    }
                }
                HBN_Clear_IRQ(HBN_INT_GPIO16 + i);
                iot2lp_para->wakeup_reason |= LPFW_WAKEUP_IO;
                iot2lp_para->wake_io_bits |= (1 << (16 + i));
            }
        }

        /* check acomp0 Wakeup Source */
        if (HBN_Get_INT_State(HBN_INT_ACOMP0)) {
            iot2lp_para->wakeup_reason |= LPFW_WAKEUP_ACOMP;
            iot2lp_para->wake_acomp_bits |= (1 << 0);
            iot2lp_para->wake_acomp_edge_bits |= (qc7xx_acomp_get_result(AON_ACOMP0_ID) << 0);
            HBN_Clear_IRQ(HBN_INT_ACOMP0);
        }

        /* check acomp1 Wakeup Source */
        if (HBN_Get_INT_State(HBN_INT_ACOMP1)) {
            iot2lp_para->wakeup_reason |= LPFW_WAKEUP_ACOMP;
            iot2lp_para->wake_acomp_bits |= (1 << 1);
            iot2lp_para->wake_acomp_edge_bits |= (qc7xx_acomp_get_result(AON_ACOMP1_ID) << 1);
            HBN_Clear_IRQ(HBN_INT_ACOMP1);
        }

        /* check RTC Wakeup Source */
        if (HBN_Get_INT_State(HBN_INT_RTC)) {
            HBN_Clear_IRQ(HBN_INT_RTC);
            HBN_Clear_RTC_INT();
            iot2lp_para->wakeup_reason |= LPFW_WAKEUP_TIME_OUT;
        }
    }

    PDS_IntClear();

    return wake_flag;
}

extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

extern uint32_t __ram_data_start__;
extern uint32_t __ram_data_end__;

extern uint32_t __ram_data_copy_start__;
extern uint32_t __ram_data_copy_end__;

/* copy .data to .data_copy */
void start_load_frist(void)
{
    uint32_t *pSrc, *pDest;
    uint32_t *pTable __attribute__((unused));

    /* .bss */
    /* align to 32byte (4*8) */
    pDest = &__bss_start__;
    for (; pDest < &__bss_end__; pDest += 8) {
        pDest[0] = 0ul;
        pDest[1] = 0ul;
        pDest[2] = 0ul;
        pDest[3] = 0ul;
        pDest[4] = 0ul;
        pDest[5] = 0ul;
        pDest[6] = 0ul;
        pDest[7] = 0ul;
    }

    /* .data */
    /* align to 32byte (4*8) */
    pSrc = &__ram_data_start__;
    pDest = &__ram_data_copy_start__;
    for (; pDest < &__ram_data_copy_end__; pDest += 8, pSrc += 8) {
        pDest[0] = pSrc[0];
        pDest[1] = pSrc[1];
        pDest[2] = pSrc[2];
        pDest[3] = pSrc[3];
        pDest[4] = pSrc[4];
        pDest[5] = pSrc[5];
        pDest[6] = pSrc[6];
        pDest[7] = pSrc[7];

        __DCACHE_CPA((uint32_t)pDest);
    }
}

/* copy .data to .data_copy, and clean .bss */
void lpfw_start_load(void)
{
    uint32_t *pSrc, *pDest;
    uint32_t *pTable __attribute__((unused));

    /* .bss */
    /* align to 32byte (4*8) */
    pDest = &__bss_start__;
    for (; pDest < &__bss_end__; pDest += 8) {
        pDest[0] = 0ul;
        pDest[1] = 0ul;
        pDest[2] = 0ul;
        pDest[3] = 0ul;
        pDest[4] = 0ul;
        pDest[5] = 0ul;
        pDest[6] = 0ul;
        pDest[7] = 0ul;
    }

    /* .data */
    /* align to 32byte (4*8) */
    pSrc = &__ram_data_copy_start__;
    pDest = &__ram_data_start__;
    for (; pDest < &__ram_data_end__; pDest += 8, pSrc += 8) {
        pDest[0] = pSrc[0];
        pDest[1] = pSrc[1];
        pDest[2] = pSrc[2];
        pDest[3] = pSrc[3];
        pDest[4] = pSrc[4];
        pDest[5] = pSrc[5];
        pDest[6] = pSrc[6];
        pDest[7] = pSrc[7];
    }
}
#ifdef SHARED_FUNC_EN
void load_shared_func(void)
{
    uint32_t *p = NULL;

    p = iot2lp_para->shared_func_p;

    shared_cpu_get_mtimer_counter = (uint64_t (*)(void))(*(p + 0));
    shared_arch_delay_ms = (void (*)(uint32_t))(*(p + 1));
    shared_arch_delay_us = (void (*)(uint32_t))(*(p + 2));
    shared_cpu_reset_mtimer = (int32_t (*)(void))(*(p + 3));
    shared_lpfw_calculate_beacon_delay = (int32_t (*)(uint64_t, uint64_t, uint32_t))(*(p + 4));
    // shared_lpfw_beacon_delay_sliding_win_update = (int32_t (*)(int32_t,uint64_t))(*(p+5));
    shared_lpfw_beacon_delay_sliding_win_get_average = (int32_t (*)(void))(*(p + 6));
    shared_aon_set_ldo11_soc_sstart_delay = (int32_t (*)(uint32_t))(*(p + 7));
    shared_pds_default_level_config = (int32_t (*)(uint32_t *, uint32_t))(*(p + 8));
    L1C_DCache_Clean_All();
}
#endif
extern uint32_t qc7xx_lp_set_acomp_pin(uint32_t acomp_chan);
void lpfw_interrupt_handler(void);
extern void lpfw_Interrupt_Handler_Register(pFunc interruptFun);
ATTR_ROM_WIFI_SECTION void lp_board_init(void)
{

    /* update sleep_pds time */
    qc7xx_lp_time_info_update_pds();

    if (iot2lp_para->lpfw_wakeup_cnt == 0) {
        /* copy .data to .data_copy */
        start_load_frist();
    } else {
        /* copy .data to .data_copy, and clean .bss */
        lpfw_start_load();
    }

#ifdef SHARED_FUNC_EN
    load_shared_func();

    shared_cpu_reset_mtimer();
#else
    CPU_Reset_MTimer();
#endif

    /* lpfw wakeup num */
    iot2lp_para->lpfw_wakeup_cnt++;

#if (LPFW_JTAG_EN == 1)
    uint8_t jtaggpio[] = { GLB_GPIO_PIN_0, GLB_GPIO_PIN_1, GLB_GPIO_PIN_2, GLB_GPIO_PIN_3 };

    GLB_GPIO_Func_Init(GPIO_FUN_JTAG, jtaggpio, 4);
#endif

    // CPU_Set_MTimer_CLK(ENABLE, 1/* QC7XX_MTIMER_SOURCE_CLOCK_MCU_CLK */, Clock_System_Clock_Get(1/*QC7XX_SYSTEM_CLOCK_MCU_CLK*/) / 1000000 - 1);
#if (LPFW_LOG_EN != 0) || (LPFW_SHELL_EN)
    GLB_Set_UART_CLK(1, 2 /*HBN_UART_CLK_XCLK*/, 0);
    lp_uart_int_enable(0, 0, 1);
    lpp_uart_init(0, 21, 22, 255, 255, 2000000);
    lp_uart_init(0, 2000000);
#endif

    lpfw_Interrupt_Handler_Register(lpfw_interrupt_handler);

#if 1
    /* get aon_io wakeup mask bits */
    uint32_t mask_val = 0;
    mask_val = QC7XX_RD_REG(HBN_BASE, HBN_IRQ_MODE);
    mask_val = QC7XX_GET_REG_BITS_VAL(mask_val, HBN_PIN_WAKEUP_MASK);
    /* enable aon_io and rtc interrupt */
    if ((mask_val != 0xF) || (0 != iot2lp_para->rtc_wakeup_en)) {
        qc7xx_irq_enable(HBN_OUT0_IRQn);
    }

    if (SET == lpfw_set_glbio_int()) {
        qc7xx_irq_enable(GPIO_INT0_IRQn);
    }

    uint32_t tmpVal, tmpChan, irq_acomp_en, hbn1_en = 0;
    for (uint8_t i = 0; i < 2; i++) {
        tmpVal = QC7XX_RD_REG(HBN_BASE, HBN_IRQ_MODE);
        if (i == 0) {
            irq_acomp_en = QC7XX_GET_REG_BITS_VAL(tmpVal, HBN_IRQ_ACOMP0_EN);
        } else {
            irq_acomp_en = QC7XX_GET_REG_BITS_VAL(tmpVal, HBN_IRQ_ACOMP1_EN);
        }

        if (irq_acomp_en) {
            hbn1_en = 1;
            tmpChan = qc7xx_acomp_get_postive_input(i);
            qc7xx_lp_set_acomp_pin(tmpChan);
            if (i == 0) {
                HBN_Clear_IRQ(20);
            } else {
                HBN_Clear_IRQ(22);
            }
        }
    }

    if (hbn1_en) {
        qc7xx_irq_enable(HBN_OUT1_IRQn);
    }
#endif
}

#if LPFW_WIFI_RF_DEBUG

#include "rwnx.h"
#include "rwnx_platform.h"
#include "rxl_hwdesc.h"
#include "ke_event.h"

static volatile iot2lp_para_t *iot2lp_para = (iot2lp_para_t *)IOT2LP_PARA_ADDR;

bool rxu_cntrl_frame_handle(struct rxdesc *rxdesc)
{
    struct rx_dmadesc *dma_hdrdesc = rxl_dmadesc_get(rxdesc);
    struct rx_hd *rhd = &(dma_hdrdesc->hd);
    struct rx_pbd *pbd = HW2CPU(rhd->first_pbd_ptr);
    struct bcn_frame *bcn = HW2CPU(pbd->datastartptr);

    static uint64_t beacon_stamp_last = 0;

    if (beacon_stamp_last == 0) {
        beacon_stamp_last = bcn->tsf;
        lpfw_printf_debug("LPFW Frist beacon timestamp %lld\r\n", bcn->tsf);
        rxl_mpdu_free(rxdesc);
        return (false);
    }

    /* interval */
    uint32_t interval = (bcn->tsf - beacon_stamp_last + 51200) / 102400;

    /* The number that should be received */
    iot2lp_para->lpfw_recv_cnt += interval;

    /* loss cnt */
    iot2lp_para->lpfw_loss_cnt += interval - 1;

    lpfw_printf_debug("LPFW beacon timestamp %lld, ", bcn->tsf);
    lpfw_printf_debug("want receive %ld, loss %ld, ", iot2lp_para->lpfw_recv_cnt, iot2lp_para->lpfw_loss_cnt);
    if (iot2lp_para->lpfw_recv_cnt) {
        lpfw_printf_debug("loss rate %ld.%ld%%, ", iot2lp_para->lpfw_loss_cnt * 100 / iot2lp_para->lpfw_recv_cnt, (iot2lp_para->lpfw_loss_cnt * 10000 / iot2lp_para->lpfw_recv_cnt) % 100);
    }
    lpfw_printf_debug("RSSI %d \r\n", rhd->rx_vec_1.rssi1);

    beacon_stamp_last = bcn->tsf;

    rxl_mpdu_free(rxdesc);

    return (false);
}

#endif

void lpfw_interrupt_handler(void)
{
    extern void hbn_rtc_timeout_handler(void);
    extern void mtimer_irq_handler(void);
    extern uint32_t CPU_Interrupt_Pending_Get(uint32_t irq_num);
    extern uint32_t CPU_Interrupt_En_Get(uint32_t irq_num);
    extern void hbn_io_interrupt_handler(void);
    extern void glb_io_interrupt_handler(void);
    extern void hbn_acomp_interrupt_handler(void);

    extern volatile uint8_t g_wifi_int_flag;

    if (iot2lp_para->pds_level == 1 &&
        CPU_Interrupt_En_Get(PDS_WAKEUP_IRQn) &&
        CPU_Interrupt_Pending_Get(PDS_WAKEUP_IRQn)) {
        pm_pds_wakeup_irq_quiesce();
        return;
    }

    if (CPU_Interrupt_En_Get(GPIO_INT0_IRQn) && CPU_Interrupt_Pending_Get(GPIO_INT0_IRQn)) {
        glb_io_interrupt_handler();
    }

    if (CPU_Interrupt_En_Get(HBN_OUT0_IRQn) && CPU_Interrupt_Pending_Get(HBN_OUT0_IRQn)) {
        hbn_rtc_timeout_handler();
        hbn_io_interrupt_handler();
    }

    if (CPU_Interrupt_En_Get(HBN_OUT1_IRQn) && CPU_Interrupt_Pending_Get(HBN_OUT1_IRQn)) {
        hbn_acomp_interrupt_handler();
    }

    if (CPU_Interrupt_En_Get(MTIME_IRQn) && CPU_Interrupt_Pending_Get(MTIME_IRQn)) {
        if (iot2lp_para->pds_level == 1) {
            qc7xx_irq_disable(MTIME_IRQn);
        }
        mtimer_irq_handler();
    }

    if (CPU_Interrupt_En_Get(WIFI_IRQn) && CPU_Interrupt_Pending_Get(WIFI_IRQn)) {
#if !LPFW_WIFI_RF_DEBUG
        if (iot2lp_para->pds_level == 1) {
            qc7xx_irq_disable(WIFI_IRQn);
        }
#endif
        g_wifi_int_flag = 1;
        lpfw_printf_debug("wifi interrupt");

#if LPFW_WIFI_RF_DEBUG
        interrupt0_handler();
        ke_evt_schedule();
#endif
        // CPU_Interrupt_Disable(MTIME_IRQn); /* fix: close mtimer timeout irq */
    }
}

// void crm_macwtclk_ratio_sel_setf(uint8_t macwtclkratiosel);
// void crm_maccoreclk_freq_sel_setf(uint8_t macwtclkratiosel);
QC7XX_Err_Type GLB_Fast_Set_MCU_System_CLK(uint8_t clkFreq);

__STATIC_INLINE void csi_idcache_enable(void)
{
    uint32_t cache;

    __DSB();
    // __ISB();
    // __DCACHE_IALL(); /* invalidate all dcache */
    cache = __get_MHCR();
    cache |= (CACHE_MHCR_DE_Msk | CACHE_MHCR_WB_Msk | CACHE_MHCR_WA_Msk | CACHE_MHCR_RS_Msk | CACHE_MHCR_BPE_Msk | CACHE_MHCR_L0BTB_Msk | CACHE_MHCR_IE_Msk); /* enable all Cache */
    __set_MHCR(cache);

    __DSB();
    // __ISB();
}

ATTR_ROM_WIFI_SECTION void lp_clock_init(void)
{
    uint8_t jump_en = 0;
    csi_idcache_enable();
    //csi_icache_enable();

    GLB_Fast_Power_On_XTAL_40M_And_WIFIPLL();

    // GLB_Set_MCU_System_CLK(5/*GLB_MCU_SYS_CLK_TOP_WIFIPLL_320M*/);
    GLB_Fast_Set_MCU_System_CLK(5);

    /* check if jump APP */
    jump_en |= lpfw_wake_bits_get();
    if (iot2lp_para->pds_level == 1) {
        pm_pds_wakeup_irq_ack();
    }
    if (jump_en) {
        iot2lp_para->do_xip_recovery = 1;
    } else {
        if (iot2lp_para->unkown_io_wakeup_en == 1) {
            if (iot2lp_para->wakeup_reason & LPFW_WAKEUP_IO) {
                iot2lp_para->do_xip_recovery = 1;
            }
        } else {
            if (iot2lp_para->wakeup_reason & LPFW_WAKEUP_IO) {
                iot2lp_para->wakeup_reason &= ~LPFW_WAKEUP_IO;
            }
        }

        if (iot2lp_para->tim_wakeup_en == 0) {
            iot2lp_para->do_xip_recovery = 1;
        }
    }

    if (iot2lp_para->do_xip_recovery == 0) {
        // GLB_Set_MCU_System_CLK_Div(3, 0);
        // *((volatile uint32_t*)0x200103C0)=CPU_Get_MTimer_Counter();
        /* clk gate */
        /* Gate CCI Bus Master Clock (BCLK) */
        /* Gate dma Bus Master Clock (BCLK) */
        /* Gate SEC Bus Master Clock (BCLK) */
        /* Gate SDU Bus Master Clock (BCLK) */
        /* Gate CPU Bus Master Clock (BCLK) */
        /* En CPU Bus Master Clock (BCLK) */
        *((volatile uint32_t *)0x20000580) = 1;
        /* Gate I2S、CAN、I2C1、MIPI_DBI、Checksum、IR、TIMER、PWM、I2C0、SPI、UART1、USB、DMA、SF_CTRL、EF_CTRL、TZ、SEC、GPIP */
        /* En UART0 and uart_clk_0 */
        *((volatile uint32_t *)0x20000584) = (1 << 16);
        /* Gate PIO、EMAC、SDH、AUDIO、PSRAM1_CTRL、MM_MISC、wifi */
        *((volatile uint32_t *)0x20000588) = 0;

        /* power off aupll */
        *((volatile uint32_t *)0x20008750) = 0xf;

        /* close usb psw_rref output */
        *((volatile uint32_t *)0x2000e504) &= ~(1 << 6);

// crm_macwtclk_ratio_sel_setf(2);
// crm_maccoreclk_freq_sel_setf(6);
#define CRM_CLKRST_CNTL_ADDR 0x24940008
        QC7XX_WR_WORD(CRM_CLKRST_CNTL_ADDR, (QC7XX_RD_WORD(CRM_CLKRST_CNTL_ADDR) & ~((uint32_t)0x00003000)) | ((uint32_t)1 << 12));
        QC7XX_WR_WORD(CRM_CLKRST_CNTL_ADDR, (QC7XX_RD_WORD(CRM_CLKRST_CNTL_ADDR) & ~((uint32_t)0x00000F00)) | ((uint32_t)4 << 8));
    } else {
        GLB_Set_MCU_System_CLK_Div(0, 3);
    }
}

ATTR_ROM_WIFI_SECTION void lp_SystemInit(void)
{
    /* enable mstatus FS */
    uint32_t mstatus = __get_MSTATUS();
    mstatus |= (1 << 13);
    __set_MSTATUS(mstatus);

    /* enable mxstatus THEADISAEE */
    uint32_t mxstatus = __get_MXSTATUS();
    mxstatus |= (1 << 22);
    /* enable mxstatus MM */
    mxstatus |= (1 << 15);
    __set_MXSTATUS(mxstatus);

    /* disable mexstatus SPUSHEN and SPSWAPEN for ipush/ipop*/
    uint32_t mexstatus = __get_MEXSTATUS();
    mexstatus &= ~(0x3 << 16);
    __set_MEXSTATUS(mexstatus);
}

/* mtimer init */
ATTR_ROM_WIFI_SECTION void mtimer_init(uint32_t timeout_us)
{
    uint32_t m_cnt_l, m_cnt_h;

    /* read mtimer cnt */
    m_cnt_l = *(volatile uint32_t *)(0xE000BFF8);
    m_cnt_h = *(volatile uint32_t *)(0xE000BFFC);

    m_cnt_l += timeout_us;
    if (m_cnt_l < timeout_us) {
        m_cnt_h += 1;
    }

    /* write mtimer compare cnt */
    *(volatile uint32_t *)(0xE0004000) = m_cnt_l;
    *(volatile uint32_t *)(0xE0004004) = m_cnt_h;

    qc7xx_irq_enable(MTIME_IRQn);
}
