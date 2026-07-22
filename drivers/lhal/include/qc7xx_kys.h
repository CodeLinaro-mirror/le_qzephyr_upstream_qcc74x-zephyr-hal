/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_KYS_H
#define _QC7XX_KYS_H

#include "qc7xx_core.h"

struct qc7xx_kys_config_s {
    uint8_t col;           /* Col of keyboard,max:8 */
    uint8_t row;           /* Row of keyboard,max:8 */
    uint8_t deglitch_en;   /* Disable deglitch function */
    uint8_t deglitch_cnt;  /* Deglitch count */
    uint8_t idle_duration; /* Idle duration between column scans */
    uint8_t ghost_en;      /* Disable ghost key event detection */
};

/* keyscan interrupt enable define */
#define KEYSCAN_INT_EN_DONE          (0x1 << 7)
#define KEYSCAN_INT_EN_FIFOFULL      (0x1 << 8)
#define KEYSCAN_INT_EN_FIFOHALF      (0x1 << 9)
#define KEYSCAN_INT_EN_FIFOQUARTER   (0x1 << 10)
#define KEYSCAN_INT_EN_FIFO_NONEMPTY (0x1 << 11)
#define KEYSCAN_INT_EN_GHOST         (0x1 << 12)

/* keyscan interrupt clear */

#ifdef __cplusplus
extern "C" {
#endif

void qc7xx_kys_init(struct qc7xx_device_s *dev, const struct qc7xx_kys_config_s *config);
void qc7xx_kys_enable(struct qc7xx_device_s *dev);
void qc7xx_kys_disable(struct qc7xx_device_s *dev);
void qc7xx_kys_int_enable(struct qc7xx_device_s *dev, uint32_t flag, bool enable);
void qc7xx_kys_int_clear(struct qc7xx_device_s *dev, uint32_t flag);
uint32_t qc7xx_kys_get_int_status(struct qc7xx_device_s *dev);
uint8_t qc7xx_kys_read_keyvalue(struct qc7xx_device_s *dev, uint8_t index);

#ifdef __cplusplus
}
#endif

#endif
