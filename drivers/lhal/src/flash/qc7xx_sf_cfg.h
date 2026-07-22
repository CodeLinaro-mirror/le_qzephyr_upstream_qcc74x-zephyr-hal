/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_SF_CFG_H
#define _QC7XX_SF_CFG_H

#include "qc7xx_gpio.h"
#include "qc7xx_sflash.h"

/** @addtogroup  SF_CFG
 *  @{
 */

/** @defgroup  SF_CFG_Public_Types
 *  @{
 */

/*@} end of group SF_CFG_Public_Types */

/** @defgroup  SF_CFG_Public_Constants
 *  @{
 */

/*@} end of group SF_CFG_Public_Constants */

/** @defgroup  SF_CFG_Public_Macros
 *  @{
 */
#define QC7XX_GPIO_FUNC_SF 2
#if defined(QCC74X)
/* Flash option sf2 */
/* Flash CLK */
#define QC7XX_EXTFLASH_CLK0_GPIO   GPIO_PIN_8
/* FLASH CS */
#define QC7XX_EXTFLASH_CS0_GPIO    GPIO_PIN_4
/* FLASH DATA */
#define QC7XX_EXTFLASH_DATA00_GPIO GPIO_PIN_7
#define QC7XX_EXTFLASH_DATA10_GPIO GPIO_PIN_5
#define QC7XX_EXTFLASH_DATA20_GPIO GPIO_PIN_6
#define QC7XX_EXTFLASH_DATA30_GPIO GPIO_PIN_9
/* Flash option sf3 */
/* Flash CLK */
#define QC7XX_EXTFLASH_CLK1_GPIO   GPIO_PIN_15
/* FLASH CS */
#define QC7XX_EXTFLASH_CS1_GPIO    GPIO_PIN_20
/* FLASH DATA */
#define QC7XX_EXTFLASH_DATA01_GPIO GPIO_PIN_16
#define QC7XX_EXTFLASH_DATA11_GPIO GPIO_PIN_19
#define QC7XX_EXTFLASH_DATA21_GPIO GPIO_PIN_18
#define QC7XX_EXTFLASH_DATA31_GPIO GPIO_PIN_14
#elif defined(QCC75X)
/* Flash option sf2 */
/* Flash CLK */
#define QC7XX_EXTFLASH_CLK0_GPIO      GPIO_PIN_39
/* FLASH CS */
#define QC7XX_EXTFLASH_CS0_GPIO       GPIO_PIN_34
/* FLASH DATA */
#define QC7XX_EXTFLASH_DATA00_GPIO    GPIO_PIN_35
#define QC7XX_EXTFLASH_DATA10_GPIO    GPIO_PIN_36
#define QC7XX_EXTFLASH_DATA20_GPIO    GPIO_PIN_37
#define QC7XX_EXTFLASH_DATA30_GPIO    GPIO_PIN_38
/* Flash option sf3 */
/* Flash CLK */
#define QC7XX_EXTFLASH_CLK1_GPIO      GPIO_PIN_48
/* FLASH CS */
#define QC7XX_EXTFLASH_CS1_GPIO       GPIO_PIN_43
/* FLASH DATA */
#define QC7XX_EXTFLASH_DATA01_GPIO    GPIO_PIN_44
#define QC7XX_EXTFLASH_DATA11_GPIO    GPIO_PIN_45
#define QC7XX_EXTFLASH_DATA21_GPIO    GPIO_PIN_46
#define QC7XX_EXTFLASH_DATA31_GPIO    GPIO_PIN_47
#endif
#define QC7XX_FLASH_ID_VALID_FLAG 0x80000000
#define QC7XX_FLASH_ID_VALID_MASK 0x7FFFFFFF

/*@} end of group SF_CFG_Public_Macros */

/** @defgroup  SF_CFG_Public_Functions
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

int qc7xx_sf_cfg_get_flash_cfg_need_lock(uint32_t flash_id, spi_flash_cfg_type *p_flash_cfg,
                                        uint8_t group, uint8_t bank);
int qc7xx_sf_cfg_get_flash_cfg_need_lock_ext(uint32_t flash_id, spi_flash_cfg_type *p_flash_cfg,
                                            uint8_t group, uint8_t bank);
int qc7xx_sf_cfg_init_flash_gpio(uint8_t flash_pin_cfg, uint8_t restore_default);
#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
int qc7xx_sf_cfg_init_flash2_gpio(uint8_t swap);
#endif
int qc7xx_sf_cfg_init_ext_flash_gpio(uint8_t ext_flash_pin);
int qc7xx_sf_cfg_deinit_ext_flash_gpio(uint8_t ext_flash_pin);
uint32_t qc7xx_sf_cfg_flash_identify(uint8_t call_from_flash, uint8_t flash_pin_cfg, uint8_t restore_default,
                                    spi_flash_cfg_type *p_flash_cfg, uint8_t group, uint8_t bank);
uint32_t qc7xx_sf_cfg_flash_identify_ext(uint8_t callfromflash, uint8_t flash_pin_cfg, uint8_t restore_default,
                                        spi_flash_cfg_type *p_flash_cfg, uint8_t group, uint8_t bank);
#if defined(QCC74X) || defined(QCC75X)
int qc7xx_sf_cfg_flash_init(uint8_t sel, const struct sf_ctrl_cfg_type *p_sfctrl_cfg,
                           const struct sf_ctrl_bank2_cfg *p_bank2_cfg);
#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
int qc7xx_sf_cfg_sbus2_flash_init(uint8_t sel, const struct sf_ctrl_bank2_cfg *p_bank2_cfg);
#endif
#else
int qc7xx_sf_cfg_flash_init(uint8_t sel, const struct sf_ctrl_cfg_type *p_sfctrl_cfg);
#endif

#ifdef __cplusplus
}
#endif

/*@} end of group SF_CFG_Public_Functions */

/*@} end of group SF_CFG */

#endif /* _QC7XX_SF_CFG_H */
