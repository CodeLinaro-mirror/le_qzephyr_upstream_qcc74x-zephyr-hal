/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_CKS_H
#define _QC7XX_CKS_H

#include "qc7xx_core.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup CKS
  * @{
  */

/** @defgroup CKS_ENDIAN cks endian definition
  * @{
  */
#define CKS_LITTLE_ENDIAN 0
#define CKS_BIG_ENDIAN    1
/**
  * @}
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Reset checksum module.
 *
 * @param [in] dev device handle
 */
void qc7xx_cks_reset(struct qc7xx_device_s *dev);

/**
 * @brief Set checksum bitorder.
 *
 * @param [in] dev device handle
 * @param [in] endian cks endian, use @ref CKS_ENDIAN
 */
void qc7xx_cks_set_endian(struct qc7xx_device_s *dev, uint8_t endian);

/**
 * @brief Compute data with checksum.
 *
 * @param [in] dev device handle
 * @param [in] data input data buffer
 * @param [in] length data length
 * @return checksum value
 */
uint16_t qc7xx_cks_compute(struct qc7xx_device_s *dev, uint8_t *data, uint32_t length);

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
