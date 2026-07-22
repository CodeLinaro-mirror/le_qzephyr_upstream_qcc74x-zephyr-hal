/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef  __BLOCK2RASTER_REG_H__
#define  __BLOCK2RASTER_REG_H__

/****************************************************************************
 * Pre-processor Definitions
****************************************************************************/

/* Register offsets *********************************************************/

#define BLOCK2RASTER_B2R_BASIC_CFG0_OFFSET                      (0x000)/* b2r_basic_cfg0 */
#define BLOCK2RASTER_B2R_FRAME_CFG0_OFFSET                      (0x004)/* b2r_frame_cfg0 */
#define BLOCK2RASTER_B2R_FRAME_CFG1_OFFSET                      (0x008)/* b2r_frame_cfg1 */
#define BLOCK2RASTER_B2R_MEM_LAYOUT_CFG0_OFFSET                 (0x010)/* b2r_mem_layout_cfg0 */
#define BLOCK2RASTER_B2R_MEM_LAYOUT_CFG1_OFFSET                 (0x014)/* b2r_mem_layout_cfg1 */
#define BLOCK2RASTER_B2R_MEM_LAYOUT_CFG2_OFFSET                 (0x018)/* b2r_mem_layout_cfg2 */
#define BLOCK2RASTER_B2R_MEM_LAYOUT_CFG3_OFFSET                 (0x01C)/* b2r_mem_layout_cfg3 */
#define BLOCK2RASTER_B2R_BUS_CFG_OFFSET                         (0x020)/* b2r_bus_cfg */
#define BLOCK2RASTER_B2R_R_W1P_OFFSET                           (0x030)/* b2r_r_w1p */

/* Register Bitfield definitions *****************************************************/

/* 0x000 : b2r_basic_cfg0 */
#define BLOCK2RASTER_REG_ENABLE                                 (1<<0U)
#define BLOCK2RASTER_REG_B2R_SW_MODE                            (1<<1U)
#define BLOCK2RASTER_REG_B2R_DVP_MODE                           (1<<2U)
#define BLOCK2RASTER_REG_B2R_BP_WAIT                            (1<<3U)
#define BLOCK2RASTER_REG_C_ODD_LINE_VLD                         (1<<4U)
#define BLOCK2RASTER_REG_B2R_IN_MODE                            (1<<5U)
#define BLOCK2RASTER_REG_C_BLOCK_ORDER                          (1<<6U)
#define BLOCK2RASTER_REG_B2R_SWAP_MODE                          (1<<7U)
#define BLOCK2RASTER_REG_B2R_SWAP_IDX_SWM                       (1<<8U)
#define BLOCK2RASTER_REG_B2R_SWAP_IDX_SWV                       (1<<9U)

/* 0x004 : b2r_frame_cfg0 */
#define BLOCK2RASTER_REG_W_BLK_SHIFT                            (0U)
#define BLOCK2RASTER_REG_W_BLK_MASK                             (0xfff<<BLOCK2RASTER_REG_W_BLK_SHIFT)
#define BLOCK2RASTER_REG_H_BLK_SHIFT                            (16U)
#define BLOCK2RASTER_REG_H_BLK_MASK                             (0xfff<<BLOCK2RASTER_REG_H_BLK_SHIFT)

/* 0x008 : b2r_frame_cfg1 */
#define BLOCK2RASTER_REG_W_BLANK_SHIFT                          (0U)
#define BLOCK2RASTER_REG_W_BLANK_MASK                           (0xfff<<BLOCK2RASTER_REG_W_BLANK_SHIFT)
#define BLOCK2RASTER_REG_H_BLANK_SHIFT                          (16U)
#define BLOCK2RASTER_REG_H_BLANK_MASK                           (0xfff<<BLOCK2RASTER_REG_H_BLANK_SHIFT)

/* 0x010 : b2r_mem_layout_cfg0 */
#define BLOCK2RASTER_REG_Y_ADDR_STRT_SHIFT                      (0U)
#define BLOCK2RASTER_REG_Y_ADDR_STRT_MASK                       (0xffffffff<<BLOCK2RASTER_REG_Y_ADDR_STRT_SHIFT)

/* 0x014 : b2r_mem_layout_cfg1 */
#define BLOCK2RASTER_REG_C_ADDR_STRT_SHIFT                      (0U)
#define BLOCK2RASTER_REG_C_ADDR_STRT_MASK                       (0xffffffff<<BLOCK2RASTER_REG_C_ADDR_STRT_SHIFT)

/* 0x018 : b2r_mem_layout_cfg2 */
#define BLOCK2RASTER_REG_Y_ADDR_SWAP_SHIFT                      (0U)
#define BLOCK2RASTER_REG_Y_ADDR_SWAP_MASK                       (0xffffffff<<BLOCK2RASTER_REG_Y_ADDR_SWAP_SHIFT)

/* 0x01C : b2r_mem_layout_cfg3 */
#define BLOCK2RASTER_REG_C_ADDR_SWAP_SHIFT                      (0U)
#define BLOCK2RASTER_REG_C_ADDR_SWAP_MASK                       (0xffffffff<<BLOCK2RASTER_REG_C_ADDR_SWAP_SHIFT)

/* 0x020 : b2r_bus_cfg */
#define BLOCK2RASTER_REG_B2R_OSTD_MAX_SHIFT                     (0U)
#define BLOCK2RASTER_REG_B2R_OSTD_MAX_MASK                      (0x3<<BLOCK2RASTER_REG_B2R_OSTD_MAX_SHIFT)
#define BLOCK2RASTER_REG_QOS_SW_MODE                            (1<<8U)
#define BLOCK2RASTER_REG_QOS_SW                                 (1<<9U)

/* 0x030 : b2r_r_w1p */
#define BLOCK2RASTER_REG_B2R_SW_KICK                            (1<<0U)
#define BLOCK2RASTER_STS_SW_FRAME_CNT_SHIFT                     (4U)
#define BLOCK2RASTER_STS_SW_FRAME_CNT_MASK                      (0x1f<<BLOCK2RASTER_STS_SW_FRAME_CNT_SHIFT)

#endif  /* __BLOCK2RASTER_REG_H__ */
