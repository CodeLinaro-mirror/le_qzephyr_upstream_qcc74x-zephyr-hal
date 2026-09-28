/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
// #include "qc7xx_irq.h"
// #include "hal_sys.h"
#include "qcc74x_glb.h"
#include "qcc74x_glb_gpio.h"
#include "qcc743_uart.h"
#include "lp_uart.h"
#include "qcc74x_common.h"
#include "glb_reg.h"

#define ATTR_ROM_WIFI_SECTION               //__attribute__((section(".romwifi_code." ATTR_UNI_SYMBOL)))

// static struct qc7xx_device_s *uart0;


int lpp_uart_init(uint8_t id, uint8_t tx_pin, uint8_t rx_pin, uint8_t cts_pin, uint8_t rts_pin, uint32_t baudrate)
{
    UART_CFG_Type uart_dbg_cfg = {
      UART_CLOCK_FREQ, /*UART clock*/
      baudrate,          /* baudrate  */
      UART_DATABITS_8,  /* data bits  */
      UART_STOPBITS_1,  /* stop bits */
      UART_PARITY_NONE, /* parity  */
      DISABLE,          /* Disable auto flow control */
      DISABLE,          /* Disable rx input de-glitch function */
      DISABLE,          /* Disable RTS output SW control mode */
      DISABLE,          /* Disable tx output SW control mode */
      DISABLE,          /* Disable tx lin mode */
      DISABLE,          /* Disable rx lin mode */
      0,                /* Tx break bit count for lin mode */
      UART_LSB_FIRST,   /* UART each data byte is send out LSB-first */
    };

    UART_FifoCfg_Type fifoCfg = {
      16,      /* TX FIFO threshold */
      16,      /* RX FIFO threshold */
      DISABLE, /* Disable tx dma req/ack interface */
      DISABLE  /* Disable rx dma req/ack interface */
    };

    if (id == 0) {
        GLB_UART_Fun_Sel(tx_pin%12,GLB_UART_SIG_FUN_UART0_TXD);
        GLB_UART_Fun_Sel(rx_pin%12,GLB_UART_SIG_FUN_UART0_RXD);
    } else if (id == 1) {
        GLB_UART_Fun_Sel(tx_pin%12,GLB_UART_SIG_FUN_UART1_TXD);
        GLB_UART_Fun_Sel(rx_pin%12,GLB_UART_SIG_FUN_UART1_RXD);
    }
    uint8_t gpioPins[2] = {tx_pin, rx_pin};
    GLB_GPIO_Func_Init(GPIO_FUN_UART,gpioPins,sizeof(gpioPins)/sizeof(gpioPins[0]));


    if (baudrate != 0) {
        uart_dbg_cfg.baudRate = baudrate;
    }

    /* disable all interrupt */
    UART_IntMask(id, UART_INT_ALL, MASK);

    /* disable uart before config */
    UART_Disable(id, UART_TXRX);

    /* uart init with default configuration */
    UART_Init(id, &uart_dbg_cfg);

    /* UART fifo configuration */
    UART_FifoConfig(id, &fifoCfg);

    /* Enable tx free run mode */
    UART_TxFreeRun(id, ENABLE);

    /* Set rx time-out value */
    UART_SetRxTimeoutValue(id, 80);

    /* enable uart */
    UART_Enable(id, UART_TXRX);
    return 0;
}


ATTR_ROM_WIFI_SECTION void lp_uart_init(int uart_num, int bandrate)
{
#define UART_CLOCK        (32 * 1000 * 1000)
#define CR_UTX_EN_BIT     (1 << 0)
#define CR_UTX_FRM_EN_BIT (1 << 2)
#define CR_URX_EN_BIT     (1 << 0)
    uint32_t val;
    uint32_t uart_base = (0x2000A000 + uart_num * 0x100);

    /*bandrate*/
#ifdef CHIP_CODE_QCC743
    *((volatile uint32_t *)(uart_base + 0x08)) = (19 << 16) | (19 << 0);
#elif defined CHIP_CODE_WB03
    *((volatile uint32_t *)(uart_base + 0x08)) = (15 << 16) | (15 << 0);
#endif
    /*tx enable bit*/
    val = *((volatile uint32_t *)(uart_base + 0x00));
    val |= CR_UTX_EN_BIT;
    val |= CR_UTX_FRM_EN_BIT;
    *((volatile uint32_t *)(uart_base + 0x00)) = val;

    val = *((volatile uint32_t *)(uart_base + 0x04));
    val |= CR_URX_EN_BIT;
    *((volatile uint32_t *)(uart_base + 0x04)) = val;
}


ATTR_ROM_WIFI_SECTION void lp_uart_int_enable(int uart_num, int tx, int rx)
{
    uint32_t val;
    uint32_t uart_base = (0x2000A000 + uart_num * 0x100);

#define CR_UTX_FRDY_MASK_BIT (1 << 2)
#define CR_URX_FRDY_MASK_BIT (1 << 3)

    if (rx) {
        val = *((volatile uint32_t *)(uart_base + 0x24));
        val &= (~CR_URX_FRDY_MASK_BIT);
        *((volatile uint32_t *)(uart_base + 0x24)) = val;
    }

    if (tx) {
        val = *((volatile uint32_t *)(uart_base + 0x24));
        val &= (~CR_UTX_FRDY_MASK_BIT);
        *((volatile uint32_t *)(uart_base + 0x24)) = val;
    }
}

ATTR_ROM_WIFI_SECTION void lp_uart_tx(int uart_num, const char c)
{
    int count;
    uint32_t uart_base = (0x2000A000 + uart_num * 0x100);

    do {
        count = *((volatile uint32_t *)(uart_base + 0x84)) & 0x3F;
    } while (count == 0);

    *((volatile uint32_t *)(uart_base + 0x88)) = c;
}


#if LPFW_LOG_EN
// extern void qc7xx_uart_set_console(struct qc7xx_device_s *dev);

// ATTR_ROM_WIFI_SECTION int lpfw_uart_init(void)
// {
//     struct qc7xx_uart_config_s cfg = { 0 };
//     cfg.baudrate = 2000000;
//     cfg.data_bits = UART_DATA_BITS_8;
//     cfg.stop_bits = UART_STOP_BITS_1;
//     cfg.parity = UART_PARITY_NONE;
//     cfg.flow_ctrl = 0;
//     cfg.tx_fifo_threshold = 7;
//     cfg.rx_fifo_threshold = 7;
//     cfg.bit_order = UART_LSB_FIRST;

//     uart0 = qc7xx_device_get_by_name("uart0");

//     qc7xx_uart_init(uart0, &cfg);
//     qc7xx_uart_set_console(uart0);

//     return 0;
// }

/* get uart tx fifo status */
ATTR_ROM_WIFI_SECTION int lpfw_uart_tx_isbusy(uint8_t uart_id)
{
    uint32_t cnt;
    uint32_t uartAddr[2] = { UART0_BASE, UART1_BASE };

    cnt = QC7XX_GET_REG_BITS_VAL(QC7XX_RD_REG(uartAddr[uart_id], UART_FIFO_CONFIG_1), UART_TX_FIFO_CNT);

    if (cnt < 32) {
        return 1;
    } else {
        return 0;
    }

    // return qc7xx_uart_txempty(uart0);
}

#endif
