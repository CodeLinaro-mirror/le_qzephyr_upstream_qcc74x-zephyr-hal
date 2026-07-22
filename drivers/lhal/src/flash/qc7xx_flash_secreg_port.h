/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_FLASH_SECREG_PORT_H
#define _QC7XX_FLASH_SECREG_PORT_H

#include <stdint.h>
#include <stdbool.h>
#include "qc7xx_flash.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/** @defgroup FLASH_SECREG_API flash secreg api definition
  * @{
  */
#define QC7XX_FLASH_SECREG_API_TYPE_GENERAL 0U
#define QC7XX_FLASH_SECREG_API_TYPE_ISSI    1U
#define QC7XX_FLASH_SECREG_API_TYPE_MXIC    2U
/**
  * @}
  */

#if defined(QCC74X)
#define QC7XX_SF_CTRL_BUF_BASE ((uint32_t)0x2000B600)
#elif defined(QCC75X)
#define QC7XX_SF_CTRL_BUF_BASE ((uint32_t)0x20082600)
#endif

struct flash_secreg_api {
    int (*read)(const spi_flash_cfg_type *flash_cfg, uint32_t address, void *data, uint32_t len);
    int (*write)(const spi_flash_cfg_type *flash_cfg, uint32_t address, const void *data, uint32_t len);
    int (*erase)(const spi_flash_cfg_type *flash_cfg, uint32_t address);
    int (*get_lock)(const qc7xx_flash_secreg_param_t *param, uint8_t *lb);
    int (*set_lock)(const spi_flash_cfg_type *flash_cfg, const qc7xx_flash_secreg_param_t *param, uint8_t lb);
};

extern const struct flash_secreg_api flash_secreg_apis[];

#endif
