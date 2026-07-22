/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_SF_CTRL_H
#define _QC7XX_SF_CTRL_H

#include "qc7xx_core.h"

/** @addtogroup  SF_CTRL
 *  @{
 */

/** @defgroup  SF_CTRL_Public_Types
 *  @{
 */

#if defined(QCC74X)
#define QC7XX_SF_CTRL_BASE      ((uint32_t)0x2000b000)
#elif defined(QCC75X)
#define QC7XX_SF_CTRL_BASE      ((uint32_t)0x20082000)
#endif

#if defined(QCC74X)
#define QC7XX_FLASH_XIP_BASE    (0xA0000000)
#define QC7XX_FLASH_XIP_END     (0xA0000000 + 64 * 1024 * 1024)
#elif defined(QCC75X)
#define QC7XX_FLASH_XIP_BASE    (0x80000000)
#define QC7XX_FLASH_XIP_END     (0x80000000 + 64 * 1024 * 1024)
#endif

#if defined(QCC74X) || defined(QCC75X)
#ifndef CONFIG_DISABLE_SBUS2_ENABLE_SUPPORT
#define QC7XX_SF_CTRL_SBUS2_ENABLE
#endif
#endif
#if defined(QCC74X) || defined(QCC75X)
#define QC7XX_SF_CTRL_32BITS_ADDR_ENABLE
#define QC7XX_SF_CTRL_AES_XTS_ENABLE
#endif

/**
 *  @brief Serial flash pad type definition
 */
#define SF_CTRL_PAD1                                    0    /*!< SF Ctrl pad 1 */
#define SF_CTRL_PAD2                                    1    /*!< SF Ctrl pad 2 */
#define SF_CTRL_PAD3                                    2    /*!< SF Ctrl pad 3 */

/**
 *  @brief Serial flash config pin select type definition
 */
#if defined(QCC74X) || defined(QCC75X)
#define SF_IO_EMB_SWAP_IO3IO0                           0x0  /*!< SF select embedded flash swap io3 with io0 */
#define SF_IO_EMB_SWAP_IO3IO0_IO2CS                     0x1  /*!< SF select embedded flash swap io3 with io0 and io2 with cs */
#define SF_IO_EMB_SWAP_NONE                             0x2  /*!< SF select embedded flash no swap */
#define SF_IO_EMB_SWAP_IO2CS                            0x3  /*!< SF select embedded flash swap io2 with cs */
#define SF_IO_EXT_SF2_SWAP_IO3IO0                       0x4  /*!< SF select external flash SF2 use gpio4-9 and SF2 swap io3 with io0 */
#define SF_IO_EXT_SF3                                   0x8  /*!< SF select external flash SF3 use gpio10-15 */
#define SF_IO_EMB_SWAP_IO3IO0_AND_SF2_SWAP_IO3IO0       0x14 /*!< SF select embedded flash swap io3 with io0 and SF2 swap io3 with io0*/
#define SF_IO_EMB_SWAP_IO3IO0_IO2CS_AND_SF2_SWAP_IO3IO0 0x15 /*!< SF select embedded flash swap io3 with io0、io2 with cs and SF2 swap io3 with io0 */
#define SF_IO_EMB_SWAP_NONE_AND_SF2_SWAP_IO3IO0         0x16 /*!< SF select embedded flash no swap and SF2 swap io3 with io0 */
#define SF_IO_EMB_SWAP_IO2CS_AND_SF2_SWAP_IO3IO0        0x17 /*!< SF select embedded flash swap io2 with cs, and SF2 swap io3 with io0 */
#define SF_IO_EXT_SF2                                   0x24 /*!< SF select external flash SF2 use gpio4-9 */
#define SF_IO_EMB_SWAP_IO3IO0_AND_SF2                   0x34 /*!< SF select embedded flash swap io3 with io0 and SF2 use gpio4-9 */
#define SF_IO_EMB_SWAP_IO3IO0_IO2CS_AND_SF2             0x35 /*!< SF select embedded flash swap io3 with io0、io2 with cs and SF2 use gpio4-9 */
#define SF_IO_EMB_SWAP_NONE_AND_SF2                     0x36 /*!< SF select embedded flash no swap and SF2 use gpio4-9 */
#define SF_IO_EMB_SWAP_IO2CS_AND_SF2                    0x37 /*!< SF select embedded flash swap io2 with cs and SF2 use gpio4-9 */
#endif

/**
 *  @brief Serial flash select bank control type definition
 */
#define SF_CTRL_FLASH_BANK0                             0    /*!< SF Ctrl select flash bank0 */
#define SF_CTRL_FLASH_BANK1                             1    /*!< SF Ctrl select flash bank1 */

/**
 *  @brief Serial flash controller wrap mode type definition
 */
#define SF_CTRL_WRAP_MODE_0                             0    /*!< Cmds bypass wrap commands to macro, original mode */
#define SF_CTRL_WRAP_MODE_1                             1    /*!< Cmds handle wrap commands, original mode */
#define SF_CTRL_WRAP_MODE_2                             2    /*!< Cmds bypass wrap commands to macro, cmds force wrap16*4 splitted into two wrap8*4 */
#define SF_CTRL_WRAP_MODE_3                             3    /*!< Cmds handle wrap commands, cmds force wrap16*4 splitted into two wrap8*4 */
/**
 *  @brief Serial flash controller wrap mode len type definition
 */
#define SF_CTRL_WRAP_LEN_8                              0    /*!< SF Ctrl wrap length: 8 */
#define SF_CTRL_WRAP_LEN_16                             1    /*!< SF Ctrl wrap length: 16 */
#define SF_CTRL_WRAP_LEN_32                             2    /*!< SF Ctrl wrap length: 32 */
#define SF_CTRL_WRAP_LEN_64                             3    /*!< SF Ctrl wrap length: 64 */
#define SF_CTRL_WRAP_LEN_128                            4    /*!< SF Ctrl wrap length: 128 */
#define SF_CTRL_WRAP_LEN_256                            5    /*!< SF Ctrl wrap length: 256 */
#define SF_CTRL_WRAP_LEN_512                            6    /*!< SF Ctrl wrap length: 512 */
#define SF_CTRL_WRAP_LEN_1024                           7    /*!< SF Ctrl wrap length: 1024 */
#define SF_CTRL_WRAP_LEN_2048                           8    /*!< SF Ctrl wrap length: 2048 */
#define SF_CTRL_WRAP_LEN_4096                           9    /*!< SF Ctrl wrap length: 4096 */

/**
 *  @brief Serial flash controller memory remap type define
 */
#define SF_CTRL_ORIGINAL_MEMORY_MAP                     0    /*!< Remap none, use two addr map when use dual flash */
#define SF_CTRL_REMAP_16MB                              1    /*!< Remap HADDR>16MB region to psram port HADDR[24] -> HADDR[28] */
#define SF_CTRL_REMAP_8MB                               2    /*!< Remap HADDR>8MB region to psram port HADDR[23] -> HADDR[28] */
#define SF_CTRL_REMAP_4MB                               3    /*!< Remap HADDR>4MB region to psram port HADDR[22] -> HADDR[28] */

/**
 *  @brief Serial flash controller select clock type definition
 */
#define SF_CTRL_OWNER_SAHB                              0    /*!< System AHB bus control serial flash controller */
#define SF_CTRL_OWNER_IAHB                              1    /*!< I-Code AHB bus control serial flash controller */

/**
 *  @brief Serial flash controller select clock type definition
 */
#define SF_CTRL_SAHB_CLOCK                              0    /*!< Serial flash controller select default sahb clock */
#define SF_CTRL_FLASH_CLOCK                             1    /*!< Serial flash controller select flash clock */

/**
 *  @brief Read and write type definition
 */
#define SF_CTRL_READ                                    0    /*!< Serial flash read command flag */
#define SF_CTRL_WRITE                                   1    /*!< Serial flash write command flag */

/**
 *  @brief Serial flash interface IO type definition
 */
#define SF_CTRL_NIO_MODE                                0    /*!< Normal IO mode define */
#define SF_CTRL_DO_MODE                                 1    /*!< Dual Output mode define */
#define SF_CTRL_QO_MODE                                 2    /*!< Quad Output mode define */
#define SF_CTRL_DIO_MODE                                3    /*!< Dual IO mode define */
#define SF_CTRL_QIO_MODE                                4    /*!< Quad IO mode define */

/**
 *  @brief Serial flash controller interface mode type definition
 */
#define SF_CTRL_SPI_MODE                                0    /*!< SPI mode define */
#define SF_CTRL_QPI_MODE                                1    /*!< QPI mode define */

/**
 *  @brief Serial flash controller command mode type definition
 */
#define SF_CTRL_CMD_1_LINE                              0    /*!< Command in one line mode */
#define SF_CTRL_CMD_4_LINES                             1   /*!< Command in four lines mode */

/**
 *  @brief Serial flash controller address mode type definition
 */
#define SF_CTRL_ADDR_1_LINE                             0    /*!< Address in one line mode */
#define SF_CTRL_ADDR_2_LINES                            1    /*!< Address in two lines mode */
#define SF_CTRL_ADDR_4_LINES                            2    /*!< Address in four lines mode */

/**
 *  @brief Serial flash controller dummy mode type definition
 */
#define SF_CTRL_DUMMY_1_LINE                            0    /*!< Dummy in one line mode */
#define SF_CTRL_DUMMY_2_LINES                           1    /*!< Dummy in two lines mode */
#define SF_CTRL_DUMMY_4_LINES                           2    /*!< Dummy in four lines mode */

/**
 *  @brief Serial flash controller data mode type definition
 */
#define SF_CTRL_DATA_1_LINE                             0    /*!< Data in one line mode */
#define SF_CTRL_DATA_2_LINES                            1    /*!< Data in two lines mode */
#define SF_CTRL_DATA_4_LINES                            2    /*!< Data in four lines mode */

/**
 *  @brief Serial flash controller AES mode type definition
 */
#define SF_CTRL_AES_CTR_MODE                            0    /*!< Serial flash AES CTR mode */
#define SF_CTRL_AES_XTS_MODE                            1    /*!< Serial flash AES XTS mode */

/**
 *  @brief Serial flash controller AES key len type definition
 */
#define SF_CTRL_AES_128BITS                             0    /*!< Serial flash AES key 128 bits length */
#define SF_CTRL_AES_256BITS                             1    /*!< Serial flash AES key 256 bits length */
#define SF_CTRL_AES_192BITS                             2    /*!< Serial flash AES key 192 bits length */
#define SF_CTRL_AES_128BITS_DOUBLE_KEY                  3    /*!< Serial flash AES key 128 bits length double key */

/**
 *  @brief Serial flash controller configuration structure type definition
 */
struct sf_ctrl_cfg_type {
    uint8_t owner;                       /*!< Sflash interface bus owner */
#ifdef QC7XX_SF_CTRL_HAS_SAHB_CLOCK
    uint8_t sahb_clock;                  /*!< Sflash clock sahb sram select */
#endif
#ifdef QC7XX_SF_CTRL_HAS_AHB2SIF_MODE
    uint8_t ahb2sif_mode;                /*!< Sflash ahb2sif mode */
#endif
#ifdef QC7XX_SF_CTRL_32BITS_ADDR_ENABLE
    uint8_t en32b_addr;                  /*!< Sflash enable 32-bits address */
#endif
    uint8_t clk_delay;                   /*!< Clock count for read due to pad delay */
    uint8_t clk_invert;                  /*!< Clock invert */
    uint8_t rx_clk_invert;               /*!< RX clock invert */
    uint8_t do_delay;                    /*!< Data out delay */
    uint8_t di_delay;                    /*!< Data in delay */
    uint8_t oe_delay;                    /*!< Output enable delay */
};

/**
 *  @brief SF Ctrl bank2 controller configuration structure type definition
 */
struct sf_ctrl_bank2_cfg {
    uint8_t sbus2_select;                /*!< Select sbus2 as 2nd flash controller */
    uint8_t bank2_rx_clk_invert_src;     /*!< Select bank2 rx clock invert source */
    uint8_t bank2_rx_clk_invert_sel;     /*!< Select inveted bank2 rx clock */
    uint8_t bank2_delay_src;             /*!< Select bank2 read delay source */
    uint8_t bank2_clk_delay;             /*!< Bank2 read delay cycle = n + 1 */
    uint8_t do_delay;                    /*!< Data out delay */
    uint8_t di_delay;                    /*!< Data in delay */
    uint8_t oe_delay;                    /*!< Output enable delay */
    uint8_t remap;                       /*!< Select dual flash memory remap set */
    uint8_t remap_lock;                  /*!< Select memory remap lock */
};

#ifdef QC7XX_SF_CTRL_PSRAM_ENABLE
/**
 *  @brief SF Ctrl psram controller configuration structure type definition
 */
struct sf_ctrl_psram_cfg {
    uint8_t owner;                   /*!< Psram interface bus owner */
    uint8_t pad_sel;                 /*!< SF Ctrl pad select */
    uint8_t bank_sel;                /*!< SF Ctrl bank select */
    uint8_t psram_rx_clk_invert_src; /*!< Select psram rx clock invert source */
    uint8_t psram_rx_clk_invert_sel; /*!< Select inveted psram rx clock */
    uint8_t psram_delay_src;         /*!< Select psram read delay source */
    uint8_t psram_clk_delay;         /*!< Psram read delay cycle = n + 1 */
} ;
#endif

/**
 *  @brief SF Ctrl cmds configuration structure type definition
 */
struct sf_ctrl_cmds_cfg {
#if defined(QCC74X) || defined(QCC75X)
    uint8_t ack_latency;                 /*!< SF Ctrl ack latency cycles */
    uint8_t cmds_core_en;                /*!< SF Ctrl cmds core enable */
#endif
    uint8_t cmds_en;                     /*!< SF Ctrl cmds enable */
    uint8_t cmds_wrap_mode;              /*!< SF Ctrl cmds wrap mode */
    uint8_t cmds_wrap_len;               /*!< SF Ctrl cmds wrap length */
};

/**
 *  @brief Serial flash command configuration structure type definition
 */
struct sf_ctrl_cmd_cfg_type {
    uint8_t rw_flag;                     /*!< Read write flag */
    uint8_t cmd_mode;                    /*!< Command mode */
    uint8_t addr_mode;                   /*!< Address mode */
    uint8_t addr_size;                   /*!< Address size */
    uint8_t dummy_clks;                  /*!< Dummy clocks */
    uint8_t dummy_mode;                  /*!< Dummy mode */
    uint8_t data_mode;                   /*!< Data mode */
    uint8_t rsv[1];                      /*!< Reserved */
    uint32_t nb_data;                    /*!< Transfer number of bytes */
    uint32_t cmd_buf[2];                 /*!< Command buffer */
};

struct qc7xx_sf_ctrl_decrypt_type {
    uint8_t mode;               /*!< Serial flash AES CTR/XTS mode */
    uint8_t type;               /*!< Serial flash AES key bit length */
    uint8_t aes_region;         /*!< Serial flash AES region 0/1/2 */
    uint8_t lock;               /*!< Serial flash AES region config lock */
    uint32_t addr;              /*!< Serial flash AES decrypt start address */
    uint32_t len;               /*!< Serial flash AES decrypt length */
    uint8_t *key;               /*!< Serial flash AES key */
    uint8_t *iv;                /*!< Serial flash AES iv */
};

struct qc7xx_sf_ctrl_io_cs_clk_delay_cfg {
    uint8_t do_delay;               /*!< DO delay select */
    uint8_t di_delay;               /*!< DI delay select */
    uint8_t oe_delay;               /*!< OE delay select */
    uint8_t cs_delay;               /*!< CS delay select */
    uint8_t cs_clk_delay;           /*!< CS clock delay select */
    uint8_t rsv[3];                 /*!< Reserved */
};

/*@} end of group SF_CTRL_Public_Types */

/** @defgroup  SF_CTRL_Public_Macros
 *  @{
 */
#define SF_CTRL_BUSY_STATE_TIMEOUT  (5 * 320 * 1000)
#define SF_CTRL_NO_ADDRESS          0xFFFFFFFF
#define NOR_FLASH_CTRL_BUF_SIZE     256
#define NAND_FLASH_CTRL_BUF_SIZE    512

#if defined(QCC74X) || defined(QCC75X)
#define IS_SF_CTRL_PIN_SELECT(type) (((type) == SF_IO_EMB_SWAP_IO3IO0) ||                           \
                                     ((type) == SF_IO_EMB_SWAP_IO3IO0_IO2CS) ||                     \
                                     ((type) == SF_IO_EMB_SWAP_NONE) ||                             \
                                     ((type) == SF_IO_EMB_SWAP_IO2CS) ||                            \
                                     ((type) == SF_IO_EXT_SF2_SWAP_IO3IO0) ||                       \
                                     ((type) == SF_IO_EXT_SF3) ||                                   \
                                     ((type) == SF_IO_EMB_SWAP_IO3IO0_AND_SF2_SWAP_IO3IO0) ||       \
                                     ((type) == SF_IO_EMB_SWAP_IO3IO0_IO2CS_AND_SF2_SWAP_IO3IO0) || \
                                     ((type) == SF_IO_EMB_SWAP_NONE_AND_SF2_SWAP_IO3IO0) ||         \
                                     ((type) == SF_IO_EMB_SWAP_IO2CS_AND_SF2_SWAP_IO3IO0) ||        \
                                     ((type) == SF_IO_EXT_SF2) ||                                   \
                                     ((type) == SF_IO_EMB_SWAP_IO3IO0_AND_SF2) ||                   \
                                     ((type) == SF_IO_EMB_SWAP_IO3IO0_IO2CS_AND_SF2) ||             \
                                     ((type) == SF_IO_EMB_SWAP_NONE_AND_SF2) ||                     \
                                     ((type) == SF_IO_EMB_SWAP_IO2CS_AND_SF2))
#endif

/*@} end of group SF_CTRL_Public_Macros */

/** @defgroup  SF_CTRL_Public_Functions
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

void qc7xx_sf_ctrl_enable(const struct sf_ctrl_cfg_type *cfg);
void qc7xx_sf_ctrl_set_io_delay(uint8_t pad, uint8_t do_delay, uint8_t di_delay, uint8_t oe_delay);
void qc7xx_sf_ctrl_get_io_delay(uint8_t pad, uint8_t *do_delay, uint8_t *di_delay, uint8_t *oe_delay);
void qc7xx_sf_ctrl_set_cs_clk_delay(uint8_t pad, uint8_t cs_delay, uint8_t clk_delay);
void qc7xx_sf_ctrl_get_cs_clk_delay(uint8_t pad, uint8_t *cs_delay, uint8_t *clk_delay);
void qc7xx_sf_ctrl_set_flash_io_cs_clk_delay(struct qc7xx_sf_ctrl_io_cs_clk_delay_cfg cfg);
void qc7xx_sf_ctrl_get_flash_io_cs_clk_delay(struct qc7xx_sf_ctrl_io_cs_clk_delay_cfg *cfg);
void qc7xx_sf_ctrl_set_dqs_delay(uint8_t pad, uint8_t dodelay, uint8_t didelay, uint8_t oedelay);

#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
void qc7xx_sf_ctrl_bank2_enable(const struct sf_ctrl_bank2_cfg *bank2cfg);
void qc7xx_sf_ctrl_sbus2_hold_sram(void);
void qc7xx_sf_ctrl_sbus2_release_sram(void);
uint8_t sf_ctrl_is_sbus2_enable(void);
void qc7xx_sf_ctrl_sbus2_replace(uint8_t pad);
void qc7xx_sf_ctrl_sbus2_revoke_replace(void);
void qc7xx_sf_ctrl_sbus2_set_delay(uint8_t clk_delay, uint8_t rx_clk_invert);
void qc7xx_sf_ctrl_remap_set(uint8_t remap, uint8_t lock);
#endif
#ifdef QC7XX_SF_CTRL_32BITS_ADDR_ENABLE
void qc7xx_sf_ctrl_32bits_addr_en(uint8_t en_32bit_saddr);
#endif
#ifdef QC7XX_SF_CTRL_PSRAM_ENABLE
void qc7xx_sf_ctrl_psram_init(struct sf_ctrl_psram_cfg *psram_cfg);
#endif
uint8_t qc7xx_sf_ctrl_get_clock_delay(void);
void qc7xx_sf_ctrl_set_clock_delay(uint8_t delay);
uint8_t qc7xx_sf_ctrl_get_wrap_queue_value(void);
void qc7xx_sf_ctrl_cmds_set(struct sf_ctrl_cmds_cfg *cmds_cfg, uint8_t sel);
void qc7xx_sf_ctrl_select_pad(uint8_t sel);
void qc7xx_sf_ctrl_sbus_select_bank(uint8_t bank);
void qc7xx_sf_ctrl_set_owner(uint8_t owner);
uint8_t qc7xx_sf_ctrl_get_owner();
void qc7xx_sf_ctrl_set_owner_flag(uint8_t owner);
uint8_t qc7xx_sf_ctrl_get_owner_flag(void);
void qc7xx_sf_ctrl_disable(void);
void qc7xx_sf_ctrl_aes_enable_be(void);
void qc7xx_sf_ctrl_aes_enable_le(void);
void qc7xx_sf_ctrl_aes_set_region(uint8_t region, uint8_t enable, uint8_t hwkey,
                                 uint32_t start_addr, uint32_t end_addr, uint8_t locked);
void qc7xx_sf_ctrl_aes_set_key(uint8_t region, uint8_t *key, uint8_t key_type);
void qc7xx_sf_ctrl_aes_set_key_be(uint8_t region, uint8_t *key, uint8_t key_type);
void qc7xx_sf_ctrl_aes_set_iv(uint8_t region, uint8_t *iv, uint32_t addr_offset);
void qc7xx_sf_ctrl_aes_set_iv_be(uint8_t region, uint8_t *iv, uint32_t addr_offset);
void qc7xx_sf_ctrl_aes_set_region_offset(uint8_t region,uint32_t addr_offset);
#ifdef QC7XX_SF_CTRL_AES_XTS_ENABLE
void qc7xx_sf_ctrl_aes_xts_set_key(uint8_t region, uint8_t *key, uint8_t key_type);
void qc7xx_sf_ctrl_aes_xts_set_key_be(uint8_t region, uint8_t *key, uint8_t key_type);
void qc7xx_sf_ctrl_aes_xts_set_iv(uint8_t region, uint8_t *iv, uint32_t addr_offset);
void qc7xx_sf_ctrl_aes_xts_set_iv_be(uint8_t region, uint8_t *iv, uint32_t addr_offset);
#endif
void qc7xx_sf_ctrl_aes_set_mode(uint8_t mode);
void qc7xx_sf_ctrl_aes_enable(void);
void qc7xx_sf_ctrl_aes_disable(void);
uint8_t qc7xx_sf_ctrl_is_aes_enable(void);
void qc7xx_sf_ctrl_set_flash_image_offset(uint32_t addr_offset, uint8_t group, uint8_t bank);
uint32_t qc7xx_sf_ctrl_get_flash_image_offset(uint8_t group, uint8_t bank);
void qc7xx_sf_ctrl_lock_flash_image_offset(uint8_t lock);
void qc7xx_sf_ctrl_select_clock(uint8_t sahb_sram_sel);
void qc7xx_sf_ctrl_sendcmd(struct sf_ctrl_cmd_cfg_type *cfg);
void qc7xx_sf_ctrl_disable_wrap_access(uint8_t disable);
void qc7xx_sf_ctrl_xip_set(struct sf_ctrl_cmd_cfg_type *cfg, uint8_t cmd_valid);
#ifdef QC7XX_SF_CTRL_SBUS2_ENABLE
void qc7xx_sf_ctrl_xip2_set(struct sf_ctrl_cmd_cfg_type *cfg, uint8_t cmd_valid);
#endif
#ifdef QC7XX_SF_CTRL_PSRAM_ENABLE
void qc7xx_sf_ctrl_psram_write_set(struct sf_ctrl_cmd_cfg_type *cfg, uint8_t cmd_valid);
void qc7xx_sf_ctrl_psram_read_set(struct sf_ctrl_cmd_cfg_type *cfg, uint8_t cmd_valid);
#endif
uint8_t qc7xx_sf_ctrl_get_busy_state(void);
void qc7xx_sf_ctrl_aes_get_iv_be(uint8_t region, uint8_t *iv);
void qc7xx_sf_ctrl_aes_get_iv_le(uint8_t region, uint8_t *iv);
int32_t qc7xx_sf_ctrl_aes_set_decrypt_region_be(struct qc7xx_sf_ctrl_decrypt_type *parm);
int32_t qc7xx_sf_ctrl_aes_set_decrypt_region_le(struct qc7xx_sf_ctrl_decrypt_type *parm);

#ifdef __cplusplus
}
#endif

/*@} end of group SF_CTRL_Public_Functions */

/*@} end of group SF_CTRL */

#endif /* _QC7XX_SF_CTRL_H */
