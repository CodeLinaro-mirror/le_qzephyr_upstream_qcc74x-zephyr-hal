/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCR373_CLOCK_H__
#define __QCR373_CLOCK_H__

#include "glb_reg.h"
#include "mcu_misc_reg.h"
#include "pds_reg.h"
#include "qcr373_glb.h"
#include "qcr373_hbn.h"
#include "qcr373_common.h"
#include "hardware/timer_reg.h"
#include "hardware/pwm_v2_reg.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup  QCR373_Peripheral_Driver
 *  @{
 */

/** @addtogroup  CLOCK
 *  @{
 */

/** @defgroup  CLOCK_Public_Types
 *  @{
 */

/**
 *  @brief System clock type definition
 */
typedef enum {
    QC7XX_SYSTEM_CLOCK_MCU_ROOT_CLK,  /*!< MCU root clock */
    QC7XX_SYSTEM_CLOCK_MCU_CLK,       /*!< MCU Fast clock/CPU clock */
    QC7XX_SYSTEM_CLOCK_MCU_BCLK,      /*!< MCU BUS clock */
    QC7XX_SYSTEM_CLOCK_MCU_PBCLK,     /*!< MCU peri BUS clock */
    QC7XX_SYSTEM_CLOCK_F32K,          /*!< F32K clock */
    QC7XX_SYSTEM_CLOCK_XCLK,          /*!< XCLK:RC32M or XTAL */
    QC7XX_SYSTEM_CLOCK_XTAL,          /*!< XTAL clock */
    QC7XX_SYSTEM_CLOCK_WL_XCLK,       /*!< wireless part XCLK:RC32M or XTAL */
    QC7XX_SYSTEM_CLOCK_WL_ROOT_CLK,   /*!< wireless part root clock */
    QC7XX_SYSTEM_CLOCK_WL_CPU_CLK,    /*!< wireless part fast clock/CPU clock */
    QC7XX_SYSTEM_CLOCK_WL_BCLK,       /*!< wireless part BUS clock */
    QC7XX_SYSTEM_CLOCK_WL_PBCLK,      /*!< wireless part peri BUS clock */
    QC7XX_SYSTEM_CLOCK_MINI_XCLK,     /*!< mini part XCLK:RC32M or XTAL */
    QC7XX_SYSTEM_CLOCK_MINI_ROOT_CLK, /*!< mini part root clock */
    QC7XX_SYSTEM_CLOCK_MINI_CPU_CLK,  /*!< mini part fast clock/CPU clock */
    QC7XX_SYSTEM_CLOCK_MINI_BCLK,     /*!< mini part BUS clock */
    QC7XX_SYSTEM_CLOCK_MINI_PBCLK,    /*!< mini part peri BUS clock */
    QC7XX_SYSTEM_CLOCK_MAX,           /*!< MAX type of system clock */
} QCR373_System_Clock_Type;

typedef enum {
    QC7XX_MTIMER_SOURCE_CLOCK_MCU_XCLK, /*!< MCU xclk clock */
    QC7XX_MTIMER_SOURCE_CLOCK_MCU_CLK,  /*!< MCU root clock */
} QCR373_MTimer_Source_Clock_Type;

typedef enum {
    CLOCK_CPUPLL_DIV1,
    CLOCK_CPUPLL_DIV2,
    CLOCK_CPUPLL_DIV3,
    CLOCK_CPUPLL_DIV4,
    CLOCK_CPUPLL_DIV5,
    CLOCK_CPUPLL_DIV6,
    CLOCK_CPUPLL_DIV8,
    CLOCK_CPUPLL_DIV10,
    CLOCK_CPUPLL_DIV12,
    CLOCK_CPUPLL_DIV20,
    CLOCK_CPUPLL_DIV30,
} CLOCK_CPUPLL_Type;

/**
 *  @brief Peripheral clock type definition
 */
typedef enum {
    QCR373_PERIPHERAL_CLOCK_UART0,    /*!< UART0 clock */
    QCR373_PERIPHERAL_CLOCK_UART1,    /*!< UART1 clock */
    QCR373_PERIPHERAL_CLOCK_UART2,    /*!< UART2 clock */
    QCR373_PERIPHERAL_CLOCK_UART3,    /*!< UART3 clock */
    QCR373_PERIPHERAL_CLOCK_SPI0,     /*!< SPI0 clock */
    QCR373_PERIPHERAL_CLOCK_SPI1,     /*!< SPI1 clock */
    QCR373_PERIPHERAL_CLOCK_SPI2,     /*!< SPI2 clock */
    QCR373_PERIPHERAL_CLOCK_SPI3,     /*!< MINI SPI clock */
    QCR373_PERIPHERAL_CLOCK_RESERVED, /*!< reserved clock */
    QCR373_PERIPHERAL_CLOCK_DBI,      /*!< DBI clock */
    QCR373_PERIPHERAL_CLOCK_EMI,      /*!< EMI clock */
    QCR373_PERIPHERAL_CLOCK_I2C0,     /*!< I2C0 clock */
    QCR373_PERIPHERAL_CLOCK_I2C1,     /*!< I2C1 clock */
    QCR373_PERIPHERAL_CLOCK_I2C2,     /*!< I2C2 clock */
    QCR373_PERIPHERAL_CLOCK_PSRAMB,   /*!< PSRAMB clock */
    QCR373_PERIPHERAL_CLOCK_FLASH,    /*!< FLASH clock */
    QCR373_PERIPHERAL_CLOCK_I2S,      /*!< I2S clock */
    QCR373_PERIPHERAL_CLOCK_IR,       /*!< IR clock */
    QCR373_PERIPHERAL_CLOCK_AUADC,    /*!< ADC clock */
    QCR373_PERIPHERAL_CLOCK_GPADC,    /*!< GPADC clock */
    QCR373_PERIPHERAL_CLOCK_GPDAC,    /*!< GPDAC clock */
    QCR373_PERIPHERAL_CLOCK_CAM_REF,  /*!< CAM_REF clock */
    QCR373_PERIPHERAL_CLOCK_CAM,      /*!< CAM clock */
    QCR373_PERIPHERAL_CLOCK_SDH,      /*!< SDH clock */
    QCR373_PERIPHERAL_CLOCK_SEC_PKA,  /*!< PKA clock */
    QCR373_PERIPHERAL_CLOCK_RTC,      /*!< RTC clock */
    QCR373_PERIPHERAL_CLOCK_PEC,      /*!< PEC clock  */
    QCR373_PERIPHERAL_CLOCK_TIMER0,   /*!< TIMER0 clock */
    QCR373_PERIPHERAL_CLOCK_TIMER1,   /*!< TIMER1 clock */
    QCR373_PERIPHERAL_CLOCK_TIMER2,   /*!< TIMER2 clock */
    QCR373_PERIPHERAL_CLOCK_TIMER3,   /*!< TIMER3 clock */
    QCR373_PERIPHERAL_CLOCK_PWM0,     /*!< PWM0 clock */
    QCR373_PERIPHERAL_CLOCK_PWM1,     /*!< PWM1 clock */
    QCR373_PERIPHERAL_CLOCK_DISPLAY,  /*!< DISPLAY clock */
    QCR373_PERIPHERAL_CLOCK_MAX,
} QCR373_Peripheral_Type;

/*@} end of group CLOCK_Public_Types */

/** @defgroup  CLOCK_Public_Constants
 *  @{
 */

/** @defgroup  QC7XX_SYSTEM_CLOCK_TYPE
 *  @{
 */

/*@} end of group CLOCK_Public_Constants */

/** @defgroup  CLOCK_Public_Macros
 *  @{
 */

/*@} end of group CLOCK_Public_Macros */

/** @defgroup  CLOCK_Public_Functions
 *  @{
 */

uint32_t Clock_System_Clock_Get(QCR373_System_Clock_Type type);
uint32_t Clock_Peripheral_Clock_Get(QCR373_Peripheral_Type type);
uint32_t SystemCoreClockGet(void);
QCR373_Err_Type CPU_Set_MTimer_RST(uint8_t rstEn);
QCR373_Err_Type CPU_Reset_MTimer(void);
QCR373_Err_Type CPU_Set_MTimer_CLK(uint8_t enable, uint8_t clock_src, uint16_t div);
uint32_t CPU_Get_MTimer_Source_Clock(void);
uint32_t CPU_Get_MTimer_Clock(void);
uint64_t CPU_Get_MTimer_Counter(void);
uint64_t CPU_Get_CPU_Cycle(void);
uint64_t CPU_Get_MTimer_US(void);
uint64_t CPU_Get_MTimer_MS(void);
QCR373_Err_Type CPU_MTimer_Delay_US(uint32_t cnt);
QCR373_Err_Type CPU_MTimer_Delay_MS(uint32_t cnt);
uint32_t qcr373_peripheral_clock_get_by_id(uint8_t peri);
int qcr373_peripheral_clock_control_by_id(uint8_t peri, bool enable);
int qcr373_peripheral_clock_status_get_by_id(uint8_t peri);

/*@} end of group CLOCK_Public_Functions */

/*@} end of group CLOCK */

/*@} end of group QCR373_Peripheral_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __QCR373_CLOCK_H__ */
