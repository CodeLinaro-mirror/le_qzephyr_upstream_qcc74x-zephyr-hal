/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_mtimer.h"
#include "qc7xx_core.h"
#include <csi_core.h>

static void (*systick_callback)(void);
static uint64_t current_set_ticks = 0;

static void systick_isr(int irq, void *arg)
{
    csi_coret_config(current_set_ticks, 7);
    systick_callback();
}

void qc7xx_mtimer_config(uint64_t ticks, void (*interruptfun)(void))
{
    qc7xx_irq_disable(7);

    current_set_ticks = ticks;
    systick_callback = interruptfun;
    csi_coret_config_use(ticks, 7);

    qc7xx_irq_attach(7, systick_isr, NULL);
    qc7xx_irq_enable(7);
}

__WEAK uint32_t ATTR_TCM_SECTION qc7xx_mtimer_get_freq(void)
{
    return 1 * 1000 * 1000;
}

uint64_t ATTR_TCM_SECTION qc7xx_mtimer_get_time_us(void)
{
    volatile uint32_t timeout = 0;
#ifdef romapi_qc7xx_mtimer_get_time_us
    return romapi_qc7xx_mtimer_get_time_us();
#else
    volatile uint64_t tmp_low, tmp_high, tmp_low1, tmp_high1;

    do {
        tmp_high = (uint64_t)csi_coret_get_valueh();
        tmp_low = (uint64_t)csi_coret_get_value();
        tmp_low1 = (uint64_t)csi_coret_get_value();
        tmp_high1 = (uint64_t)csi_coret_get_valueh();
        timeout++;
        if (timeout == 1000) {
            return 0;
        }
    } while (tmp_low > tmp_low1 || tmp_high != tmp_high1);
#ifdef CONFIG_MTIMER_CUSTOM_FREQUENCE
    return ((uint64_t)(((tmp_high1 << 32) + tmp_low1)) * ((uint64_t)(1 * 1000 * 1000)) / qc7xx_mtimer_get_freq());
#else
    return (uint64_t)(((tmp_high1 << 32) + tmp_low1));
#endif
#endif
}

uint32_t ATTR_TCM_SECTION __attribute__((weak)) __div64_32(uint64_t *n, uint32_t base)
{
    uint64_t rem = *n;
    uint64_t b = base;
    uint64_t res, d = 1;
    uint32_t high = rem >> 32;

    res = 0;
    if (high >= base) {
        high /= base;
        res = (uint64_t)high << 32;
        rem -= (uint64_t)(high * base) << 32;
    }
    while ((int64_t)b > 0 && b < rem) {
        b = b + b;
        d = d + d;
    }

    do {
        if (rem >= b) {
            rem -= b;
            res += d;
        }
        b >>= 1;
        d >>= 1;
    } while (d);

    *n = res;
    return rem;
}

uint64_t ATTR_TCM_SECTION qc7xx_mtimer_get_time_ms(void)
{
#ifdef romapi_qc7xx_mtimer_get_time_ms
    return romapi_qc7xx_mtimer_get_time_ms();
#else
#ifdef QC7XX_BOOT2
    uint64_t ret = qc7xx_mtimer_get_time_us();
    __div64_32(&ret, 1000);
    return ret;
#else
    return qc7xx_mtimer_get_time_us() / 1000;
#endif
#endif
}

void ATTR_TCM_SECTION qc7xx_mtimer_delay_us(uint32_t time)
{
#ifdef romapi_qc7xx_mtimer_delay_us
    return romapi_qc7xx_mtimer_delay_us(time);
#else
    uint64_t start_time = qc7xx_mtimer_get_time_us();

    while (qc7xx_mtimer_get_time_us() - start_time < time) {
    }
#endif
}

void ATTR_TCM_SECTION qc7xx_mtimer_delay_ms(uint32_t time)
{
#ifdef romapi_qc7xx_mtimer_delay_ms
    return romapi_qc7xx_mtimer_delay_ms(time);
#else
    uint64_t start_time = qc7xx_mtimer_get_time_us();

    while (qc7xx_mtimer_get_time_us() - start_time < ((uint64_t)time * 1000)) {
    }
#endif
}

