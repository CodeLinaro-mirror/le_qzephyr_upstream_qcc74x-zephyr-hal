/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qcc74x_glb.h"

/* vectors.S must dispatch every IRQ through this single LPFW handler. */
#if !QCC74x_WIFI_LP_FW
#error "LPFW requires single-handler IRQ dispatch"
#endif


uint32_t CPU_Interrupt_En_Get(uint32_t irq_num)
{
    return csi_vic_get_enabled_irq(irq_num);
}

uint32_t CPU_Interrupt_Pending_Get(uint32_t irq_num)
{
    return csi_vic_get_pending_irq(irq_num);
}

typedef void (*pFunc)(void);

pFunc g_lpfw_irqvector[1] __attribute__((aligned(64)));

void lpfw_Interrupt_Handler_Register(pFunc interruptFun)
{
    g_lpfw_irqvector[0] = interruptFun;
}
