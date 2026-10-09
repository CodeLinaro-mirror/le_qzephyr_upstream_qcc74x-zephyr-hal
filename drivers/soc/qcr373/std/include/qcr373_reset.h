/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCR373_RESET_H__
#define __QCR373_RESET_H__

#include "qc7xx_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup  QCR373_Peripheral_Driver
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
 * @param  peri: QCR373_PERIPHERAL_xxx
 * @return int
 */
int qcr373_peripheral_reset_by_id(uint8_t peri);

/*@} end of group RESET_Public_Functions */

/*@} end of group RESET */

/*@} end of group QCR373_Peripheral_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __QCR373_RESET_H__ */
