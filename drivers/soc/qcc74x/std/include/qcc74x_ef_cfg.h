/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCC74X_EF_CFG_H__
#define __QCC74X_EF_CFG_H__

#include "qc7xx_ef_ctrl.h"
#include "qcc74x_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup  QCC74X_Peripheral_Driver
 *  @{
 */

/** @addtogroup  EF_CTRL
 *  @{
 */

/** @defgroup  EF_CTRL_Public_Types
 *  @{
 */

/*@} end of group EF_CTRL_Public_Types */

/** @defgroup  EF_CTRL_Public_Constants
 *  @{
 */

/*@} end of group EF_CTRL_Public_Constants */

/** @defgroup  EF_CTRL_Public_Macros
 *  @{
 */

/*@} end of group EF_CTRL_Public_Macros */

/** @defgroup  EF_CTRL_Public_Functions
 *  @{
 */
void qc7xx_efuse_read_secure_boot(uint8_t *sign, uint8_t *aes);

int qc7xx_efuse_enable_aes(uint8_t aes_type, uint8_t xts_mode);

int qc7xx_efuse_rw_lock_aes_key(uint8_t key_index, uint8_t rd_lock, uint8_t wr_lock);

int qc7xx_efuse_rw_lock_dbg_key(uint8_t rd_lock, uint8_t wr_lock);

int qc7xx_efuse_write_lock_pk_hash(uint32_t pkhash_len);

int qc7xx_efuse_write_lock_usb_pid_vid(void);

/*@} end of group EF_CTRL_Public_Functions */

/*@} end of group EF_CTRL */

/*@} end of group QCC74X_Peripheral_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __QCC74X_EF_CFG_H__ */
