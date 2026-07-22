/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qcc74x_sec_dbg.h"

/** @addtogroup  QCC74X_Peripheral_Driver
 *  @{
 */

/** @addtogroup  SEC_DBG
 *  @{
 */

/** @defgroup  SEC_DBG_Private_Macros
 *  @{
 */

/*@} end of group SEC_DBG_Private_Macros */

/** @defgroup  SEC_DBG_Private_Types
 *  @{
 */

/*@} end of group SEC_DBG_Private_Types */

/** @defgroup  SEC_DBG_Private_Variables
 *  @{
 */

/*@} end of group SEC_DBG_Private_Variables */

/** @defgroup  SEC_DBG_Global_Variables
 *  @{
 */

/*@} end of group SEC_DBG_Global_Variables */

/** @defgroup  SEC_DBG_Private_Fun_Declaration
 *  @{
 */

/*@} end of group SEC_DBG_Private_Fun_Declaration */

/** @defgroup  SEC_DBG_Public_Functions
 *  @{
 */

/****************************************************************************/ /**
 * @brief  Sec Dbg read chip ID
 *
 * @param  id[8]: chip ID buffer
 *
 * @return None
 *
*******************************************************************************/
void Sec_Dbg_Read_Chip_ID(uint8_t id[8])
{
    uint32_t idLow, idHigh;

    idLow = QC7XX_RD_REG(SEC_DBG_BASE, SEC_DBG_SD_CHIP_ID_LOW);
    QC7XX_WRWD_TO_BYTEP(id, idLow);

    idHigh = QC7XX_RD_REG(SEC_DBG_BASE, SEC_DBG_SD_CHIP_ID_HIGH);
    QC7XX_WRWD_TO_BYTEP((id + 4), idHigh);
}

/***************************************************************************** /
 * @brief  get Sec Dbg state
 *
 * @param  None
 *
 * @return Sec Dbg state
 *
*******************************************************************************/
uint32_t Sec_Dbg_Read_Dbg_State(void)
{
    uint32_t ret;
    uint32_t mode = 0;
    uint32_t ena = 0;

    mode = QC7XX_GET_REG_BITS_VAL(QC7XX_RD_REG(SEC_DBG_BASE, SEC_DBG_SD_STATUS), SEC_DBG_SD_DBG_MODE);
    ena = QC7XX_GET_REG_BITS_VAL(QC7XX_RD_REG(SEC_DBG_BASE, SEC_DBG_SD_STATUS), SEC_DBG_SD_DBG_ENA);

    if(0x0 == mode){
        ret = SEC_DBG_STATE_OPEN_MODE;
    }else if((0x1 == mode)||(0x2 == mode)||(0x3 == mode)){
        if(0xF == ena){
            ret = SEC_DBG_STATE_OPEN_MODE;
        }else{
            ret = SEC_DBG_STATE_PASSWORD_MODE;
        }
    }else{
        ret = SEC_DBG_STATE_CLOSE_MODE;
    }

    return ret;
}

/****************************************************************************/ /**
 * @brief  Sec Dbg read debug mode
 *
 * @param  None
 *
 * @return debug mode status
 *
*******************************************************************************/
uint32_t Sec_Dbg_Read_Dbg_Mode(void)
{
    return QC7XX_GET_REG_BITS_VAL(QC7XX_RD_REG(SEC_DBG_BASE, SEC_DBG_SD_STATUS), SEC_DBG_SD_DBG_MODE);
}

/****************************************************************************/ /**
 * @brief  Sec Dbg read debug enable status
 *
 * @param  None
 *
 * @return enable status
 *
*******************************************************************************/
uint32_t Sec_Dbg_Read_Dbg_Enable(void)
{
    return QC7XX_GET_REG_BITS_VAL(QC7XX_RD_REG(SEC_DBG_BASE, SEC_DBG_SD_STATUS), SEC_DBG_SD_DBG_ENA);
}

/***************************************************************************** /
 * @brief  Sec Dbg set PWD
 *
 * @param  pwd: PWD buffer
 *
 * @return None
 *
*******************************************************************************/
void Sec_Dbg_Set_Dbg_Pwd(const uint32_t pwd[4])
{
    QC7XX_WR_REG(SEC_DBG_BASE, SEC_DBG_SD_DBG_PWD_LOW, pwd[0]);
    QC7XX_WR_REG(SEC_DBG_BASE, SEC_DBG_SD_DBG_PWD_HIGH, pwd[1]);
    QC7XX_WR_REG(SEC_DBG_BASE, SEC_DBG_SD_DBG_PWD2_LOW, pwd[2]);
    QC7XX_WR_REG(SEC_DBG_BASE, SEC_DBG_SD_DBG_PWD2_HIGH, pwd[3]);
}

/****************************************************************************/ /**
 * @brief  Sec Dbg Set debug trigger status
 *
 * @param  None
 *
 * @return None
 *
*******************************************************************************/
void Sec_Dbg_Set_Dbg_Trigger(void)
{
    uint32_t tmpVal;
    tmpVal = QC7XX_RD_REG(SEC_DBG_BASE, SEC_DBG_SD_STATUS);
    tmpVal = QC7XX_SET_REG_BIT(tmpVal,SEC_DBG_SD_DBG_PWD_TRIG);
    QC7XX_WR_REG(SEC_DBG_BASE, SEC_DBG_SD_STATUS, tmpVal);
}

/****************************************************************************/ /**
 * @brief  Sec Dbg wait ready
 *
 * @param  None
 *
 * @return busy status
 *
*******************************************************************************/
uint32_t Sec_Dbg_Wait_Ready(void)
{
    uint32_t cnt = 1024;
    uint32_t busy;

    do{
        busy = QC7XX_GET_REG_BITS_VAL(QC7XX_RD_REG(SEC_DBG_BASE, SEC_DBG_SD_STATUS), SEC_DBG_SD_DBG_PWD_BUSY);
    }while((busy) && (cnt--));

    return busy;
}

/*@} end of group SEC_DBG_Public_Functions */

/*@} end of group SEC_DBG */

/*@} end of group QCC74X_Peripheral_Driver */
