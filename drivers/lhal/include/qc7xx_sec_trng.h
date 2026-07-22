/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_SEC_TRNG_H
#define _QC7XX_SEC_TRNG_H

#include "qc7xx_core.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup TRNG
  * @{
  */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Read trng data.
 *
 * @param [in] dev device handle
 * @param [in] data pointer to trng data
 * @return A negated errno value on failure.
 */
int qc7xx_trng_read(struct qc7xx_device_s *dev, uint8_t data[32]);

/**
 * @brief Read trng data with custom length.
 *
 * @param [in] data pointer to trng data
 * @param [in] len length to read
 * @return A negated errno value on failure.
 */
int qc7xx_trng_readlen(uint8_t *data, uint32_t len);

/**
 * @brief Get trng data.
 *
 * @return trng data with word
 */
long random(void);

/**
 * @brief Enable trng in group0.
 *
 * @param [in] dev device handle
 */
void qc7xx_group0_request_trng_access(struct qc7xx_device_s *dev);

/**
 * @brief Disable trng in group0.
 *
 * @param [in] dev device handle
 */
void qc7xx_group0_release_trng_access(struct qc7xx_device_s *dev);

/**
 * @brief Enable trng in group1.
 *
 * @param [in] dev device handle
 */
void qc7xx_group1_request_trng_access(struct qc7xx_device_s *dev);

/**
 * @brief Disable trng in group1.
 *
 * @param [in] dev device handle
 */
void qc7xx_group1_release_trng_access(struct qc7xx_device_s *dev);


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