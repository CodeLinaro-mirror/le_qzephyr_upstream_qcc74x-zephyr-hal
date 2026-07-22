/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_reset.h"
#if defined(QCC74X)
#include "qcc74x_reset.h"
#elif defined(QCC75X)
#include "qcc75x_reset.h"
#endif
/****************************************************************************/ /**
 * @brief  reset peri
 *
 * @param  peri: QC7XX_PERIPHERAL_xxx
 *
 * @return int
 *
*******************************************************************************/
int qc7xx_peripheral_reset(uint8_t peri)
{
    return qc7xx_peripheral_reset_by_id(peri);
}