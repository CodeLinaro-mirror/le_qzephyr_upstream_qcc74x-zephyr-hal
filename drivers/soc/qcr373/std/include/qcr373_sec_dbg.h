/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCR373_SEC_DBG_H__
#define __QCR373_SEC_DBG_H__

#include "sec_dbg_reg.h"

/** @addtogroup  QCR373_Peripheral_Driver
 *  @{
 */

/** @addtogroup  SEC_DBG
 *  @{
 */

/** @defgroup  SEC_DBG_Public_Types
 *  @{
 */
typedef enum {
    SEC_DBG_DEBUG_OPEN_MODE,       /*!< debug open mode */
    SEC_DBG_DEBUG_PASSWORD_MODE,   /*!< debug password mode */
    SEC_DBG_DEBUG_CLOSE_MODE,      /*!< debug close mode */
} SEC_DBG_STATE_Type;

/*@} end of group SEC_DBG_Public_Types */

/** @defgroup  SEC_DBG_Public_Constants
 *  @{
 */

/*@} end of group SEC_DBG_Public_Constants */

/** @defgroup  SEC_DBG_Public_Macros
 *  @{
 */
#define IS_SEC_DBG_STATE_TYPE(type) (((type) == SEC_DBG_DEBUG_OPEN_MODE) || \
                                     ((type) == SEC_DBG_DEBUG_PASSWORD_MODE) || \
                                     ((type) == SEC_DBG_DEBUG_CLOSE_MODE))

/*@} end of group SEC_DBG_Public_Macros */

/** @defgroup  SEC_DBG_Public_Functions
 *  @{
 */
void Sec_Dbg_Read_Chip_ID(uint8_t id[8]);
void Sec_Dbg_Set_Dbg_Pwd(const uint32_t pwd[4]);
SEC_DBG_STATE_Type Sec_Dbg_Read_Dbg_State(void);
uint32_t Sec_Dbg_Read_Dbg_Mode(void);
uint32_t Sec_Dbg_Read_Dbg_Enable(void);
void Sec_Dbg_Set_Dbg_Trigger(void);
uint32_t Sec_Dbg_Wait_Ready(void);

/*@} end of group SEC_DBG_Public_Functions */

/*@} end of group SEC_DBG */

/*@} end of group QCR373_Peripheral_Driver */

#endif /* __QCR373_SEC_DBG_H__ */
