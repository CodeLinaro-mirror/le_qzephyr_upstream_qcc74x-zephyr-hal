/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCR373_H__
#define __QCR373_H__

/** @addtogroup Configuration_section_for_RISCV
  * @{
  */

/**
  * @brief Configuration of the Processor and Core Peripherals
   */

#define CORE_ID_ADDRESS (0xF0000000)
#define CORE_ID_M0 (0xA900A900)
#define CORE_ID_M1 (0xF900F900)
#define CORE_ID_LP (0xDEAD300D)

/**
  * @}
  */

/** @addtogroup Peripheral_interrupt_number_definition
  * @{
  */

#ifdef ARCH_RISCV
#if (__riscv_xlen == 64)
#define IRQ_NUM_BASE 16 /* PLIC ===> MEXT_IRQn */
#else
#define IRQ_NUM_BASE 19
#endif
#endif
/**
 * @brief QCR373 Interrupt Number Definition, according to the selected device
 *        in @ref Library_configuration_section
 */
typedef enum {
#ifdef ARCH_RISCV
    SSOFT_IRQn = 1,           /*!< 1 RISCV supervisor software Interrupt                             */
    MSOFT_IRQn = 3,           /*!< 3 RISCV machine software Interrupt                                */
    STIME_IRQn = 5,           /*!< 5 RISCV supervisor time Interrupt                                 */
    MTIME_IRQn = 7,           /*!< 7 RISCV machine time Interrupt                                    */
    SEXT_IRQn = 9,            /*!< 9 RISCV S-mode external  Interrupt                                */
    MEXT_IRQn = 11,           /*!< 11 RISCV M-mode external  Interrupt                               */
    CLIC_SOFT_PEND_IRQn = 12, /*!< 12 RISCV CLIC software pending  Interrupt                         */
#if (__riscv_xlen == 64)
    HPM_OVF_IRQn = 17, /*!< 17 RISCV HPM counter overflow Interrupt                           */
#endif
#endif
    /******  QCR373 specific Interrupt Numbers **********************************************************************/
    BMX_MCU_BUS_ERR_IRQn = IRQ_NUM_BASE + 0,                    /*!< bmx mcu bus_err_int Interrupt                  */
    MJDEC_IRQn = IRQ_NUM_BASE + 1,                              /*!< MJDEC Interrupt                                */
    DISPLAY_IRQn = IRQ_NUM_BASE + 2,                            /*!< Display Interrupt                              */
    USB_IRQn = IRQ_NUM_BASE + 3,                                /*!< USB Interrupt                                  */
    WIFI_IRQn = IRQ_NUM_BASE + 4,                               /*!< WIFI To CPU Interrupt                          */
    RF_TOP_INT0_IRQn = IRQ_NUM_BASE + 5,                        /*!< RF_TOP_INT0 Interrupt                          */
    RF_TOP_INT1_IRQn = IRQ_NUM_BASE + 6,                        /*!< RF_TOP_INT1 Interrupt                          */
    SDIO_IRQn = IRQ_NUM_BASE + 7,                               /*!< SDIO Interrupt                                 */
    WIFI_TBTT_SLEEP_IRQn = IRQ_NUM_BASE + 8,                    /*!< WIFI TBTT Sleep Interrupt                      */
    SEC_ENG_ID1_SHA_AES_TRNG_PKA_GMAC_IRQn = IRQ_NUM_BASE + 9,  /*!< Sec_eng_id1 Interrupt                          */
    SEC_ENG_ID0_SHA_AES_TRNG_PKA_GMAC_IRQn = IRQ_NUM_BASE + 10, /*!< Sec_eng_id0 Interrupt                          */
    SEC_ENG_ID1_CDET_IRQn = IRQ_NUM_BASE + 11,                  /*!< Sec_eng_id1_cdet  Interrupt                    */
    SEC_ENG_ID0_CDET_IRQn = IRQ_NUM_BASE + 12,                  /*!< Sec_eng_id0_cdet Interrupt                     */
    SF_CTRL_ID1_IRQn = IRQ_NUM_BASE + 13,                       /*!< Sf_ctrl_id1 Interrupt                          */
    SF_CTRL_ID0_IRQn = IRQ_NUM_BASE + 14,                       /*!< Sf_ctrl_id0 Interrupt                          */
    DMA0_ALL_IRQn = IRQ_NUM_BASE + 15,                          /*!< DMA0_INTR_ALL Interrupt                        */
    DVP2BUS_INT0_IRQn = IRQ_NUM_BASE + 16,                      /*!< DVP2BUS_INT0 Interrupt                         */
    SDH_IRQn = IRQ_NUM_BASE + 17,                               /*!< SDH Interrupt                                  */
    DVP2BUS_INT1_IRQn = IRQ_NUM_BASE + 18,                      /*!< DVP2BUS_INT1 Interrupt                         */
    WIFI_TBTT_WAKEUP_IRQn = IRQ_NUM_BASE + 19,                  /*!< WIFI TBTT Wakeup Interrupt                     */
    IR_IRQn = IRQ_NUM_BASE + 20,                                /*!< IR Interrupt                                   */
    DMA1_ALL_IRQn = IRQ_NUM_BASE + 21,                          /*!< DMA1_INTR_ALL Interrupt                        */
    AUADC_IRQn = IRQ_NUM_BASE + 22,                             /*!< auadc_int Interrupt                            */
    MJPEG_IRQn = IRQ_NUM_BASE + 23,                             /*!< MJPEG Interrupt                                */
    EMAC_IRQn = IRQ_NUM_BASE + 24,                              /*!< EMAC  Interrupt                                */
    GPADC_DMA_IRQn = IRQ_NUM_BASE + 25,                         /*!< GPADC_DMA Interrupt                            */
    EFUSE_IRQn = IRQ_NUM_BASE + 26,                             /*!< Efuse Interrupt                                */
    SPI0_IRQn = IRQ_NUM_BASE + 27,                              /*!< SPI0 Interrupt                                 */
    UART0_IRQn = IRQ_NUM_BASE + 28,                             /*!< UART0 Interrupt                                */
    UART1_IRQn = IRQ_NUM_BASE + 29,                             /*!< UART1 Interrupt                                */
    UART2_IRQn = IRQ_NUM_BASE + 30,                             /*!< UART2 Interrupt                                */
    GPIO_DMA_IRQn = IRQ_NUM_BASE + 31,                          /*!< GPIO DMA Interrupt                             */
    I2C0_IRQn = IRQ_NUM_BASE + 32,                              /*!< I2C0 Interrupt                                 */
    PWM_IRQn = IRQ_NUM_BASE + 33,                               /*!< PWM Interrupt                                  */
    PEC_INT0_IRQn = IRQ_NUM_BASE + 34,                          /*!< PEC_INT0 Interrupt                             */
    PEC_INT1_IRQn = IRQ_NUM_BASE + 35,                          /*!< PEC_INT1 Interrupt                             */
    TIMER0_CH0_IRQn = IRQ_NUM_BASE + 36,                        /*!< Timer0 Channel 0 Interrupt                     */
    TIMER0_CH1_IRQn = IRQ_NUM_BASE + 37,                        /*!< Timer0 Channel 1 Interrupt                     */
    TIMER0_WDT_IRQn = IRQ_NUM_BASE + 38,                        /*!< Timer0 Watch Dog Interrupt                     */
    I2C1_IRQn = IRQ_NUM_BASE + 39,                              /*!< I2C1 Interrupt                                 */
    I2S_IRQn = IRQ_NUM_BASE + 40,                               /*!< I2S Interrupt                                  */
    ANA_OCP_OUT_TO_CPU_IRQn = IRQ_NUM_BASE + 41,                /*!< ana_ocp_out_to_cpu_irq Interrupt               */
    RESERVED42_IRQn = IRQ_NUM_BASE + 42,                        /*!< Reserved Interrupt                             */
    XTAL_RDY_SCAN_IRQn = IRQ_NUM_BASE + 43,                     /*!< Xtal Ready Scan Interrupt                      */
    GPIO_INT0_IRQn = IRQ_NUM_BASE + 44,                         /*!< GPIO Interrupt                                 */
    DM_IRQn = IRQ_NUM_BASE + 45,                                /*!< DM Interrupt                                   */
    BT_IRQn = IRQ_NUM_BASE + 46,                                /*!< BT Interrupt                                   */
    M154_REQ_ACK_IRQn = IRQ_NUM_BASE + 47,                      /*!< M154 req enh ack Interrupt                     */
    M154_INT_IRQn = IRQ_NUM_BASE + 48,                          /*!< M154 Interrupt                                 */
    M154_AES_IRQn = IRQ_NUM_BASE + 49,                          /*!< m154 aes Interrupt                             */
    PDS_WAKEUP_IRQn = IRQ_NUM_BASE + 50,                        /*!< PDS Wakeup Interrupt                           */
    HBN_OUT0_IRQn = IRQ_NUM_BASE + 51,                          /*!< Hibernate out 0 Interrupt                      */
    HBN_OUT1_IRQn = IRQ_NUM_BASE + 52,                          /*!< Hibernate out 1 Interrupt                      */
    BOD_IRQn = IRQ_NUM_BASE + 53,                               /*!< BOR Interrupt                                  */
    DMA2D_IRQn = IRQ_NUM_BASE + 54,                             /*!< DMA2D Interrupt                                */
    BZ_PHY_INT_IRQn = IRQ_NUM_BASE + 55,                        /*!< BZ phy Interrupt                               */
    BLE_IRQn = IRQ_NUM_BASE + 56,                               /*!< BLE Interrupt                                  */
    MAC_INT_TIMER_IRQn = IRQ_NUM_BASE + 57,                     /*!< MAC Tx Rx Timer Interrupt                      */
    MAC_INT_MISC_IRQn = IRQ_NUM_BASE + 58,                      /*!< MAC Tx Rx Misc Interrupt                       */
    MAC_INT_RX_TRIGGER_IRQn = IRQ_NUM_BASE + 59,                /*!< MAC Rx Trigger Interrupt                       */
    MAC_INT_TX_TRIGGER_IRQn = IRQ_NUM_BASE + 60,                /*!< MAC tx Trigger Interrupt                       */
    MAC_INT_GEN_IRQn = IRQ_NUM_BASE + 61,                       /*!< MAC Gen Interrupt                              */
    MAC_INT_PROT_TRIGGER_IRQn = IRQ_NUM_BASE + 62,              /*!< MAC Prot Trigger Interrupt                     */
    WIFI_IPC_IRQn = IRQ_NUM_BASE + 63,                          /*!< WIFI IPC Interrupt                             */
    IPC0_AP2NP_IRQn = IRQ_NUM_BASE + 64,                        /*!< IPC0 AP To NP Interrupt                        */
    IPC0_NP2AP_IRQn = IRQ_NUM_BASE + 65,                        /*!< IPC0 NP To AP Interrupt                        */
    IPC1_AP2NP_IRQn = IRQ_NUM_BASE + 66,                        /*!< IPC1 AP To NP Interrupt                        */
    IPC1_NP2AP_IRQn = IRQ_NUM_BASE + 67,                        /*!< IPC1 NP To AP Interrupt                        */
    CANFD_IRQn = IRQ_NUM_BASE + 68,  /*!< CANFD Interrupt                              */
    CANFD1_IRQn = IRQ_NUM_BASE + 69, /*!< CANFD1 Interrupt                             */
    CANFD2_IRQn = IRQ_NUM_BASE + 70, /*!< CANFD2 Interrupt                             */
    DMA2_INT0_IRQn = IRQ_NUM_BASE + 71,                         /*!< DMA2_INT0 Interrupt                            */
    DMA2_INT1_IRQn = IRQ_NUM_BASE + 72,                         /*!< DMA2_INT1 Interrupt                            */
    DMA2_INT2_IRQn = IRQ_NUM_BASE + 73,                         /*!< DMA2_INT2 Interrupt                            */
    DMA2_INT3_IRQn = IRQ_NUM_BASE + 74,                         /*!< DMA2_INT3 Interrupt                            */
    RESERVED75_IRQn = IRQ_NUM_BASE + 75,                        /*!< Reserved Interrupt                             */
    RESERVED76_IRQn = IRQ_NUM_BASE + 76,                        /*!< Reserved Interrupt                             */
    RESERVED77_IRQn = IRQ_NUM_BASE + 77,                        /*!< Reserved Interrupt                             */
    RESERVED78_IRQn = IRQ_NUM_BASE + 78,                        /*!< Reserved Interrupt                             */
    PWM1_IRQn = IRQ_NUM_BASE + 79,                              /*!< PWM1 Interrupt                                 */
    I2C2_IRQn = IRQ_NUM_BASE + 80,                              /*!< I2C2 Interrupt                                 */
    RESERVED81_IRQn = IRQ_NUM_BASE + 81,                        /*!< Reserved Interrupt                             */
    TIMER2_IRQn = IRQ_NUM_BASE + 82,                            /*!< Timer1 Channel 0 Interrupt                     */
    TIMER3_IRQn = IRQ_NUM_BASE + 83,                            /*!< Timer1 Channel 1 Interrupt                     */
    WDG1_IRQn = IRQ_NUM_BASE + 84,                              /*!< Timer1 Watch Dog Interrupt                     */
    UART3_IRQn = IRQ_NUM_BASE + 85,                             /*!< UART3 Interrupt                                */
    SPI3_IRQn = IRQ_NUM_BASE + 86,   /*!< SPI3 Interrupt                                 */
    RASTER2BLOCK_IRQn = IRQ_NUM_BASE + 87,                      /*!< Raster2block Interrupt                         */
    DTSRC_IRQn = IRQ_NUM_BASE + 88,                             /*!< DTSRC Interrupt                                */
    OSD0_IRQn = IRQ_NUM_BASE + 89,                              /*!< OSD0 Interrupt                                 */
    OSD1_IRQn = IRQ_NUM_BASE + 90,                              /*!< OSD1 Interrupt                                 */
    MM_SEOF0_IRQn = IRQ_NUM_BASE + 91,                          /*!< MM SEOF0 Interrupt                             */
    MM_SEOF1_IRQn = IRQ_NUM_BASE + 92,                          /*!< MM SEOF1 Interrupt                             */
    DSI_IRQn = IRQ_NUM_BASE + 93,                               /*!< DSI Interrupt                                  */
    SPI1_IRQn = IRQ_NUM_BASE + 94,                              /*!< SPI1 Interrupt                                 */
    SPI2_IRQn = IRQ_NUM_BASE + 95,                              /*!< SPI2 Interrupt                                 */
    TOUCH_IRQn = IRQ_NUM_BASE + 96,                             /*!< Touch Interrupt                                */
    IRQn_LAST,
} IRQn_Type;

/**
 * @brief QCR373 Memory Map Definitions
 */
#define QCR373_OCRAM_BASE           (0x21000000)
#define QCR373_OCRAM_END            (0x21000000 + 320 * 1024)
#define QCR373_OCRAM_CACHEABLE_BASE (0xA1000000)
#define QCR373_OCRAM_CACHEABLE_END  (0xA1000000 + 320 * 1024)
//#define QCR373_OCRAM_BUSREMAP_BASE            (0x23000000)
//#define QCR373_OCRAM_BUSREMAP_END             (0x23000000 + 320 * 1024)
//#define QCR373_OCRAM_BUSREMAP_CACHEABLE_BASE  (0x63000000)
//#define QCR373_OCRAM_BUSREMAP_CACHEABLE_END   (0x63000000 + 320 * 1024)

#define QCR373_WRAM_BASE           (0x21070000)
#define QCR373_WRAM_END            (0x21070000 + 320 * 1024)
#define QCR373_WRAM_CACHEABLE_BASE (0xA1070000)
#define QCR373_WRAM_CACHEABLE_END  (0xA1070000 + 320 * 1024)
//#define QCR373_WRAM_BUSREMAP_BASE             (0x23050000)
//#define QCR373_WRAM_BUSREMAP_END              (0x23050000 + 320 * 1024)
//#define QCR373_WRAM_BUSREMAP_CACHEABLE_BASE   (0x63050000)
//#define QCR373_WRAM_BUSREMAP_CACHEABLE_END    (0x63050000 + 320 * 1024)

#define QCR373_L2RAM_BASE           (0x21050000)
#define QCR373_L2RAM_END            (0x21050000 + 128 * 1024)
#define QCR373_L2RAM_CACHEABLE_BASE (0xA1050000)
#define QCR373_L2RAM_CACHEABLE_END  (0xA1050000 + 128 * 1024)
//#define QCR373_L2RAM_BUSREMAP_BASE            (0x230A0000)
//#define QCR373_L2RAM_BUSREMAP_END             (0x230A0000 + 160 * 1024)
//#define QCR373_L2RAM_BUSREMAP_CACHEABLE_BASE  (0x630A0000)
//#define QCR373_L2RAM_BUSREMAP_CACHEABLE_END   (0x630A0000 + 160 * 1024)

#define QCR373_XRAM_BASE           (0x210C0000)
#define QCR373_XRAM_END            (0x210C0000 + 16 * 1024)
#define QCR373_XRAM_CACHEABLE_BASE (0xA10C0000)
#define QCR373_XRAM_CACHEABLE_END  (0xA10C0000 + 16 * 1024)
//#define QCR373_XRAM_BUSREMAP_BASE             (0x230C8000)
//#define QCR373_XRAM_BUSREMAP_END              (0x230C8000 + 16 * 1024)
//#define QCR373_XRAM_BUSREMAP_CACHEABLE_BASE   (0x630C8000)
//#define QCR373_XRAM_BUSREMAP_CACHEABLE_END    (0x630C8000 + 16 * 1024)

#define QCR373_LPRAM_BASE (0x00010000)
#define QCR373_LPRAM_END  (0x00010000 + 64 * 1024)

#define QCR373_FLASH_XIP_BASE        (0x30000000)
#define QCR373_FLASH_XIP_END         (0x30000000 + 64 * 1024 * 1024)
#define QCR373_FLASH_XIP_REMAP0_BASE (0xB0000000)
#define QCR373_FLASH_XIP_REMAP0_END  (0xB0000000 + 64 * 1024 * 1024)
//#define QCR373_FLASH_XIP_BUSREMAP_BASE        (0x10000000)
//#define QCR373_FLASH_XIP_BUSREMAP_END         (0x10000000 + 64 * 1024 * 1024)

#define QCR373_FLASH2_XIP_BASE        (0x34000000)
#define QCR373_FLASH2_XIP_END         (0x34000000 + 64 * 1024 * 1024)
#define QCR373_FLASH2_XIP_REMAP0_BASE (0xB4000000)
#define QCR373_FLASH2_XIP_REMAP0_END  (0xB4000000 + 64 * 1024 * 1024)
//#define QCR373_FLASH2_XIP_BUSREMAP_BASE       (0x14000000)
//#define QCR373_FLASH2_XIP_BUSREMAP_END        (0x14000000 + 64 * 1024 * 1024)

#define QCR373_PSRAM_BASE        (0x38000000)
#define QCR373_PSRAM_END         (0x38000000 + 128 * 1024 * 1024)
#define QCR373_PSRAM_REMAP0_BASE (0xB8000000)
#define QCR373_PSRAM_REMAP0_END  (0xB8000000 + 128 * 1024 * 1024)
//#define QCR373_PSRAM_BUSREMAP_BASE            (0xA8000000)
//#define QCR373_PSRAM_BUSREMAP_END             (0xA8000000 + 128 * 1024 * 1024)

#define QCR373_ALLRAM_BASE           (0x21000000)
#define QCR373_ALLRAM_END            (0x21000000 + 320 * 1024 + 128 * 1024 + 320 * 1024 + 16 * 1024)
#define QCR373_ALLRAM_CACHEABLE_BASE (0xA1000000)
#define QCR373_ALLRAM_CACHEABLE_END  (0xA1000000 + 320 * 1024 + 128 * 1024 + 320 * 1024 + 16 * 1024)
//#define QCR373_ALLRAM_BUSREMAP_BASE           (0x23000000)
//#define QCR373_ALLRAM_BUSREMAP_END            (0x23000000 + 320 * 1024 + 320 * 1024 + 160 * 1024 + 16 * 1024 )
//#define QCR373_ALLRAM_BUSREMAP_CACHEABLE_BASE (0x63000000)
//#define QCR373_ALLRAM_BUSREMAP_CACHEABLE_END  (0x63000000 + 320 * 1024 + 320 * 1024 + 160 * 1024 + 16 * 1024 )

/*@} end of group Memory_Map_Section */

/* QCR373 peripherals base address */
/* WLSYS */
#define GLB_BASE     ((uint32_t)0x20000000)
#define DSI_PLL_BASE ((uint32_t)0x20000700)
#define RF_BASE      ((uint32_t)0x20001000)
#define GPIP_BASE    ((uint32_t)0x20002000)
#define SEC_DBG_BASE ((uint32_t)0x20003000)
#define RF2_BASE     ((uint32_t)0x20004000)
#define DMA2D_BASE ((uint32_t)0x20005000)
#define TZ1_BASE      ((uint32_t)0x20006000)
#define TZC_SEC_BASE  ((uint32_t)0x20006000)
#define TZ2_BASE      ((uint32_t)0x20007000)
#define TZC_NSEC_BASE ((uint32_t)0x20007000)
#define CCI_BASE      ((uint32_t)0x20008000)
#define MCU_MISC_BASE ((uint32_t)0x20009000)
#define USB2_PHY_BASE ((uint32_t)0x2000A000)
#define SPI_BASE ((uint32_t)0x2000B000)
#define EFUSE_BASE      ((uint32_t)0x2000C000)
#define EF_DATA_BASE    ((uint32_t)0x2000C000)
#define EF_CTRL_BASE    ((uint32_t)0x2000C000)
#define PSRAM_CTRL_BASE ((uint32_t)0x2000D000)
#define CANFD_BASE  ((uint32_t)0x2000E000)
#define CANFD1_BASE ((uint32_t)0x2000E800)
#define CANFD2_BASE ((uint32_t)0x2000F000)
#define KYS_BASE   ((uint32_t)0x2000F800)
#define UART0_BASE ((uint32_t)0x20010000)
#define UART1_BASE ((uint32_t)0x20011000)
#define UART2_BASE ((uint32_t)0x20012000)
#define IPC0_BASE  ((uint32_t)0x20013000)
#define I2C0_BASE  ((uint32_t)0x20014000)
#define I2C1_BASE  ((uint32_t)0x20015000)
#define IPC1_BASE  ((uint32_t)0x20016000)
#define AUADC_BASE ((uint32_t)0x20017C00)
#define SPI1_BASE  ((uint32_t)0x20018000)
#define PWM_BASE   ((uint32_t)0x20019000)
#define TIMER_BASE ((uint32_t)0x2001A000)
#define IR_BASE    ((uint32_t)0x2001B000)
#define CKS_BASE   ((uint32_t)0x2001C000)
#define DBI_BASE   ((uint32_t)0x2001D000)
#define I2S_BASE   ((uint32_t)0x2001E000)
#define PEC_BASE   ((uint32_t)0x2001F000)
#define DMA1_BASE  ((uint32_t)0x20021000)
#define AUDAC_BASE ((uint32_t)0x20022000)
#define SPI2_BASE  ((uint32_t)0x20023000)
#define SEC_ENG_BASE     ((uint32_t)0x20080000)
#define DMA_BASE         ((uint32_t)0x20081000)
#define QSPI_BASE        ((uint32_t)0x20082000)
#define SF_CTRL_BASE     ((uint32_t)0x20082000)
#define SF_CTRL_BUF_BASE ((uint32_t)0x20082600)
#define EMAC0_BASE       ((uint32_t)0x20083000)
#define SDH_BASE         ((uint32_t)0x20084000)
#define SDU_BASE         ((uint32_t)0x20085000)
#define USB_BASE          ((uint32_t)0x20087000)
#define EMAC1_BASE        ((uint32_t)0x20089000)
#define GMAC0_BASE        ((uint32_t)0x2008A000)
#define GMAC1_BASE        ((uint32_t)0x2008C000)
#define PDS_BASE          ((uint32_t)0x2008E000)
#define HBN_BASE          ((uint32_t)0x2008F000)
#define AON_BASE          ((uint32_t)0x2008F000)
#define HBN_RAM_BASE      ((uint32_t)0x20090000)

/* MMSYS */
#define MM_MISC_BASE      ((uint32_t)0x20040000)
#define DVP2AXI0_BASE     ((uint32_t)0x20041000)
#define DVP2AXI1_BASE     ((uint32_t)0x20042000)
#define MJPEG_BASE        ((uint32_t)0x20043000)
#define MJDEC_BASE        ((uint32_t)0x20044000)
#define DTSRC_BASE        ((uint32_t)0x20045000)
#define RASTER2BLOCK_BASE ((uint32_t)0x20046000)
#define BLOCK2RASTER_BASE ((uint32_t)0x20047000)
#define DVP2SRAM0_BASE    ((uint32_t)0x20048000)
#define DVP2SRAM1_BASE    ((uint32_t)0x20049000)
#define OSD_BASE          ((uint32_t)0x2004A000)
#define GLB_WL_BASE       ((uint32_t)0x24000000)

/* LPSYS */
#define MINI_MISC         ((uint32_t)0x00080000)
#define PWM1_BASE         ((uint32_t)0x00081000)
#define I2C2_BASE         ((uint32_t)0x00082000)
#define TIMER1_BASE       ((uint32_t)0x00083000)
#define UART3_BASE        ((uint32_t)0x00084000)
#define SPI3_BASE ((uint32_t)0x00085000)
#define DMA2_BASE ((uint32_t)0x000C0000)

/**
  * @}
  */
#include <stdint.h>

/* RISCV CPU include files */
#ifdef __GNUC__
#include <nmsis_core.h>
#endif

#endif
