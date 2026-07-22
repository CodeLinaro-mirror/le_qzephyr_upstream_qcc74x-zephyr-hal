/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_CSI_H
#define _QC7XX_CSI_H

#include "qc7xx_core.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup CSI
  * @{
  */

/** @defgroup CSI_LANE_NUMBER CSI lane number definition
  * @{
  */
#define CSI_LANE_NUMBER_1 0
#define CSI_LANE_NUMBER_2 1
/**
  * @}
  */

/** @defgroup CSI_INTSTS CSI interrupt status definition
  * @{
  */
#define CSI_INTSTS_GENERIC_PACKET        (1 << 0)
#define CSI_INTSTS_LANE_MERGE_ERROR      (1 << 1)
#define CSI_INTSTS_ECC_ERROR             (1 << 2)
#define CSI_INTSTS_CRC_ERROR             (1 << 3)
#define CSI_INTSTS_PHY_HS_SOT_ERROR      (1 << 4)
#define CSI_INTSTS_PHY_HS_SOT_SYNC_ERROR (1 << 5)
/**
  * @}
  */

/** @defgroup CSI_INTMASK CSI interrupt mask definition
  * @{
  */
#define CSI_INTMASK_GENERIC_PACKET        (1 << 0)
#define CSI_INTMASK_LANE_MERGE_ERROR      (1 << 1)
#define CSI_INTMASK_ECC_ERROR             (1 << 2)
#define CSI_INTMASK_CRC_ERROR             (1 << 3)
#define CSI_INTMASK_PHY_HS_SOT_ERROR      (1 << 4)
#define CSI_INTMASK_PHY_HS_SOT_SYNC_ERROR (1 << 5)
/**
  * @}
  */

/** @defgroup CSI_INTCLR CSI interrupt clear definition
  * @{
  */
#define CSI_INTCLR_GENERIC_PACKET        (1 << 0)
#define CSI_INTCLR_LANE_MERGE_ERROR      (1 << 1)
#define CSI_INTCLR_ECC_ERROR             (1 << 2)
#define CSI_INTCLR_CRC_ERROR             (1 << 3)
#define CSI_INTCLR_PHY_HS_SOT_ERROR      (1 << 4)
#define CSI_INTCLR_PHY_HS_SOT_SYNC_ERROR (1 << 5)
/**
  * @}
  */

// clang-format off
#define IS_CSI_LANE_NUMBER(type)      ((type) <= CSI_LANE_NUMBER_2)

// clang-format on

/**
 * @brief CSI configuration structure
 *
 * @param lane_number   CSI lane number, use @ref CSI_LANE_NUMBER
 * @param tx_clk_escape CSI tx clock in escape mode
 * @param data_rate     CSI data rate
 */
struct qc7xx_csi_config_s {
    uint8_t lane_number;
    uint32_t tx_clk_escape;
    uint32_t data_rate;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize csi.
 *
 * @param [in] dev device handle
 * @param [in] config pointer to csi configure structure
 */
void qc7xx_csi_init(struct qc7xx_device_s *dev, const struct qc7xx_csi_config_s *config);

/**
 * @brief Enable csi.
 *
 * @param [in] dev device handle
 */
void qc7xx_csi_start(struct qc7xx_device_s *dev);

/**
 * @brief Disable csi.
 *
 * @param [in] dev device handle
 */
void qc7xx_csi_stop(struct qc7xx_device_s *dev);

/**
 * @brief Set threshold of line buffer, data will be sent to following module when threshold reached.
 *
 * @param [in] dev device handle
 * @param [in] resolution_x number of columns
 * @param [in] pixel_clock pixel clock
 * @param [in] dsp_clock dsp clock
 */
void qc7xx_csi_set_line_threshold(struct qc7xx_device_s *dev, uint16_t resolution_x, uint32_t pixel_clock, uint32_t dsp_clock);

/**
 * @brief Mask or unmask csi interrupt.
 *
 * @param [in] dev device handle
 * @param [in] int_type csi interrupt mask type, use @ref CSI_INTMASK
 * @param [in] mask mask or unmask
 */
void qc7xx_csi_int_mask(struct qc7xx_device_s *dev, uint32_t int_type, bool mask);

/**
 * @brief Clear csi interrupt.
 *
 * @param [in] dev device handle
 * @param [in] int_type csi interrupt clear type, use @ref CSI_INTCLR
 */
void qc7xx_csi_int_clear(struct qc7xx_device_s *dev, uint32_t int_type);

/**
 * @brief Get csi interrupt status.
 *
 * @param [in] dev device handle
 * @return Interrupt status
 */
uint32_t qc7xx_csi_get_intstatus(struct qc7xx_device_s *dev);

/**
 * @brief Control csi feature.
 *
 * @param [in] dev device handle
 * @param [in] cmd feature command
 * @param [in] arg user data
 * @return A negated errno value on failure
 */
int qc7xx_csi_feature_control(struct qc7xx_device_s *dev, int cmd, size_t arg);

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
  */

#endif
