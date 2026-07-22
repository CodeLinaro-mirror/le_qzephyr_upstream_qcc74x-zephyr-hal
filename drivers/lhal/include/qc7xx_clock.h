/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_CLOCK_H
#define _QC7XX_CLOCK_H

#include "qc7xx_core.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup CLOCK
  * @{
  */

/** @defgroup QC7XX_SYSTEM_CLOCK system clock definition
  * @{
  */
#define QC7XX_SYSTEM_ROOT_CLOCK 0
#define QC7XX_SYSTEM_CPU_CLK    1
#define QC7XX_SYSTEM_PBCLK      2
#define QC7XX_SYSTEM_XCLK       3
#define QC7XX_SYSTEM_32K_CLK    4
/**
  * @}
  */

#if defined(QCC74X) || defined(QCC75X)
#define QC7XX_GLB_CGEN0_BASE (0x20000000 + 0x580)
#define QC7XX_GLB_CGEN1_BASE (0x20000000 + 0x584)
#define QC7XX_GLB_CGEN2_BASE (0x20000000 + 0x588)
#endif

#define PERIPHERAL_CLOCK_ADC_DAC_ENABLE()                         \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 2);                                       \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#define PERIPHERAL_CLOCK_SEC_ENABLE()                             \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 3);                                       \
        regval |= (1 << 4);                                       \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#define PERIPHERAL_CLOCK_DMA0_ENABLE()                            \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 12);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#define PERIPHERAL_CLOCK_UART0_ENABLE()                           \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 16);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#define PERIPHERAL_CLOCK_UART1_ENABLE()                           \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 17);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#define PERIPHERAL_CLOCK_SPI0_ENABLE()                            \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 18);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#if defined(QCC75X)
#define PERIPHERAL_CLOCK_SPI1_ENABLE()                            \
    do {                                                          \
        volatile uint32_t regval = getreg32(0x200005cc);          \
        regval &= ~(0x1 << 31);                                   \
        putreg32(regval, 0x200005cc);                             \
    } while (0)
#endif

#define PERIPHERAL_CLOCK_I2C0_ENABLE()                            \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 19);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#if defined(QCC74X) || defined(QCC75X)
#define PERIPHERAL_CLOCK_I2C1_ENABLE()                            \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 25);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#endif

#define PERIPHERAL_CLOCK_PWM0_ENABLE()                            \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 20);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#define PERIPHERAL_CLOCK_TIMER0_1_WDG_ENABLE()                    \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 21);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#define PERIPHERAL_CLOCK_IR_ENABLE()                              \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 22);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#define PERIPHERAL_CLOCK_CKS_ENABLE()                             \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 23);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)

#if defined(QCC74X) || defined(QCC75X)
#define PERIPHERAL_CLOCK_CAN_ENABLE()                             \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 26);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)
#endif

#if defined(QCC74X)
#define PERIPHERAL_CLOCK_USB_ENABLE()                             \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 13);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)
#elif defined(QCC75X)
#define PERIPHERAL_CLOCK_USB_ENABLE()                             \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN2_BASE); \
        regval |= (1 << 19);                                      \
        putreg32(regval, QC7XX_GLB_CGEN2_BASE);                    \
    } while (0)
#endif

#if defined(QCC74X) || defined(QCC75X)
#define PERIPHERAL_CLOCK_I2S_ENABLE()                             \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 27);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)
#endif

#if defined(QCC74X) || defined(QCC75X)
#define PERIPHERAL_CLOCK_SDH_ENABLE()                             \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN2_BASE); \
        regval |= (1 << 22);                                      \
        putreg32(regval, QC7XX_GLB_CGEN2_BASE);                    \
    } while (0)
#endif

#if defined(QCC74X) || defined(QCC75X)
#define PERIPHERAL_CLOCK_EMAC_ENABLE()                            \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN2_BASE); \
        regval |= (1 << 23);                                      \
        putreg32(regval, QC7XX_GLB_CGEN2_BASE);                    \
    } while (0)
#endif

#if defined(QCC74X)
#define PERIPHERAL_CLOCK_AUDIO_ENABLE()                           \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN2_BASE); \
        regval |= (1 << 21);                                      \
        putreg32(regval, QC7XX_GLB_CGEN2_BASE);                    \
    } while (0)
#endif

#if defined(QCC74X)
#define PERIPHERAL_CLOCK_DBI_ENABLE()                             \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN1_BASE); \
        regval |= (1 << 24);                                      \
        putreg32(regval, QC7XX_GLB_CGEN1_BASE);                    \
    } while (0)
#endif

#if defined(QCC75X)
#define PERIPHERAL_CLOCK_PEC_ENABLE()                             \
    do {                                                          \
        volatile uint32_t regval = getreg32(QC7XX_GLB_CGEN2_BASE); \
        regval |= (1 << 25);                                      \
        putreg32(regval, QC7XX_GLB_CGEN2_BASE);                    \
    } while (0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  enable/disable peri clock
 *
 * @param  peri: QC7XX_PERIPHERAL_xxx
 * @param  enable: Boolean value to enable or disable the clock
 * @return int
 */
int qc7xx_peripheral_clock_control(uint8_t peri, bool enable);

/**
 * @brief  get peri clock value
 *
 * @param  peri: QC7XX_PERIPHERAL_xxx
 * @return int
 */
uint32_t qc7xx_peripheral_clock_get(uint8_t peri);

/**
 * @brief  get peri clock status
 *
 * @param  peri: QC7XX_PERIPHERAL_xxx
 * @return int
 */
int qc7xx_peripheral_clock_status_get(uint8_t peri);

/**
 * @brief Get system clock frequence
 *
 * @param [in] type system clock type
 * @return frequence
 */
uint32_t qc7xx_clk_get_system_clock(uint8_t type);

/**
 * @brief Get peripheral clock frequence
 *
 * @param [in] type peripheral type
 * @param [in] idx peripheral index
 * @return frequence
 */
uint32_t qc7xx_clk_get_peripheral_clock(uint8_t type, uint8_t idx);

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
  */

#endif