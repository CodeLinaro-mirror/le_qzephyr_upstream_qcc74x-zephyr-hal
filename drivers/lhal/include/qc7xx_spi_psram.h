/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QC7XX_SPI_PSRAM_H__
#define __QC7XX_SPI_PSRAM_H__

#include "qc7xx_sf_ctrl.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup SPI_PSRAM
  * @{
  */

/**
 *  @brief Psram drive strength type definition
 */
#define PSRAM_DRIVE_STRENGTH_50_OHMS      0  /*!< Drive strength 50 ohms(default) */
#define PSRAM_DRIVE_STRENGTH_100_OHMS     1  /*!< Drive strength 100 ohms */
#define PSRAM_DRIVE_STRENGTH_200_OHMS     2  /*!< Drive strength 200 ohms */

/**
 *  @brief Psram burst length size type definition
 */
#define PSRAM_BURST_LENGTH_16_BYTES       0  /*!< Burst Length 16 bytes */
#define PSRAM_BURST_LENGTH_32_BYTES       1  /*!< Burst Length 32 bytes */
#define PSRAM_BURST_LENGTH_64_BYTES       2  /*!< Burst Length 64 bytes */
#define PSRAM_BURST_LENGTH_512_BYTES      3 /*!< Burst Length 512 bytes(default) */

/**
 *  @brief Psram ctrl mode type definition
 */
#define PSRAM_SPI_CTRL_MODE               0  /*!< Psram SPI ctrl mode */
#define PSRAM_QPI_CTRL_MODE               1  /*!< Psram QPI ctrl mode */

/**
 *  @brief Psram ctrl configuration structure type definition
 */
struct spi_psram_cfg_type {
    uint8_t read_id_cmd;                  /*!< Read ID command */
    uint8_t read_id_dmy_clk;              /*!< Read ID command dummy clock */
    uint8_t burst_toggle_cmd;             /*!< Burst toggle length command */
    uint8_t reset_enable_cmd;             /*!< Psram reset enable command */
    uint8_t reset_cmd;                    /*!< Psram reset command */
    uint8_t enter_quad_mode_cmd;          /*!< Psram enter quad mode command */
    uint8_t exit_quad_mode_cmd;           /*!< Psram exit quad mode command */
    uint8_t read_reg_cmd;                 /*!< Read register command */
    uint8_t read_reg_dmy_clk;             /*!< Read register command dummy clock */
    uint8_t write_reg_cmd;                /*!< Write register command */
    uint8_t read_cmd;                     /*!< Psram read command */
    uint8_t read_dmy_clk;                 /*!< Psram read command dummy clock */
    uint8_t f_read_cmd;                   /*!< Psram fast read command */
    uint8_t f_read_dmy_clk;               /*!< Psram fast read command dummy clock */
    uint8_t f_read_quad_cmd;              /*!< Psram fast read quad command */
    uint8_t f_read_quad_dmy_clk;          /*!< Psram fast read quad command dummy clock */
    uint8_t write_cmd;                    /*!< Psram write command */
    uint8_t quad_write_cmd;               /*!< Psram quad write command */
    uint16_t page_size;                   /*!< Psram page size */
    uint8_t ctrl_mode;                    /*!< Psram ctrl mode */
    uint8_t drive_strength;               /*!< Psram drive strength */
    uint8_t burst_length;                 /*!< Psram burst length size */
};

#ifdef __cplusplus
extern "C" {
#endif

void qc7xx_psram_init(struct spi_psram_cfg_type *psram_cfg, struct sf_ctrl_cmds_cfg *cmds_cfg,
                     struct sf_ctrl_psram_cfg *sf_ctrl_psram_cfg);
void qc7xx_psram_readreg(struct spi_psram_cfg_type *psram_cfg, uint8_t *reg_value);
void qc7xx_psram_writereg(struct spi_psram_cfg_type *psram_cfg, uint8_t *reg_value);
int qc7xx_psram_setdrivestrength(struct spi_psram_cfg_type *psram_cfg);
int qc7xx_psram_setburstwrap(struct spi_psram_cfg_type *psram_cfg);
void qc7xx_psram_readid(struct spi_psram_cfg_type *psram_cfg, uint8_t *data);
int qc7xx_psram_enterquadmode(struct spi_psram_cfg_type *psram_cfg);
int qc7xx_psram_exitquadmode(struct spi_psram_cfg_type *psram_cfg);
int qc7xx_psram_toggleburstlength(struct spi_psram_cfg_type *psram_cfg, uint8_t ctrl_mode);
int qc7xx_psram_softwarereset(struct spi_psram_cfg_type *psram_cfg, uint8_t ctrl_mode);
int qc7xx_psram_set_idbus_cfg(struct spi_psram_cfg_type *psram_cfg, uint8_t io_mode, uint32_t addr, uint32_t len);
int qc7xx_psram_cache_write_set(struct spi_psram_cfg_type *psram_cfg, uint8_t io_mode, uint8_t wt_en,
                               uint8_t wb_en, uint8_t wa_en);
int qc7xx_psram_write(struct spi_psram_cfg_type *psram_cfg, uint8_t io_mode, uint32_t addr, uint8_t *data, uint32_t len);
int qc7xx_psram_read(struct spi_psram_cfg_type *psram_cfg, uint8_t io_mode, uint32_t addr, uint8_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
  */

#endif /* __QC7XX_SPI_PSRAM_H__ */
