/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <qcc74x_glb.h>
#include <qcc743_timer.h>

#define QC7XX_WDT_ID         WDT0_ID
#define QC7XX_WDT_VAL_MAX    (65535)

static void Timer_Watchdog_Case(int ms)
{
    WDT_Disable(QC7XX_WDT_ID);

    GLB_PER_Clock_UnGate(GLB_AHB_CLOCK_TIMER);
    WDT_Set_Clock(QC7XX_WDT_ID, TIMER_CLKSRC_32K, 31);

    WDT_SetCompValue(QC7XX_WDT_ID, 1 * ms);
    WDT_ResetCounterValue(QC7XX_WDT_ID);
    WDT_IntMask(QC7XX_WDT_ID, WDT_INT, MASK);

    WDT_Enable(QC7XX_WDT_ID);
}

void lp_wdt_feed(void)
{
    WDT_ResetCounterValue(QC7XX_WDT_ID);
}

void lp_wdt_disable(void)
{
    WDT_Disable(QC7XX_WDT_ID);
}

int lp_wdt_init(int ms)
{
    if (ms >= QC7XX_WDT_VAL_MAX) {
        return -1;
    }
    Timer_Watchdog_Case(ms);
    return 0;
}
