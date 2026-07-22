/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_gpio.h"
#include "hardware/gpio_reg.h"
#include <csi_core.h>

struct qc7xx_gpio_irq_callback {
    void (*handler)(uint8_t pin);
};

#if defined(QCC74X)
struct qc7xx_gpio_irq_callback g_gpio_irq_callback[35] = { 0 };
#elif defined(QCC75X)
struct qc7xx_gpio_irq_callback g_gpio_irq_callback[53] = { 0 };
#endif

#if defined(QCC74X)
extern void qc7xx_gpio_pad_check(uint8_t pin);
#endif

void qc7xx_gpio_init(struct qc7xx_device_s *dev, uint8_t pin, uint32_t cfgset)
{
#ifdef romapi_qc7xx_gpio_init
    romapi_qc7xx_gpio_init(dev, pin, cfgset);
#else
    uint32_t cfg = 0;
    uint32_t reg_base;
    uint32_t cfg_address;
    uint8_t drive;
    uint8_t function;
    uint16_t mode;

    reg_base = dev->reg_base;

    function = (cfgset & GPIO_FUNC_MASK) >> GPIO_FUNC_SHIFT;
    mode = (cfgset & GPIO_MODE_MASK);
    drive = (cfgset & GPIO_DRV_MASK) >> GPIO_DRV_SHIFT;

#if defined(QCC74X)
    qc7xx_gpio_pad_check(pin);

    /* disable muxed to be xtal32k */
    if (pin == GPIO_PIN_16) {
        *(volatile uint32_t *)(0x2000f000 + 0x38) &= ~(1 << 20);
    } else if (pin == GPIO_PIN_17) {
        *(volatile uint32_t *)(0x2000f000 + 0x38) &= ~(1 << 21);
    }
#endif

#if defined(QCC74X) || defined(QCC75X)
    cfg_address = reg_base + GLB_GPIO_CFG0_OFFSET + (pin << 2);
    cfg = 0;
    cfg |= GLB_REG_GPIO_0_INT_MASK;

    if (mode & GPIO_ANALOG) {
        function = 10;
    } else if (mode & GPIO_ALTERNATE) {
        cfg |= GLB_REG_GPIO_0_IE;
    } else {
        function = 11;

        if (mode & GPIO_INPUT) {
            cfg |= GLB_REG_GPIO_0_IE;
        }

        if (mode & GPIO_OUTPUT) {
            cfg |= GLB_REG_GPIO_0_OE;
        }
    }

    if (cfgset & GPIO_PULLUP) {
        cfg |= GLB_REG_GPIO_0_PU;
    } else if (cfgset & GPIO_PULLDOWN) {
        cfg |= GLB_REG_GPIO_0_PD;
    } else {
    }

    if (cfgset & GPIO_SMT_EN) {
        cfg |= GLB_REG_GPIO_0_SMT;
    }

    cfg |= (drive << GLB_REG_GPIO_0_DRV_SHIFT);
    cfg |= (function << GLB_REG_GPIO_0_FUNC_SEL_SHIFT);

    /* configure output mode:set and clr mode */
    cfg |= 0x1 << GLB_REG_GPIO_0_MODE_SHIFT;
#endif
    putreg32(cfg, cfg_address);
#endif
}

void qc7xx_gpio_deinit(struct qc7xx_device_s *dev, uint8_t pin)
{
#ifdef romapi_qc7xx_gpio_deinit
    romapi_qc7xx_gpio_deinit(dev, pin);
#else
#if defined(QCC74X)
    qc7xx_gpio_pad_check(pin);
#endif
    qc7xx_gpio_init(dev, pin, GPIO_INPUT | GPIO_FLOAT);
#endif
}

void qc7xx_gpio_output_value_mode_enable(struct qc7xx_device_s *dev, uint8_t pin)
{
#ifdef romapi_qc7xx_gpio_output_value_mode_enable
    romapi_qc7xx_gpio_output_value_mode_enable(dev, pin);
#else
    uint32_t reg_base;
    uint32_t regval;

    reg_base = dev->reg_base;

    regval = getreg32(reg_base + GLB_GPIO_CFG0_OFFSET + (pin << 2));
    /* configure output mode: output value mode */
    regval &= ~GLB_REG_GPIO_0_MODE_MASK;
    putreg32(regval, reg_base + GLB_GPIO_CFG0_OFFSET + (pin << 2));
#endif
}

void qc7xx_gpio_pin0_31_output(struct qc7xx_device_s *dev, uint32_t value)
{
#ifdef romapi_qc7xx_gpio_pin0_31_output
    romapi_qc7xx_gpio_pin0_31_output(dev, value);
#else
#if defined(QCC74X) || defined(QCC75X)
#if defined(QCC74X)
    uint32_t pin;
    for (pin = 0; pin < 32; pin++) {
        if (value & (1 << pin)) {
            qc7xx_gpio_pad_check(pin);
        }
    }
#endif
    putreg32(value, dev->reg_base + GLB_GPIO_CFG136_OFFSET);
#endif
#endif
}

#if defined(QCC74X) || defined(QCC75X)
void qc7xx_gpio_pin32_63_output(struct qc7xx_device_s *dev, uint32_t value)
{
#ifdef romapi_qc7xx_gpio_pin32_63_output
    romapi_qc7xx_gpio_pin32_63_output(dev, value);
#else
#if defined(QCC74X)
    uint32_t pin;
    for (pin = 0; pin < 32; pin++) {
        if (value & (1 << pin)) {
            qc7xx_gpio_pad_check(pin);
        }
    }
#endif
    putreg32(value, dev->reg_base + GLB_GPIO_CFG137_OFFSET);
#endif
}
#endif

void qc7xx_gpio_set(struct qc7xx_device_s *dev, uint8_t pin)
{
#ifdef romapi_qc7xx_gpio_set
    romapi_qc7xx_gpio_set(dev, pin);
#else
#if defined(QCC74X) || defined(QCC75X)
#if defined(QCC74X)
    qc7xx_gpio_pad_check(pin);
#endif
    putreg32(1 << (pin & 0x1f), dev->reg_base + GLB_GPIO_CFG138_OFFSET + ((pin >> 5) << 2));
#endif
#endif
}

void qc7xx_gpio_pin0_31_set(struct qc7xx_device_s *dev, uint32_t value)
{
#ifdef romapi_qc7xx_gpio_pin0_31_set
    romapi_qc7xx_gpio_pin0_31_set(dev, value);
#else
#if defined(QCC74X) || defined(QCC75X)
#if defined(QCC74X)
    uint32_t pin;
    for (pin = 0; pin < 32; pin++) {
        if (value & (1 << pin)) {
            qc7xx_gpio_pad_check(pin);
        }
    }
#endif
    putreg32(value, dev->reg_base + GLB_GPIO_CFG138_OFFSET);
#endif
#endif
}

#if defined(QCC74X) || defined(QCC75X)
void qc7xx_gpio_pin32_63_set(struct qc7xx_device_s *dev, uint32_t value)
{
#ifdef romapi_qc7xx_gpio_pin32_63_set
    romapi_qc7xx_gpio_pin32_63_set(dev, value);
#else
#if defined(QCC74X)
    uint32_t pin;
    for (pin = 0; pin < 32; pin++) {
        if (value & (1 << pin)) {
            qc7xx_gpio_pad_check(pin + 32);
        }
    }
#endif
    putreg32(value, dev->reg_base + GLB_GPIO_CFG139_OFFSET);
#endif
}
#endif

void qc7xx_gpio_reset(struct qc7xx_device_s *dev, uint8_t pin)
{
#ifdef romapi_qc7xx_gpio_reset
    romapi_qc7xx_gpio_reset(dev, pin);
#else
#if defined(QCC74X) || defined(QCC75X)
#if defined(QCC74X)
    qc7xx_gpio_pad_check(pin);
#endif
    putreg32(1 << (pin & 0x1f), dev->reg_base + GLB_GPIO_CFG140_OFFSET + ((pin >> 5) << 2));
#endif
#endif
}

void qc7xx_gpio_pin0_31_reset(struct qc7xx_device_s *dev, uint32_t value)
{
#ifdef romapi_qc7xx_gpio_pin0_31_reset
    romapi_qc7xx_gpio_pin0_31_reset(dev, value);
#else
#if defined(QCC74X) || defined(QCC75X)
#if defined(QCC74X)
    uint32_t pin;
    for (pin = 0; pin < 32; pin++) {
        if (value & (1 << pin)) {
            qc7xx_gpio_pad_check(pin);
        }
    }
#endif
    putreg32(value, dev->reg_base + GLB_GPIO_CFG140_OFFSET);
#endif
#endif
}

#if defined(QCC74X) || defined(QCC75X)
void qc7xx_gpio_pin32_63_reset(struct qc7xx_device_s *dev, uint32_t value)
{
#ifdef romapi_qc7xx_gpio_pin32_63_reset
    romapi_qc7xx_gpio_pin32_63_reset(dev, value);
#else
#if defined(QCC74X)
    uint32_t pin;
    for (pin = 0; pin < 32; pin++) {
        if (value & (1 << pin)) {
            qc7xx_gpio_pad_check(pin + 32);
        }
    }
#endif
    putreg32(value, dev->reg_base + GLB_GPIO_CFG141_OFFSET);
#endif
}
#endif

bool qc7xx_gpio_read(struct qc7xx_device_s *dev, uint8_t pin)
{
#ifdef romapi_qc7xx_gpio_read
    return romapi_qc7xx_gpio_read(dev, pin);
#else
#if defined(QCC74X) || defined(QCC75X)
#if defined(QCC74X)
    qc7xx_gpio_pad_check(pin);
#endif
    return (getreg32(dev->reg_base + GLB_GPIO_CFG0_OFFSET + (pin << 2)) & GLB_REG_GPIO_0_I);
#endif
#endif
}

uint32_t qc7xx_gpio_pin0_31_read(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_gpio_pin0_31_read
    return romapi_qc7xx_gpio_pin0_31_read(dev);
#else
#if defined(QCC74X) || defined(QCC75X)
    return (getreg32(dev->reg_base + GLB_GPIO_CFG128_OFFSET));
#endif
#endif
}

#if defined(QCC74X) || defined(QCC75X)
uint32_t qc7xx_gpio_pin32_63_read(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_gpio_pin32_63_read
    return romapi_qc7xx_gpio_pin32_63_read(dev);
#else
    return (getreg32(dev->reg_base + GLB_GPIO_CFG129_OFFSET));
#endif
}
#endif

void qc7xx_gpio_int_init(struct qc7xx_device_s *dev, uint8_t pin, uint8_t trig_mode)
{
#ifdef romapi_qc7xx_gpio_int_init
    romapi_qc7xx_gpio_int_init(dev, pin, trig_mode);
#else
    uint32_t reg_base;
    uint32_t cfg_address;
    uint32_t regval;

    reg_base = dev->reg_base;

    qc7xx_gpio_int_mask(dev, pin, true);
    qc7xx_gpio_int_clear(dev, pin);

#if defined(QCC74X) || defined(QCC75X)
    cfg_address = reg_base + GLB_GPIO_CFG0_OFFSET + (pin << 2);
    regval = getreg32(cfg_address);
    regval &= ~GLB_REG_GPIO_0_INT_MODE_SET_MASK;
    regval |= (trig_mode << GLB_REG_GPIO_0_INT_MODE_SET_SHIFT);
#endif
    putreg32(regval, cfg_address);
#endif
}

void qc7xx_gpio_int_mask(struct qc7xx_device_s *dev, uint8_t pin, bool mask)
{
#ifdef romapi_qc7xx_gpio_int_mask
    romapi_qc7xx_gpio_int_mask(dev, pin, mask);
#else
    uint32_t reg_base;
    uint32_t cfg_address;
    uint32_t regval;

    reg_base = dev->reg_base;
#if defined(QCC74X) || defined(QCC75X)
    cfg_address = reg_base + GLB_GPIO_CFG0_OFFSET + (pin << 2);

    regval = getreg32(cfg_address);
    if (mask) {
        regval |= GLB_REG_GPIO_0_INT_MASK;
    } else {
        regval &= ~GLB_REG_GPIO_0_INT_MASK;
    }
#endif
    putreg32(regval, cfg_address);
#endif
}

bool qc7xx_gpio_get_intstatus(struct qc7xx_device_s *dev, uint8_t pin)
{
#ifdef romapi_qc7xx_gpio_get_intstatus
    return romapi_qc7xx_gpio_get_intstatus(dev, pin);
#else
#if defined(QCC74X) || defined(QCC75X)
    return (getreg32(dev->reg_base + GLB_GPIO_CFG0_OFFSET + (pin << 2)) & GLB_GPIO_0_INT_STAT);
#endif
#endif
}

uint32_t qc7xx_gpio_get_intstatus_pin0_31(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_gpio_get_intstatus_pin0_31
    return romapi_qc7xx_gpio_get_intstatus_pin0_31(dev);
#else
#if defined(QCC74X) || defined(QCC75X)
    uint32_t status = 0;
    uint32_t index = 0;
    int pin_count = 0;
    if (GPIO_PIN_MAX > 32) {
        pin_count = 32;
    } else {
        pin_count = GPIO_PIN_MAX;
    }
    for (index = 0; index < pin_count; index++) {
        if ((getreg32(dev->reg_base + GLB_GPIO_CFG0_OFFSET + (index << 2)) & GLB_GPIO_0_INT_STAT)) {
            status |= (1 << index);
        }
    }
    return status;
#endif
#endif
}

#if defined(QCC74X) || defined(QCC75X)
uint32_t qc7xx_gpio_get_intstatus_pin32_63(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_gpio_get_intstatus_pin32_63
    return romapi_qc7xx_gpio_get_intstatus_pin32_63(dev);
#else
    uint32_t status = 0;
    uint32_t index = 0;
    int pin_count = 0;
    if (GPIO_PIN_MAX > 64) {
        pin_count = 32;
    } else {
        pin_count = GPIO_PIN_MAX - 32;
    }
    for (index = 0; index < pin_count; index++) {
        if ((getreg32(dev->reg_base + GLB_GPIO_CFG0_OFFSET + ((index + 32) << 2)) & GLB_GPIO_0_INT_STAT)) {
            status |= (1 << index);
        }
    }
    return status;
#endif
}
#endif

void qc7xx_gpio_int_clear(struct qc7xx_device_s *dev, uint8_t pin)
{
#ifdef romapi_qc7xx_gpio_int_clear
    romapi_qc7xx_gpio_int_clear(dev, pin);
#else
    uint32_t reg_base;
    uint32_t cfg_address;
    uint32_t regval;

    reg_base = dev->reg_base;
#if defined(QCC74X) || defined(QCC75X)
    cfg_address = reg_base + GLB_GPIO_CFG0_OFFSET + (pin << 2);

    regval = getreg32(cfg_address);
    regval |= GLB_REG_GPIO_0_INT_CLR;
    putreg32(regval, cfg_address);
    regval &= ~GLB_REG_GPIO_0_INT_CLR;
    putreg32(regval, cfg_address);
#endif
#endif
}

void qc7xx_gpio_int_clear_pin0_31(struct qc7xx_device_s *dev, uint32_t pins)
{
#ifdef romapi_qc7xx_gpio_int_clear_pin0_31
    romapi_qc7xx_gpio_int_clear_pin0_31(dev, pins);
#else
    uint32_t reg_base;
    uint32_t cfg_address;
    uint32_t regval;

    reg_base = dev->reg_base;
#if defined(QCC74X) || defined(QCC75X)
    for (int pin = 0; pin < 32; pin++) {
        if (pins & (1 << pin)) {
            cfg_address = reg_base + GLB_GPIO_CFG0_OFFSET + (pin << 2);
            regval = getreg32(cfg_address);
            regval |= GLB_REG_GPIO_0_INT_CLR;
            putreg32(regval, cfg_address);
            regval &= ~GLB_REG_GPIO_0_INT_CLR;
            putreg32(regval, cfg_address);
        }
    }
#endif
#endif
}

#if defined(QCC74X) || defined(QCC75X)
void qc7xx_gpio_int_clear_pin32_63(struct qc7xx_device_s *dev, uint32_t pins)
{
#ifdef romapi_qc7xx_gpio_int_clear_pin32_63
    romapi_qc7xx_gpio_int_clear_pin32_63(dev, pins);
#else

    uint32_t reg_base;
    uint32_t cfg_address;
    uint32_t regval;

    reg_base = dev->reg_base;

    for (int pin = 0; pin < 32; pin++) {
        if (pins & (1 << pin)) {
            cfg_address = reg_base + GLB_GPIO_CFG0_OFFSET + ((pin + 32) << 2);
            regval = getreg32(cfg_address);
            regval |= GLB_REG_GPIO_0_INT_CLR;
            putreg32(regval, cfg_address);
            regval &= ~GLB_REG_GPIO_0_INT_CLR;
            putreg32(regval, cfg_address);
        }
    }
#endif
}
#endif

void qc7xx_gpio_uart_init(struct qc7xx_device_s *dev, uint8_t pin, uint8_t uart_func)
{
#ifdef romapi_qc7xx_gpio_uart_init
    romapi_qc7xx_gpio_uart_init(dev, pin, uart_func);
#else
    uint32_t reg_base;
    uint32_t regval;
    uint8_t sig;
    uint8_t sig_pos;

    reg_base = dev->reg_base;
#if defined(QCC74X)
    qc7xx_gpio_pad_check(pin);
#endif
#if defined(QCC74X) || defined(QCC75X)
#define GLB_UART_CFG1_OFFSET (0x154)
#define GLB_UART_CFG2_OFFSET (0x158)
    uint32_t regval2;
    sig = pin % 12;

    if (sig < 8) {
        sig_pos = sig << 2;

        regval = getreg32(reg_base + GLB_UART_CFG1_OFFSET);
        regval &= (~(0x0f << sig_pos));
        regval |= (uart_func << sig_pos);

        for (uint8_t i = 0; i < 8; i++) {
            /* reset other sigs which are the same with uart_func */
            sig_pos = i << 2;
            if (((regval & (0x0f << sig_pos)) == (uart_func << sig_pos)) && (i != sig) && (uart_func != 0x0f)) {
                regval &= (~(0x0f << sig_pos));
                regval |= (0x0f << sig_pos);
            }
        }
        regval2 = getreg32(reg_base + GLB_UART_CFG2_OFFSET);

        for (uint8_t i = 8; i < 12; i++) {
            /* reset other sigs which are the same with uart_func */
            sig_pos = (i - 8) << 2;
            if (((regval2 & (0x0f << sig_pos)) == (uart_func << sig_pos)) && (i != sig) && (uart_func != 0x0f)) {
                regval2 &= (~(0x0f << sig_pos));
                regval2 |= (0x0f << sig_pos);
            }
        }
        putreg32(regval, reg_base + GLB_UART_CFG1_OFFSET);
        putreg32(regval2, reg_base + GLB_UART_CFG2_OFFSET);
    } else {
        sig_pos = (sig - 8) << 2;

        regval = getreg32(reg_base + GLB_UART_CFG2_OFFSET);
        regval &= (~(0x0f << sig_pos));
        regval |= (uart_func << sig_pos);

        for (uint8_t i = 8; i < 12; i++) {
            /* reset other sigs which are the same with uart_func */
            sig_pos = (i - 8) << 2;
            if (((regval & (0x0f << sig_pos)) == (uart_func << sig_pos)) && (i != sig) && (uart_func != 0x0f)) {
                regval &= (~(0x0f << sig_pos));
                regval |= (0x0f << sig_pos);
            }
        }
        regval2 = getreg32(reg_base + GLB_UART_CFG1_OFFSET);

        for (uint8_t i = 0; i < 8; i++) {
            /* reset other sigs which are the same with uart_func */
            sig_pos = i << 2;
            if (((regval2 & (0x0f << sig_pos)) == (uart_func << sig_pos)) && (i != sig) && (uart_func != 0x0f)) {
                regval2 &= (~(0x0f << sig_pos));
                regval2 |= (0x0f << sig_pos);
            }
        }
        putreg32(regval, reg_base + GLB_UART_CFG2_OFFSET);
        putreg32(regval2, reg_base + GLB_UART_CFG1_OFFSET);
    }
#endif
    qc7xx_gpio_init(dev, pin, (7 << GPIO_FUNC_SHIFT) | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
#endif
}

int qc7xx_gpio_feature_control(struct qc7xx_device_s *dev, int cmd, size_t arg)
{
#ifdef romapi_qc7xx_gpio_feature_control
    return romapi_qc7xx_gpio_feature_control(dev, cmd, arg);
#else
    int ret = 0;
    uint32_t reg_base;
    uint32_t regval;
    uint8_t pin = arg;

    reg_base = dev->reg_base;
    switch (cmd) {
        case GPIO_CMD_GET_GPIO_FUN:
#if defined(QCC74X) || defined(QCC75X)
            regval = getreg32(reg_base + GLB_GPIO_CFG0_OFFSET + (pin << 2)) & GLB_REG_GPIO_0_FUNC_SEL_MASK;
            regval >>= GLB_REG_GPIO_0_FUNC_SEL_SHIFT;
#endif
            return regval;
        default:
            ret = -EPERM;
            break;
    }
    return ret;
#endif
}

struct qc7xx_device_s *g_gpio_int = NULL;

void gpio_all_isr(int irq, void *arg)
{
    char log_buf[64];

    if (NULL == g_gpio_int) {
        snprintf(log_buf, sizeof(log_buf), "GPIO interrupt not init\r\n");
        qc7xx_lhal_assert_func(__FILE__, __LINE__, __func__, log_buf);
    }
    for (uint8_t i = 0; i < sizeof(g_gpio_irq_callback) / sizeof(struct qc7xx_gpio_irq_callback); i++) {
        if (qc7xx_gpio_get_intstatus(g_gpio_int, i)) {
            if (g_gpio_irq_callback[i].handler) {
                qc7xx_gpio_int_clear(g_gpio_int, i);
                g_gpio_irq_callback[i].handler(i);
            } else {
                snprintf(log_buf, sizeof(log_buf), "GPIO %d interrupt not register\r\n", i);
                qc7xx_lhal_assert_func(__FILE__, __LINE__, __func__, log_buf);
            }
        }
    }
}

void qc7xx_gpio_irq_attach(uint8_t pin, void (*callback)(uint8_t pin))
{
    g_gpio_int = qc7xx_device_get_by_name("gpio");
    qc7xx_gpio_int_mask(g_gpio_int, pin, true);
    g_gpio_irq_callback[pin].handler = callback;
    qc7xx_gpio_int_mask(g_gpio_int, pin, false);
    qc7xx_irq_attach(g_gpio_int->irq_num, gpio_all_isr, NULL);
}

void qc7xx_gpio_irq_detach(uint8_t pin)
{
    g_gpio_int = qc7xx_device_get_by_name("gpio");
    qc7xx_gpio_int_mask(g_gpio_int, pin, true);
    g_gpio_irq_callback[pin].handler = NULL;
}