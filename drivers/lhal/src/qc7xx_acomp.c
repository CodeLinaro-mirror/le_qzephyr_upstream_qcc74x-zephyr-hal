/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_acomp.h"
#include "qc7xx_core.h"
#include "qc7xx_gpio.h"
#include "hardware/acomp_reg.h"

#if defined(QCC74X)
#define ACOMP_BASE ((uint32_t)0x2000f000)
#endif

void qc7xx_acomp_init(uint8_t acomp_id, const struct qc7xx_acomp_config_s *config)
{
#ifdef romapi_qc7xx_acomp_init
    romapi_qc7xx_acomp_init(acomp_id, config);
#else
    uint32_t regval;
    uint32_t reg_base;

    if (AON_ACOMP0_ID == acomp_id) {
        reg_base = ACOMP_BASE + AON_ACOMP_REG_ACOMP0_CTRL_OFFSET;
    } else {
        reg_base = ACOMP_BASE + AON_ACOMP_REG_ACOMP1_CTRL_OFFSET;
    }

    /* disable ACOMPx */
    regval = getreg32(reg_base);
    regval &= ~AON_ACOMP_EN;
    putreg32(regval, reg_base);

    /* set ACOMPx config */
    if (config->mux_en) {
        regval |= AON_ACOMP_MUX_EN;
    } else {
        regval &= ~AON_ACOMP_MUX_EN;
    }
    regval &= ~AON_ACOMP_POS_SEL_MASK;
    regval |= (config->pos_chan_sel << AON_ACOMP_POS_SEL_SHIFT);
    regval &= ~AON_ACOMP_NEG_SEL_MASK;
    regval |= (config->neg_chan_sel << AON_ACOMP_NEG_SEL_SHIFT);
    regval &= ~AON_ACOMP_LEVEL_SEL_MASK;
    regval |= (config->scaling_factor << AON_ACOMP_LEVEL_SEL_SHIFT);
    regval &= ~AON_ACOMP_BIAS_PROG_MASK;
    regval |= (config->bias_prog << AON_ACOMP_BIAS_PROG_SHIFT);
    regval &= ~AON_ACOMP_HYST_SELP_MASK;
    regval |= (config->hysteresis_pos_volt << AON_ACOMP_HYST_SELP_SHIFT);
    regval &= ~AON_ACOMP_HYST_SELN_MASK;
    regval |= (config->hysteresis_neg_volt << AON_ACOMP_HYST_SELN_SHIFT);
    putreg32(regval, reg_base);

    reg_base = ACOMP_BASE + AON_ACOMP_REG_ACOMP_CFG_OFFSET;
    regval = getreg32(reg_base);
    regval &= ~AON_ACOMP_VREF_SEL_MASK;
    regval |= (config->vio_sel << AON_ACOMP_VREF_SEL_SHIFT);
    putreg32(regval, reg_base);
#endif
}

void qc7xx_acomp_enable(uint8_t acomp_id)
{
#ifdef romapi_qc7xx_acomp_enable
    romapi_qc7xx_acomp_enable(acomp_id);
#else
    uint32_t regval;
    uint32_t reg_base;

    if (AON_ACOMP0_ID == acomp_id) {
        reg_base = ACOMP_BASE + AON_ACOMP_REG_ACOMP0_CTRL_OFFSET;
    } else {
        reg_base = ACOMP_BASE + AON_ACOMP_REG_ACOMP1_CTRL_OFFSET;
    }

    regval = getreg32(reg_base);
    regval |= AON_ACOMP_EN;
    putreg32(regval, reg_base);
#endif
}

void qc7xx_acomp_disable(uint8_t acomp_id)
{
#ifdef romapi_qc7xx_acomp_disable
    romapi_qc7xx_acomp_disable(acomp_id);
#else
    uint32_t regval;
    uint32_t reg_base;

    if (AON_ACOMP0_ID == acomp_id) {
        reg_base = ACOMP_BASE + AON_ACOMP_REG_ACOMP0_CTRL_OFFSET;
    } else {
        reg_base = ACOMP_BASE + AON_ACOMP_REG_ACOMP1_CTRL_OFFSET;
    }

    /* disable ACOMPx */
    regval = getreg32(reg_base);
    regval &= ~AON_ACOMP_EN;
    putreg32(regval, reg_base);
#endif
}

uint32_t qc7xx_acomp_get_result(uint8_t acomp_id)
{
#ifdef romapi_qc7xx_acomp_get_result
    return romapi_qc7xx_acomp_get_result(acomp_id);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = ACOMP_BASE + AON_ACOMP_REG_ACOMP_CFG_OFFSET;
    regval = getreg32(reg_base);

    if (AON_ACOMP0_ID == acomp_id) {
        return (regval & AON_ACOMP0_OUT_RAW_DATA_MASK) >> AON_ACOMP0_OUT_RAW_DATA_SHIFT;
    } else {
        return (regval & AON_ACOMP1_OUT_RAW_DATA_MASK) >> AON_ACOMP1_OUT_RAW_DATA_SHIFT;
    }
#endif
}

uint32_t qc7xx_acomp_get_postive_input(uint8_t acomp_id)
{
    uint32_t regval;
    uint32_t reg_base;

    if (acomp_id == AON_ACOMP0_ID) {
        reg_base = ACOMP_BASE + AON_ACOMP_REG_ACOMP0_CTRL_OFFSET;
    } else {
        reg_base = ACOMP_BASE + AON_ACOMP_REG_ACOMP1_CTRL_OFFSET;
    }

    regval = getreg32(reg_base);

    return (regval & AON_ACOMP_POS_SEL_MASK) >> AON_ACOMP_POS_SEL_SHIFT;
}

int qc7xx_acomp_gpio_2_chanid(uint32_t pin, uint32_t *channel)
{
#ifdef romapi_qc7xx_acomp_gpio_2_chanid
    return romapi_qc7xx_acomp_gpio_2_chanid(pin, channel);
#else
#if defined(QCC74X)
    if (pin == GPIO_PIN_20) {
        *channel = AON_ACOMP_CHAN_ADC0;
    } else if (pin == GPIO_PIN_19) {
        *channel = AON_ACOMP_CHAN_ADC1;
    } else if (pin == GPIO_PIN_2) {
        *channel = AON_ACOMP_CHAN_ADC2;
    } else if (pin == GPIO_PIN_3) {
        *channel = AON_ACOMP_CHAN_ADC3;
    } else if (pin == GPIO_PIN_14) {
        *channel = AON_ACOMP_CHAN_ADC4;
    } else if (pin == GPIO_PIN_13) {
        *channel = AON_ACOMP_CHAN_ADC5;
    } else if (pin == GPIO_PIN_12) {
        *channel = AON_ACOMP_CHAN_ADC6;
    } else if (pin == GPIO_PIN_10) {
        *channel = AON_ACOMP_CHAN_ADC7;
    } else {
        return -1;
    }
#else
    return -1;
#endif
    return 0;
#endif
}

int qc7xx_acomp_chanid_2_gpio(uint32_t channel, uint32_t *pin)
{
#ifdef romapi_qc7xx_acomp_chanid_2_gpio
    return romapi_qc7xx_acomp_chanid_2_gpio(channel, pin);
#else
#if defined(QCC74X)
    if (channel == AON_ACOMP_CHAN_ADC0) {
        *pin = GPIO_PIN_20;
    } else if (channel == AON_ACOMP_CHAN_ADC1) {
        *pin = GPIO_PIN_19;
    } else if (channel == AON_ACOMP_CHAN_ADC2) {
        *pin = GPIO_PIN_2;
    } else if (channel == AON_ACOMP_CHAN_ADC3) {
        *pin = GPIO_PIN_3;
    } else if (channel == AON_ACOMP_CHAN_ADC4) {
        *pin = GPIO_PIN_14;
    } else if (channel == AON_ACOMP_CHAN_ADC5) {
        *pin = GPIO_PIN_13;
    } else if (channel == AON_ACOMP_CHAN_ADC6) {
        *pin = GPIO_PIN_12;
    } else if (channel == AON_ACOMP_CHAN_ADC7) {
        *pin = GPIO_PIN_10;
    } else {
        return -1;
    }
#else
    return -1;
#endif
    return 0;
#endif
}
