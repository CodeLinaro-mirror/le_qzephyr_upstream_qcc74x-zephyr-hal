/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qcc74x_glb_gpio.h"

void qc7xx_gpio_pad_check(uint8_t pin)
{
    if (GLB_GPIO_Pad_LeadOut_Sts(pin) == RESET) {
        // throw exception
        asm("ebreak");
        while(1);
    }
}