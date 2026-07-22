/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCC74X_RESET_H__
#define __QCC74X_RESET_H__

#include "qc7xx_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup  QCC74X_Peripheral_Driver
 *  @{
 */

/** @addtogroup  RESET
 *  @{
 */

/** @defgroup  RESET_Public_Functions
 *  @{
 */

/**
 * @brief  reset peri by id
 *
 * @param  peri: QC7XX_PERIPHERAL_xxx
 * @return int
 */
int qc7xx_peripheral_reset_by_id(uint8_t peri);

/*@} end of group RESET_Public_Functions */

/*@} end of group RESET */

/*@} end of group QCC74X_Peripheral_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __QCC74X_RESET_H__ */
