/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qcc74x_clock.h"
#include "qcc74x_glb.h"
#include "qcc74x_hbn.h"
#include "qcc74x_pds.h"
#include "qcc74x_glb_gpio.h"

/** @addtogroup  QCC74X_Peripheral_Driver
 *  @{
 */

/** @addtogroup  GLB_GPIO
 *  @{
 */

/** @defgroup  GLB_GPIO_Private_Macros
 *  @{
 */
#define GLB_GPIO_INT0_NUM           (GLB_GPIO_PIN_MAX)
#define GLB_GPIO_INT0_CLEAR_TIMEOUT (32)

/*@} end of group GLB_GPIO_Private_Macros */
#define GLB_GPIO_TIMEOUT_COUNT (160 * 1000)
/** @defgroup  GLB_GPIO_Private_Types
 *  @{
 */

/*@} end of group GLB_GPIO_Private_Types */

/** @defgroup  GLB_GPIO_Private_Variables
 *  @{
 */

#ifndef QC7XX_USE_HAL_DRIVER
static intCallback_Type *ATTR_TCM_CONST_SECTION glbGpioInt0CbfArra[GLB_GPIO_INT0_NUM] = { NULL };

static intCallback_Type *ATTR_TCM_CONST_SECTION glbGpioFifoCbfArra[GLB_GPIO_FIFO_INT_ALL] = { NULL };
#endif

/*@} end of group GLB_GPIO_Private_Variables */

/** @defgroup  GLB_GPIO_Global_Variables
 *  @{
 */

/*@} end of group GLB_GPIO_Global_Variables */

/** @defgroup  GLB_GPIO_Private_Fun_Declaration
 *  @{
 */

/*@} end of group GLB_GPIO_Private_Fun_Declaration */

/** @defgroup  GLB_GPIO_Private_Functions
 *  @{
 */

/*@} end of group GLB_GPIO_Private_Functions */

/** @defgroup  GLB_GPIO_Public_Functions
 *  @{
 */

/****************************************************************************/ /**
 * @brief  GLB GPIO CHECK PAD Whether Lead Out no not
 *
 * @param  None
 *
 * @return RESET or SET
 *
*******************************************************************************/
QC7XX_Sts_Type ATTR_TCM_SECTION GLB_GPIO_Pad_LeadOut_Sts(uint8_t gpioPin)
{
    uint8_t package_type;

    package_type = GLB_Get_Package_Type();

    if ( GLB_PACKAGE_TYPE_QFN56 == package_type ){
        return SET;
    }

    /* */
    if ((( gpioPin >= GLB_GPIO_PIN_0) && (gpioPin < GLB_GPIO_PIN_4))
    || (( gpioPin > GLB_GPIO_PIN_9) && (gpioPin < GLB_GPIO_PIN_18))
    || (( gpioPin > GLB_GPIO_PIN_19) && (gpioPin < GLB_GPIO_PIN_23))
    || (( gpioPin > GLB_GPIO_PIN_26) && (gpioPin < GLB_GPIO_PIN_31))) {
        return SET;
    }

    return RESET;
}

/****************************************************************************/ /**
 * @brief  GPIO initialization
 *
 * @param  cfg: GPIO configuration
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type ATTR_TCM_SECTION GLB_GPIO_Init(GLB_GPIO_Cfg_Type *cfg)
{
    uint8_t gpioPin = cfg->gpioPin;
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    if (RESET == GLB_GPIO_Pad_LeadOut_Sts(gpioPin)) {
        return ERROR;
    }

    /* drive strength(drive) = 0  <=>  8.0mA  @ 3.3V */
    /* drive strength(drive) = 1  <=>  9.6mA  @ 3.3V */
    /* drive strength(drive) = 2  <=>  11.2mA @ 3.3V */
    /* drive strength(drive) = 3  <=>  12.8mA @ 3.3V */

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);

    /* Disable output anyway*/
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_OE);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    /* input/output, pull up/down, drive, smt, function */
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);

    if (cfg->gpioMode != GPIO_MODE_ANALOG) {
        /* not analog mode */

        if (cfg->gpioMode == GPIO_MODE_OUTPUT) {
            tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_IE);
            tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_REG_GPIO_0_OE);
        } else {
            tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_REG_GPIO_0_IE);
            tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_OE);
        }

        if (cfg->pullType == GPIO_PULL_UP) {
            tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_REG_GPIO_0_PU);
            tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_PD);
        } else if (cfg->pullType == GPIO_PULL_DOWN) {
            tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_PU);
            tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_REG_GPIO_0_PD);
        } else {
            tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_PU);
            tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_PD);
        }
    } else {
        /* analog mode */

        /* clear ie && oe */
        tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_IE);
        tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_OE);

        /* clear pu && pd */
        tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_PU);
        tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_PD);
    }

    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_DRV, cfg->drive);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_SMT, cfg->smtCtrl);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_FUNC_SEL, cfg->gpioFun);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_MODE, cfg->outputMode);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  init GPIO function in pin list
 *
 * @param  gpioFun: GPIO pin function, this parameter can be one of the following values:
 *           @arg GPIO_FUN_SDH
 *           @arg GPIO_FUN_SPI
 *           @arg GPIO_FUN_FLASH
 *           @arg GPIO_FUN_I2S
 *           @arg GPIO_FUN_PDM
 *           @arg GPIO_FUN_I2C0
 *           @arg GPIO_FUN_I2C1
 *           @arg GPIO_FUN_UART
 *           @arg GPIO_FUN_ETHER_MAC
 *           @arg GPIO_FUN_CAM
 *           @arg GPIO_FUN_ANALOG
 *           @arg GPIO_FUN_GPIO
 *           @arg GPIO_FUN_SDIO
 *           @arg GPIO_FUN_PWM0
 *           @arg GPIO_FUN_MD_JTAG
 *           @arg GPIO_FUN_MD_UART
 *           @arg GPIO_FUN_MD_PWM
 *           @arg GPIO_FUN_MD_SPI
 *           @arg GPIO_FUN_MD_I2S
 *           @arg GPIO_FUN_DBI_B
 *           @arg GPIO_FUN_DBI_C
 *           @arg GPIO_FUN_DISP_QSPI
 *           @arg GPIO_FUN_AUPWM
 *           @arg GPIO_FUN_JTAG
 *           @arg GPIO_FUN_CLOCK_OUT
 * @param  pinList: GPIO pin list, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 * @param  cnt: GPIO pin count
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_Func_Init(uint8_t gpioFun, uint8_t *pinList, uint8_t cnt)
{
    GLB_GPIO_Cfg_Type gpioCfg = {
        .gpioPin = GLB_GPIO_PIN_0,
        .gpioFun = (uint8_t)gpioFun,
        .gpioMode = GPIO_MODE_AF,
        .pullType = GPIO_PULL_UP,
        .drive = 1,
        .smtCtrl = 1
    };

    if (gpioFun == GPIO_FUN_ANALOG) {
        gpioCfg.gpioMode = GPIO_MODE_ANALOG;
    }

    for (uint8_t i = 0; i < cnt; i++) {
        gpioCfg.gpioPin = pinList[i];
        GLB_GPIO_Init(&gpioCfg);
    }

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  GPIO set input function enable
 *
 * @param  gpioPin: GPIO pin, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type ATTR_TCM_SECTION GLB_GPIO_Input_Enable(uint8_t gpioPin)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);

    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_REG_GPIO_0_IE);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  Embedded flash set input function enable
 *
 * @param  swapIo2Cs: Select embedded flash swap cs with io2
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type ATTR_TCM_SECTION GLB_Embedded_Flash_Pad_Enable(uint8_t swapIo2Cs)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;
    uint8_t gpioPin[4];
    uint8_t i;

    if (swapIo2Cs) {
        gpioPin[0] = 37;
        gpioPin[1] = 38;
        gpioPin[2] = 36;
        gpioPin[3] = 40;
    } else {
        gpioPin[0] = 37;
        gpioPin[1] = 38;
        gpioPin[2] = 39;
        gpioPin[3] = 40;
    }
    for (i = 0; i < 4; i++) {
        gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin[i] << 2);
        tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
        tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_REG_GPIO_0_IE);
        QC7XX_WR_WORD(gpioCfgAddress, tmpVal);
    }

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  GPIO set input function disable
 *
 * @param  gpioPin: GPIO pin, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type ATTR_TCM_SECTION GLB_GPIO_Input_Disable(uint8_t gpioPin)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);

    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_IE);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  GPIO set output function enable
 *
 * @param  gpioPin: GPIO pin, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type ATTR_TCM_SECTION GLB_GPIO_Output_Enable(uint8_t gpioPin)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    if (RESET == GLB_GPIO_Pad_LeadOut_Sts(gpioPin)) {
        return ERROR;
    }

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);

    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_REG_GPIO_0_OE);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  GPIO set output function disable
 *
 * @param  gpioPin: GPIO pin, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type ATTR_TCM_SECTION GLB_GPIO_Output_Disable(uint8_t gpioPin)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);

    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_OE);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  GPIO set High-Z
 *
 * @param  gpioPin: GPIO pin, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type ATTR_TCM_SECTION GLB_GPIO_Set_HZ(uint8_t gpioPin)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    if (RESET == GLB_GPIO_Pad_LeadOut_Sts(gpioPin)) {
        return ERROR;
    }

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);

    /* Disable output anyway*/
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_OE);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    /* ie=0, oe=0, drive=0, smt=0, pu=1 (pull up), pd=0, func=swgpio */
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_IE);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_OE);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_DRV, 0);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_SMT, 0);
    tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_REG_GPIO_0_PU); /* pull up */
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_PD);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_FUNC_SEL, 0xB);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  Get GPIO function
 *
 * @param  gpioPin: GPIO type, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return GPIO function
 *
*******************************************************************************/
uint8_t ATTR_TCM_SECTION GLB_GPIO_Get_Fun(uint8_t gpioPin)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);

    return QC7XX_GET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_FUNC_SEL);
}

/****************************************************************************/ /**
 * @brief  Read GPIO
 *
 * @param  gpioPin: GPIO type, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return GPIO value
 *
*******************************************************************************/
uint32_t GLB_GPIO_Read(uint8_t gpioPin)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);

    return QC7XX_GET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_I) ? SET : RESET;
}

/****************************************************************************/ /**
 * @brief  Write GPIO
 *
 * @param  gpioPin: GPIO type, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 * @param  val: GPIO value
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_Write(uint8_t gpioPin, uint32_t val)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    if (RESET == GLB_GPIO_Pad_LeadOut_Sts(gpioPin)) {
        return ERROR;
    }

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);

    if (val) {
        tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_REG_GPIO_0_O);
    } else {
        tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_O);
    }

    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  turn GPIO output high
 *
 * @param  gpioPin: GPIO type, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_Set(uint8_t gpioPin)
{
    if (RESET == GLB_GPIO_Pad_LeadOut_Sts(gpioPin)) {
        return ERROR;
    }

    if (gpioPin < GLB_GPIO_PIN_32) {
        QC7XX_WR_WORD(GLB_BASE + GLB_GPIO_CFG138_OFFSET, 1 << gpioPin);
    } else {
        QC7XX_WR_WORD(GLB_BASE + GLB_GPIO_CFG139_OFFSET, 1 << (gpioPin - GLB_GPIO_PIN_32));
    }

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  turn GPIO output low
 *
 * @param  gpioPin: GPIO type, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_Clr(uint8_t gpioPin)
{
    if (gpioPin < GLB_GPIO_PIN_32) {
        QC7XX_WR_WORD(GLB_BASE + GLB_GPIO_CFG140_OFFSET, 1 << gpioPin);
    } else {
        QC7XX_WR_WORD(GLB_BASE + GLB_GPIO_CFG141_OFFSET, 1 << (gpioPin - GLB_GPIO_PIN_32));
    }

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  GPIO interrupt initialization
 *
 * @param  intCfg: GPIO interrupt configuration
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_Int_Init(GLB_GPIO_INT_Cfg_Type *intCfg)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;
    uint32_t gpioPin = intCfg->gpioPin;

    CHECK_PARAM(IS_GLB_GPIO_INT_TRIG_TYPE(intCfg->trig));
    CHECK_PARAM(IS_QC7XX_MASK_TYPE(intCfg->intMask));

    if (RESET == GLB_GPIO_Pad_LeadOut_Sts(gpioPin)) {
        return ERROR;
    }

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_INT_MODE_SET, intCfg->trig);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_INT_MASK, intCfg->intMask);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  Set GLB GPIO interrupt mask
 *
 * @param  gpioPin: GPIO type, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 * @param  intMask: GPIO interrupt MASK or UNMASK
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_IntMask(uint8_t gpioPin, QC7XX_Mask_Type intMask)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_REG_GPIO_0_INT_MASK, intMask);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  Get GLB GPIO interrupt status
 *
 * @param  gpioPin: GPIO type, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return SET or RESET
 *
*******************************************************************************/
QC7XX_Sts_Type GLB_Get_GPIO_IntStatus(uint8_t gpioPin)
{
    uint32_t gpioCfgAddress;

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);

    return QC7XX_GET_REG_BITS_VAL(QC7XX_RD_WORD(gpioCfgAddress), GLB_GPIO_0_INT_STAT) ? SET : RESET;
}

/****************************************************************************/ /**
 * @brief  Clear GLB GPIO interrupt status
 *
 * @param  gpioPin: GPIO type, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_Clr_GPIO_IntStatus(uint8_t gpioPin)
{
    uint32_t gpioCfgAddress;
    uint32_t tmpVal;

    gpioCfgAddress = GLB_BASE + GLB_GPIO_CFG0_OFFSET + (gpioPin << 2);

    /* clr=1 */
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_REG_GPIO_0_INT_CLR);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    /* clr=0 */
    tmpVal = QC7XX_RD_WORD(gpioCfgAddress);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_REG_GPIO_0_INT_CLR);
    QC7XX_WR_WORD(gpioCfgAddress, tmpVal);

    return SUCCESS;
}

#ifndef QC7XX_USE_HAL_DRIVER
void GPIO_FIFO_IRQHandler(void)
{
    uint32_t tmpVal;

    tmpVal = QC7XX_RD_REG(GLB_BASE, GLB_GPIO_CFG143);

    if (QC7XX_IS_REG_BIT_SET(tmpVal, GLB_R_GPIO_TX_END_INT) && !QC7XX_IS_REG_BIT_SET(tmpVal, GLB_CR_GPIO_TX_END_MASK)) {
        if (glbGpioFifoCbfArra[GLB_GPIO_FIFO_INT_END] != NULL) {
            glbGpioFifoCbfArra[GLB_GPIO_FIFO_INT_END]();
        }
    }

    if (QC7XX_IS_REG_BIT_SET(tmpVal, GLB_R_GPIO_TX_FER_INT) && !QC7XX_IS_REG_BIT_SET(tmpVal, GLB_CR_GPIO_TX_FER_MASK)) {
        if (glbGpioFifoCbfArra[GLB_GPIO_FIFO_INT_FER] != NULL) {
            glbGpioFifoCbfArra[GLB_GPIO_FIFO_INT_FER]();
        }
    }

    if (QC7XX_IS_REG_BIT_SET(tmpVal, GLB_R_GPIO_TX_FIFO_INT) && !QC7XX_IS_REG_BIT_SET(tmpVal, GLB_CR_GPIO_TX_FIFO_MASK)) {
        if (glbGpioFifoCbfArra[GLB_GPIO_FIFO_INT_FIFO] != NULL) {
            glbGpioFifoCbfArra[GLB_GPIO_FIFO_INT_FIFO]();
        }
    }
}
#endif

/****************************************************************************/ /**
 * @brief  GPIO INT0 IRQHandler install
 *
 * @param  None
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_INT0_IRQHandler_Install(void)
{
#ifndef QC7XX_USE_HAL_DRIVER
    Interrupt_Handler_Register(GPIO_INT0_IRQn, GPIO_INT0_IRQHandler);
#endif

    return SUCCESS;
}

QC7XX_Err_Type GLB_GPIO_FIFO_IRQHandler_Install(void)
{
#ifndef QC7XX_USE_HAL_DRIVER
    Interrupt_Handler_Register(GPIO_DMA_IRQn, GPIO_FIFO_IRQHandler);
#endif

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  GPIO interrupt IRQ handler callback install
 *
 * @param  gpioPin: GPIO pin type, this parameter can be GLB_GPIO_PIN_xx where xx is 0~34
 * @param  cbFun: callback function
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
#ifndef QC7XX_USE_HAL_DRIVER
QC7XX_Err_Type GLB_GPIO_INT0_Callback_Install(uint8_t gpioPin, intCallback_Type *cbFun)
{
    if (gpioPin < GLB_GPIO_PIN_MAX) {
        glbGpioInt0CbfArra[gpioPin] = cbFun;
    }

    return SUCCESS;
}
#endif
#ifndef QC7XX_USE_HAL_DRIVER
QC7XX_Err_Type GLB_GPIO_Fifo_Callback_Install(uint8_t intType, intCallback_Type *cbFun)
{
    /* Check the parameters */
    CHECK_PARAM(IS_GLB_GPIO_FIFO_INT_TYPE(intType));

    glbGpioFifoCbfArra[intType] = cbFun;

    return SUCCESS;
}
#endif

/****************************************************************************/ /**
 * @brief  GPIO interrupt IRQ handler
 *
 * @param  None
 *
 * @return None
 *
*******************************************************************************/
#ifndef QC7XX_USE_HAL_DRIVER
void GPIO_INT0_IRQHandler(void)
{
    uint8_t gpioPin;
    uint32_t timeOut = 0;

    for (gpioPin = GLB_GPIO_PIN_0; gpioPin < GLB_GPIO_PIN_MAX; gpioPin++) {
        if (SET == GLB_Get_GPIO_IntStatus(gpioPin)) {
            GLB_Clr_GPIO_IntStatus(gpioPin);

            /* timeout check */
            timeOut = GLB_GPIO_INT0_CLEAR_TIMEOUT;

            do {
                timeOut--;
            } while ((SET == GLB_Get_GPIO_IntStatus(gpioPin)) && timeOut);

            if (!timeOut) {
                //MSG("WARNING: Clear GPIO interrupt status fail.\r\n");
            }

            if (glbGpioInt0CbfArra[gpioPin] != NULL) {
                /* Call the callback function */
                glbGpioInt0CbfArra[gpioPin]();
            }
        }
    }
}
#endif

/****************************************************************************/ /**
 * @brief  GPIO fifo function initialization
 *
 * @param  cfg: GPIO fifo configuration
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_Fifo_Init(GLB_GPIO_FIFO_CFG_Type *cfg)
{
    uint32_t tmpVal;

    /* Check the parameters */
    CHECK_PARAM(IS_GLB_GPIO_FIFO_PHASE_Type(cfg->code0Phase));
    CHECK_PARAM(IS_GLB_GPIO_FIFO_PHASE_Type(cfg->code1Phase));
    CHECK_PARAM(IS_GLB_GPIO_FIFO_PHASE_Type(cfg->latch));

    tmpVal = QC7XX_RD_REG(GLB_BASE, GLB_GPIO_CFG143);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_CR_GPIO_DMA_TX_EN, cfg->fifoDmaEnable);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_CR_GPIO_TX_FIFO_TH, cfg->fifoDmaThreshold);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_CR_GPIO_DMA_OUT_SEL_LATCH, cfg->latch);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_CR_GPIO_DMA_PARK_VALUE, cfg->idle);
    tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_GPIO_TX_FIFO_CLR);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_GPIO_TX_END_CLR);
    QC7XX_WR_REG(GLB_BASE, GLB_GPIO_CFG143, tmpVal);

    tmpVal = QC7XX_RD_REG(GLB_BASE, GLB_GPIO_CFG142);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_CR_CODE0_HIGH_TIME, cfg->code0FirstTime);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_CR_CODE1_HIGH_TIME, cfg->code1FirstTime);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_CR_CODE_TOTAL_TIME, cfg->codeTotalTime);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_CR_INVERT_CODE0_HIGH, cfg->code0Phase);
    tmpVal = QC7XX_SET_REG_BITS_VAL(tmpVal, GLB_CR_INVERT_CODE1_HIGH, cfg->code1Phase);
    tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_CR_GPIO_TX_EN);
    QC7XX_WR_REG(GLB_BASE, GLB_GPIO_CFG142, tmpVal);

    GLB_GPIO_FIFO_IRQHandler_Install();

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  Push data to GPIO fifo
 *
 * @param  data: the pointer of data buffer
 * @param  len: the len of data buffer
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_Fifo_Push(uint16_t *data, uint16_t len)
{
    uint32_t txLen = 0;
    uint32_t timeoutCnt = GLB_GPIO_TIMEOUT_COUNT;

    while (txLen < len) {
        if (GLB_GPIO_Fifo_GetCount() > 0) {
            QC7XX_WR_REG(GLB_BASE, GLB_GPIO_CFG144, data[txLen++]);
            timeoutCnt = GLB_GPIO_TIMEOUT_COUNT;
        } else {
            timeoutCnt--;

            if (timeoutCnt == 0) {
                return TIMEOUT;
            }
        }
    }

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  Get GPIO fifo available count
 *
 * @param  None
 *
 * @return The count of available count
 *
*******************************************************************************/
uint32_t GLB_GPIO_Fifo_GetCount(void)
{
    return QC7XX_GET_REG_BITS_VAL(QC7XX_RD_REG(GLB_BASE, GLB_GPIO_CFG143), GLB_GPIO_TX_FIFO_CNT);
}

/****************************************************************************/ /**
 * @brief  Clear GPIO fifo
 *
 * @param  None
 *
 * @return SUCCESS
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_Fifo_Clear(void)
{
    QC7XX_WR_REG(GLB_BASE, GLB_GPIO_CFG143, QC7XX_SET_REG_BIT(QC7XX_RD_REG(GLB_BASE, GLB_GPIO_CFG143), GLB_GPIO_TX_FIFO_CLR));

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  Mask or Unmask GPIO FIFO Interrupt
 *
 * @param  intType: interrupt type, this parameter can be one of the following values:
 *           @arg GLB_GPIO_FIFO_INT_FER
 *           @arg GLB_GPIO_FIFO_INT_FIFO
 *           @arg GLB_GPIO_FIFO_INT_END
 *           @arg GLB_GPIO_FIFO_INT_ALL
 * @param  intMask: MASK or UNMASK
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_Fifo_IntMask(uint8_t intType, QC7XX_Mask_Type intMask)
{
    uint32_t tmpVal;

    /* Check the parameters */
    CHECK_PARAM(IS_GLB_GPIO_FIFO_INT_TYPE(intType));
    CHECK_PARAM(IS_QC7XX_MASK_TYPE(intMask));

    tmpVal = QC7XX_RD_REG(GLB_BASE, GLB_GPIO_CFG143);

    /* Mask or unmask certain or all interrupt */
    if (MASK == intMask) {
        switch (intType) {
            case GLB_GPIO_FIFO_INT_FER:
                tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_CR_GPIO_TX_FER_MASK);
                break;

            case GLB_GPIO_FIFO_INT_FIFO:
                tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_CR_GPIO_TX_FIFO_MASK);
                break;

            case GLB_GPIO_FIFO_INT_END:
                tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_CR_GPIO_TX_END_MASK);
                break;

            case GLB_GPIO_FIFO_INT_ALL:
                tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_CR_GPIO_TX_END_MASK);
                tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_CR_GPIO_TX_FIFO_MASK);
                tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_CR_GPIO_TX_FER_MASK);
                break;

            default:
                break;
        }
    } else {
        switch (intType) {
            case GLB_GPIO_FIFO_INT_FER:
                tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_CR_GPIO_TX_FER_MASK);
                break;

            case GLB_GPIO_FIFO_INT_FIFO:
                tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_CR_GPIO_TX_FIFO_MASK);
                break;

            case GLB_GPIO_FIFO_INT_END:
                tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_CR_GPIO_TX_END_MASK);
                break;

            case GLB_GPIO_FIFO_INT_ALL:
                tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_CR_GPIO_TX_END_MASK);
                tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_CR_GPIO_TX_FIFO_MASK);
                tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_CR_GPIO_TX_FER_MASK);
                break;

            default:
                break;
        }
    }

    /* Write back */
    QC7XX_WR_REG(GLB_BASE, GLB_GPIO_CFG143, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  Clear GPIO fifo interrupt
 *
 * @param  intType: interrupt type, this parameter can be one of the following values:
 *           @arg GLB_GPIO_FIFO_INT_FER
 *           @arg GLB_GPIO_FIFO_INT_FIFO
 *           @arg GLB_GPIO_FIFO_INT_END
 *           @arg GLB_GPIO_FIFO_INT_ALL
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Err_Type GLB_GPIO_Fifo_IntClear(uint8_t intType)
{
    uint32_t tmpVal;

    /* Check the parameters */
    CHECK_PARAM(IS_GLB_GPIO_FIFO_INT_TYPE(intType));

    tmpVal = QC7XX_RD_REG(GLB_BASE, GLB_GPIO_CFG143);

    /* Clear certain or all interrupt */
    switch (intType) {
        case GLB_GPIO_FIFO_INT_FER:
            tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_GPIO_TX_FIFO_CLR);
            break;

        case GLB_GPIO_FIFO_INT_FIFO:
            //tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_GPIO_TX_FIFO_CLR);
            break;

        case GLB_GPIO_FIFO_INT_END:
            tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_GPIO_TX_END_CLR);
            break;

        case GLB_GPIO_FIFO_INT_ALL:
            tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_GPIO_TX_FIFO_CLR);
            tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_GPIO_TX_END_CLR);
            break;

        default:
            break;
    }

    /* Write back */
    QC7XX_WR_REG(GLB_BASE, GLB_GPIO_CFG143, tmpVal);

    return SUCCESS;
}

/****************************************************************************/ /**
 * @brief  Get GPIO fifo interrupt status
 *
 * @param  intType: interrupt type, this parameter can be one of the following values:
 *           @arg GLB_GPIO_FIFO_INT_FER
 *           @arg GLB_GPIO_FIFO_INT_FIFO
 *           @arg GLB_GPIO_FIFO_INT_END
 *           @arg GLB_GPIO_FIFO_INT_ALL
 *
 * @return SUCCESS or ERROR
 *
*******************************************************************************/
QC7XX_Sts_Type GLB_GPIO_Fifo_GetIntStatus(uint8_t intType)
{
    uint32_t tmpVal;

    /* Check the parameters */
    CHECK_PARAM(IS_GLB_GPIO_FIFO_INT_TYPE(intType));

    /* Get certain or all interrupt status */
    tmpVal = QC7XX_RD_REG(GLB_BASE, GLB_GPIO_CFG143);

    if (GLB_GPIO_FIFO_INT_ALL == intType) {
        if (QC7XX_IS_REG_BIT_SET(tmpVal, GLB_R_GPIO_TX_END_INT) ||
            QC7XX_IS_REG_BIT_SET(tmpVal, GLB_R_GPIO_TX_FIFO_INT) ||
            QC7XX_IS_REG_BIT_SET(tmpVal, GLB_R_GPIO_TX_FER_INT)) {
            return SET;
        } else {
            return RESET;
        }
    } else {
        switch (intType) {
            case GLB_GPIO_FIFO_INT_FER:
                return QC7XX_IS_REG_BIT_SET(tmpVal, GLB_R_GPIO_TX_FER_INT);

            case GLB_GPIO_FIFO_INT_FIFO:
                return QC7XX_IS_REG_BIT_SET(tmpVal, GLB_R_GPIO_TX_FIFO_INT);

            case GLB_GPIO_FIFO_INT_END:
                return QC7XX_IS_REG_BIT_SET(tmpVal, GLB_R_GPIO_TX_END_INT);

            default:
                return RESET;
        }
    }
}
QC7XX_Err_Type GLB_GPIO_Fifo_Enable(void)
{
    uint32_t tmpVal;

    tmpVal = QC7XX_RD_REG(GLB_BASE, GLB_GPIO_CFG142);
    tmpVal = QC7XX_SET_REG_BIT(tmpVal, GLB_CR_GPIO_TX_EN);
    QC7XX_WR_REG(GLB_BASE, GLB_GPIO_CFG142, tmpVal);

    return SUCCESS;
}
QC7XX_Err_Type GLB_GPIO_Fifo_Disable(void)
{
    uint32_t tmpVal;

    tmpVal = QC7XX_RD_REG(GLB_BASE, GLB_GPIO_CFG142);
    tmpVal = QC7XX_CLR_REG_BIT(tmpVal, GLB_CR_GPIO_TX_EN);
    QC7XX_WR_REG(GLB_BASE, GLB_GPIO_CFG142, tmpVal);

    return SUCCESS;
}

/*@} end of group GLB_GPIO_Public_Functions */

/*@} end of group GLB_GPIO */

/*@} end of group QCC74X_Peripheral_Driver */
