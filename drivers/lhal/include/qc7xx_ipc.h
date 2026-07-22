/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_IPC_H_
#define _QC7XX_IPC_H_

#include "qc7xx_core.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup IPC
  * @{
  */

#define IPC_BITS_MAX   (32)
#define IPC_BITS_ALL   (0xffffffff)
#define IPC_BIT_NUM(n) ((0x01 << n) & IPC_BITS_ALL)

#ifdef __cplusplus
extern "C" {
#endif

void qc7xx_ipc_init(struct qc7xx_device_s *dev);
void qc7xx_ipc_deinit(struct qc7xx_device_s *dev);

void qc7xx_ipc_int_mask(struct qc7xx_device_s *dev, uint32_t ipc_bits);
void qc7xx_ipc_int_unmask(struct qc7xx_device_s *dev, uint32_t ipc_bits);

void qc7xx_ipc_trig(struct qc7xx_device_s *dev, uint32_t ipc_bits);
void qc7xx_ipc_clear(struct qc7xx_device_s *dev, uint32_t ipc_bits);

uint32_t qc7xx_ipc_get_sta(struct qc7xx_device_s *dev);
uint32_t qc7xx_ipc_get_intsta(struct qc7xx_device_s *dev);

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