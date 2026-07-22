/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_SEC_IRQ_H
#define _QC7XX_SEC_IRQ_H

#include "qc7xx_core.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup SEC_IRQ
  * @{
  */

#define QC7XX_SEC_ENG_IRQ_TYPE_AES  0
#define QC7XX_SEC_ENG_IRQ_TYPE_SHA  1
#define QC7XX_SEC_ENG_IRQ_TYPE_PKA  2
#define QC7XX_SEC_ENG_IRQ_TYPE_TRNG 3
#define QC7XX_SEC_ENG_IRQ_TYPE_GMAC 4
#define QC7XX_SEC_ENG_IRQ_TYPE_CDET 5

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *
 * @param [in] sec_type
 * @param [in] callback
 * @param [in] arg
 */
void qc7xx_sec_irq_attach(uint8_t sec_type, void (*callback)(void *arg), void *arg);

/**
 * @brief
 *
 * @param [in] sec_type
 */
void qc7xx_sec_irq_detach(uint8_t sec_type);

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