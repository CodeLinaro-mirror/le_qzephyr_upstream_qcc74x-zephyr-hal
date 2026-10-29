/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "qc7xx_irq.h"
#include "qcc74x_pm.h"
#include "lpfw_printf.h"
#include "qcc74x_glb.h"

void vApplicationMallocFailedHook()
{

}

void lp_fw_main();
int bfl_main(void)
{
    lpfw_printf_debug2("\r\n\r\n-> qcc74x sdk LPFW is booting...\r\n");
    lp_fw_main();
    return 0;
}
