/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_common.h"
#include "qc7xx_xip_sflash.h"

/** @addtogroup  XIP_SFLASH
 *  @{
 */

/** @defgroup  XIP_SFLASH_Private_Macros
 *  @{
 */

/*@} end of group XIP_SFLASH_Private_Macros */

/** @defgroup  XIP_SFLASH_Private_Types
 *  @{
 */

/*@} end of group XIP_SFLASH_Private_Types */

/** @defgroup  XIP_SFLASH_Private_Variables
 *  @{
 */

/*@} end of group XIP_SFLASH_Private_Variables */

/** @defgroup  XIP_SFLASH_Global_Variables
 *  @{
 */

/*@} end of group XIP_SFLASH_Global_Variables */

/** @defgroup  XIP_SFLASH_Private_Fun_Declaration
 *  @{
 */

/*@} end of group XIP_SFLASH_Private_Fun_Declaration */

/** @defgroup  XIP_SFLASH_Private_Functions
 *  @{
 */

/****************************************************************************/ /**
 * @brief  Save flash controller state
 *
 * @param  p_flash_cfg: Flash config pointer
 * @param  offset: CPU XIP flash offset pointer
 * @param  group: CPU group id 0 or 1
 * @param  bank: Flash bank select
 *
 * @return QC7XX_RET:0 means success and other value means error
 *
*******************************************************************************/
__WEAK
int ATTR_TCM_SECTION qc7xx_xip_sflash_state_save(spi_flash_cfg_type *p_flash_cfg, uint32_t *offset,
                                                uint8_t group, uint8_t bank)
{
#ifdef romapi_qc7xx_xip_sflash_state_save
    return romapi_qc7xx_xip_sflash_state_save(p_flash_cfg, offset, group, bank);
#else
    /* XIP_SFlash_Delay */
    uint32_t i = 0;
    uint8_t sfc_owner = SF_CTRL_OWNER_IAHB;

    for ( i = 0; i < 64; i++) {
        __asm__ volatile ("nop");
    }

    sfc_owner = qc7xx_sf_ctrl_get_owner();
    qc7xx_sf_ctrl_set_owner_flag(sfc_owner);
    if (sfc_owner == SF_CTRL_OWNER_SAHB) {
        return 0;
    }

#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
    if (bank == SF_CTRL_FLASH_BANK1) {
        qc7xx_sf_ctrl_sbus2_replace(SF_CTRL_PAD2);
    }
#endif
    qc7xx_sf_ctrl_set_owner(SF_CTRL_OWNER_SAHB);
    /* Exit from continous read for accepting command */
    qc7xx_sflash_reset_continue_read(p_flash_cfg);
    /* For disable command that is setting register instead of send command, we need write enable */
    qc7xx_sflash_disable_burst_wrap(p_flash_cfg);
#ifdef QC7XX_SF_CTRL_32BITS_ADDR_ENABLE
    /* Enable 32Bits addr mode again in case reset command make it reset */
    qc7xx_sflash_set_32bits_addr_mode(p_flash_cfg, 1);
#endif
    if ((p_flash_cfg->io_mode & 0x0f) == SF_CTRL_QO_MODE || (p_flash_cfg->io_mode & 0x0f) == SF_CTRL_QIO_MODE) {
        /* Enable QE again in case reset command make it reset */
        qc7xx_sflash_qspi_enable(p_flash_cfg);
    }
    /* Deburst again to make sure */
    qc7xx_sflash_disable_burst_wrap(p_flash_cfg);

    /* Clear offset setting */
    *offset = qc7xx_sf_ctrl_get_flash_image_offset(group, bank);
    qc7xx_sf_ctrl_set_flash_image_offset(0, group, bank);

    return 0;
#endif
}

/****************************************************************************/ /**
 * @brief  Restore flash controller state
 *
 * @param  p_flash_cfg: Flash config pointer
 * @param  offset: CPU XIP flash offset
 * @param  group: CPU group id 0 or 1
 * @param  bank: Flash bank select
 *
 * @return QC7XX_RET:0 means success and other value means error
 *
*******************************************************************************/
__WEAK
int ATTR_TCM_SECTION qc7xx_xip_sflash_state_restore(spi_flash_cfg_type *p_flash_cfg, uint32_t offset,
                                                   uint8_t group, uint8_t bank)
{
#ifdef romapi_qc7xx_xip_sflash_state_restore
    return romapi_qc7xx_xip_sflash_state_restore(p_flash_cfg, offset, group, bank);
#else
    uint32_t tmp[1];
    uint8_t io_mode = p_flash_cfg->io_mode & 0xf;
    uint8_t sfc_owner = SF_CTRL_OWNER_IAHB;

    sfc_owner = qc7xx_sf_ctrl_get_owner_flag();
    if (sfc_owner == SF_CTRL_OWNER_SAHB) {
        return 0;
    }

    qc7xx_sf_ctrl_set_flash_image_offset(offset, group, bank);

    if (((p_flash_cfg->io_mode >> 4) & 0x01) == 0) {
        if ((p_flash_cfg->io_mode & 0x0f) == SF_CTRL_QO_MODE || (p_flash_cfg->io_mode & 0x0f) == SF_CTRL_QIO_MODE) {
            qc7xx_sflash_set_burst_wrap(p_flash_cfg);
        }
    }
#ifdef QC7XX_SF_CTRL_32BITS_ADDR_ENABLE
    qc7xx_sflash_set_32bits_addr_mode(p_flash_cfg, 1);
#endif
    qc7xx_sflash_read(p_flash_cfg, io_mode, 1, 0x0, (uint8_t *)tmp, sizeof(tmp));
    qc7xx_sflash_set_xip_cfg(p_flash_cfg, io_mode, 1, 0, 32, bank);
#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
    if (bank == SF_CTRL_FLASH_BANK1) {
        qc7xx_sf_ctrl_sbus2_revoke_replace();
    }
#endif

    return 0;
#endif
}

/*@} end of group XIP_SFLASH_Private_Functions */

/** @defgroup  XIP_SFLASH_Public_Functions
 *  @{
 */

/****************************************************************************/ /**
 * @brief  Erase flash one region
 *
 * @param  p_flash_cfg: Flash config pointer
 * @param  start_addr: start address to erase
 * @param  len: data length to erase
 * @param  group: CPU group id 0 or 1
 * @param  bank: Flash bank select
 *
 * @return QC7XX_RET:0 means success and other value means error
 *
*******************************************************************************/
__WEAK
int ATTR_TCM_SECTION qc7xx_xip_sflash_erase_need_lock(spi_flash_cfg_type *p_flash_cfg, uint32_t start_addr,
                                                     int len, uint8_t group, uint8_t bank)
{
#ifdef romapi_qc7xx_xip_sflash_erase_need_lock
    return romapi_qc7xx_xip_sflash_erase_need_lock(p_flash_cfg, start_addr, len, group, bank);
#else
    int stat = 0;
    uint32_t offset = 0;
    uint8_t aes_enable = 0;
    uint8_t io_mode = p_flash_cfg->io_mode & 0xf;

    qc7xx_xip_sflash_opt_enter(&aes_enable);
    stat = qc7xx_xip_sflash_state_save(p_flash_cfg, &offset, group, bank);

    if (stat != 0) {
        qc7xx_sflash_set_xip_cfg(p_flash_cfg, io_mode, 1, 0, 32, bank);
    } else {
        stat = qc7xx_sflash_erase(p_flash_cfg, start_addr, start_addr + len - 1);
        qc7xx_xip_sflash_state_restore(p_flash_cfg, offset, group, bank);
    }

    qc7xx_xip_sflash_opt_exit(aes_enable);

    return stat;
#endif
}

/****************************************************************************/ /**
 * @brief  Program flash one region
 *
 * @param  p_flash_cfg: Flash config pointer
 * @param  addr: start address to be programed
 * @param  data: data pointer to be programed
 * @param  len: data length to be programed
 * @param  group: CPU group id 0 or 1
 * @param  bank: Flash bank select
 *
 * @return QC7XX_RET:0 means success and other value means error
 *
*******************************************************************************/
__WEAK
int ATTR_TCM_SECTION qc7xx_xip_sflash_write_need_lock(spi_flash_cfg_type *p_flash_cfg, uint32_t addr, uint8_t *data,
                                                     uint32_t len, uint8_t group, uint8_t bank)
{
#ifdef romapi_qc7xx_xip_sflash_write_need_lock
    return romapi_qc7xx_xip_sflash_write_need_lock(p_flash_cfg, addr, data, len, group, bank);
#else
    int stat = 0;
    uint32_t offset = 0;
    uint8_t aes_enable = 0;
    uint8_t io_mode = p_flash_cfg->io_mode & 0xf;

    qc7xx_xip_sflash_opt_enter(&aes_enable);
    stat = qc7xx_xip_sflash_state_save(p_flash_cfg, &offset, group, bank);

    if (stat != 0) {
        qc7xx_sflash_set_xip_cfg(p_flash_cfg, io_mode, 1, 0, 32, bank);
    } else {
        stat = qc7xx_sflash_program(p_flash_cfg, io_mode, addr, data, len);
        qc7xx_xip_sflash_state_restore(p_flash_cfg, offset, group, bank);
    }

    qc7xx_xip_sflash_opt_exit(aes_enable);

    return stat;
#endif
}

/****************************************************************************/ /**
 * @brief  Read data from flash
 *
 * @param  p_flash_cfg: Flash config pointer
 * @param  addr: flash read start address
 * @param  data: data pointer to store data read from flash
 * @param  len: data length to read
 * @param  group: CPU group id 0 or 1
 * @param  bank: Flash bank select
 *
 * @return QC7XX_RET:0 means success and other value means error
 *
*******************************************************************************/
__WEAK
int ATTR_TCM_SECTION qc7xx_xip_sflash_read_need_lock(spi_flash_cfg_type *p_flash_cfg, uint32_t addr, uint8_t *data,
                                                    uint32_t len, uint8_t group, uint8_t bank)
{
#ifdef romapi_qc7xx_xip_sflash_read_need_lock
    return romapi_qc7xx_xip_sflash_read_need_lock(p_flash_cfg, addr, data, len, group, bank);
#else
    int stat = 0;
    uint32_t offset = 0;
    uint8_t aes_enable = 0;
    uint8_t io_mode = p_flash_cfg->io_mode & 0xf;

    qc7xx_xip_sflash_opt_enter(&aes_enable);
    stat = qc7xx_xip_sflash_state_save(p_flash_cfg, &offset, group, bank);

    if (stat != 0) {
        qc7xx_sflash_set_xip_cfg(p_flash_cfg, io_mode, 1, 0, 32, bank);
    } else {
        stat = qc7xx_sflash_read(p_flash_cfg, io_mode, 0, addr, data, len);
        qc7xx_xip_sflash_state_restore(p_flash_cfg, offset, group, bank);
    }

    qc7xx_xip_sflash_opt_exit(aes_enable);

    return stat;
#endif
}

/****************************************************************************/ /**
 * @brief  Get Flash Jedec ID
 *
 * @param  p_flash_cfg: Flash config pointer
 * @param  data: data pointer to store Jedec ID Read from flash
 * @param  group: CPU group id 0 or 1
 * @param  bank: Flash bank select
 *
 * @return QC7XX_RET:0 means success and other value means error
 *
*******************************************************************************/
__WEAK
int ATTR_TCM_SECTION qc7xx_xip_sflash_get_jedecid_need_lock(spi_flash_cfg_type *p_flash_cfg, uint8_t *data,
                                                           uint8_t group, uint8_t bank)
{
#ifdef romapi_qc7xx_xip_sflash_get_jedecid_need_lock
    return romapi_qc7xx_xip_sflash_get_jedecid_need_lock(p_flash_cfg, data, group, bank);
#else
    int stat = 0;
    uint32_t offset = 0;
    uint8_t aes_enable = 0;
    uint8_t io_mode = p_flash_cfg->io_mode & 0xf;

    qc7xx_xip_sflash_opt_enter(&aes_enable);
    stat = qc7xx_xip_sflash_state_save(p_flash_cfg, &offset, group, bank);

    if (stat != 0) {
        qc7xx_sflash_set_xip_cfg(p_flash_cfg, io_mode, 1, 0, 32, bank);
    } else {
        qc7xx_sflash_get_jedecid(p_flash_cfg, data);
        qc7xx_xip_sflash_state_restore(p_flash_cfg, offset, group, bank);
    }

    qc7xx_xip_sflash_opt_exit(aes_enable);

    return 0;
#endif
}

/****************************************************************************/ /**
 * @brief  Get Flash Device ID
 *
 * @param  p_flash_cfg: Flash config pointer
 * @param  is_32bits_addr: Is flash addr mode in 32-bits
 * @param  data: data pointer to store Device ID Read from flash
 * @param  group: CPU group id 0 or 1
 * @param  bank: Flash bank select
 *
 * @return QC7XX_RET:0 means success and other value means error
 *
*******************************************************************************/
__WEAK
int ATTR_TCM_SECTION qc7xx_xip_sflash_get_deviceid_need_lock(spi_flash_cfg_type *p_flash_cfg, uint8_t is_32bits_addr,
                                                            uint8_t *data, uint8_t group, uint8_t bank)
{
#ifdef romapi_qc7xx_xip_sflash_get_deviceid_need_lock
    return romapi_qc7xx_xip_sflash_get_deviceid_need_lock(p_flash_cfg, is_32bits_addr, data, group, bank);
#else
    int stat = 0;
    uint32_t offset = 0;
    uint8_t aes_enable = 0;
    uint8_t io_mode = p_flash_cfg->io_mode & 0xf;

    qc7xx_xip_sflash_opt_enter(&aes_enable);
    stat = qc7xx_xip_sflash_state_save(p_flash_cfg, &offset, group, bank);

    if (stat != 0) {
        qc7xx_sflash_set_xip_cfg(p_flash_cfg, io_mode, 1, 0, 32, bank);
    } else {
        qc7xx_sflash_get_deviceid(data, is_32bits_addr);
        qc7xx_xip_sflash_state_restore(p_flash_cfg, offset, group, bank);
    }

    qc7xx_xip_sflash_opt_exit(aes_enable);

    return 0;
#endif
}

/****************************************************************************/ /**
 * @brief  Get Flash Unique ID
 *
 * @param  p_flash_cfg: Flash config pointer
 * @param  data: data pointer to store Device ID Read from flash
 * @param  id_len: Unique id len
 * @param  group: CPU group id 0 or 1
 * @param  bank: Flash bank select
 *
 * @return QC7XX_RET:0 means success and other value means error
 *
*******************************************************************************/
__WEAK
int ATTR_TCM_SECTION qc7xx_xip_sflash_get_uniqueid_need_lock(spi_flash_cfg_type *p_flash_cfg, uint8_t *data,
                                                            uint8_t id_len, uint8_t group, uint8_t bank)
{
#ifdef romapi_qc7xx_xip_sflash_get_uniqueid_need_lock
    return romapi_qc7xx_xip_sflash_get_uniqueid_need_lock(p_flash_cfg, data, id_len, group, bank);
#else
    int stat = 0;
    uint32_t offset = 0;
    uint8_t aes_enable = 0;
    uint8_t io_mode = p_flash_cfg->io_mode & 0xf;

    qc7xx_xip_sflash_opt_enter(&aes_enable);
    stat = qc7xx_xip_sflash_state_save(p_flash_cfg, &offset, group, bank);

    if (stat != 0) {
        qc7xx_sflash_set_xip_cfg(p_flash_cfg, io_mode, 1, 0, 32, bank);
    } else {
        qc7xx_sflash_get_uniqueid(data, id_len);
        qc7xx_xip_sflash_state_restore(p_flash_cfg, offset, group, bank);
    }

    qc7xx_xip_sflash_opt_exit(aes_enable);

    return 0;
#endif
}

/****************************************************************************/ /**
 * @brief  Clear flash status register need lock
 *
 * @param  p_flash_cfg: Flash config pointer
 * @param  group: CPU group id 0 or 1
 * @param  bank: Flash bank select
 *
 * @return QC7XX_RET:0 means success and other value means error
 *
*******************************************************************************/
__WEAK
int ATTR_TCM_SECTION qc7xx_xip_sflash_clear_status_register_need_lock(spi_flash_cfg_type *p_flash_cfg,
                                                                     uint8_t group, uint8_t bank)
{
#ifdef romapi_qc7xx_xip_sflash_clear_status_register_need_lock
    return romapi_qc7xx_xip_sflash_clear_status_register_need_lock(p_flash_cfg, group, bank);
#else
    int stat = 0;
    uint32_t offset = 0;
    uint8_t aes_enable = 0;
    uint8_t io_mode = p_flash_cfg->io_mode & 0xf;

    qc7xx_xip_sflash_opt_enter(&aes_enable);
    stat = qc7xx_xip_sflash_state_save(p_flash_cfg, &offset, group, bank);

    if (stat != 0) {
        qc7xx_sflash_set_xip_cfg(p_flash_cfg, io_mode, 1, 0, 32, bank);
    } else {
        stat = qc7xx_sflash_clear_status_register(p_flash_cfg);
        qc7xx_xip_sflash_state_restore(p_flash_cfg, offset, group, bank);
    }

    qc7xx_xip_sflash_opt_exit(aes_enable);

    return 0;
#endif
}

/****************************************************************************/ /**
 * @brief  Read data from flash via XIP
 *
 * @param  addr: flash read start address
 * @param  data: data pointer to store data read from flash
 * @param  len: data length to read
 * @param  group: CPU group id 0 or 1
 * @param  bank: Flash bank select
 *
 * @return QC7XX_RET:0 means success and other value means error
 *
*******************************************************************************/
__WEAK
int ATTR_TCM_SECTION qc7xx_xip_sflash_read_via_cache_need_lock(uint32_t addr, uint8_t *data, uint32_t len,
                                                              uint8_t group, uint8_t bank)
{
#ifdef romapi_qc7xx_xip_sflash_read_via_cache_need_lock
    return romapi_qc7xx_xip_sflash_read_via_cache_need_lock(addr, data, len, group, bank);
#else
    uint32_t offset = 0;

    addr = addr & (QC7XX_FLASH_XIP_END - QC7XX_FLASH_XIP_BASE - 1);
    addr |= QC7XX_FLASH_XIP_BASE;

    offset = qc7xx_sf_ctrl_get_flash_image_offset(group, bank);
    qc7xx_sf_ctrl_set_flash_image_offset(0, group, bank);
    /* Flash read */
    arch_memcpy_fast(data, (void *)(uintptr_t)(addr - qc7xx_sf_ctrl_get_flash_image_offset(group, bank)), len);
    qc7xx_sf_ctrl_set_flash_image_offset(offset, group, bank);

    return 0;
#endif
}

/****************************************************************************/ /**
 * @brief  XIP SFlash option save
 *
 * @param  aes_enable: AES enable status pointer
 *
 * @return None
 *
*******************************************************************************/
__WEAK
void ATTR_TCM_SECTION qc7xx_xip_sflash_opt_enter(uint8_t *aes_enable)
{
#ifdef romapi_qc7xx_xip_sflash_opt_enter
    romapi_qc7xx_xip_sflash_opt_enter(aes_enable);
#else
    *aes_enable = qc7xx_sf_ctrl_is_aes_enable();

    if (*aes_enable) {
        qc7xx_sf_ctrl_aes_disable();
    }
#endif
}

/****************************************************************************/ /**
 * @brief  XIP SFlash option restore
 *
 * @param  aes_enable: AES enable status
 *
 * @return None
 *
*******************************************************************************/
__WEAK
void ATTR_TCM_SECTION qc7xx_xip_sflash_opt_exit(uint8_t aes_enable)
{
#ifdef romapi_qc7xx_xip_sflash_opt_exit
    romapi_qc7xx_xip_sflash_opt_exit(aes_enable);
#else
    if (aes_enable) {
        qc7xx_sf_ctrl_aes_enable();
    }
#endif
}

/*@} end of group XIP_SFLASH_Public_Functions */

/*@} end of group XIP_SFLASH */
