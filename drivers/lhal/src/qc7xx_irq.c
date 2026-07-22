/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_irq.h"
#include "qc7xx_core.h"
#include <csi_core.h>

#if defined(QCC75X)
#include "qcc75x_irq.h"
#include "qcc75x_glb.h"
#endif

#ifndef QC7XX_IOT_SDK
extern struct qc7xx_irq_info_s g_irqvector[];

static void irq_unexpected_isr(int irq, void *arg)
{
    printf("irq :%d unregistered\r\n", irq);
}
#endif
void qc7xx_irq_initialize(void)
{
#ifndef QC7XX_IOT_SDK
    int i;

    /* Point all interrupt vectors to the unexpected interrupt */
    for (i = 0; i < CONFIG_IRQ_NUM; i++) {
        g_irqvector[i].handler = irq_unexpected_isr;
        g_irqvector[i].arg = NULL;
    }
#endif
}

ATTR_TCM_SECTION uintptr_t qc7xx_irq_save(void)
{
#ifdef romapi_qc7xx_irq_save
    return romapi_qc7xx_irq_save();
#else
    uintptr_t oldstat;

    /* Read mstatus & clear machine interrupt enable (MIE) in mstatus */

    asm volatile("csrrc %0, mstatus, %1"
                 : "=r"(oldstat)
                 : "r"(MSTATUS_MIE));
    return oldstat;
#endif
}

ATTR_TCM_SECTION void qc7xx_irq_restore(uintptr_t flags)
{
#ifdef romapi_qc7xx_irq_restore
    romapi_qc7xx_irq_restore(flags);
#else
    /* Write flags to mstatus */

    asm volatile("csrw mstatus, %0"
                 : /* no output */
                 : "r"(flags));
#endif
}

#if (defined(QCC75X) && defined(CPU_LP))
void qcc75x_level2_irq_handler(int irq, void *arg0)
{
    struct qc7xx_irq_info_s *vector;
    uint64_t level2_irq_state = 0;

    level2_irq_state = GLB_Get_NP2MINI_Interrupt_Status();

    const uint32_t base = QCC75X_IRQ_LEVEL2_BASE + 1;
    while (level2_irq_state) {
        uint32_t i = __builtin_ctzll(level2_irq_state);
        level2_irq_state &= ~(1UL << i);
        const uint32_t irq_num = base + i;
        vector = &g_irqvector[irq_num];
        if (vector->handler) {
            vector->handler(irq_num, vector->arg);
            GLB_Set_NP2MINI_Interrupt_Clear(irq_num);
        } else {
            qc7xx_lhal_assert_func(__FILE__, __LINE__, __FUNCTION__, "handler is NULL");
        }
    }
}
#endif

int qc7xx_irq_attach(int irq, irq_callback isr, void *arg)
{
    if (irq >= CONFIG_IRQ_NUM) {
        return -EINVAL;
    }
#ifndef QC7XX_IOT_SDK
#if (defined(QCC75X) && defined(CPU_LP))
    if (irq >= QCC75X_IRQ_LEVEL2_BASE) {
        g_irqvector[QCC75X_IRQ_LEVEL2_BASE].handler = qcc75x_level2_irq_handler;
        g_irqvector[QCC75X_IRQ_LEVEL2_BASE].arg = arg;
    }
#endif
    g_irqvector[irq].handler = isr;
    g_irqvector[irq].arg = arg;
#else
    extern void qc7xx_irq_register_with_ctx(int irqnum, void *handler, void *ctx);
    qc7xx_irq_register_with_ctx(irq, (void *)isr, arg);
#endif
    return 0;
}

int qc7xx_irq_detach(int irq)
{
    if (irq >= CONFIG_IRQ_NUM) {
        return -EINVAL;
    }
#ifndef QC7XX_IOT_SDK
    g_irqvector[irq].handler = irq_unexpected_isr;
    g_irqvector[irq].arg = NULL;
#endif
    return 0;
}

void qc7xx_irq_enable(int irq)
{
#ifdef romapi_qc7xx_irq_enable
    romapi_qc7xx_irq_enable(irq);
#else
#if (0) && defined(CPU_D0)
    if (csi_vic_get_prio(irq) == 0) {
        csi_vic_set_prio(irq, 1);
    }
#endif
#if (defined(QCC75X) && defined(CPU_LP))
    if (irq >= QCC75X_IRQ_LEVEL2_BASE) {
        csi_vic_enable_irq(QCC75X_IRQ_LEVEL2_BASE);
    } else {
        csi_vic_enable_irq(irq);
    }
#else
    csi_vic_enable_irq(irq);
#endif
#endif
}

void qc7xx_irq_disable(int irq)
{
#ifdef romapi_qc7xx_irq_disable
    romapi_qc7xx_irq_disable(irq);
#else
#if (defined(QCC75X) && defined(CPU_LP))
    if (irq >= QCC75X_IRQ_LEVEL2_BASE) {
        csi_vic_disable_irq(QCC75X_IRQ_LEVEL2_BASE);
    } else {
        csi_vic_disable_irq(irq);
    }
#else
    csi_vic_disable_irq(irq);
#endif
#endif
}

void qc7xx_irq_set_pending(int irq)
{
#ifdef romapi_qc7xx_irq_set_pending
    romapi_qc7xx_irq_set_pending(irq);
#else
#if (defined(QCC75X) && defined(CPU_LP))
    if (irq >= QCC75X_IRQ_LEVEL2_BASE) {
        csi_vic_set_pending_irq(QCC75X_IRQ_LEVEL2_BASE);
    } else {
        csi_vic_set_pending_irq(irq);
    }
#else
    csi_vic_set_pending_irq(irq);
#endif
#endif
}

void qc7xx_irq_clear_pending(int irq)
{
#ifdef romapi_qc7xx_irq_clear_pending
    romapi_qc7xx_irq_clear_pending(irq);
#else
#if (defined(QCC75X) && defined(CPU_LP))
    if (irq >= QCC75X_IRQ_LEVEL2_BASE) {
        csi_vic_clear_pending_irq(QCC75X_IRQ_LEVEL2_BASE);
    } else {
        csi_vic_clear_pending_irq(irq);
    }
#else
    csi_vic_clear_pending_irq(irq);
#endif
#endif
}

void qc7xx_irq_set_nlbits(uint8_t nlbits)
{
#ifdef romapi_qc7xx_irq_set_nlbits
    romapi_qc7xx_irq_set_nlbits(nlbits);
#else
#if !defined(CPU_D0)
    CLIC->CLICCFG = ((nlbits & 0xf) << 1) | 1;
#endif
#endif
}

void qc7xx_irq_set_priority(int irq, uint8_t preemptprio, uint8_t subprio)
{
    if (irq >= CONFIG_IRQ_NUM) {
        return;
    }
#ifdef romapi_qc7xx_irq_set_priority
    romapi_qc7xx_irq_set_priority(irq, preemptprio, subprio);
#else
#if (defined(QCC75X) && defined(CPU_LP))
    if (irq >= QCC75X_IRQ_LEVEL2_BASE) {
        csi_vic_set_prio(QCC75X_IRQ_LEVEL2_BASE, preemptprio);
    } else {
        csi_vic_set_prio(irq, preemptprio);
    }
#else
    csi_vic_set_prio(irq, preemptprio);
#endif
#endif
}
