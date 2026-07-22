/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCC74X_SEC_DBG_H__
#define __QCC74X_SEC_DBG_H__

#include "sec_dbg_reg.h"
#include "qcc74x_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup  QCC74X_Peripheral_Driver
 *  @{
 */

/** @addtogroup  SEC_DBG
 *  @{
 */

/** @defgroup  SEC_DBG_Public_Types
 *  @{
 */

/**
 *  @brief SEC DBG debug mode type definition
 */
#define SEC_DBG_STATE_OPEN_MODE     (0) /*!< debug open mode */
#define SEC_DBG_STATE_PASSWORD_MODE (1) /*!< debug password mode */
#define SEC_DBG_STATE_CLOSE_MODE    (2) /*!< debug close mode */

/*@} end of group SEC_DBG_Public_Types */

/** @defgroup  SEC_DBG_Public_Constants
 *  @{
 */

#define IS_SEC_DBG_STATE_TYPE(type) (((type) == SEC_DBG_STATE_OPEN_MODE) ||     \
                                     ((type) == SEC_DBG_STATE_PASSWORD_MODE) || \
                                     ((type) == SEC_DBG_STATE_CLOSE_MODE))

/*@} end of group SEC_DBG_Public_Constants */

/** @defgroup  SEC_DBG_Public_Macros
 *  @{
 */

/*@} end of group SEC_DBG_Public_Macros */

/** @defgroup  SEC_DBG_Public_Functions
 *  @{
 */
/*----------*/
void Sec_Dbg_Read_Chip_ID(uint8_t id[8]);
uint32_t Sec_Dbg_Read_Dbg_State(void);
uint32_t Sec_Dbg_Read_Dbg_Mode(void);
uint32_t Sec_Dbg_Read_Dbg_Enable(void);
/*----------*/
void Sec_Dbg_Set_Dbg_Pwd(const uint32_t pwd[4]);
void Sec_Dbg_Set_Dbg_Trigger(void);
/*----------*/
uint32_t Sec_Dbg_Wait_Ready(void);

/*@} end of group SEC_DBG_Public_Functions */

/*@} end of group SEC_DBG */

/*@} end of group QCC74X_Peripheral_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __QCC74X_SEC_DBG_H__ */
