/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_RESET_H
#define _QC7XX_RESET_H

#include "qc7xx_core.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup RESET
  * @{
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  reset peri
 *
 * @param  peri: QC7XX_PERIPHERAL_xxx
 * @return int
 */
int qc7xx_peripheral_reset(uint8_t peri);

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