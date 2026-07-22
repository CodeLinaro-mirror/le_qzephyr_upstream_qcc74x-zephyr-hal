/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCC74X_CLOCK_H__
#define __QCC74X_CLOCK_H__

#include "glb_reg.h"
#include "mcu_misc_reg.h"
#include "pds_reg.h"
#include "qcc74x_glb.h"
#include "qcc74x_hbn.h"
#include "qcc74x_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup  QCC74X_Peripheral_Driver
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
    QC7XX_SYSTEM_CLOCK_MCU_ROOT_CLK, /*!< MCU root clock */
    QC7XX_SYSTEM_CLOCK_MCU_CLK,      /*!< MCU Fast clock/CPU clock */
    QC7XX_SYSTEM_CLOCK_MCU_BCLK,     /*!< MCU BUS clock */
    QC7XX_SYSTEM_CLOCK_MCU_PBCLK,    /*!< MCU peri BUS clock */
    QC7XX_SYSTEM_CLOCK_F32K,         /*!< F32K clock */
    QC7XX_SYSTEM_CLOCK_XCLK,         /*!< XCLK:RC32M or XTAL */
    QC7XX_SYSTEM_CLOCK_XTAL,         /*!< XTAL clock */
    QC7XX_SYSTEM_CLOCK_MAX,          /*!< MAX type of system clock */
} QC7XX_System_Clock_Type;

typedef enum {
    QC7XX_MTIMER_SOURCE_CLOCK_MCU_XCLK, /*!< MCU xclk clock */
    QC7XX_MTIMER_SOURCE_CLOCK_MCU_CLK,  /*!< MCU root clock */
} QC7XX_MTimer_Source_Clock_Type;

/**
 *  @brief SOC clock config type
 */
typedef struct
{
    uint32_t magic;                              /*!< Clock config magic */
    uint32_t systemClock[QC7XX_SYSTEM_CLOCK_MAX];   /*!< System lock value */
    uint32_t peripheralClock[QC7XX_AHB_SLAVE1_MAX]; /*!< Pewripherals clock value */
    uint32_t i2sClock;                           /*!< I2S clock */
} Clock_Cfg_Type;

typedef enum {
    CLOCK_AUPLL_DIV1,
    CLOCK_AUPLL_DIV2,
    CLOCK_AUPLL_DIV2P5,
    CLOCK_AUPLL_DIV3,
    CLOCK_AUPLL_DIV4,
    CLOCK_AUPLL_DIV5,
    CLOCK_AUPLL_DIV6,
    CLOCK_AUPLL_DIV10,
    CLOCK_AUPLL_DIV15,
} CLOCK_AUPLL_Type;
/**
 *  @brief Peripheral clock type definition
 */
typedef enum {
    QC7XX_PERIPHERAL_CLOCK_UART0,    /*!< UART0 clock */
    QC7XX_PERIPHERAL_CLOCK_UART1,    /*!< UART1 clock */
    QC7XX_PERIPHERAL_CLOCK_UART2,    /*!< UART2 clock */
    QC7XX_PERIPHERAL_CLOCK_SPI,      /*!< SPI clock */
    QC7XX_PERIPHERAL_CLOCK_RESERVED, /*!< reserved clock */
    QC7XX_PERIPHERAL_CLOCK_DBI,      /*!< DBI clock */
    QC7XX_PERIPHERAL_CLOCK_EMI,      /*!< EMI clock */
    QC7XX_PERIPHERAL_CLOCK_I2C0,     /*!< I2C0 clock */
    QC7XX_PERIPHERAL_CLOCK_I2C1,     /*!< I2C1 clock */
    QC7XX_PERIPHERAL_CLOCK_PSRAMB,   /*!< PSRAMB clock */
    QC7XX_PERIPHERAL_CLOCK_FLASH,    /*!< FLASH clock */
    QC7XX_PERIPHERAL_CLOCK_I2S,      /*!< I2S clock */
    QC7XX_PERIPHERAL_CLOCK_IR,       /*!< IR clock */
    QC7XX_PERIPHERAL_CLOCK_ADC,      /*!< ADC clock */
    QC7XX_PERIPHERAL_CLOCK_GPADC,    /*!< GPADC clock */
    QC7XX_PERIPHERAL_CLOCK_GPDAC,    /*!< GPDAC clock */
    QC7XX_PERIPHERAL_CLOCK_CAM_REF,  /*!< CAM_REF clock */
    QC7XX_PERIPHERAL_CLOCK_CAM,      /*!< CAM clock */
    QC7XX_PERIPHERAL_CLOCK_SDH,      /*!< SDH clock */
    QC7XX_PERIPHERAL_CLOCK_SEC_PKA,  /*!< PKA clock */
    QC7XX_PERIPHERAL_CLOCK_RTC,      /*!< RTC clock */
    QC7XX_PERIPHERAL_CLOCK_MAX,
} QC7XX_Peripheral_Type;

/*@} end of group CLOCK_Public_Types */

/** @defgroup  CLOCK_Public_Constants
 *  @{
 */

/** @defgroup  QC7XX_SYSTEM_CLOCK_TYPE
 *  @{
 */
#define IS_QC7XX_SYSTEM_CLOCK_TYPE(type) (((type) == QC7XX_SYSTEM_CLOCK_MCU_ROOT_CLK) || \
                                       ((type) == QC7XX_SYSTEM_CLOCK_MCU_CLK) ||      \
                                       ((type) == QC7XX_SYSTEM_CLOCK_MCU_BCLK) ||     \
                                       ((type) == QC7XX_SYSTEM_CLOCK_MCU_PBCLK) ||    \
                                       ((type) == QC7XX_SYSTEM_CLOCK_DSP_ROOT_CLK) || \
                                       ((type) == QC7XX_SYSTEM_CLOCK_DSP_CLK) ||      \
                                       ((type) == QC7XX_SYSTEM_CLOCK_DSP_BCLK) ||     \
                                       ((type) == QC7XX_SYSTEM_CLOCK_DSP_PBCLK) ||    \
                                       ((type) == QC7XX_SYSTEM_CLOCK_LP_CLK) ||       \
                                       ((type) == QC7XX_SYSTEM_CLOCK_F32K) ||         \
                                       ((type) == QC7XX_SYSTEM_CLOCK_XCLK) ||         \
                                       ((type) == QC7XX_SYSTEM_CLOCK_XTAL) ||         \
                                       ((type) == QC7XX_SYSTEM_CLOCK_MAX))

/*@} end of group CLOCK_Public_Constants */

/** @defgroup  CLOCK_Public_Macros
 *  @{
 */
#define SYS_CLOCK_CFG_MAGIC (0x12345678)
#define SYS_CLOCK_CFG_ADDR  (0x20010000 + 4 * 1024 - 512)

/*@} end of group CLOCK_Public_Macros */

/** @defgroup  CLOCK_Public_Functions
 *  @{
 */
/*----------*/
uint32_t Clock_System_Clock_Get(QC7XX_System_Clock_Type type);
uint32_t Clock_Peripheral_Clock_Get(QC7XX_Peripheral_Type type);
uint32_t Clock_Audio_ADC_Clock_Get(void);
uint32_t Clock_Audio_DAC_Clock_Get(void);
/*----------*/
uint32_t SystemCoreClockGet(void);
/*----------*/
QC7XX_Err_Type CPU_Set_MTimer_RST(uint8_t rstEn);
QC7XX_Err_Type CPU_Reset_MTimer(void);
QC7XX_Err_Type CPU_Set_MTimer_CLK(uint8_t enable, QC7XX_MTimer_Source_Clock_Type mTimerSourceClockType, uint16_t div);
uint32_t CPU_Get_MTimer_Source_Clock(void);
uint32_t CPU_Get_MTimer_Clock(void);
uint64_t CPU_Get_MTimer_Counter(void);
uint64_t CPU_Get_CPU_Cycle(void);
uint64_t CPU_Get_MTimer_US(void);
uint64_t CPU_Get_MTimer_MS(void);
QC7XX_Err_Type CPU_MTimer_Delay_US(uint32_t cnt);
QC7XX_Err_Type CPU_MTimer_Delay_MS(uint32_t cnt);
uint32_t Clock_Get_EMI_Clk(void);
uint32_t Clock_Get_BLAI_Clk(void);
uint32_t Clock_Get_Display_Clk(void);
uint32_t Clock_Get_PSRAMB_Clk(void);
uint32_t Clock_Get_H264_Clk(void);
uint32_t qc7xx_peripheral_clock_get_by_id(uint8_t peri);
int qc7xx_peripheral_clock_control_by_id(uint8_t peri, bool enable);
int qc7xx_peripheral_clock_status_get_by_id(uint8_t peri);

/*----------*/

/*@} end of group CLOCK_Public_Functions */

/*@} end of group CLOCK */

/*@} end of group QCC74X_Peripheral_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __QCC74X_CLOCK_H__ */
