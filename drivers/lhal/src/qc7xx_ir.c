/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_ir.h"
#include "qc7xx_clock.h"
#include "hardware/ir_reg.h"

#define DIVIDE_ROUND(a, b) ((2 * a + b) / (2 * b))

#if !defined(QCC74X)
void qc7xx_ir_tx_init(struct qc7xx_device_s *dev, const struct qc7xx_ir_tx_config_s *config)
{
#ifdef romapi_qc7xx_ir_tx_init
    romapi_qc7xx_ir_tx_init(dev, config);
#else
    uint32_t reg_base;
    uint32_t regval;
    uint32_t ir_clock;
    struct qc7xx_ir_tx_config_s *tx_config = (struct qc7xx_ir_tx_config_s *)config;

    *(uint32_t *)0x20000144 |= 1 << 31;

    if (qc7xx_clk_get_peripheral_clock(QC7XX_DEVICE_TYPE_IR, 0)) {
        ir_clock = qc7xx_clk_get_peripheral_clock(QC7XX_DEVICE_TYPE_IR, 0);
    } else {
        ir_clock = 2000000;
    }

    if (tx_config->tx_mode == IR_TX_NEC) {
        tx_config->data_bits = 32;
        tx_config->tail_inverse = 0;
        tx_config->tail_enable = 1;
        tx_config->head_inverse = 0;
        tx_config->head_enable = 1;
        tx_config->logic1_inverse = 0;
        tx_config->logic0_inverse = 0;
        tx_config->data_enable = 1;
        tx_config->swm_enable = 0;
        tx_config->output_modulation = 1;
        tx_config->output_inverse = 0;
        tx_config->freerun_enable = 0;
        tx_config->continue_enable = 0;
        tx_config->fifo_width = IR_TX_FIFO_WIDTH_32BIT;
        tx_config->fifo_threshold = 0;
        tx_config->logic0_pulse_width_1 = 0;
        tx_config->logic0_pulse_width_0 = 0;
        tx_config->logic1_pulse_width_1 = 2;
        tx_config->logic1_pulse_width_0 = 0;
        tx_config->head_pulse_width_1 = 7;
        tx_config->head_pulse_width_0 = 15;
        tx_config->tail_pulse_width_1 = 0;
        tx_config->tail_pulse_width_0 = 0;
        tx_config->pulse_width_unit = (ir_clock * 10 / 17777 - 1) & 0xfff;
    } else if (tx_config->tx_mode == IR_TX_RC5) {
        tx_config->data_bits = 13;
        tx_config->tail_inverse = 0;
        tx_config->tail_enable = 0;
        tx_config->head_inverse = 1;
        tx_config->head_enable = 1;
        tx_config->logic1_inverse = 1;
        tx_config->logic0_inverse = 0;
        tx_config->data_enable = 1;
        tx_config->swm_enable = 0;
        tx_config->output_modulation = 1;
        tx_config->output_inverse = 0;
        tx_config->freerun_enable = 0;
        tx_config->continue_enable = 0;
        tx_config->fifo_width = IR_TX_FIFO_WIDTH_32BIT;
        tx_config->fifo_threshold = 0;
        tx_config->logic0_pulse_width_1 = 0;
        tx_config->logic0_pulse_width_0 = 0;
        tx_config->logic1_pulse_width_1 = 0;
        tx_config->logic1_pulse_width_0 = 0;
        tx_config->head_pulse_width_1 = 0;
        tx_config->head_pulse_width_0 = 0;
        tx_config->tail_pulse_width_1 = 0;
        tx_config->tail_pulse_width_0 = 0;
        tx_config->pulse_width_unit = (ir_clock * 10 / 11248 - 1) & 0xfff;
    } else if (tx_config->tx_mode == IR_TX_SWM) {
        tx_config->swm_enable = 1;
        tx_config->output_modulation = 1;
        tx_config->output_inverse = 0;
        tx_config->fifo_width = IR_TX_FIFO_WIDTH_32BIT;
        tx_config->fifo_threshold = 0;
    }

    if (tx_config->tx_mode != IR_TX_CUSTOMIZE) {
        tx_config->modu_width_1 = ((ir_clock / 11310 + 5) / 10 - 1) & 0xff;
        tx_config->modu_width_0 = ((ir_clock / 5655 + 5) / 10 - 1) & 0xff;
    } else {
        if (tx_config->output_modulation != 0 && tx_config->freerun_enable != 0) {
            tx_config->continue_enable = 0;
            if (tx_config->tail_pulse_width_1 < 5) {
                tx_config->tail_pulse_width_1 = 5;
            }
            if (tx_config->tail_pulse_width_0 < 5) {
                tx_config->tail_pulse_width_0 = 5;
            }
        }
    }

    reg_base = dev->reg_base;
    regval = (tx_config->pulse_width_unit & 0xfff) | tx_config->modu_width_1 << 16 | tx_config->modu_width_0 << 24;
    putreg32(regval, reg_base + IRTX_PULSE_WIDTH_OFFSET);

    regval = getreg32(reg_base + IR_FIFO_CONFIG_1_OFFSET);
    regval &= ~IR_TX_FIFO_TH_MASK;
    regval |= tx_config->fifo_threshold << IR_TX_FIFO_TH_SHIFT;
    putreg32(regval, reg_base + IR_FIFO_CONFIG_1_OFFSET);

    regval = getreg32(reg_base + IRTX_CONFIG_OFFSET);
    regval &= ~(IR_CR_IRTX_SWM_EN | IR_CR_IRTX_MOD_EN | IR_CR_IRTX_OUT_INV | IR_CR_IRTX_FRM_FRAME_SIZE_MASK);
    if (tx_config->swm_enable) {
        regval |= IR_CR_IRTX_SWM_EN;
    }
    if (tx_config->output_modulation) {
        regval |= IR_CR_IRTX_MOD_EN;
    }
    if (tx_config->output_inverse) {
        regval |= IR_CR_IRTX_OUT_INV;
    }
    regval |= (tx_config->fifo_width & 0x3) << IR_CR_IRTX_FRM_FRAME_SIZE_SHIFT;
    if (tx_config->tx_mode == IR_TX_SWM) {
        putreg32(regval, reg_base + IRTX_CONFIG_OFFSET);
        return;
    }
    regval &= IR_CR_IRTX_SWM_EN | IR_CR_IRTX_MOD_EN | IR_CR_IRTX_OUT_INV | IR_CR_IRTX_FRM_FRAME_SIZE_MASK;
    regval |= (tx_config->data_bits - 1) << IR_CR_IRTX_DATA_NUM_SHIFT;
    if (tx_config->tail_inverse) {
        regval |= IR_CR_IRTX_TAIL_HL_INV;
    }
    if (tx_config->tail_enable) {
        regval |= IR_CR_IRTX_TAIL_EN;
    }
    if (tx_config->head_inverse) {
        regval |= IR_CR_IRTX_HEAD_HL_INV;
    }
    if (tx_config->head_enable) {
        regval |= IR_CR_IRTX_HEAD_EN;
    }
    if (tx_config->logic1_inverse) {
        regval |= IR_CR_IRTX_LOGIC1_HL_INV;
    }
    if (tx_config->logic0_inverse) {
        regval |= IR_CR_IRTX_LOGIC0_HL_INV;
    }
    if (tx_config->data_enable) {
        regval |= IR_CR_IRTX_DATA_EN;
    }
    if (tx_config->freerun_enable) {
        regval |= IR_CR_IRTX_FRM_EN;
    }
    if (tx_config->continue_enable) {
        regval |= IR_CR_IRTX_FRM_CONT_EN;
    }
    putreg32(regval, reg_base + IRTX_CONFIG_OFFSET);

    regval = tx_config->logic0_pulse_width_0 | tx_config->logic0_pulse_width_1 << 8 |
             tx_config->logic1_pulse_width_0 << 16 | tx_config->logic1_pulse_width_1 << 24;
    putreg32(regval, reg_base + IRTX_PW_0_OFFSET);

    regval = tx_config->head_pulse_width_0 | tx_config->head_pulse_width_1 << 8 |
             tx_config->tail_pulse_width_0 << 16 | tx_config->tail_pulse_width_1 << 24;
    putreg32(regval, reg_base + IRTX_PW_1_OFFSET);
#endif
}

void qc7xx_ir_send(struct qc7xx_device_s *dev, uint32_t *data, uint32_t length)
{
#ifdef romapi_qc7xx_ir_send
    romapi_qc7xx_ir_send(dev, data, length);
#else
    uint32_t reg_base;
    uint32_t regval;
    uint32_t i = 0;
    uint32_t data_bits;

    qc7xx_ir_txint_clear(dev);

    reg_base = dev->reg_base;

    regval = getreg32(reg_base + IRTX_CONFIG_OFFSET);
    regval |= IR_CR_IRTX_EN;
    putreg32(regval, reg_base + IRTX_CONFIG_OFFSET);

    if ((regval & IR_CR_IRTX_FRM_EN) == 0) {
        data_bits = (regval & IR_CR_IRTX_DATA_NUM_MASK) >> IR_CR_IRTX_DATA_NUM_SHIFT;
        data_bits = data_bits / 32 + 1;
        length = length < data_bits ? length : data_bits;
    }
    while (i < length) {
        if (qc7xx_ir_get_txfifo_cnt(dev) > 0) {
            putreg32(data[i], reg_base + IR_FIFO_WDATA_OFFSET);
            i++;
        }
    }

    if ((getreg32(reg_base + IRTX_CONFIG_OFFSET) & IR_CR_IRTX_FRM_EN) == 0) {
        while ((qc7xx_ir_get_txint_status(dev) & IR_TX_INTSTS_END) == 0) {
            /* Waiting for sending */
        }
    } else {
        while (qc7xx_ir_get_txfifo_cnt(dev) < 4) {
            /* Waiting for sending */
        }
    }
    while ((qc7xx_ir_get_txint_status(dev) & IR_TX_INTSTS_END) == 0) {
        /* Waiting for sending */
    }

    regval &= ~IR_CR_IRTX_EN;
    putreg32(regval, reg_base + IRTX_CONFIG_OFFSET);

    qc7xx_ir_txint_clear(dev);
#endif
}

void qc7xx_ir_swm_send(struct qc7xx_device_s *dev, uint16_t *data, uint32_t length)
{
#ifdef romapi_qc7xx_ir_swm_send
    romapi_qc7xx_ir_swm_send(dev, data, length);
#else
    uint32_t reg_base;
    uint32_t regval;
    uint16_t min_data = data[0];
    uint32_t count;
    uint32_t pwval = 0;
    uint32_t i, j;

#if defined(QCC75X)
    if (length > 65536) {
        length = 65536;
    }
#else
    if (length > 128) {
        length = 128;
    }
#endif

#if defined(QCC75X)
    count = (length + 1) / 2;
#else
    count = (length + 3) / 4;
#endif

    qc7xx_ir_txint_clear(dev);

    /* Search for min value */
    for (i = 1; i < length; i++) {
        if (min_data > data[i] && data[i] != 0) {
            min_data = data[i];
        }
    }

    /* Set min value as pulse width unit */
    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRTX_PULSE_WIDTH_OFFSET);
    regval &= ~IR_CR_IRTX_PW_UNIT_MASK;
    regval |= min_data << IR_CR_IRTX_PW_UNIT_SHIFT;
    putreg32(regval, reg_base + IRTX_PULSE_WIDTH_OFFSET);

    regval = getreg32(reg_base + IRTX_CONFIG_OFFSET);
    regval &= ~IR_CR_IRTX_DATA_NUM_MASK;
    regval |= (length - 1) << IR_CR_IRTX_DATA_NUM_SHIFT;
    regval |= IR_CR_IRTX_EN;
    putreg32(regval, reg_base + IRTX_CONFIG_OFFSET);

    /* Calculate tx SWM pulse width data as multiples of pulse width unit */
    for (i = 0; i < count; i++) {
        pwval = 0;

#if defined(QCC75X)
        if (i < count - 1) {
            /* Put every two pulse width together as a 32-bit value to tx fifo */
            for (j = 0; j < 2; j++) {
                /* Every pulse width divided by pulse width unit */
                regval = (DIVIDE_ROUND(data[j + i * 2], min_data) - 1) & 0xffff;
                /* Tx fifo 32-bit value: pwval[15:0]:first pulse width, pwval[31:16]:second pulse width */
                pwval |= regval << (16 * j);
            }
        } else {
            /* Deal with pulse width data remained which is less than 4 */
            for (j = 0; j < length % 2; j++) {
                regval = (DIVIDE_ROUND(data[j + i * 2], min_data) - 1) & 0xffff;
                pwval |= regval << (16 * j);
            }
        }
#else
        if (i < count - 1) {
            /* Put every four pulse width together as a 32-bit value to tx fifo */
            for (j = 0; j < 4; j++) {
                /* Every pulse width divided by pulse width unit */
                regval = (DIVIDE_ROUND(data[j + i * 4], min_data) - 1) & 0xff;
                /* Tx fifo 32-bit value: pwval[7:0]:first pulse width, pwval[15:8]:second pulse width... */
                pwval |= regval << (8 * j);
            }
        } else {
            /* Deal with pulse width data remained which is less than 4 */
            for (j = 0; j < length % 4; j++) {
                regval = (DIVIDE_ROUND(data[j + i * 4], min_data) - 1) & 0xff;
                pwval |= regval << (8 * j);
            }
        }
#endif

        /* Write to tx fifo */
        while (qc7xx_ir_get_txfifo_cnt(dev) == 0) {}
        putreg32(pwval, reg_base + IR_FIFO_WDATA_OFFSET);
    }

    while ((qc7xx_ir_get_txint_status(dev) & IR_TX_INTSTS_END) == 0) {
        /* Waiting for sending */
    }

    regval = getreg32(reg_base + IRTX_CONFIG_OFFSET);
    regval &= ~IR_CR_IRTX_EN;
    putreg32(regval, reg_base + IRTX_CONFIG_OFFSET);

    qc7xx_ir_txint_clear(dev);
#endif
}

void qc7xx_ir_tx_enable(struct qc7xx_device_s *dev, bool enable)
{
#ifdef romapi_qc7xx_ir_tx_enable
    romapi_qc7xx_ir_tx_enable(dev, enable);
#else
    uint32_t reg_base;
    uint32_t regval;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRTX_CONFIG_OFFSET);
    if (enable) {
        regval |= IR_CR_IRTX_EN;
    } else {
        regval &= ~IR_CR_IRTX_EN;
    }
    putreg32(regval, reg_base + IRTX_CONFIG_OFFSET);
#endif
}

void qc7xx_ir_txint_mask(struct qc7xx_device_s *dev, uint8_t int_type, bool mask)
{
#ifdef romapi_qc7xx_ir_txint_mask
    romapi_qc7xx_ir_txint_mask(dev, int_type, mask);
#else
    uint32_t reg_base;
    uint32_t regval;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRTX_INT_STS_OFFSET);
    if (mask) {
        regval |= (int_type & 0x7) << 8;
    } else {
        regval &= ~((int_type & 0x7) << 8);
    }
    putreg32(regval, reg_base + IRTX_INT_STS_OFFSET);
#endif
}

void qc7xx_ir_txint_clear(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_ir_txint_clear
    romapi_qc7xx_ir_txint_clear(dev);
#else
    uint32_t reg_base;
    uint32_t regval;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRTX_INT_STS_OFFSET);
    regval |= IR_CR_IRTX_END_CLR;
    putreg32(regval, reg_base + IRTX_INT_STS_OFFSET);
#endif
}

uint32_t qc7xx_ir_get_txint_status(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_ir_get_txint_status
    return romapi_qc7xx_ir_get_txint_status(dev);
#else
    uint32_t reg_base;

    reg_base = dev->reg_base;
    return (getreg32(reg_base + IRTX_INT_STS_OFFSET) & 0x7);
#endif
}

void qc7xx_ir_link_txdma(struct qc7xx_device_s *dev, bool enable)
{
    uint32_t reg_base;
    uint32_t regval;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IR_FIFO_CONFIG_0_OFFSET);
    if (enable) {
        regval |= IRTX_DMA_EN;
    } else {
        regval &= ~IRTX_DMA_EN;
    }
    putreg32(regval, reg_base + IR_FIFO_CONFIG_0_OFFSET);
}

uint8_t qc7xx_ir_get_txfifo_cnt(struct qc7xx_device_s *dev)
{
    uint32_t reg_base;

    reg_base = dev->reg_base;
    return ((getreg32(reg_base + IR_FIFO_CONFIG_1_OFFSET) & IR_TX_FIFO_CNT_MASK) >> IR_TX_FIFO_CNT_SHIFT);
}

void qc7xx_ir_txfifo_clear(struct qc7xx_device_s *dev)
{
    uint32_t reg_base;
    uint32_t regval;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IR_FIFO_CONFIG_0_OFFSET);
    regval |= IR_TX_FIFO_CLR;
    putreg32(regval, reg_base + IR_FIFO_CONFIG_0_OFFSET);
}
#endif

void qc7xx_ir_rx_init(struct qc7xx_device_s *dev, const struct qc7xx_ir_rx_config_s *config)
{
    uint32_t reg_base;
    uint32_t regval;
    uint32_t ir_clock;
    uint16_t data_threshold, end_threshold;

    if (qc7xx_clk_get_peripheral_clock(QC7XX_DEVICE_TYPE_IR, 0)) {
        ir_clock = qc7xx_clk_get_peripheral_clock(QC7XX_DEVICE_TYPE_IR, 0);
    } else {
        ir_clock = 2000000;
    }

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRRX_CONFIG_OFFSET);
    regval &= ~IR_CR_IRRX_MODE_MASK;
    regval |= (config->rx_mode & 0x3) << IR_CR_IRRX_MODE_SHIFT;
    if (config->input_inverse) {
        regval |= IR_CR_IRRX_IN_INV;
    } else {
        regval &= ~IR_CR_IRRX_IN_INV;
    }
    if (!config->deglitch_enable) {
        regval &= ~IR_CR_IRRX_DEG_EN;
    } else {
        regval |= IR_CR_IRRX_DEG_EN;
        regval &= ~IR_CR_IRRX_DEG_CNT_MASK;
        regval |= config->deglitch_cnt << IR_CR_IRRX_DEG_CNT_SHIFT;
    }
    putreg32(regval, reg_base + IRRX_CONFIG_OFFSET);

    if (config->rx_mode == IR_RX_NEC) {
        data_threshold = (ir_clock / 588 - 1) & 0xffff;
        end_threshold = (ir_clock / 222 - 1) & 0xffff;
    } else if (config->rx_mode == IR_RX_RC5) {
        data_threshold = (ir_clock / 750 - 1) & 0xffff;
        end_threshold = (ir_clock / 400 - 1) & 0xffff;
    } else {
        data_threshold = config->data_threshold;
        end_threshold = config->end_threshold;
    }
    regval = getreg32(reg_base + IRRX_PW_CONFIG_OFFSET);
    regval = end_threshold << IR_CR_IRRX_END_TH_SHIFT | data_threshold;
    putreg32(regval, reg_base + IRRX_PW_CONFIG_OFFSET);

    regval = getreg32(reg_base + IR_FIFO_CONFIG_1_OFFSET);
    regval &= ~IR_RX_FIFO_TH_MASK;
    regval |= config->fifo_threshold << IR_RX_FIFO_TH_SHIFT;
    putreg32(regval, reg_base + IR_FIFO_CONFIG_1_OFFSET);
}

uint16_t qc7xx_ir_receive(struct qc7xx_device_s *dev, uint64_t *data)
{
    uint32_t reg_base;
    uint32_t regval;

    qc7xx_ir_rxint_clear(dev);

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRRX_CONFIG_OFFSET);
    regval |= IR_CR_IRRX_EN;
    putreg32(regval, reg_base + IRRX_CONFIG_OFFSET);

    while ((qc7xx_ir_get_rxint_status(dev) & IR_RX_INTSTS_END) == 0) {
        /* Waiting for receiving */
    }

    regval &= ~IR_CR_IRRX_EN;
    putreg32(regval, reg_base + IRRX_CONFIG_OFFSET);

    qc7xx_ir_rxint_clear(dev);

    regval = getreg32(reg_base + IRRX_DATA_COUNT_OFFSET) & IR_STS_IRRX_DATA_CNT_MASK;
    if (regval <= 32) {
        *data = getreg32(reg_base + IRRX_DATA_WORD0_OFFSET);
    } else {
        *data = getreg32(reg_base + IRRX_DATA_WORD0_OFFSET) | (uint64_t)getreg32(reg_base + IRRX_DATA_WORD1_OFFSET) << 32;
    }

    return regval;
}

uint16_t qc7xx_ir_swm_receive(struct qc7xx_device_s *dev, uint16_t *data, uint16_t length)
{
    uint32_t reg_base;
    uint32_t regval;
    uint32_t i = 0;

    qc7xx_ir_rxint_clear(dev);

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRRX_CONFIG_OFFSET);
    regval |= IR_CR_IRRX_EN;
    putreg32(regval, reg_base + IRRX_CONFIG_OFFSET);

    while ((qc7xx_ir_get_rxint_status(dev) & IR_RX_INTSTS_END) == 0) {
        if (qc7xx_ir_get_rxfifo_cnt(dev) != 0 && i < length) {
            data[i] = getreg32(reg_base + IR_FIFO_RDATA_OFFSET);
            i++;
        }
    }

    regval = getreg32(reg_base + IRRX_CONFIG_OFFSET);
    regval &= ~IR_CR_IRRX_EN;
    putreg32(regval, reg_base + IRRX_CONFIG_OFFSET);

    qc7xx_ir_rxint_clear(dev);

    return (getreg32(reg_base + IRRX_DATA_COUNT_OFFSET) & IR_STS_IRRX_DATA_CNT_MASK);
}

void qc7xx_ir_rx_enable(struct qc7xx_device_s *dev, bool enable)
{
    uint32_t reg_base;
    uint32_t regval;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRRX_CONFIG_OFFSET);
    if (enable) {
        regval |= IR_CR_IRRX_EN;
    } else {
        regval &= ~IR_CR_IRRX_EN;
    }
    putreg32(regval, reg_base + IRRX_CONFIG_OFFSET);
}

void qc7xx_ir_rxint_mask(struct qc7xx_device_s *dev, uint8_t int_type, bool mask)
{
    uint32_t reg_base;
    uint32_t regval;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRRX_INT_STS_OFFSET);
    if (mask) {
        regval |= (int_type & 0x7) << 8;
    } else {
        regval &= ~((int_type & 0x7) << 8);
    }
    putreg32(regval, reg_base + IRRX_INT_STS_OFFSET);
}

void qc7xx_ir_rxint_clear(struct qc7xx_device_s *dev)
{
    uint32_t reg_base;
    uint32_t regval;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IRRX_INT_STS_OFFSET);
    regval |= IR_CR_IRRX_END_CLR;
    putreg32(regval, reg_base + IRRX_INT_STS_OFFSET);
}

uint32_t qc7xx_ir_get_rxint_status(struct qc7xx_device_s *dev)
{
    uint32_t reg_base;

    reg_base = dev->reg_base;
    return (getreg32(reg_base + IRRX_INT_STS_OFFSET) & 0x7);
}

uint8_t qc7xx_ir_get_rxfifo_cnt(struct qc7xx_device_s *dev)
{
    uint32_t reg_base;

    reg_base = dev->reg_base;
    return ((getreg32(reg_base + IR_FIFO_CONFIG_1_OFFSET) & IR_RX_FIFO_CNT_MASK) >> IR_RX_FIFO_CNT_SHIFT);
}

void qc7xx_ir_rxfifo_clear(struct qc7xx_device_s *dev)
{
    uint32_t reg_base;
    uint32_t regval;

    reg_base = dev->reg_base;
    regval = getreg32(reg_base + IR_FIFO_CONFIG_0_OFFSET);
    regval |= IR_RX_FIFO_CLR;
    putreg32(regval, reg_base + IR_FIFO_CONFIG_0_OFFSET);
}

int qc7xx_ir_feature_control(struct qc7xx_device_s *dev, int cmd, size_t arg)
{
#ifdef romapi_qc7xx_ir_feature_control
    return romapi_qc7xx_ir_feature_control(dev, cmd, arg);
#else
    int ret = 0;
#if !defined(QCC74X)
    uint32_t regval;
#endif

    switch (cmd) {
#if !defined(QCC74X)
        case IR_CMD_SWM_SET_DATA_LEN:
            regval = getreg32(dev->reg_base + IRTX_CONFIG_OFFSET);
            regval &= ~IR_CR_IRTX_DATA_NUM_MASK;
            regval |= arg << IR_CR_IRTX_DATA_NUM_SHIFT;
            putreg32(regval, dev->reg_base + IRTX_CONFIG_OFFSET);
            break;

        case IR_CMD_SWM_WRITE_TX_FIFO:
            putreg32(arg, dev->reg_base + IR_FIFO_WDATA_OFFSET);
            break;
#endif

        case IR_CMD_SWM_READ_RX_FIFO:
            ret = getreg32(dev->reg_base + IR_FIFO_RDATA_OFFSET);
            break;

        default:
            ret = -EPERM;
            break;
    }
    return ret;
#endif
}
