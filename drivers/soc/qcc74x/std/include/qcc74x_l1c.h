/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCC74X_L1C_H__
#define __QCC74X_L1C_H__

// #include "pds_reg.h"
#include "qcc74x_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup  QCC74X_Peripheral_Driver
 *  @{
 */

/** @addtogroup  L1C
 *  @{
 */

/** @defgroup  L1C_Public_Types
 *  @{
 */

/**
 *  @brief L1C configuration structure type definition
 */
typedef struct
{
    uint8_t cacheEn;
    uint8_t wayDis;
    uint8_t wa;
    uint8_t wb;
    uint8_t wt;
    uint8_t rsvd[3];
    uint32_t cacheRangeL;
    uint32_t cacheRangeH;
} L1C_CACHE_Cfg_Type;

/*@} end of group L1C_Public_Types */

/** @defgroup  L1C_Public_Constants
 *  @{
 */

/*@} end of group L1C_Public_Constants */

/** @defgroup  L1C_Public_Macros
 *  @{
 */
#define L1C_WAY_DISABLE_NONE       0x00
#define L1C_WAY_DISABLE_ONE        0x01
#define L1C_WAY_DISABLE_TWO        0x03
#define L1C_WAY_DISABLE_ALL        0x03
#define L1C_WAY_DISABLE_NOT_CAHNGE 0xFF

/*@} end of group L1C_Public_Macros */

/** @defgroup  L1C_Public_Functions
 *  @{
 */

/*----------*/
QC7XX_Err_Type L1C_ICache_Enable(uint8_t wayDsiable);
QC7XX_Err_Type L1C_DCache_Enable(uint8_t wayDsiable);
QC7XX_Err_Type L1C_ICache_Disable(void);
QC7XX_Err_Type L1C_DCache_Disable(void);
void L1C_DCache_Write_Set(QC7XX_Fun_Type wtEn, QC7XX_Fun_Type wbEn, QC7XX_Fun_Type waEn);
QC7XX_Err_Type L1C_DCache_Clean_All(void);
QC7XX_Err_Type L1C_DCache_Clean_Invalid_All(void);
QC7XX_Err_Type L1C_ICache_Invalid_All(void);
QC7XX_Err_Type L1C_DCache_Invalid_All(void);
QC7XX_Err_Type L1C_DCache_Clean_By_Addr(uintptr_t addr, uint32_t len);
QC7XX_Err_Type L1C_DCache_Clean_Invalid_By_Addr(uintptr_t addr, uint32_t len);
QC7XX_Err_Type L1C_ICache_Invalid_By_Addr(uintptr_t addr, uint32_t len);
QC7XX_Err_Type L1C_DCache_Invalid_By_Addr(uintptr_t addr, uint32_t len);
/*----------*/
QC7XX_Err_Type L1C_Set_Wrap(uint8_t en);
QC7XX_Err_Type L1C_Set_Cache_Setting_By_ID(uint8_t core, L1C_CACHE_Cfg_Type *cacheSetting);
/*----------*/
int L1C_Is_DCache_Range(uintptr_t addr);
int L1C_Get_None_Cache_Addr(uintptr_t addr);
/*@} end of group L1C_Public_Functions */

/*@} end of group L1C */

/*@} end of group QCC74X_Peripheral_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __QCC74X_L1C_H__ */
