/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCC743_UART_H__
#define __QCC743_UART_H__

#include "uart_reg.h"
#include "qcc74x_common.h"

/** @addtogroup  QCC743_Peripheral_Driver
 *  @{
 */

/** @addtogroup  UART
 *  @{
 */

/** @defgroup  UART_Public_Types
 *  @{
 */

/**
 *  @brief UART port type definition
 */
#define UART0_ID        (0)
#define UART1_ID        (1)
#define UART_ID_MAX     (2)


/**
 *  @brief UART direction type definition
 */
#define UART_TX     (0)
#define UART_RX     (1)
#define UART_TXRX   (2)


/**
 *  @brief UART parity type definition
 */
#define UART_PARITY_NONE    (0)
#define UART_PARITY_ODD     (1)
#define UART_PARITY_EVEN    (2)


/**
 *  @brief UART data bits type definiton
 */
#define UART_DATABITS_5     (0)
#define UART_DATABITS_6     (1)
#define UART_DATABITS_7     (2)
#define UART_DATABITS_8     (3)


/**
 *  @brief UART stop bits type definiton
 */
#define UART_STOPBITS_0_5   (0)
#define UART_STOPBITS_1     (1)
#define UART_STOPBITS_1_5   (2)
#define UART_STOPBITS_2     (3)


/**
 *  @brief UART each data byte is send out LSB-first or MSB-first type definiton
 */
#define UART_LSB_FIRST      (0)
#define UART_MSB_FIRST      (1)


/**
 *  @brief UART auto baudrate detection using codeword 0x55 or start bit definiton
 */
#define UART_AUTOBAUD_0X55          (0)
#define UART_AUTOBAUD_STARTBIT      (1)


/**
 *  @brief UART RS-485 mode DE pin polarity definiton
 */
#define UART_RS485_DE_ACTIVE_LOW    (0)
#define UART_RS485_DE_ACTIVE_HIGH   (1)


/**
 *  @brief UART interrupt type definition
 */
#define UART_INT_TX_END         (0) /*!< UART tx transfer end interrupt */
#define UART_INT_RX_END         (1) /*!< UART rx transfer end interrupt */
#define UART_INT_TX_FIFO_REQ    (2) /*!< UART tx fifo interrupt when tx fifo count reaches auto clear */
#define UART_INT_RX_FIFO_REQ    (3) /*!< UART rx fifo interrupt when rx fifo count reaches auto clear */
#define UART_INT_RTO            (4) /*!< UART rx time-out interrupt */
#define UART_INT_PCE            (5) /*!< UART rx parity check error interrupt */
#define UART_INT_TX_FER         (6) /*!< UART tx fifo overflow/underflow error interrupt */
#define UART_INT_RX_FER         (7) /*!< UART rx fifo overflow/underflow error interrupt */
#define UART_INT_LSE            (8) /*!< UART rx lin mode sync field error interrupt */
#define UART_INT_BCR            (9) /*!< UART rx byte count reached interrupt */
#define UART_INT_STARTBIT       (10) /*!< UART rx auto baudrate detection finish interrupt using start bit */
#define UART_INT_0X55           (11) /*!< UART rx auto baudrate detection finish interrupt using codeword 0x55 */
#define UART_INT_ALL            (12) /*!< All the interrupt */


/**
 *  @brief UART overflow or underflow type definition
 */
#define UART_TX_OVERFLOW        (0)
#define UART_TX_UNDERFLOW       (1)
#define UART_RX_OVERFLOW        (2)
#define UART_RX_UNDERFLOW       (3)


/**
 *  @brief UART configuration structure type definition
 */
typedef struct
{
    uint32_t uartClk;                        /*!< Uart module clock */
    uint32_t baudRate;                       /*!< Uart baudrate */
    uint8_t dataBits;             /*!< Uart frame length of data bit */
    uint8_t stopBits;             /*!< Uart frame length of stop bit */
    uint8_t parity;                 /*!< Uart parity check type */
    uint8_t ctsFlowControl;              /*!< Enable or disable tx CTS flow control */
    uint8_t rxDeglitch;                  /*!< Enable or disable rx input de-glitch function */
    uint8_t rtsSoftwareControl;          /*!< Enable or disable rx RTS output SW control mode */
    uint8_t txSoftwareControl;           /*!< Enable or disable tx output SW control mode */
    uint8_t txLinMode;                   /*!< Enable or disable tx LIN mode,LIN header will be sent before sending data */
    uint8_t rxLinMode;                   /*!< Enable or disable rx LIN mode,LIN header will be required and checked before receiving data */
    uint8_t txBreakBitCnt;                   /*!< Uart tx break bit count,additional 8 bit times will be added since LIN break field requires at
                                                 least 13 bit times */
    uint8_t byteBitInverse; /*!< Uart each data byte is send out LSB-first or MSB-first */
} UART_CFG_Type;

/**
 *  @brief UART FIFO configuration structure type definition
 */
typedef struct
{
    uint8_t txFifoDmaThreshold;  /*!< TX FIFO threshold, dma tx request will not be asserted if tx fifo count is less than this value */
    uint8_t rxFifoDmaThreshold;  /*!< RX FIFO threshold, dma rx request will not be asserted if rx fifo count is less than this value */
    uint8_t txFifoDmaEnable; /*!< Enable or disable tx dma req/ack interface */
    uint8_t rxFifoDmaEnable; /*!< Enable or disable rx dma req/ack interface */
} UART_FifoCfg_Type;

/**
 *  @brief UART infrared configuration structure type definition
 */
typedef struct
{
    uint8_t txIrEnable;  /*!< Enable or disable uart tx ir mode */
    uint8_t rxIrEnable;  /*!< Enable or disable uart rx ir mode */
    uint8_t txIrInverse; /*!< Enable or disable inverse signal of uart tx output in ir mode */
    uint8_t rxIrInverse; /*!< Enable or disable inverse signal of uart rx input in ir mode */
    uint16_t txIrPulseStart; /*!< Set start position of uart tx ir pulse */
    uint16_t txIrPulseStop;  /*!< Set stop position of uart tx ir pulse */
    uint16_t rxIrPulseStart; /*!< Set start position of uart rx pulse recovered from ir signal */
} UART_IrCfg_Type;

/*@} end of group UART_Public_Types */

/** @defgroup  UART_Public_Constants
 *  @{
 */

/** @defgroup  UART_ID_TYPE
 *  @{
 */
#define IS_UART_ID_TYPE(type) (((type) == UART0_ID) || \
                               ((type) == UART1_ID) || \
                               ((type) == UART2_ID) || \
                               ((type) == UART_ID_MAX))

/** @defgroup  UART_DIRECTION_TYPE
 *  @{
 */
#define IS_UART_DIRECTION_TYPE(type) (((type) == UART_TX) || \
                                      ((type) == UART_RX) || \
                                      ((type) == UART_TXRX))

/** @defgroup  UART_PARITY_TYPE
 *  @{
 */
#define IS_UART_PARITY_TYPE(type) (((type) == UART_PARITY_NONE) || \
                                   ((type) == UART_PARITY_ODD) ||  \
                                   ((type) == UART_PARITY_EVEN))

/** @defgroup  UART_DATABITS_TYPE
 *  @{
 */
#define IS_UART_DATABITS_TYPE(type) (((type) == UART_DATABITS_5) || \
                                     ((type) == UART_DATABITS_6) || \
                                     ((type) == UART_DATABITS_7) || \
                                     ((type) == UART_DATABITS_8))

/** @defgroup  UART_STOPBITS_TYPE
 *  @{
 */
#define IS_UART_STOPBITS_TYPE(type) (((type) == UART_STOPBITS_1) ||   \
                                     ((type) == UART_STOPBITS_1_5) || \
                                     ((type) == UART_STOPBITS_2))

/** @defgroup  UART_BYTEBITINVERSE_TYPE
 *  @{
 */
#define IS_UART_BYTEBITINVERSE_TYPE(type) (((type) == UART_LSB_FIRST) || \
                                           ((type) == UART_MSB_FIRST))

/** @defgroup  UART_AUTOBAUDDETECTION_TYPE
 *  @{
 */
#define IS_UART_AUTOBAUDDETECTION_TYPE(type) (((type) == UART_AUTOBAUD_0X55) || \
                                              ((type) == UART_AUTOBAUD_STARTBIT))

/** @defgroup  UART_RS485POLARITY_TYPE
 *  @{
 */
#define IS_UART_RS485POLARITY_TYPE(type) (((type) == UART_RS485_DE_ACTIVE_LOW) || \
                                          ((type) == UART_RS485_DE_ACTIVE_HIGH))

/** @defgroup  UART_INT_TYPE
 *  @{
 */
#define IS_UART_INT_TYPE(type) (((type) == UART_INT_TX_END) ||      \
                                ((type) == UART_INT_RX_END) ||      \
                                ((type) == UART_INT_TX_FIFO_REQ) || \
                                ((type) == UART_INT_RX_FIFO_REQ) || \
                                ((type) == UART_INT_RTO) ||         \
                                ((type) == UART_INT_PCE) ||         \
                                ((type) == UART_INT_TX_FER) ||      \
                                ((type) == UART_INT_RX_FER) ||      \
                                ((type) == UART_INT_LSE) ||         \
                                ((type) == UART_INT_BCR) ||         \
                                ((type) == UART_INT_STARTBIT) ||    \
                                ((type) == UART_INT_0X55) ||        \
                                ((type) == UART_INT_ALL))

/** @defgroup  UART_OVERFLOW_TYPE
 *  @{
 */
#define IS_UART_OVERFLOW_TYPE(type) (((type) == UART_TX_OVERFLOW) ||  \
                                     ((type) == UART_TX_UNDERFLOW) || \
                                     ((type) == UART_RX_OVERFLOW) ||  \
                                     ((type) == UART_RX_UNDERFLOW))

/*@} end of group UART_Public_Constants */

/** @defgroup  UART_Public_Macros
 *  @{
 */
#define UART_RX_FIFO_SIZE         128
#define UART_TX_FIFO_SIZE         128
#define UART_DEFAULT_RECV_TIMEOUT 80

/*@} end of group UART_Public_Macros */

/** @defgroup  UART_Public_Functions
 *  @{
 */

/**
 *  @brief UART Functions
 */
#if (defined BOOTROM) || (!defined QC7XX_USE_HAL_DRIVER) || (defined QC7XX_EFLASH_LOADER)
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_IRQHandler(void);
#endif
QC7XX_Err_Type UART_SetBaudRate(uint8_t uartId, uint32_t baudRate);
QC7XX_Err_Type UART_Init(uint8_t uartId, UART_CFG_Type *uartCfg);
QC7XX_Err_Type UART_DeInit(uint8_t uartId);
QC7XX_Err_Type UART_FifoConfig(uint8_t uartId, UART_FifoCfg_Type *fifoCfg);
QC7XX_Err_Type UART_IrConfig(uint8_t uartId, UART_IrCfg_Type *irCfg);
QC7XX_Err_Type UART_Enable(uint8_t uartId, uint8_t direct);
QC7XX_Err_Type UART_Disable(uint8_t uartId, uint8_t direct);
QC7XX_Err_Type UART_SetTxDataLength(uint8_t uartId, uint16_t length);
QC7XX_Err_Type UART_SetRxDataLength(uint8_t uartId, uint16_t length);
QC7XX_Err_Type UART_SetRxTimeoutValue(uint8_t uartId, uint8_t time);
QC7XX_Err_Type UART_SetRxByteCount(uint8_t uartId, uint16_t count);
QC7XX_Err_Type UART_SetDeglitchCount(uint8_t uartId, uint8_t deglitchCnt);
QC7XX_Err_Type UART_ApplyAbrResult(uint8_t uartId, uint8_t autoBaudDet);
QC7XX_Err_Type UART_SetRtsValue(uint8_t uartId);
QC7XX_Err_Type UART_ClrRtsValue(uint8_t uartId);
QC7XX_Err_Type UART_SetTxValue(uint8_t uartId);
QC7XX_Err_Type UART_ClrTxValue(uint8_t uartId);
QC7XX_Err_Type UART_TxFreeRun(uint8_t uartId, QC7XX_Fun_Type txFreeRun);
QC7XX_Err_Type UART_AutoBaudDetection(uint8_t uartId, QC7XX_Fun_Type autoBaud);
QC7XX_Err_Type UART_SetAllowableError0X55(uint8_t uartId, uint8_t allowableError);
QC7XX_Err_Type UART_GetBitWidth0X55(uint8_t uartId, uint16_t *width);
QC7XX_Err_Type UART_SetRS485(uint8_t uartId, QC7XX_Fun_Type enable, uint8_t polarity);
QC7XX_Err_Type UART_TxFifoClear(uint8_t uartId);
QC7XX_Err_Type UART_RxFifoClear(uint8_t uartId);
QC7XX_Err_Type UART_IntMask(uint8_t uartId, uint8_t intType, QC7XX_Mask_Type intMask);
QC7XX_Err_Type UART_IntClear(uint8_t uartId, uint8_t intType);
QC7XX_Err_Type UART_Int_Callback_Install(uint8_t uartId, uint8_t intType, intCallback_Type *cbFun);
QC7XX_Err_Type UART_SendData(uint8_t uartId, uint8_t *data, uint32_t len);
QC7XX_Err_Type UART_SendDataBlock(uint8_t uartId, uint8_t *data, uint32_t len);
uint32_t UART_ReceiveData(uint8_t uartId, uint8_t *data, uint32_t maxLen);
uint16_t UART_GetAutoBaudCount(uint8_t uartId, uint8_t autoBaudDet);
uint16_t UART_GetRxByteCount(uint8_t uartId);
uint8_t UART_GetTxFifoCount(uint8_t uartId);
uint8_t UART_GetRxFifoCount(uint8_t uartId);
QC7XX_Sts_Type UART_GetIntStatus(uint8_t uartId, uint8_t intType);
QC7XX_Sts_Type UART_GetTxBusBusyStatus(uint8_t uartId);
QC7XX_Sts_Type UART_GetRxBusBusyStatus(uint8_t uartId);
QC7XX_Sts_Type UART_GetOverflowStatus(uint8_t uartId, uint8_t overflow);

/*@} end of group UART_Public_Functions */

/*@} end of group UART */

/*@} end of group QCC743_Peripheral_Driver */

#endif /* __QCC743_UART_H__ */
