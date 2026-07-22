/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(QCC75X)
#include "qcc75x_memorymap.h"
#include "qcc75x_glb.h"
#elif defined(QCC74X)
#include "qcc74x_memorymap.h"
#include "qcc74x_glb.h"
#include "qcc74x_ef_cfg.h"
#endif
#include "qc7xx_xip_sflash.h"
#include "qc7xx_sf_cfg.h"
#include "qc7xx_sflash.h"
#include "qc7xx_flash.h"
#include "hardware/sf_ctrl_reg.h"
#include "qc7xx_efuse.h"

#if defined(QCC74X) || defined(QCC75X)
static uint32_t flash1_size = 4 * 1024 * 1024;
static uint32_t flash2_size = 2 * 1024 * 1024;
#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
static uint32_t g_jedec_id2 = 0;
#endif
#endif
static uint32_t g_jedec_id = 0;
static spi_flash_cfg_type g_flash_cfg = {
    .reset_c_read_cmd = 0xff,
    .reset_c_read_cmd_size = 3,
    .mid = 0xc8,

    .de_burst_wrap_cmd = 0x77,
    .de_burst_wrap_cmd_dmy_clk = 0x3,
    .de_burst_wrap_data_mode = SF_CTRL_DATA_4_LINES,
    .de_burst_wrap_data = 0xF0,

    /*reg*/
    .write_enable_cmd = 0x06,
    .wr_enable_index = 0x00,
    .wr_enable_bit = 0x01,
    .wr_enable_read_reg_len = 0x01,

    .qe_index = 1,
    .qe_bit = 0x01,
    .qe_write_reg_len = 0x01,
    .qe_read_reg_len = 0x1,

    .busy_index = 0,
    .busy_bit = 0x00,
    .busy_read_reg_len = 0x1,
    .release_powerdown = 0xab,

    .read_reg_cmd[0] = 0x05,
    .read_reg_cmd[1] = 0x35,
    .write_reg_cmd[0] = 0x01,
    .write_reg_cmd[1] = 0x31,

    .fast_read_qio_cmd = 0xeb,
    .fr_qio_dmy_clk = 16 / 8,
    .c_read_support = 0,
    .c_read_mode = 0x20,

    .burst_wrap_cmd = 0x77,
    .burst_wrap_cmd_dmy_clk = 0x3,
    .burst_wrap_data_mode = SF_CTRL_DATA_4_LINES,
    .burst_wrap_data = 0x40,
    /*erase*/
    .chip_erase_cmd = 0xc7,
    .sector_erase_cmd = 0x20,
    .blk32_erase_cmd = 0x52,
    .blk64_erase_cmd = 0xd8,
    /*write*/
    .page_program_cmd = 0x02,
    .qpage_program_cmd = 0x32,
    .qpp_addr_mode = SF_CTRL_ADDR_1_LINE,

    .io_mode = 0x11,
    .clk_delay = 0,
    .clk_invert = 0x03,

    .reset_en_cmd = 0x66,
    .reset_cmd = 0x99,
    .c_rexit = 0xff,
    .wr_enable_write_reg_len = 0x00,

    /*id*/
    .jedec_id_cmd = 0x9f,
    .jedec_id_cmd_dmy_clk = 0,
    .enter_32bits_addr_cmd = 0xb7,
    .exit_32bits_addr_cmd = 0xe9,
    .sector_size = 4,
    .page_size = 256,

    /*read*/
    .fast_read_cmd = 0x0b,
    .fr_dmy_clk = 8 / 8,
    .qpi_fast_read_cmd = 0x0b,
    .qpi_fr_dmy_clk = 8 / 8,
    .fast_read_do_cmd = 0x3b,
    .fr_do_dmy_clk = 8 / 8,
    .fast_read_dio_cmd = 0xbb,
    .fr_dio_dmy_clk = 0,
    .fast_read_qo_cmd = 0x6b,
    .fr_qo_dmy_clk = 8 / 8,

    .qpi_fast_read_qio_cmd = 0xeb,
    .qpi_fr_qio_dmy_clk = 16 / 8,
    .qpi_page_program_cmd = 0x02,
    .write_vreg_enable_cmd = 0x50,

    /* qpi mode */
    .enter_qpi = 0x38,
    .exit_qpi = 0xff,

    /*AC*/
    .time_e_sector = 300,
    .time_e_32k = 1200,
    .time_e_64k = 1200,
    .time_page_pgm = 5,
    .time_ce = 33 * 1000,
    .pd_delay = 20,
    .qe_data = 0,
};
#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
static spi_flash_cfg_type g_flash2_cfg = {
    .reset_c_read_cmd = 0xff,
    .reset_c_read_cmd_size = 3,
    .mid = 0xc8,

    .de_burst_wrap_cmd = 0x77,
    .de_burst_wrap_cmd_dmy_clk = 0x3,
    .de_burst_wrap_data_mode = SF_CTRL_DATA_4_LINES,
    .de_burst_wrap_data = 0xF0,

    /*reg*/
    .write_enable_cmd = 0x06,
    .wr_enable_index = 0x00,
    .wr_enable_bit = 0x01,
    .wr_enable_read_reg_len = 0x01,

    .qe_index = 1,
    .qe_bit = 0x01,
    .qe_write_reg_len = 0x01,
    .qe_read_reg_len = 0x1,

    .busy_index = 0,
    .busy_bit = 0x00,
    .busy_read_reg_len = 0x1,
    .release_powerdown = 0xab,

    .read_reg_cmd[0] = 0x05,
    .read_reg_cmd[1] = 0x35,
    .write_reg_cmd[0] = 0x01,
    .write_reg_cmd[1] = 0x31,

    .fast_read_qio_cmd = 0xeb,
    .fr_qio_dmy_clk = 16 / 8,
    .c_read_support = 0,
    .c_read_mode = 0x20,

    .burst_wrap_cmd = 0x77,
    .burst_wrap_cmd_dmy_clk = 0x3,
    .burst_wrap_data_mode = SF_CTRL_DATA_4_LINES,
    .burst_wrap_data = 0x40,
    /*erase*/
    .chip_erase_cmd = 0xc7,
    .sector_erase_cmd = 0x20,
    .blk32_erase_cmd = 0x52,
    .blk64_erase_cmd = 0xd8,
    /*write*/
    .page_program_cmd = 0x02,
    .qpage_program_cmd = 0x32,
    .qpp_addr_mode = SF_CTRL_ADDR_1_LINE,

    .io_mode = 0x10,
    .clk_delay = 0,
    .clk_invert = 0x03,

    .reset_en_cmd = 0x66,
    .reset_cmd = 0x99,
    .c_rexit = 0xff,
    .wr_enable_write_reg_len = 0x00,

    /*id*/
    .jedec_id_cmd = 0x9f,
    .jedec_id_cmd_dmy_clk = 0,
    .enter_32bits_addr_cmd = 0xb7,
    .exit_32bits_addr_cmd = 0xe9,
    .sector_size = 4,
    .page_size = 256,

    /*read*/
    .fast_read_cmd = 0x0b,
    .fr_dmy_clk = 8 / 8,
    .qpi_fast_read_cmd = 0x0b,
    .qpi_fr_dmy_clk = 8 / 8,
    .fast_read_do_cmd = 0x3b,
    .fr_do_dmy_clk = 8 / 8,
    .fast_read_dio_cmd = 0xbb,
    .fr_dio_dmy_clk = 0,
    .fast_read_qo_cmd = 0x6b,
    .fr_qo_dmy_clk = 8 / 8,

    .qpi_fast_read_qio_cmd = 0xeb,
    .qpi_fr_qio_dmy_clk = 16 / 8,
    .qpi_page_program_cmd = 0x02,
    .write_vreg_enable_cmd = 0x50,

    /* qpi mode */
    .enter_qpi = 0x38,
    .exit_qpi = 0xff,

    /*AC*/
    .time_e_sector = 300,
    .time_e_32k = 1200,
    .time_e_64k = 1200,
    .time_page_pgm = 5,
    .time_ce = 33 * 1000,
    .pd_delay = 20,
    .qe_data = 0,
};

static qc7xx_efuse_device_info_type device_info;
#endif

#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
uint32_t qc7xx_flash2_get_jedec_id(void)
{
    uint32_t jid = 0;

    jid = ((g_jedec_id2 & 0xff) << 16) + (g_jedec_id2 & 0xff00) + ((g_jedec_id2 & 0xff0000) >> 16);
    return jid;
}
#endif

/**
 * @brief get flash size from flash jedec id
 *
 * @return QC7XX_Err_Type
 */
static uint32_t ATTR_TCM_SECTION flash_get_size_from_jedecid(uint32_t jedec_id)
{
    uint8_t flash_size_level = 0;
    uint32_t flash_size = 0;
    uint32_t jid = 0;

    jid = ((jedec_id & 0xff) << 16) + (jedec_id & 0xff00) + ((jedec_id & 0xff0000) >> 16);

    if (jid == 0) {
        return 0;
    }

    flash_size_level = (jid & 0x1f);
    flash_size_level -= 0x13;
    flash_size = (1 << flash_size_level) * 512 * 1024;

    return flash_size;
}

#if defined(QCC74X) || defined(QCC75X)
static int flash_get_clock_delay(spi_flash_cfg_type *cfg)
{
    uint32_t reg_base = 0;
    uint32_t regval = 0;

    reg_base = QC7XX_SF_CTRL_BASE;

    regval = getreg32(reg_base + SF_CTRL_0_OFFSET);
    /* bit0-3 for clk delay */
    if (regval & SF_CTRL_SF_IF_READ_DLY_EN) {
        cfg->clk_delay = ((regval & SF_CTRL_SF_IF_READ_DLY_N_MASK) >> SF_CTRL_SF_IF_READ_DLY_N_SHIFT) + 1;
    } else {
        cfg->clk_delay = 0;
    }
    cfg->clk_invert = 0;
    /* bit0 for clk invert */
    cfg->clk_invert |= (((regval & SF_CTRL_SF_CLK_OUT_INV_SEL) ? 1 : 0) << 0);
    /* bit1 for rx clk invert */
    cfg->clk_invert |= (((regval & SF_CTRL_SF_CLK_SF_RX_INV_SEL) ? 1 : 0) << 1);

    regval = getreg32(reg_base + SF_CTRL_SF_IF_IO_DLY_1_OFFSET);
    /* bit4-6 for do delay */
    cfg->clk_delay |= (((regval & SF_CTRL_SF_IO_0_DO_DLY_SEL_MASK) >> SF_CTRL_SF_IO_0_DO_DLY_SEL_SHIFT) << 4);
    /* bit2-4 for di delay */
    cfg->clk_invert |= (((regval & SF_CTRL_SF_IO_0_DI_DLY_SEL_MASK) >> SF_CTRL_SF_IO_0_DI_DLY_SEL_SHIFT) << 2);
    /* bit5-7 for oe delay */
    cfg->clk_invert |= (((regval & SF_CTRL_SF_IO_0_OE_DLY_SEL_MASK) >> SF_CTRL_SF_IO_0_OE_DLY_SEL_SHIFT) << 5);

    return 0;
}
#endif

static void ATTR_TCM_SECTION flash_set_qspi_enable(spi_flash_cfg_type *p_flash_cfg)
{
    if ((p_flash_cfg->io_mode & 0x0f) == SF_CTRL_QO_MODE || (p_flash_cfg->io_mode & 0x0f) == SF_CTRL_QIO_MODE) {
        qc7xx_sflash_qspi_enable(p_flash_cfg);
    }
}

static void ATTR_TCM_SECTION flash_set_l1c_wrap(spi_flash_cfg_type *p_flash_cfg)
{
    if ((p_flash_cfg->io_mode & 0x1f) == SF_CTRL_QIO_MODE) {
        qc7xx_sflash_set_burst_wrap(p_flash_cfg);
    } else {
        qc7xx_sflash_disable_burst_wrap(p_flash_cfg);
    }
}

#if defined(QCC74X) || defined(QCC75X)
static void ATTR_TCM_SECTION qc7xx_flash_set_cmds(spi_flash_cfg_type *p_flash_cfg)
{
    struct sf_ctrl_cmds_cfg cmds_cfg;

    cmds_cfg.ack_latency = 1;
    cmds_cfg.cmds_core_en = 1;
    cmds_cfg.cmds_en = 1;
    cmds_cfg.cmds_wrap_mode = 1;
    cmds_cfg.cmds_wrap_len = 9;

    if ((p_flash_cfg->io_mode & 0x1f) == SF_CTRL_QIO_MODE) {
        cmds_cfg.cmds_wrap_mode = 2;
        cmds_cfg.cmds_wrap_len = 2;
    }
    qc7xx_sf_ctrl_cmds_set(&cmds_cfg, 0);
}
#endif

/**
 * @brief flash_config_init
 *
 * @return int
 */
static int ATTR_TCM_SECTION flash_config_init(spi_flash_cfg_type *p_flash_cfg, uint8_t *jedec_id)
{
    int ret = -1;
    uint8_t is_aes_enable = 0;
    uint32_t jid = 0;
    uint32_t offset = 0;
    uintptr_t flag;

    flag = qc7xx_irq_save();
    qc7xx_xip_sflash_opt_enter(&is_aes_enable);
    qc7xx_xip_sflash_state_save(p_flash_cfg, &offset, 0, 0);
    qc7xx_sflash_get_jedecid(p_flash_cfg, (uint8_t *)&jid);
    arch_memcpy(jedec_id, (uint8_t *)&jid, 3);
    jid &= 0xFFFFFF;
    g_jedec_id = jid;
    ret = qc7xx_sf_cfg_get_flash_cfg_need_lock_ext(jid, p_flash_cfg, 0, 0);
    if (ret == 0) {
        p_flash_cfg->mid = (jid & 0xff);
    }

    /* Winbond W25Q32JW-IQ not support continue read */
    if (jid == 0x1660EF) {
        p_flash_cfg->io_mode = SF_CTRL_QIO_MODE;
        p_flash_cfg->c_read_support = 0x00; 
    }

#ifdef CONFIG_FLASH_2LINE
#if (CONFIG_FLASH_2LINE != 0x11) && (CONFIG_FLASH_2LINE != 0x13)
#error flash 2 line only supports 0x11 or 0x13
#endif
    p_flash_cfg->io_mode = CONFIG_FLASH_2LINE;
    p_flash_cfg->c_read_support = 0x00;
#endif
    /* Set flash controler from p_flash_cfg */
#if defined(QCC74X) || defined(QCC75X)
    qc7xx_flash_set_cmds(p_flash_cfg);
#endif
    flash_set_qspi_enable(p_flash_cfg);
    flash_set_l1c_wrap(p_flash_cfg);
    qc7xx_xip_sflash_state_restore(p_flash_cfg, offset, 0, 0);
    qc7xx_xip_sflash_opt_exit(is_aes_enable);
    qc7xx_irq_restore(flag);

    return ret;
}

#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
/**
 * @brief flash2 init
 *
 * @return int
 */
static int ATTR_TCM_SECTION flash2_init(void)
{
    int stat = -1;
    uint8_t flash2_enable = 0;
    uint32_t ret = 0;
    uint32_t jid = 0;

    struct sf_ctrl_bank2_cfg sf_bank2_cfg;
    struct sf_ctrl_cmds_cfg cmds_cfg;

    sf_bank2_cfg.sbus2_select = 1;
    sf_bank2_cfg.bank2_rx_clk_invert_src = 0;
    sf_bank2_cfg.bank2_rx_clk_invert_sel = 0;
    sf_bank2_cfg.bank2_delay_src = 0;
    sf_bank2_cfg.bank2_clk_delay = 1;
    sf_bank2_cfg.do_delay = 0;
    sf_bank2_cfg.di_delay = 0;
    sf_bank2_cfg.oe_delay = 0;
    sf_bank2_cfg.remap = SF_CTRL_REMAP_4MB;
    sf_bank2_cfg.remap_lock = 1;

    cmds_cfg.ack_latency = 1;
    cmds_cfg.cmds_core_en = 1;
    cmds_cfg.cmds_en = 1;
    cmds_cfg.cmds_wrap_mode = 1;
    cmds_cfg.cmds_wrap_len = SF_CTRL_WRAP_LEN_4096;

    if (device_info.flash_info == 3) {
        /* memoryInfo==3, embedded 4MB+2MB flash */
        flash2_enable = 1;
    }

    if (flash2_enable > 0) {
        qc7xx_sf_cfg_sbus2_flash_init(SF_IO_EMB_SWAP_IO3IO0_AND_SF2, &sf_bank2_cfg);
        qc7xx_sf_ctrl_sbus2_replace(SF_CTRL_PAD2);
        ret = qc7xx_sf_cfg_flash_identify_ext(0, SF_IO_EMB_SWAP_IO3IO0_AND_SF2, 0, &g_flash2_cfg, 0, SF_CTRL_FLASH_BANK1);
        if ((ret & QC7XX_FLASH_ID_VALID_FLAG) == 0) {
            return -1;
        }
        g_flash2_cfg.io_mode = 0x11;
        g_flash2_cfg.c_read_support = 0;
        g_flash2_cfg.c_read_mode = 0xff;
        qc7xx_sflash_get_jedecid(&g_flash2_cfg, (uint8_t *)&jid);
        jid &= 0xFFFFFF;
        g_jedec_id2 = jid;

        qc7xx_sf_ctrl_cmds_set(&cmds_cfg, SF_CTRL_FLASH_BANK1);
        stat = qc7xx_sflash_xip_read_enable(&g_flash2_cfg, (g_flash2_cfg.io_mode & 0xf), 0, SF_CTRL_FLASH_BANK1);
        if (0 != stat) {
            return -1;
        }
        qc7xx_sf_ctrl_sbus2_revoke_replace();

#if defined(QCC74X) || defined(QCC75X)
        flash2_size = flash_get_size_from_jedecid(g_jedec_id2);
#endif
    }

    return 0;
}
#endif

/**
 * @brief multi flash adapter
 *
 * @return int
 */
int ATTR_TCM_SECTION qc7xx_flash_init(void)
{
    int ret = -1;
    uint32_t jedec_id = 0;

#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
    qc7xx_efuse_get_device_info(&device_info);
#endif

    jedec_id = GLB_Get_Flash_Id_Value();
    if (jedec_id != 0) {
        ret = qc7xx_sf_cfg_get_flash_cfg_need_lock_ext(jedec_id, &g_flash_cfg, 0, 0);
        if (ret == 0) {
            g_jedec_id = jedec_id;
            g_flash_cfg.mid = (jedec_id & 0xff);
            flash_get_clock_delay(&g_flash_cfg);
#if defined(QCC74X) || defined(QCC75X)
            flash1_size = flash_get_size_from_jedecid(g_jedec_id);
#endif
#if defined(QCC74X) || defined(QCC75X)
            flash2_size = 0;
#endif
            return 0;
        }
    }

    ret = flash_config_init(&g_flash_cfg, (uint8_t *)&jedec_id);

    flash_get_clock_delay(&g_flash_cfg);
    GLB_Set_Flash_Id_Value(g_jedec_id);

#if defined(QCC74X) || defined(QCC75X)
    flash1_size = flash_get_size_from_jedecid(g_jedec_id);
#endif

#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
    flash2_init();
#else
#if defined(QCC74X) || defined(QCC75X)
    flash2_size = 0;
#endif
#endif

    return ret;
}

uint32_t qc7xx_flash_get_jedec_id(void)
{
    return g_jedec_id;
}

uint32_t qc7xx_flash_get_size(void)
{
    return flash_get_size_from_jedecid(g_jedec_id);
}

#if defined(QCC74X) || defined(QCC75X)
#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
uint32_t qc7xx_flash2_get_size(void)
{
    return flash_get_size_from_jedecid(g_jedec_id2);
}
#endif
#endif

void ATTR_TCM_SECTION qc7xx_flash_get_cfg(uint8_t **cfg_addr, uint32_t *len)
{
    *cfg_addr = (uint8_t *)&g_flash_cfg;
    *len = sizeof(spi_flash_cfg_type);
}

void ATTR_TCM_SECTION qc7xx_flash_set_iomode(uint8_t iomode)
{
    uintptr_t flag = 0;
    uint8_t is_aes_enable = 0;
    uint32_t offset = 0;

    flag = qc7xx_irq_save();
    qc7xx_xip_sflash_opt_enter(&is_aes_enable);
    qc7xx_xip_sflash_state_save(&g_flash_cfg, &offset, 0, 0);

    g_flash_cfg.io_mode &= ~0x1f;
    if (iomode & 4) {
        g_flash_cfg.io_mode |= iomode;
    } else {
        g_flash_cfg.io_mode |= 0x10;
        g_flash_cfg.io_mode |= iomode;
    }

#if defined(QCC74X) || defined(QCC75X)
    qc7xx_flash_set_cmds(&g_flash_cfg);
#endif
    flash_set_qspi_enable(&g_flash_cfg);
    flash_set_l1c_wrap(&g_flash_cfg);
    qc7xx_xip_sflash_state_restore(&g_flash_cfg, offset, 0, 0);
    qc7xx_xip_sflash_opt_exit(is_aes_enable);
    qc7xx_irq_restore(flag);
}

ATTR_TCM_SECTION uint32_t qc7xx_flash_get_image_offset(void)
{
    return qc7xx_sf_ctrl_get_flash_image_offset(0, 0);
}

/**
 * @brief erase flash via sbus
 *
 * @param flash absolute startaddr
 * @param flash absolute endaddr
 * @return int
 */
int ATTR_TCM_SECTION qc7xx_flash_erase(uint32_t startaddr, uint32_t len)
{
    int stat = -1;
    uintptr_t flag;

#if defined(QCC74X) || defined(QCC75X)
    if ((startaddr + len) > (flash1_size + flash2_size)) {
        return -ENOMEM;
    } else if ((startaddr + len) <= flash1_size) {
        flag = qc7xx_irq_save();
        stat = qc7xx_xip_sflash_erase_need_lock(&g_flash_cfg, startaddr, len, 0, 0);
        qc7xx_irq_restore(flag);
#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
    } else if (startaddr >= flash1_size) {
        qc7xx_sf_ctrl_sbus2_replace(SF_CTRL_PAD2);
        stat = qc7xx_sflash_erase(&g_flash2_cfg, startaddr, startaddr + len - 1);
        qc7xx_sf_ctrl_sbus2_revoke_replace();
    } else {
        flag = qc7xx_irq_save();
        stat = qc7xx_xip_sflash_erase_need_lock(&g_flash_cfg, startaddr, flash1_size - startaddr, 0, 0);
        qc7xx_irq_restore(flag);
        if (stat != 0) {
            return stat;
        }
        qc7xx_sf_ctrl_sbus2_replace(SF_CTRL_PAD2);
        stat = qc7xx_sflash_erase(&g_flash2_cfg, flash1_size, startaddr + len - flash1_size - 1);
        qc7xx_sf_ctrl_sbus2_revoke_replace();
    }
#else
    }
#endif
#else
    if (startaddr >= QC7XX_FLASH_XIP_END - QC7XX_FLASH_XIP_BASE) {
        return -ENOMEM;
    }

    flag = qc7xx_irq_save();
    stat = qc7xx_xip_sflash_erase_need_lock(&g_flash_cfg, startaddr, len, 0, 0);
    qc7xx_irq_restore(flag);
#endif

    return stat;
}

/**
 * @brief write flash data via sbus
 *
 * @param flash absolute addr
 * @param data
 * @param len
 * @return int
 */
int ATTR_TCM_SECTION qc7xx_flash_write(uint32_t addr, uint8_t *data, uint32_t len)
{
    int stat = -1;
    uintptr_t flag;

#if defined(QCC74X) || defined(QCC75X)
    if ((addr + len) > (flash1_size + flash2_size)) {
        return -ENOMEM;
    } else if ((addr + len) <= flash1_size) {
        flag = qc7xx_irq_save();
        stat = qc7xx_xip_sflash_write_need_lock(&g_flash_cfg, addr, data, len, 0, 0);
        qc7xx_irq_restore(flag);
#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
    } else if (addr >= flash1_size) {
        qc7xx_sf_ctrl_sbus2_replace(SF_CTRL_PAD2);
        stat = qc7xx_sflash_program(&g_flash2_cfg, SF_CTRL_DO_MODE, addr, data, len);
        qc7xx_sf_ctrl_sbus2_revoke_replace();
    } else {
        flag = qc7xx_irq_save();
        stat = qc7xx_xip_sflash_write_need_lock(&g_flash_cfg, addr, data, flash1_size - addr, 0, 0);
        qc7xx_irq_restore(flag);
        if (stat != 0) {
            return stat;
        }
        qc7xx_sf_ctrl_sbus2_replace(SF_CTRL_PAD2);
        stat = qc7xx_sflash_program(&g_flash2_cfg, SF_CTRL_DO_MODE, flash1_size, data + (flash1_size - addr), addr + len - flash1_size);
        qc7xx_sf_ctrl_sbus2_revoke_replace();
    }
#else
    }
#endif
#else
    if (addr >= QC7XX_FLASH_XIP_END - QC7XX_FLASH_XIP_BASE) {
        return -ENOMEM;
    }

    flag = qc7xx_irq_save();
    stat = qc7xx_xip_sflash_write_need_lock(&g_flash_cfg, addr, data, len, 0, 0);
    qc7xx_irq_restore(flag);
#endif

    return stat;
}

/**
 * @brief read flash data via sbus
 *
 * @param flash absolute addr
 * @param data
 * @param len
 * @return int
 */
int ATTR_TCM_SECTION qc7xx_flash_read(uint32_t addr, uint8_t *data, uint32_t len)
{
    int stat = -1;
    uintptr_t flag;

#if defined(QCC74X) || defined(QCC75X)
    if ((addr + len) > (flash1_size + flash2_size)) {
        return -ENOMEM;
    } else if ((addr + len) <= flash1_size) {
        flag = qc7xx_irq_save();
        stat = qc7xx_xip_sflash_read_need_lock(&g_flash_cfg, addr, data, len, 0, 0);
        qc7xx_irq_restore(flag);
#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
    } else if (addr >= flash1_size) {
        qc7xx_sf_ctrl_sbus2_replace(SF_CTRL_PAD2);
        stat = qc7xx_sflash_read(&g_flash2_cfg, SF_CTRL_DO_MODE, 0, addr, data, len);
        qc7xx_sf_ctrl_sbus2_revoke_replace();
    } else {
        flag = qc7xx_irq_save();
        stat = qc7xx_xip_sflash_read_need_lock(&g_flash_cfg, addr, data, flash1_size - addr, 0, 0);
        qc7xx_irq_restore(flag);
        if (stat != 0) {
            return stat;
        }
        qc7xx_sf_ctrl_sbus2_replace(SF_CTRL_PAD2);
        stat = qc7xx_sflash_read(&g_flash2_cfg, SF_CTRL_DO_MODE, 0, flash1_size, data + (flash1_size - addr), addr + len - flash1_size);
        qc7xx_sf_ctrl_sbus2_revoke_replace();
    }
#else
    }
#endif
#else
    if (addr >= QC7XX_FLASH_XIP_END - QC7XX_FLASH_XIP_BASE) {
        return -ENOMEM;
    }

    flag = qc7xx_irq_save();
    stat = qc7xx_xip_sflash_read_need_lock(&g_flash_cfg, addr, data, len, 0, 0);
    qc7xx_irq_restore(flag);
#endif

    return stat;
}

/**
 * @brief read flash unique id
 *
 * @param data
 * @param id_len
 * @return int
 */
int ATTR_TCM_SECTION qc7xx_flash_get_unique_id(uint8_t *data, uint8_t id_len)
{
    int stat = -1;
    uintptr_t flag;

    flag = qc7xx_irq_save();
    stat = qc7xx_xip_sflash_get_uniqueid_need_lock(&g_flash_cfg, data, id_len, 0, 0);
    qc7xx_irq_restore(flag);

    return stat;
}

int ATTR_TCM_SECTION qc7xx_flash_set_cache(uint8_t cont_read, uint8_t cache_enable, uint8_t cache_way_disable, uint32_t flash_offset)
{
    uint8_t is_aes_enable = 0;
    uint32_t tmp[1];
    int stat;

    qc7xx_sf_ctrl_set_owner(SF_CTRL_OWNER_SAHB);

    qc7xx_xip_sflash_opt_enter(&is_aes_enable);
    /* To make it simple, exit cont read anyway */
    qc7xx_sflash_reset_continue_read(&g_flash_cfg);

    if (g_flash_cfg.c_read_support == 0) {
        cont_read = 0;
    }

    if (cont_read == 1) {
        stat = qc7xx_sflash_read(&g_flash_cfg, g_flash_cfg.io_mode & 0xf, 1, 0x00000000, (uint8_t *)tmp, sizeof(tmp));

        if (0 != stat) {
            qc7xx_xip_sflash_opt_exit(is_aes_enable);
            return -1;
        }
    }

    qc7xx_sf_ctrl_set_flash_image_offset(flash_offset, 0, 0);
    qc7xx_sflash_xip_read_enable(&g_flash_cfg, g_flash_cfg.io_mode & 0xf, cont_read, 0);

    qc7xx_xip_sflash_opt_exit(is_aes_enable);

    return 0;
}

void ATTR_TCM_SECTION qc7xx_flash_aes_init(struct qc7xx_flash_aes_config_s *config)
{
    uint8_t hw_key_enable = 0;

    if (config->key == NULL) {
        hw_key_enable = 1;
    }

    qc7xx_sf_ctrl_aes_set_key_be(config->region, (uint8_t *)config->key, config->keybits);
    qc7xx_sf_ctrl_aes_set_iv_be(config->region, (uint8_t *)config->iv, config->start_addr);
    qc7xx_sf_ctrl_aes_set_region(config->region, config->region_enable, hw_key_enable, config->start_addr, config->end_addr - 1, config->lock_enable);
}

void ATTR_TCM_SECTION qc7xx_flash_aes_enable(void)
{
    qc7xx_sf_ctrl_aes_enable_be();
    qc7xx_sf_ctrl_aes_enable();
}

void ATTR_TCM_SECTION qc7xx_flash_aes_disable(void)
{
    qc7xx_sf_ctrl_aes_disable();
}

void ATTR_TCM_SECTION qc7xx_flash_jump_encrypted_app(uint8_t index, uint32_t flash_addr, uint32_t len)
{
    void (*entry)(void);

    qc7xx_sf_ctrl_aes_set_region_offset(index, flash_addr);

    qc7xx_sf_ctrl_aes_set_region(index, 1 /*enable this region*/, 1 /*hardware key*/,
                                flash_addr,
                                flash_addr + len - 1,
                                0 /*lock*/);

    qc7xx_sf_ctrl_aes_enable_be();
    qc7xx_sf_ctrl_aes_enable();

    qc7xx_flash_set_cache(0, 1, 0, flash_addr);
    qc7xx_l1c_dcache_clean_invalidate_all();
    qc7xx_l1c_icache_invalid_all();

    entry = (void (*)(void))QC7XX_FLASH_XIP_BASE;
    entry();
}