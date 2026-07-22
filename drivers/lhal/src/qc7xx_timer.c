/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_timer.h"
#include "qc7xx_gpio.h"
#include "hardware/timer_reg.h"
#if defined(QCC74X)
#include "qcc74x_glb.h"
#elif defined(QCC75X)
#include "qcc75x_glb.h"
#endif

void qc7xx_timer_init(struct qc7xx_device_s *dev, const struct qc7xx_timer_config_s *config)
{
    LHAL_PARAM_ASSERT(dev);
    LHAL_PARAM_ASSERT(IS_TIMER_COUNTER_MODE(config->counter_mode));
    LHAL_PARAM_ASSERT(IS_TIMER_CLK_SOURCE(config->clock_source));
    LHAL_PARAM_ASSERT(IS_TIMER_CLOCK_DIV(config->clock_div));
    LHAL_PARAM_ASSERT(IS_TIMER_COMP_ID(config->trigger_comp_id));

    if (!IS_TIMER_COMP_VAL(config->comp0_val) ||
        !IS_TIMER_COMP_VAL(config->comp1_val) ||
        !IS_TIMER_COMP_VAL(config->comp2_val)) {
        qc7xx_lhal_assert_func(__FILE__, __LINE__, __func__, "comp value is error");
    }

#ifdef romapi_qc7xx_timer_init
    romapi_qc7xx_timer_init(dev, config);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;

    /* Disable timer */
    regval = getreg32(reg_base + TIMER_TCER_OFFSET);
    regval &= ~(1 << (dev->idx + 1));
    putreg32(regval, reg_base + TIMER_TCER_OFFSET);

    /* Timer interrupr clear */
    qc7xx_timer_compint_clear(dev, TIMER_COMP_ID_0);
    qc7xx_timer_compint_clear(dev, TIMER_COMP_ID_1);
    qc7xx_timer_compint_clear(dev, TIMER_COMP_ID_2);

    /* Configure clock source */
    regval = getreg32(reg_base + TIMER_TCCR_OFFSET);
    if (dev->idx == 0) {
        regval &= ~TIMER_CS_0_MASK;
        regval |= (config->clock_source << TIMER_CS_0_SHIFT);
    } else {
        regval &= ~TIMER_CS_1_MASK;
        regval |= (config->clock_source << TIMER_CS_1_SHIFT);
    }
    putreg32(regval, reg_base + TIMER_TCCR_OFFSET);

    /* Configure clock div */
    regval = getreg32(reg_base + TIMER_TCDR_OFFSET);
    regval &= ~(0xff << (TIMER_TCDR0_SHIFT + 8 * dev->idx));
    regval |= (config->clock_div << (TIMER_TCDR0_SHIFT + 8 * dev->idx));
    putreg32(regval, reg_base + TIMER_TCDR_OFFSET);

    /* Configure counter mode */
    regval = getreg32(reg_base + TIMER_TCMR_OFFSET);
    if (config->counter_mode == TIMER_COUNTER_MODE_PROLOAD) {
        regval &= ~(1 << (1 + dev->idx));
    } else {
        regval |= (1 << (1 + dev->idx));
    }
    putreg32(regval, reg_base + TIMER_TCMR_OFFSET);

    /* Configure preload trigger source */
    regval = getreg32(reg_base + TIMER_TPLCR0_OFFSET + 4 * dev->idx);
    regval &= ~TIMER_TPLCR0_MASK;
    if (config->trigger_comp_id != TIMER_COMP_NONE) {
        regval |= ((config->trigger_comp_id + 1) << TIMER_TPLCR0_SHIFT);
    }
    putreg32(regval, reg_base + TIMER_TPLCR0_OFFSET + 4 * dev->idx);

    if (config->counter_mode == TIMER_COUNTER_MODE_PROLOAD) {
        qc7xx_timer_set_preloadvalue(dev, config->preload_val);
    }

    if (config->trigger_comp_id < TIMER_COMP_ID_1) {
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_0, false);
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_1, true);
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_2, true);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_0, config->comp0_val - 2);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_1, 0xffffffff);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_2, 0xffffffff);
    } else if (config->trigger_comp_id < TIMER_COMP_ID_2) {
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_0, false);
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_1, false);
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_2, true);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_0, config->comp0_val - 2);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_1, config->comp1_val - 2);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_2, 0xffffffff);
    } else if (config->trigger_comp_id < TIMER_COMP_NONE) {
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_0, false);
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_1, false);
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_2, false);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_0, config->comp0_val - 2);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_1, config->comp1_val - 2);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_2, config->comp2_val - 2);
    } else {
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_0, true);
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_1, true);
        qc7xx_timer_compint_mask(dev, TIMER_COMP_ID_2, true);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_0, 0xffffffff);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_1, 0xffffffff);
        qc7xx_timer_set_compvalue(dev, TIMER_COMP_ID_2, 0xffffffff);
    }
#endif
}

void qc7xx_timer_deinit(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_timer_deinit
    romapi_qc7xx_timer_deinit(dev);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;

    regval = getreg32(reg_base + TIMER_TCER_OFFSET);
    regval &= ~(1 << (dev->idx + 1));
    putreg32(regval, reg_base + TIMER_TCER_OFFSET);
#endif
}

void qc7xx_timer_start(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_timer_start
    romapi_qc7xx_timer_start(dev);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;

    regval = getreg32(reg_base + TIMER_TCER_OFFSET);
    regval |= (1 << (dev->idx + 1));
    putreg32(regval, reg_base + TIMER_TCER_OFFSET);
#endif
}

void qc7xx_timer_stop(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_timer_stop
    romapi_qc7xx_timer_stop(dev);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;

    regval = getreg32(reg_base + TIMER_TCER_OFFSET);
    regval &= ~(1 << (dev->idx + 1));
    putreg32(regval, reg_base + TIMER_TCER_OFFSET);
#endif
}

void qc7xx_timer_set_preloadvalue(struct qc7xx_device_s *dev, uint32_t val)
{
#ifdef romapi_qc7xx_timer_set_preloadvalue
    romapi_qc7xx_timer_set_preloadvalue(dev, val);
#else
    uint32_t reg_base;

    reg_base = dev->reg_base;

    putreg32(val, reg_base + TIMER_TPLVR0_OFFSET + 4 * dev->idx);
#endif
}

void qc7xx_timer_set_compvalue(struct qc7xx_device_s *dev, uint8_t cmp_no, uint32_t val)
{
#ifdef romapi_qc7xx_timer_set_compvalue
    romapi_qc7xx_timer_set_compvalue(dev, cmp_no, val);
#else
    uint32_t reg_base;

    reg_base = dev->reg_base;

    putreg32(val, reg_base + TIMER_TMR0_0_OFFSET + 0x0c * dev->idx + 4 * cmp_no);
#endif
}

uint32_t qc7xx_timer_get_compvalue(struct qc7xx_device_s *dev, uint8_t cmp_no)
{
#ifdef romapi_qc7xx_timer_get_compvalue
    return romapi_qc7xx_timer_get_compvalue(dev, cmp_no);
#else
    uint32_t reg_base;

    reg_base = dev->reg_base;

    return getreg32(reg_base + TIMER_TMR0_0_OFFSET + 0x0c * dev->idx + 4 * cmp_no);
#endif
}

uint32_t qc7xx_timer_get_countervalue(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_timer_get_countervalue
    return romapi_qc7xx_timer_get_countervalue(dev);
#else
    uint32_t reg_base;

    reg_base = dev->reg_base;

    return getreg32(reg_base + TIMER_TCR0_OFFSET + 4 * dev->idx);
#endif
}

void qc7xx_timer_compint_mask(struct qc7xx_device_s *dev, uint8_t cmp_no, bool mask)
{
#ifdef romapi_qc7xx_timer_compint_mask
    return romapi_qc7xx_timer_compint_mask(dev, cmp_no, mask);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + TIMER_TIER0_OFFSET + 4 * dev->idx);
    if (mask) {
        regval &= ~(1 << cmp_no);
    } else {
        regval |= (1 << cmp_no);
    }
    putreg32(regval, reg_base + TIMER_TIER0_OFFSET + 4 * dev->idx);
#endif
}

bool qc7xx_timer_get_compint_status(struct qc7xx_device_s *dev, uint8_t cmp_no)
{
#ifdef romapi_qc7xx_timer_get_compint_status
    return romapi_qc7xx_timer_get_compint_status(dev, cmp_no);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + TIMER_TSR0_OFFSET + 4 * dev->idx);

    if (regval & (1 << cmp_no)) {
        return true;
    } else {
        return false;
    }
#endif
}

void qc7xx_timer_compint_clear(struct qc7xx_device_s *dev, uint8_t cmp_no)
{
#ifdef romapi_qc7xx_timer_compint_clear
    romapi_qc7xx_timer_compint_clear(dev, cmp_no);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + TIMER_TICR0_OFFSET + 4 * dev->idx);
    regval |= (1 << cmp_no);
    putreg32(regval, reg_base + TIMER_TICR0_OFFSET + 4 * dev->idx);
#endif
}

int qc7xx_timer_feature_control(struct qc7xx_device_s *dev, int cmd, size_t arg)
{
#ifdef romapi_qc7xx_timer_feature_control
    return romapi_qc7xx_timer_feature_control(dev, cmd, arg);
#else
    int ret = 0;

    switch (cmd) {

        default:
            ret = -EPERM;
            break;
    }

    return ret;
#endif
}

#if defined(QCC74X) || defined(QCC75X)
void qc7xx_timer_capture_init(struct qc7xx_device_s *dev, const struct qc7xx_timer_capture_config_s *config)
{
#ifdef romapi_qc7xx_timer_capture_init
    romapi_qc7xx_timer_capture_init(dev, config);
#else
    uint32_t regval;
    uint32_t reg_base;
    struct qc7xx_device_s *gpio;

    reg_base = dev->reg_base;
    GLB_Sel_MCU_TMR_GPIO_Clock(config->pin);
    gpio = qc7xx_device_get_by_name("gpio");
    qc7xx_gpio_init(gpio, config->pin, GPIO_FUNC_CLKOUT | GPIO_ALTERNATE | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_1);

    regval = getreg32(reg_base + TIMER_GPIO_OFFSET);
    /* polarity: 1->neg, 0->pos */
    if (config->polarity == TIMER_GPIO_PULSE_POLARITY_NEGATIVE) {
        regval |= (1 << (5 + dev->idx));
    } else {
        regval &= ~(1 << (5 + dev->idx));
    }
    regval |= TIMER0_GPIO_EN;
    putreg32(regval, reg_base + TIMER_GPIO_OFFSET);
#endif
}
#endif