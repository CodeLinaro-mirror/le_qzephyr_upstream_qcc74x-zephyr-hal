/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include <qcc74x_glb.h>
#include <qcc74x_gpio.h>
#include <qcc74x_glb_gpio.h>

#include <qcc74x.h>
#include "lp_gpio.h"
// #include "lp_irq.h"

extern gpio_ctx_t *pstgpio_head;
#define GPIO_FUNC_NUM_IN_QCC743   (GPIO_FUN_GPIO)

int lp_gpio_enable_output(uint8_t pin, uint8_t pullup, uint8_t pulldown)
{
    GLB_GPIO_Cfg_Type cfg;

    cfg.drive=0;
    cfg.smtCtrl=1;
    cfg.outputMode = 0;
    cfg.gpioPin = pin;
    cfg.gpioFun = GPIO_FUNC_NUM_IN_QCC743;
    cfg.gpioMode = GPIO_MODE_OUTPUT;
    cfg.pullType = GPIO_PULL_NONE;
    if (pullup) {
        cfg.pullType = GPIO_PULL_UP;
    }
    if (pulldown) {
        cfg.pullType = GPIO_PULL_DOWN;
    }

    GLB_GPIO_Init(&cfg);

    return 0;
}

int lp_gpio_enable_input(uint8_t pin, uint8_t pullup, uint8_t pulldown)
{
    GLB_GPIO_Cfg_Type cfg;

    cfg.drive=0;
    cfg.smtCtrl=1;
    cfg.outputMode = 0;
    cfg.gpioPin = pin;
    cfg.gpioFun = GPIO_FUNC_NUM_IN_QCC743;
    cfg.gpioMode = GPIO_MODE_INPUT;
    cfg.pullType = GPIO_PULL_NONE;
    if (pullup) {
        cfg.pullType = GPIO_PULL_UP;
    }
    if (pulldown) {
        cfg.pullType = GPIO_PULL_DOWN;
    }

    GLB_GPIO_Init(&cfg);

    return 0;
}

int lp_gpio_output_set(uint8_t pin, uint8_t value)
{
    GLB_GPIO_Write((uint8_t)pin, value ? 1 : 0);
    return 0;
}

int lp_gpio_input_get(uint8_t pin, uint8_t *value)
{
    *value = GLB_GPIO_Read((uint8_t)pin);
    return 0;
}

int lp_gpio_input_get_value(uint8_t pin)
{
    return GLB_GPIO_Read((uint8_t)pin);
}

void lp_gpio_intmask(uint8_t gpioPin, uint8_t mask)
{
    GLB_GPIO_IntMask(gpioPin, mask ? MASK : UNMASK);
}

void lp_set_gpio_intmod(uint8_t gpioPin, uint8_t intTrig)
{
    GLB_GPIO_INT_Cfg_Type intCfg;

    intCfg.trig = intTrig;
    intCfg.gpioPin = gpioPin;
    intCfg.intMask = MASK;

    GLB_GPIO_Int_Init(&intCfg);
}

int lp_gpio_int_clear(uint8_t gpioPin)
{
    if (gpioPin < 32) {
        /*GPIO0 ~ GPIO31*/
        GLB_Clr_GPIO_IntStatus(gpioPin);
    }

    return 0;
}
