/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_CORE_H
#define _QC7XX_CORE_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <risc-v/csr.h>
#include <risc-v/riscv_arch.h>
#include <compiler/compiler_gcc.h>
#include <compiler/compiler_ld.h>
#include "qc7xx_name.h"
#include "qc7xx_common.h"
#include "qc7xx_mtimer.h"
#include "qc7xx_irq.h"
#include "qc7xx_l1c.h"

#ifdef CONFIG_LHAL_ROMAPI
#include "qc7xx_lhal_romdriver.h"
#endif

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup CORE
  * @{
  */

#ifdef CONFIG_LHAL_PARAM_ASSERT
#define LHAL_PARAM_ASSERT(expr) ((expr) ? (void)0 : qc7xx_lhal_assert_func(__FILE__, __LINE__, __FUNCTION__, #expr))
#else
#define LHAL_PARAM_ASSERT(expr) ((void)0U)
#endif

#if defined(QCC74X)
#define QC7XX_PSRAM_BASE 0xA8000000
#elif defined(QCC75X)
#define QC7XX_PSRAM_BASE 0x88000000
#endif

#define QC7XX_DEVICE_TYPE_ADC        0
#define QC7XX_DEVICE_TYPE_DAC        1
#define QC7XX_DEVICE_TYPE_AUDIOADC   2
#define QC7XX_DEVICE_TYPE_AUDIODAC   3
#define QC7XX_DEVICE_TYPE_GPIO       4
#define QC7XX_DEVICE_TYPE_UART       5
#define QC7XX_DEVICE_TYPE_SPI        6
#define QC7XX_DEVICE_TYPE_I2C        7
#define QC7XX_DEVICE_TYPE_DMA        8
#define QC7XX_DEVICE_TYPE_I2S        9
#define QC7XX_DEVICE_TYPE_IR         10
#define QC7XX_DEVICE_TYPE_TIMER      11
#define QC7XX_DEVICE_TYPE_PWM        12
#define QC7XX_DEVICE_TYPE_CAMERA     14
#define QC7XX_DEVICE_TYPE_FLASH      15
#define QC7XX_DEVICE_TYPE_QSPI       16
#define QC7XX_DEVICE_TYPE_SDH        17
#define QC7XX_DEVICE_TYPE_SDU        18
#define QC7XX_DEVICE_TYPE_ETH        19
#define QC7XX_DEVICE_TYPE_RTC        20
#define QC7XX_DEVICE_TYPE_CRC        21
#define QC7XX_DEVICE_TYPE_RNG        22
#define QC7XX_DEVICE_TYPE_MIPI       23
#define QC7XX_DEVICE_TYPE_DPI        24
#define QC7XX_DEVICE_TYPE_DSI        25
#define QC7XX_DEVICE_TYPE_CSI        26
#define QC7XX_DEVICE_TYPE_USB        27
#define QC7XX_DEVICE_TYPE_SEC_AES    28
#define QC7XX_DEVICE_TYPE_SEC_SHA    29
#define QC7XX_DEVICE_TYPE_SEC_MD5    30
#define QC7XX_DEVICE_TYPE_SEC_TRNG   31
#define QC7XX_DEVICE_TYPE_SEC_PKA    32
#define QC7XX_DEVICE_TYPE_CKS        33
#define QC7XX_DEVICE_TYPE_MJPEG      34
#define QC7XX_DEVICE_TYPE_KYS        35
#define QC7XX_DEVICE_TYPE_DBI        36
#define QC7XX_DEVICE_TYPE_WDT        37
#define QC7XX_DEVICE_TYPE_EF_CTRL    38
#define QC7XX_DEVICE_TYPE_SDIO2      39
#define QC7XX_DEVICE_TYPE_SDIO3      40
#define QC7XX_DEVICE_TYPE_PLFMDMA    41
#define QC7XX_DEVICE_TYPE_WO         42
#define QC7XX_DEVICE_TYPE_GMAC       43
#define QC7XX_DEVICE_TYPE_IPC        44
#define QC7XX_DEVICE_TYPE_MJDEC      45
#define QC7XX_DEVICE_TYPE_PSRAM      46
#define QC7XX_DEVICE_TYPE_TOUCH      47
#define QC7XX_DEVICE_TYPE_CAN        48
#define QC7XX_DEVICE_TYPE_PEC        49
#define QC7XX_DEVICE_TYPE_EMAC_V2    50
#define QC7XX_DEVICE_TYPE_DVP_RASTER 51

struct qc7xx_device_s {
    const char *name;
    uint32_t reg_base;
    uint8_t irq_num;
    uint8_t idx;
    uint8_t sub_idx;
    uint8_t dev_type;
    void *user_data;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get device handle by name.
 *
 * @param [in] name device name
 * @return device handle
 */
struct qc7xx_device_s *qc7xx_device_get_by_name(const char *name);

/**
 * @brief Set user data into device handle.
 *
 * @param [in] device device handle
 * @param [in] user_data pointer to user data
 */
void qc7xx_device_set_userdata(struct qc7xx_device_s *device, void *user_data);

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
