#ifndef __QCR373_IRQ_H
#define __QCR373_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define QCR373_IRQ_NUM_BASE 19

#define QCR373_IRQ_SSOFT          1
#define QCR373_IRQ_MSOFT          3
#define QCR373_IRQ_STIME          5
#define QCR373_IRQ_MTIME          7
#define QCR373_IRQ_SEXT           9
#define QCR373_IRQ_MEXT           11
#define QCR373_IRQ_CLIC_SOFT_PEND 12
#if (__riscv_xlen == 64)
#define QCR373_IRQ_HPM_OVF 17
#endif

#define QCR373_IRQ_BMX_MCU_BUS_ERR                   (QCR373_IRQ_NUM_BASE + 0)
#define QCR373_IRQ_MJDEC                             (QCR373_IRQ_NUM_BASE + 1)
#define QCR373_IRQ_DISPLAY                           (QCR373_IRQ_NUM_BASE + 2)
#define QCR373_IRQ_USB                               (QCR373_IRQ_NUM_BASE + 3)
#define QCR373_IRQ_WIFI_TO_CPU                       (QCR373_IRQ_NUM_BASE + 4)
#define QCR373_IRQ_RF_TOP_INT0                       (QCR373_IRQ_NUM_BASE + 5)
#define QCR373_IRQ_RF_TOP_INT1                       (QCR373_IRQ_NUM_BASE + 6)
#define QCR373_IRQ_SDIO                              (QCR373_IRQ_NUM_BASE + 7)
#define QCR373_IRQ_WIFI_TBTT_SLEEP                   (QCR373_IRQ_NUM_BASE + 8)
#define QCR373_IRQ_SEC_ENG_ID1_SHA_AES_TRNG_PKA_GMAC (QCR373_IRQ_NUM_BASE + 9)
#define QCR373_IRQ_SEC_ENG_ID0_SHA_AES_TRNG_PKA_GMAC (QCR373_IRQ_NUM_BASE + 10)
#define QCR373_IRQ_SEC_ENG_ID1_CDET                  (QCR373_IRQ_NUM_BASE + 11)
#define QCR373_IRQ_SEC_ENG_ID0_CDET                  (QCR373_IRQ_NUM_BASE + 12)
#define QCR373_IRQ_SF_CTRL_ID1                       (QCR373_IRQ_NUM_BASE + 13)
#define QCR373_IRQ_SF_CTRL_ID0                       (QCR373_IRQ_NUM_BASE + 14)
#define QCR373_IRQ_DMA0_ALL                          (QCR373_IRQ_NUM_BASE + 15)
#define QCR373_IRQ_DVP2BUS_INT0                      (QCR373_IRQ_NUM_BASE + 16)
#define QCR373_IRQ_SDH                               (QCR373_IRQ_NUM_BASE + 17)
#define QCR373_IRQ_DVP2BUS_INT1                      (QCR373_IRQ_NUM_BASE + 18)
#define QCR373_IRQ_WIFI_TBTT_WAKEUP                  (QCR373_IRQ_NUM_BASE + 19)
#define QCR373_IRQ_IR                                (QCR373_IRQ_NUM_BASE + 20)
#define QCR373_IRQ_DMA1_ALL                          (QCR373_IRQ_NUM_BASE + 21)
#define QCR373_IRQ_AUADC                             (QCR373_IRQ_NUM_BASE + 22)
#define QCR373_IRQ_MJPEG                             (QCR373_IRQ_NUM_BASE + 23)
#define QCR373_IRQ_ETH                               (QCR373_IRQ_NUM_BASE + 24)
#define QCR373_IRQ_GPADC_DMA                         (QCR373_IRQ_NUM_BASE + 25)
#define QCR373_IRQ_EFUSE                             (QCR373_IRQ_NUM_BASE + 26)
#define QCR373_IRQ_SPI0                              (QCR373_IRQ_NUM_BASE + 27)
#define QCR373_IRQ_UART0                             (QCR373_IRQ_NUM_BASE + 28)
#define QCR373_IRQ_UART1                             (QCR373_IRQ_NUM_BASE + 29)
#define QCR373_IRQ_UART2                             (QCR373_IRQ_NUM_BASE + 30)
#define QCR373_IRQ_GPIO_DMA                          (QCR373_IRQ_NUM_BASE + 31)
#define QCR373_IRQ_I2C0                              (QCR373_IRQ_NUM_BASE + 32)
#define QCR373_IRQ_PWM                               (QCR373_IRQ_NUM_BASE + 33)
#define QCR373_IRQ_PEC_INT0                          (QCR373_IRQ_NUM_BASE + 34)
#define QCR373_IRQ_PEC_INT1                          (QCR373_IRQ_NUM_BASE + 35)
#define QCR373_IRQ_TIMER0                            (QCR373_IRQ_NUM_BASE + 36)
#define QCR373_IRQ_TIMER1                            (QCR373_IRQ_NUM_BASE + 37)
#define QCR373_IRQ_WDG                               (QCR373_IRQ_NUM_BASE + 38)
#define QCR373_IRQ_I2C1                              (QCR373_IRQ_NUM_BASE + 39)
#define QCR373_IRQ_I2S                               (QCR373_IRQ_NUM_BASE + 40)
#define QCR373_IRQ_ANA_OCP_OUT_TO_CPU                (QCR373_IRQ_NUM_BASE + 41)
#define QCR373_IRQ_RESERVED42                        (QCR373_IRQ_NUM_BASE + 42)
#define QCR373_IRQ_XTAL_RDY_SCAN                     (QCR373_IRQ_NUM_BASE + 43)
#define QCR373_IRQ_GPIO_INT0                         (QCR373_IRQ_NUM_BASE + 44)
#define QCR373_IRQ_DM                                (QCR373_IRQ_NUM_BASE + 45)
#define QCR373_IRQ_BT                                (QCR373_IRQ_NUM_BASE + 46)
#define QCR373_IRQ_M154_REQ_ACK                      (QCR373_IRQ_NUM_BASE + 47)
#define QCR373_IRQ_M154                              (QCR373_IRQ_NUM_BASE + 48)
#define QCR373_IRQ_M154_AES                          (QCR373_IRQ_NUM_BASE + 49)
#define QCR373_IRQ_PDS_WAKEUP                        (QCR373_IRQ_NUM_BASE + 50)
#define QCR373_IRQ_HBN_OUT0                          (QCR373_IRQ_NUM_BASE + 51)
#define QCR373_IRQ_HBN_OUT1                          (QCR373_IRQ_NUM_BASE + 52)
#define QCR373_IRQ_BOD                               (QCR373_IRQ_NUM_BASE + 53)
#define QCR373_IRQ_DMA2D (QCR373_IRQ_NUM_BASE + 54)
#define QCR373_IRQ_BZ_PHY_INT           (QCR373_IRQ_NUM_BASE + 55)
#define QCR373_IRQ_BLE                  (QCR373_IRQ_NUM_BASE + 56)
#define QCR373_IRQ_MAC_INT_TIMER        (QCR373_IRQ_NUM_BASE + 57)
#define QCR373_IRQ_MAC_INT_MISC         (QCR373_IRQ_NUM_BASE + 58)
#define QCR373_IRQ_MAC_INT_RX_TRIGGER   (QCR373_IRQ_NUM_BASE + 59)
#define QCR373_IRQ_MAC_INT_TX_TRIGGER   (QCR373_IRQ_NUM_BASE + 60)
#define QCR373_IRQ_MAC_INT_GEN          (QCR373_IRQ_NUM_BASE + 61)
#define QCR373_IRQ_MAC_INT_PROT_TRIGGER (QCR373_IRQ_NUM_BASE + 62)
#define QCR373_IRQ_WIFI_IPC             (QCR373_IRQ_NUM_BASE + 63)
#define QCR373_IRQ_IPC0_CH0             (QCR373_IRQ_NUM_BASE + 64)
#define QCR373_IRQ_IPC0_CH1             (QCR373_IRQ_NUM_BASE + 65)
#define QCR373_IRQ_IPC1_CH0             (QCR373_IRQ_NUM_BASE + 66)
#define QCR373_IRQ_IPC1_CH1             (QCR373_IRQ_NUM_BASE + 67)
#define QCR373_IRQ_CAN0                 (QCR373_IRQ_NUM_BASE + 68)
#define QCR373_IRQ_CAN1                 (QCR373_IRQ_NUM_BASE + 69)
#define QCR373_IRQ_CAN2                 (QCR373_IRQ_NUM_BASE + 70)
#define QCR373_IRQ_DMA2_INT0            (QCR373_IRQ_NUM_BASE + 71)
#define QCR373_IRQ_DMA2_INT1            (QCR373_IRQ_NUM_BASE + 72)
#define QCR373_IRQ_DMA2_INT2            (QCR373_IRQ_NUM_BASE + 73)
#define QCR373_IRQ_DMA2_INT3            (QCR373_IRQ_NUM_BASE + 74)
#define QCR373_IRQ_PWM1                 (QCR373_IRQ_NUM_BASE + 79)
#define QCR373_IRQ_I2C2                 (QCR373_IRQ_NUM_BASE + 80)
#define QCR373_IRQ_TIMER2               (QCR373_IRQ_NUM_BASE + 82)
#define QCR373_IRQ_TIMER3               (QCR373_IRQ_NUM_BASE + 83)
#define QCR373_IRQ_WDG1                 (QCR373_IRQ_NUM_BASE + 84)
#define QCR373_IRQ_UART3                (QCR373_IRQ_NUM_BASE + 85)
#define QCR373_IRQ_SPI3 (QCR373_IRQ_NUM_BASE + 86)
#define QCR373_IRQ_RASTER2BLOCK (QCR373_IRQ_NUM_BASE + 87)
#define QCR373_IRQ_DTSRC        (QCR373_IRQ_NUM_BASE + 88)
#define QCR373_IRQ_OSD0         (QCR373_IRQ_NUM_BASE + 89)
#define QCR373_IRQ_OSD1         (QCR373_IRQ_NUM_BASE + 90)
#define QCR373_IRQ_MM_SEOF0 (QCR373_IRQ_NUM_BASE + 91)
#define QCR373_IRQ_MM_SEOF1 (QCR373_IRQ_NUM_BASE + 92)
#define QCR373_IRQ_DSI      (QCR373_IRQ_NUM_BASE + 93)
#define QCR373_IRQ_SPI1     (QCR373_IRQ_NUM_BASE + 94)
#define QCR373_IRQ_SPI2     (QCR373_IRQ_NUM_BASE + 95)
#define QCR373_IRQ_TOUCH    (QCR373_IRQ_NUM_BASE + 96)
#define QCR373_IRQ_AUDAC    (QCR373_IRQ_NUM_BASE + 97)

#endif
