/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_clock.h"
#if defined(QCC74X)
#include "qcc74x_clock.h"
#elif defined(QCC75X)
#include "qcc75x_clock.h"
#endif

/****************************************************************************/ /**
 * @brief  get peri clock
 *
 * @param  peri: QC7XX_PERIPHERAL_xxx
 *
 * @return int
 *
*******************************************************************************/
uint32_t qc7xx_peripheral_clock_get(uint8_t peri)
{
    return qc7xx_peripheral_clock_get_by_id(peri);
}

/****************************************************************************/ /**
 * @brief  enable/disable peri clock
 *
 * @param  peri: QC7XX_PERIPHERAL_xxx
 *
 * @param  enable: Boolean value to enable or disable the clock
 * @return int
 *
*******************************************************************************/
int ATTR_CLOCK_SECTION qc7xx_peripheral_clock_control(uint8_t peri, bool enable)
{
    return qc7xx_peripheral_clock_control_by_id(peri, enable);
}

/****************************************************************************/ /**
 * @brief  get peri clock status
 *
 * @param  peri: QC7XX_PERIPHERAL_xxx
 *
 * @return int
 *
*******************************************************************************/
int qc7xx_peripheral_clock_status_get(uint8_t peri)
{
    return qc7xx_peripheral_clock_status_get_by_id(peri);
}

