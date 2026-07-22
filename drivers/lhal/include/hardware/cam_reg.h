/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __HARDWARE_CAM_H__
#define  __HARDWARE_CAM_H__

/****************************************************************************
 * Pre-processor Definitions
****************************************************************************/

/* Register offsets *********************************************************/

#define CAM_DVP2AXI_CONFIGUE_OFFSET     (0x0)/* dvp2axi_configue */
#define CAM_DVP2AXI_ADDR_START_OFFSET   (0x4)/* dvp2axi_addr_start */
#define CAM_DVP2AXI_MEM_BCNT_OFFSET     (0x8)/* dvp2axi_mem_bcnt */
#define CAM_DVP2AXI_HSYNC_CROP_OFFSET   (0x30)/* dvp2axi_hsync_crop */
#define CAM_DVP2AXI_VSYNC_CROP_OFFSET   (0x34)/* dvp2axi_vsync_crop */
#define CAM_DVP2AXI_FRAM_EXM_OFFSET     (0x38)/* dvp2axi_fram_exm */
#define CAM_FRAME_START_ADDR0_OFFSET    (0x40)/* frame_start_addr0 */
#define CAM_FRAME_START_ADDR1_OFFSET    (0x48)/* frame_start_addr1 */
#define CAM_FRAME_START_ADDR2_OFFSET    (0x50)/* frame_start_addr2 */
#define CAM_FRAME_START_ADDR3_OFFSET    (0x58)/* frame_start_addr3 */
#define CAM_DVP_STATUS_AND_ERROR_OFFSET (0xC)/* dvp_status_and_error */
#define CAM_DVP2AXI_FRAME_BCNT_OFFSET   (0x10)/* dvp2axi_frame_bcnt */
#define CAM_DVP_FRAME_FIFO_POP_OFFSET   (0x14)/* dvp_frame_fifo_pop */
#define CAM_DVP2AXI_FRAME_VLD_OFFSET    (0x18)/* dvp2axi_frame_vld */
#define CAM_DVP2AXI_FRAME_PERIOD_OFFSET (0x1C)/* dvp2axi_frame_period */
#define CAM_DVP2AXI_MISC_OFFSET         (0x20)/* dvp2axi_misc */
#define CAM_FRAME_ID_STS01_OFFSET       (0x60)/* frame_id_sts01 */
#define CAM_FRAME_ID_STS23_OFFSET       (0x64)/* frame_id_sts23 */
#define CAM_DVP_DEBUG_OFFSET            (0xF0)/* dvp_debug */
#define CAM_DVP_DUMMY_REG_OFFSET        (0xFC)/* dvp_dummy_reg */

/* Register Bitfield definitions *****************************************************/

/* 0x0 : dvp2axi_configue */
#define CAM_REG_DVP_ENABLE           (1<<0U)
#if defined(QCC75X)
#define CAM_REG_WRAP_MODE            (1<<1U)
#else
#define CAM_REG_SW_MODE              (1<<1U)
#endif
#define CAM_REG_FRAM_VLD_POL         (1<<2U)
#define CAM_REG_LINE_VLD_POL         (1<<3U)
#define CAM_REG_XLEN_SHIFT           (4U)
#define CAM_REG_XLEN_MASK            (0x7<<CAM_REG_XLEN_SHIFT)
#define CAM_REG_DVP_MODE_SHIFT       (8U)
#define CAM_REG_DVP_MODE_MASK        (0x7<<CAM_REG_DVP_MODE_SHIFT)
#if !defined(QCC75X)
#define CAM_REG_HW_MODE_FWRAP        (1<<11U)
#endif
#define CAM_REG_DROP_EN              (1<<12U)
#define CAM_REG_DROP_EVEN            (1<<13U)
#define CAM_REG_QOS_SW_MODE          (1<<14U)
#define CAM_REG_QOS_SW               (1<<15U)
#define CAM_REG_DVP_DATA_MODE_SHIFT  (16U)
#define CAM_REG_DVP_DATA_MODE_MASK   (0x7<<CAM_REG_DVP_DATA_MODE_SHIFT)
#define CAM_REG_DVP_DATA_BSEL        (1<<19U)
#if !defined(QCC75X)
#define CAM_REG_DVP_PIX_CLK_CG       (1<<20U)
#endif
#define CAM_REG_V_SUBSAMPLE_EN       (1<<22U)
#define CAM_REG_V_SUBSAMPLE_POL      (1<<23U)
#define CAM_REG_DVP_WAIT_CYCLE_SHIFT (24U)
#define CAM_REG_DVP_WAIT_CYCLE_MASK  (0xff<<CAM_REG_DVP_WAIT_CYCLE_SHIFT)

/* 0x4 : dvp2axi_addr_start */
#define CAM_REG_ADDR_START_SHIFT (0U)
#define CAM_REG_ADDR_START_MASK  (0xffffffff<<CAM_REG_ADDR_START_SHIFT)

/* 0x8 : dvp2axi_mem_bcnt */
#if defined(QCC75X)
#define CAM_REG_STRIDE_WIDTH_X8_SHIFT (0U)
#define CAM_REG_STRIDE_WIDTH_X8_MASK  (0x3fff<<CAM_REG_STRIDE_WIDTH_X8_SHIFT)
#define CAM_REG_WRAP_LCNT_SHIFT       (16U)
#define CAM_REG_WRAP_LCNT_MASK        (0x3fff<<CAM_REG_WRAP_LCNT_SHIFT)
#else
#define CAM_REG_MEM_BURST_CNT_SHIFT (0U)
#define CAM_REG_MEM_BURST_CNT_MASK  (0xffffffff<<CAM_REG_MEM_BURST_CNT_SHIFT)
#endif

/* 0xC : dvp_status_and_error */
#define CAM_REG_FRAME_CNT_TRGR_INT_SHIFT (0U)
#define CAM_REG_FRAME_CNT_TRGR_INT_MASK  (0x1f<<CAM_REG_FRAME_CNT_TRGR_INT_SHIFT)
#define CAM_REG_INT_HCNT_EN              (1<<6U)
#define CAM_REG_INT_VCNT_EN              (1<<7U)
#define CAM_REG_INT_NORMAL_EN            (1<<8U)
#if !defined(QCC75X)
#define CAM_REG_INT_MEM_EN               (1<<9U)
#define CAM_REG_INT_FRAME_EN             (1<<10U)
#endif
#define CAM_REG_INT_FIFO_EN              (1<<11U)
#define CAM_STS_NORMAL_INT               (1<<12U)
#if !defined(QCC75X)
#define CAM_STS_MEM_INT                  (1<<13U)
#define CAM_STS_FRAME_INT                (1<<14U)
#endif
#define CAM_STS_FIFO_INT                 (1<<15U)
#if !defined(QCC75X)
#define CAM_FRAME_VALID_CNT_SHIFT        (16U)
#define CAM_FRAME_VALID_CNT_MASK         (0x1f<<CAM_FRAME_VALID_CNT_SHIFT)
#endif
#define CAM_STS_HCNT_INT                 (1<<21U)
#define CAM_STS_VCNT_INT                 (1<<22U)
#define CAM_ST_BUS_IDLE                  (1<<24U)
#define CAM_ST_BUS_FUNC                  (1<<25U)
#define CAM_ST_BUS_WAIT                  (1<<26U)
#define CAM_ST_BUS_FLSH                  (1<<27U)
#define CAM_AXI_IDLE                     (1<<28U)
#if !defined(QCC75X)
#define CAM_ST_DVP_IDLE                  (1<<29U)
#endif

/* 0x10 : dvp2axi_frame_bcnt */
#if defined(QCC75X)
#define CAM_REG_FRAME_WIDTH_X8_SHIFT (0U)
#define CAM_REG_FRAME_WIDTH_X8_MASK  (0x3fff<<CAM_REG_FRAME_WIDTH_X8_SHIFT)
#define CAM_REG_FRAME_HEIGHT_SHIFT   (16U)
#define CAM_REG_FRAME_HEIGHT_MASK    (0x3fff<<CAM_REG_FRAME_HEIGHT_SHIFT)
#else
#define CAM_REG_FRAME_BYTE_CNT_SHIFT (0U)
#define CAM_REG_FRAME_BYTE_CNT_MASK  (0xffffffff<<CAM_REG_FRAME_BYTE_CNT_SHIFT)
#endif

/* 0x14 : dvp_frame_fifo_pop */
#if !defined(QCC75X)
#define CAM_RFIFO_POP            (1<<0U)
#endif
#define CAM_REG_INT_NORMAL_CLR   (1<<4U)
#if !defined(QCC75X)
#define CAM_REG_INT_MEM_CLR      (1<<5U)
#define CAM_REG_INT_FRAME_CLR    (1<<6U)
#endif
#define CAM_REG_INT_FIFO_CLR     (1<<7U)
#define CAM_REG_INT_HCNT_CLR     (1<<8U)
#define CAM_REG_INT_VCNT_CLR     (1<<9U)
#if defined(QCC75X)
#define CAM_CR_SW_SHD            (1<<16U)
#endif

/* 0x18 : dvp2axi_frame_vld */
#define CAM_REG_FRAME_N_VLD_SHIFT (0U)
#define CAM_REG_FRAME_N_VLD_MASK  (0xffffffff<<CAM_REG_FRAME_N_VLD_SHIFT)

/* 0x1C : dvp2axi_frame_period */
#define CAM_REG_FRAME_PERIOD_SHIFT (0U)
#define CAM_REG_FRAME_PERIOD_MASK  (0x1f<<CAM_REG_FRAME_PERIOD_SHIFT)
#if defined(QCC75X)
#define CAM_STS_FRAME_PERIOD_N_SHIFT (8U)
#define CAM_STS_FRAME_PERIOD_N_MASK  (0x1f<<CAM_STS_FRAME_PERIOD_N_SHIFT)
#endif

/* 0x20 : dvp2axi_misc */
#define CAM_REG_ALPHA_SHIFT      (0U)
#define CAM_REG_ALPHA_MASK       (0xff<<CAM_REG_ALPHA_SHIFT)
#define CAM_REG_FORMAT_565_SHIFT (8U)
#define CAM_REG_FORMAT_565_MASK  (0x7<<CAM_REG_FORMAT_565_SHIFT)

/* 0x30 : dvp2axi_hsync_crop */
#define CAM_REG_HSYNC_ACT_END_SHIFT   (0U)
#if defined(QCC75X)
#define CAM_REG_HSYNC_ACT_END_MASK    (0x3fff<<CAM_REG_HSYNC_ACT_END_SHIFT)
#else
#define CAM_REG_HSYNC_ACT_END_MASK    (0xffff<<CAM_REG_HSYNC_ACT_END_SHIFT)
#endif
#define CAM_REG_HSYNC_ACT_START_SHIFT (16U)
#if defined(QCC75X)
#define CAM_REG_HSYNC_ACT_START_MASK  (0x3fff<<CAM_REG_HSYNC_ACT_START_SHIFT)
#else
#define CAM_REG_HSYNC_ACT_START_MASK  (0xffff<<CAM_REG_HSYNC_ACT_START_SHIFT)
#endif

/* 0x34 : dvp2axi_vsync_crop */
#define CAM_REG_VSYNC_ACT_END_SHIFT   (0U)
#if defined(QCC75X)
#define CAM_REG_VSYNC_ACT_END_MASK    (0x3fff<<CAM_REG_VSYNC_ACT_END_SHIFT)
#else
#define CAM_REG_VSYNC_ACT_END_MASK    (0xffff<<CAM_REG_VSYNC_ACT_END_SHIFT)
#endif
#define CAM_REG_VSYNC_ACT_START_SHIFT (16U)
#if defined(QCC75X)
#define CAM_REG_VSYNC_ACT_START_MASK  (0x3fff<<CAM_REG_VSYNC_ACT_START_SHIFT)
#else
#define CAM_REG_VSYNC_ACT_START_MASK  (0xffff<<CAM_REG_VSYNC_ACT_START_SHIFT)
#endif

/* 0x38 : dvp2axi_fram_exm */
#define CAM_REG_TOTAL_HCNT_SHIFT (0U)
#if defined(QCC75X)
#define CAM_REG_TOTAL_HCNT_MASK  (0x3fff<<CAM_REG_TOTAL_HCNT_SHIFT)
#else
#define CAM_REG_TOTAL_HCNT_MASK  (0xffff<<CAM_REG_TOTAL_HCNT_SHIFT)
#endif
#define CAM_REG_TOTAL_VCNT_SHIFT (16U)
#if defined(QCC75X)
#define CAM_REG_TOTAL_VCNT_MASK  (0x3fff<<CAM_REG_TOTAL_VCNT_SHIFT)
#else
#define CAM_REG_TOTAL_VCNT_MASK  (0xffff<<CAM_REG_TOTAL_VCNT_SHIFT)
#endif

#if !defined(QCC75X)
/* 0x40 : frame_start_addr0 */
#define CAM_FRAME_START_ADDR_0_SHIFT (0U)
#define CAM_FRAME_START_ADDR_0_MASK  (0xffffffff<<CAM_FRAME_START_ADDR_0_SHIFT)

/* 0x48 : frame_start_addr1 */
#define CAM_FRAME_START_ADDR_1_SHIFT (0U)
#define CAM_FRAME_START_ADDR_1_MASK  (0xffffffff<<CAM_FRAME_START_ADDR_1_SHIFT)

/* 0x50 : frame_start_addr2 */
#define CAM_FRAME_START_ADDR_2_SHIFT (0U)
#define CAM_FRAME_START_ADDR_2_MASK  (0xffffffff<<CAM_FRAME_START_ADDR_2_SHIFT)

/* 0x58 : frame_start_addr3 */
#define CAM_FRAME_START_ADDR_3_SHIFT (0U)
#define CAM_FRAME_START_ADDR_3_MASK  (0xffffffff<<CAM_FRAME_START_ADDR_3_SHIFT)

/* 0x60 : frame_id_sts01 */
#define CAM_FRAME_ID_0_SHIFT (0U)
#define CAM_FRAME_ID_0_MASK  (0xffff<<CAM_FRAME_ID_0_SHIFT)
#define CAM_FRAME_ID_1_SHIFT (16U)
#define CAM_FRAME_ID_1_MASK  (0xffff<<CAM_FRAME_ID_1_SHIFT)

/* 0x64 : frame_id_sts23 */
#define CAM_FRAME_ID_2_SHIFT (0U)
#define CAM_FRAME_ID_2_MASK  (0xffff<<CAM_FRAME_ID_2_SHIFT)
#define CAM_FRAME_ID_3_SHIFT (16U)
#define CAM_FRAME_ID_3_MASK  (0xffff<<CAM_FRAME_ID_3_SHIFT)

/* 0xF0 : dvp_debug */
#define CAM_REG_DVP_DBG_EN          (1<<0U)
#define CAM_REG_DVP_DBG_SEL_SHIFT   (1U)
#define CAM_REG_DVP_DBG_SEL_MASK    (0x7<<CAM_REG_DVP_DBG_SEL_SHIFT)
#define CAM_REG_ID_LATCH_LINE_SHIFT (8U)
#define CAM_REG_ID_LATCH_LINE_MASK  (0xf<<CAM_REG_ID_LATCH_LINE_SHIFT)
#endif

/* 0xFC : dvp_dummy_reg */

#endif  /* __HARDWARE_CAM_H__ */
