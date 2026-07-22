/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCC74X_AON_H__
#define __QCC74X_AON_H__

#include "aon_reg.h"
#include "glb_reg.h"
#include "hbn_reg.h"
#include "pds_reg.h"
#include "qcc74x_ef_cfg.h"
#include "qcc74x_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup  QCC74X_Peripheral_Driver
 *  @{
 */

/** @addtogroup  AON
 *  @{
 */

/** @defgroup  AON_Public_Types
 *  @{
 */

/*@} end of group AON_Public_Types */

/** @defgroup  AON_Public_Constants
 *  @{
 */

/*@} end of group AON_Public_Constants */

/** @defgroup  AON_Public_Macros
 *  @{
 */

/*@} end of group AON_Public_Macros */

/** @defgroup  AON_Public_Functions
 *  @{
 */
/*----------*/
QC7XX_Err_Type AON_Power_On_MBG(void);
QC7XX_Err_Type AON_Power_Off_MBG(void);
/*----------*/
QC7XX_Err_Type AON_Power_On_XTAL(void);
QC7XX_Err_Type AON_Set_Xtal_CapCode(uint8_t capIn, uint8_t capOut);
uint8_t AON_Get_Xtal_CapCode(void);
QC7XX_Err_Type AON_Power_Off_XTAL(void);
/*----------*/
QC7XX_Err_Type AON_Power_On_BG(void);
QC7XX_Err_Type AON_Power_Off_BG(void);
/*----------*/
QC7XX_Err_Type AON_Power_On_LDO15_RF(void);
QC7XX_Err_Type AON_Power_Off_LDO15_RF(void);
QC7XX_Err_Type AON_Output_Float_LDO15_RF(void);
QC7XX_Err_Type AON_Output_Pulldown_LDO15_RF(void);
/*----------*/
QC7XX_Err_Type AON_Power_On_SFReg(void);
QC7XX_Err_Type AON_Power_Off_SFReg(void);
/*----------*/
QC7XX_Err_Type AON_Power_On_MicBias(void);
QC7XX_Err_Type AON_Power_Off_MicBias(void);
/*----------*/
QC7XX_Err_Type AON_LowPower_Enter_PDS0(void);
QC7XX_Err_Type AON_LowPower_Exit_PDS0(void);
/*----------*/
QC7XX_Err_Type AON_Trim_DcdcDis(void);
QC7XX_Err_Type AON_Trim_DcdcVoutSel(void);
QC7XX_Err_Type AON_Trim_DcdcVoutTrim(void);
QC7XX_Err_Type AON_Trim_Ldo11socVoutTrim(void);
QC7XX_Err_Type AON_Trim_Usb20RcalCode(void);
/*----------*/
QC7XX_Err_Type AON_Output_Pulldown_DCDC18(void);
QC7XX_Err_Type AON_Output_Float_DCDC18(void);

/*@} end of group AON_Public_Functions */

/*@} end of group AON */

/*@} end of group QCC74X_Peripheral_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __QCC74X_AON_H__ */
