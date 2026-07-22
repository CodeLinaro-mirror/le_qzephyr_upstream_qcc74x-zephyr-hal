/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_FLASH_SECREG_H
#define _QC7XX_FLASH_SECREG_H

#include <stdint.h>
#include <stdbool.h>
#include "qc7xx_sflash.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/** @defgroup  Flash_Secreg_Public_Types
 *  @{
 */

/**
 *  @brief Flash secreg param structure type definition
 */
typedef union {
    uint8_t raw[12];
    struct __attribute__((packed)) {
        uint32_t region_count  : 6; /*!< flash security register region count */
        uint32_t region_size   : 6; /*!< flash security register region size, unit 256B */
        uint32_t secreg_size   : 6; /*!< flash security register valid size, unit 256B */
        uint32_t api_type      : 6; /*!< flash security register api type */
        uint32_t region_offset : 4; /*!< flash security register address offset, unit region_size */
        uint32_t lb_share      : 2; /*!< lock bit effect for all region */
        uint32_t rsvd          : 2; /*!< reserved 2bit */
        uint8_t lb_offset;          /*!< lock bit offset in all status register */
        uint8_t lb_write_cmd;       /*!< lock bit write status register write command */
        uint8_t lb_write_len;       /*!< lock bit write status register write len */
        uint8_t lb_read_cmd[3];     /*!< lock bit read status register read command */
        uint8_t lb_read_len;        /*!< lock bit read status register read len */
        uint8_t lb_read_loop;       /*!< lock bit read status register read loop count */
    };
} qc7xx_flash_secreg_param_t;

/**
 *  @brief Flash secreg region info structure type definition
 */
typedef struct {
    uint32_t address;
    uint32_t physical_address;
    uint16_t region_size;
    uint16_t secreg_size;
    uint8_t index;
    uint8_t locked;
    uint8_t lockbit_share;
} qc7xx_flash_secreg_region_info_t;

/**
 *  @brief Callback info structure type definition
 */
struct callapi_content {
    uint32_t offset;
    uintptr_t flag;
    uint8_t aes_enable;
    uint8_t before_ret0;
    uint8_t rsvd[2];
    uint32_t dummy;
};

/**
 *  @brief Flash OTP configuration structure type definition
 */
typedef struct {
    const spi_flash_cfg_type *flash_cfg;    /*!< Flash configuration */
    const qc7xx_flash_secreg_param_t *param; /*!< Security register parameters */
}qc7xx_flash_otp_config_t;

/*@} end of group Flash_Secreg_Public_Types */

/*****************************************************************************
* @brief        callback type for iteration over flash security register
*               regions present on a device
* @param[in]    info        information for current region
* @param[in]    data        private data for callback
* @return       true to continue iteration, false to exit iteration.
* @see          qc7xx_flash_secreg_region_foreach()
*
*****************************************************************************/
typedef bool (*region_cb)(const qc7xx_flash_secreg_region_info_t *info, void *data);

int qc7xx_flash_secreg_callapi_before(const spi_flash_cfg_type *flash_cfg, struct callapi_content *content);
void qc7xx_flash_secreg_callapi_after(const spi_flash_cfg_type *flash_cfg, struct callapi_content *content);
int qc7xx_flash_secreg_get_param(uint32_t jid, const qc7xx_flash_secreg_param_t **param);
int qc7xx_flash_secreg_region_foreach(qc7xx_flash_otp_config_t *otp_cfg, region_cb cb, void *data);

int qc7xx_flash_secreg_get_lockbits(qc7xx_flash_otp_config_t *otp_cfg, uint8_t *lockbits);
int qc7xx_flash_secreg_set_lockbits(qc7xx_flash_otp_config_t *otp_cfg, uint8_t lockbits);
int qc7xx_flash_secreg_get_locked(qc7xx_flash_otp_config_t *otp_cfg, uint8_t index);
int qc7xx_flash_secreg_set_locked(qc7xx_flash_otp_config_t *otp_cfg, uint8_t index);

int qc7xx_flash_secreg_get_info(qc7xx_flash_otp_config_t *otp_cfg, uint32_t addr, qc7xx_flash_secreg_region_info_t *info);
int qc7xx_flash_secreg_valid(qc7xx_flash_otp_config_t *otp_cfg, uint32_t addr, uint32_t len);
int qc7xx_flash_secreg_read(qc7xx_flash_otp_config_t *otp_cfg, uint32_t addr, void *data, uint32_t len);
int qc7xx_flash_secreg_write(qc7xx_flash_otp_config_t *otp_cfg, uint32_t addr, const void *data, uint32_t len);
int qc7xx_flash_secreg_erase(qc7xx_flash_otp_config_t *otp_cfg, uint32_t addr, uint32_t len);

int qc7xx_flash_secreg_get_info_by_idx(qc7xx_flash_otp_config_t *otp_cfg, uint8_t index, qc7xx_flash_secreg_region_info_t *info);
int qc7xx_flash_secreg_valid_by_idx(qc7xx_flash_otp_config_t *otp_cfg, uint8_t index, uint32_t offset, uint32_t len);
int qc7xx_flash_secreg_read_by_idx(qc7xx_flash_otp_config_t *otp_cfg, uint8_t index, uint32_t offset, void *data, uint32_t len);
int qc7xx_flash_secreg_write_by_idx(qc7xx_flash_otp_config_t *otp_cfg, uint8_t index, uint32_t offset, const void *data, uint32_t len);
int qc7xx_flash_secreg_erase_by_idx(qc7xx_flash_otp_config_t *otp_cfg, uint8_t index);

#endif
