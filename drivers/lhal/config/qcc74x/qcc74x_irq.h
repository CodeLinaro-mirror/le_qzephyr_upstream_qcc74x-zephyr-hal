/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCC74X_IRQ_H
#define __QCC74X_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define QCC74X_IRQ_NUM_BASE       16

#define QCC74X_IRQ_SSOFT          1
#define QCC74X_IRQ_MSOFT          3
#define QCC74X_IRQ_STIME          5
#define QCC74X_IRQ_MTIME          7
#define QCC74X_IRQ_SEXT           9
#define QCC74X_IRQ_MEXT           11
#define QCC74X_IRQ_CLIC_SOFT_PEND 12
#if (__riscv_xlen == 64)
#define QCC74X_IRQ_HPM_OVF 17
#endif

#define QCC74X_IRQ_BMX_MCU_BUS_ERR                   (QCC74X_IRQ_NUM_BASE + 0)
#define QCC74X_IRQ_BMX_MCU_TO                        (QCC74X_IRQ_NUM_BASE + 1)
#define QCC74X_IRQ_DBI                               (QCC74X_IRQ_NUM_BASE + 2)
#define QCC74X_IRQ_SDU_SOFT_RST                      (QCC74X_IRQ_NUM_BASE + 3)
#define QCC74X_IRQ_AUDAC                             (QCC74X_IRQ_NUM_BASE + 4)
#define QCC74X_IRQ_RF_TOP_INT0                       (QCC74X_IRQ_NUM_BASE + 5)
#define QCC74X_IRQ_RF_TOP_INT1                       (QCC74X_IRQ_NUM_BASE + 6)
#define QCC74X_IRQ_SDIO                              (QCC74X_IRQ_NUM_BASE + 7)
#define QCC74X_IRQ_WIFI_TBTT_SLEEP                   (QCC74X_IRQ_NUM_BASE + 8)
#define QCC74X_IRQ_SEC_ENG_ID1_SHA_AES_TRNG_PKA_GMAC (QCC74X_IRQ_NUM_BASE + 9)
#define QCC74X_IRQ_SEC_ENG_ID0_SHA_AES_TRNG_PKA_GMAC (QCC74X_IRQ_NUM_BASE + 10)
#define QCC74X_IRQ_SEC_ENG_ID1_CDET                  (QCC74X_IRQ_NUM_BASE + 11)
#define QCC74X_IRQ_SEC_ENG_ID0_CDET                  (QCC74X_IRQ_NUM_BASE + 12)
#define QCC74X_IRQ_SF_CTRL_ID1                       (QCC74X_IRQ_NUM_BASE + 13)
#define QCC74X_IRQ_SF_CTRL_ID0                       (QCC74X_IRQ_NUM_BASE + 14)
#define QCC74X_IRQ_DMA0_ALL                          (QCC74X_IRQ_NUM_BASE + 15)
#define QCC74X_IRQ_DVP2BUS_INT0                      (QCC74X_IRQ_NUM_BASE + 16)
#define QCC74X_IRQ_SDH                               (QCC74X_IRQ_NUM_BASE + 17)
#define QCC74X_IRQ_DVP2BUS_INT1                      (QCC74X_IRQ_NUM_BASE + 18)
#define QCC74X_IRQ_WIFI_TBTT_WAKEUP                  (QCC74X_IRQ_NUM_BASE + 19)
#define QCC74X_IRQ_IRRX                              (QCC74X_IRQ_NUM_BASE + 20)
#define QCC74X_IRQ_USB                               (QCC74X_IRQ_NUM_BASE + 21)
#define QCC74X_IRQ_AUADC                             (QCC74X_IRQ_NUM_BASE + 22)
#define QCC74X_IRQ_MJPEG                             (QCC74X_IRQ_NUM_BASE + 23)
#define QCC74X_IRQ_EMAC                              (QCC74X_IRQ_NUM_BASE + 24)
#define QCC74X_IRQ_GPADC_DMA                         (QCC74X_IRQ_NUM_BASE + 25)
#define QCC74X_IRQ_EFUSE                             (QCC74X_IRQ_NUM_BASE + 26)
#define QCC74X_IRQ_SPI0                              (QCC74X_IRQ_NUM_BASE + 27)
#define QCC74X_IRQ_UART0                             (QCC74X_IRQ_NUM_BASE + 28)
#define QCC74X_IRQ_UART1                             (QCC74X_IRQ_NUM_BASE + 29)
#define QCC74X_IRQ_GPIO_DMA                          (QCC74X_IRQ_NUM_BASE + 31)
#define QCC74X_IRQ_I2C0                              (QCC74X_IRQ_NUM_BASE + 32)
#define QCC74X_IRQ_PWM                               (QCC74X_IRQ_NUM_BASE + 33)
#define QCC74X_IRQ_RESERVED0                         (QCC74X_IRQ_NUM_BASE + 34)
#define QCC74X_IRQ_RESERVED1                         (QCC74X_IRQ_NUM_BASE + 35)
#define QCC74X_IRQ_TIMER0                            (QCC74X_IRQ_NUM_BASE + 36)
#define QCC74X_IRQ_TIMER1                            (QCC74X_IRQ_NUM_BASE + 37)
#define QCC74X_IRQ_WDG                               (QCC74X_IRQ_NUM_BASE + 38)
#define QCC74X_IRQ_I2C1                              (QCC74X_IRQ_NUM_BASE + 39)
#define QCC74X_IRQ_I2S                               (QCC74X_IRQ_NUM_BASE + 40)
#define QCC74X_IRQ_ANA_OCP_OUT_TO_CPU_0              (QCC74X_IRQ_NUM_BASE + 41)
#define QCC74X_IRQ_ANA_OCP_OUT_TO_CPU_1              (QCC74X_IRQ_NUM_BASE + 42)
#define QCC74X_IRQ_XTAL_RDY_SCAN                     (QCC74X_IRQ_NUM_BASE + 43)
#define QCC74X_IRQ_GPIO_INT0                         (QCC74X_IRQ_NUM_BASE + 44)
#define QCC74X_IRQ_DM                                (QCC74X_IRQ_NUM_BASE + 45)
#define QCC74X_IRQ_BT                                (QCC74X_IRQ_NUM_BASE + 46)
#define QCC74X_IRQ_M154_REQ_ACK                      (QCC74X_IRQ_NUM_BASE + 47)
#define QCC74X_IRQ_M154                              (QCC74X_IRQ_NUM_BASE + 48)
#define QCC74X_IRQ_M154_AES                          (QCC74X_IRQ_NUM_BASE + 49)
#define QCC74X_IRQ_PDS_WAKEUP                        (QCC74X_IRQ_NUM_BASE + 50)
#define QCC74X_IRQ_HBN_OUT0                          (QCC74X_IRQ_NUM_BASE + 51)
#define QCC74X_IRQ_HBN_OUT1                          (QCC74X_IRQ_NUM_BASE + 52)
#define QCC74X_IRQ_BOD                               (QCC74X_IRQ_NUM_BASE + 53)
#define QCC74X_IRQ_WIFI                              (QCC74X_IRQ_NUM_BASE + 54)
#define QCC74X_IRQ_BZ_PHY_INT                        (QCC74X_IRQ_NUM_BASE + 55)
#define QCC74X_IRQ_BLE                               (QCC74X_IRQ_NUM_BASE + 56)
#define QCC74X_IRQ_MAC_INT_TIMER                     (QCC74X_IRQ_NUM_BASE + 57)
#define QCC74X_IRQ_MAC_INT_MISC                      (QCC74X_IRQ_NUM_BASE + 58)
#define QCC74X_IRQ_MAC_INT_RX_TRIGGER                (QCC74X_IRQ_NUM_BASE + 59)
#define QCC74X_IRQ_MAC_INT_TX_TRIGGER                (QCC74X_IRQ_NUM_BASE + 60)
#define QCC74X_IRQ_MAC_INT_GEN                       (QCC74X_IRQ_NUM_BASE + 61)
#define QCC74X_IRQ_MAC_INT_PROT_TRIGGER              (QCC74X_IRQ_NUM_BASE + 62)
#define QCC74X_IRQ_WIFI_IPC                          (QCC74X_IRQ_NUM_BASE + 63)

#endif
