/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __HARDWARE_CAM_FRONT_H__
#define  __HARDWARE_CAM_FRONT_H__

/****************************************************************************
 * Pre-processor Definitions
****************************************************************************/

/* Register offsets *********************************************************/

#if defined(QCC75X)
#define CAM_FRONT_MM_MISC_CTRL_OFFSET                           (0x0)/* mm_misc_ctrl */
#define CAM_FRONT_CONFIG_OFFSET                                 (0x4)/* config */
#define CAM_FRONT_DVP_AS_REGB_OFFSET                            (0x8)/* dvp_as_regb */
#define CAM_FRONT_DVP_AS_REGC_OFFSET                            (0xc)/* dvp_as_regc */
#define CAM_FRONT_DVP_MUX_SEL_REG_OFFSET                        (0x10)/* dvp_mux_sel_reg */
#define CAM_FRONT_DVP_MUX_SEL_REG2_OFFSET                       (0x14)/* dvp_mux_sel_reg2 */
#define CAM_FRONT_DVP_BT656_OFFSET                              (0x18)/* dvp_bt656 */
#define CAM_FRONT_MM_BUS_OFFSET                                 (0x20)/* mm_bus */
#define CAM_FRONT_MM_MISC_OFFSET                                (0x40)/* mm_misc */
#define CAM_FRONT_MM_MISC_RST_0_OFFSET                          (0x100)/* mm_misc_rst_0 */
#define CAM_FRONT_MM_MISC_CLK_0_OFFSET                          (0x120)/* mm_misc_clk_0 */
#else
#define CAM_FRONT_CONFIG_OFFSET                                 (0x0)/* config */
#define CAM_FRONT_DVP2BUS_SRC_SEL_1_OFFSET                      (0x8)/* dvp2bus_src_sel_1 */
#if defined(QCC74X)
#define CAM_FRONT_SNSR_CTRL_OFFSET                              (0xC)/* snsr_ctrl */
#define CAM_FRONT_EMI_MISC_OFFSET                               (0x10)/* emi_misc */
#define CAM_FRONT_ISP_ID_YUV_OFFSET                             (0x14)/* isp_id_yuv */
#endif
#endif

/* Register Bitfield definitions *****************************************************/

#if defined(QCC75X)
/* 0x0 : mm_misc_ctrl */
#define CAM_FRONT_REG_FORCE_VSYNC_SH                            (1<<0U)
#define CAM_FRONT_CR_DVP_S2P_HS_INV                             (1<<1U)
#define CAM_FRONT_CR_DVP_S2P_VS_INV                             (1<<2U)
#define CAM_FRONT_CR_DVP_S2P_DA_ORDER                           (1<<3U)
#define CAM_FRONT_CR_DVP_S2P_EN                                 (1<<4U)
#define CAM_FRONT_REG_SNSR_MODE                                 (1<<5U)
#define CAM_FRONT_REG_FORCE_PCLK_ON_SHIFT                       (16U)
#define CAM_FRONT_REG_FORCE_PCLK_ON_MASK                        (0xffff<<CAM_FRONT_REG_FORCE_PCLK_ON_SHIFT)
#endif

/* 0x0 : config */
#define CAM_FRONT_RG_DVPAS_ENABLE                               (1<<0U)
#define CAM_FRONT_RG_DVPAS_HS_INV                               (1<<1U)
#define CAM_FRONT_RG_DVPAS_VS_INV                               (1<<2U)
#if defined(QCC75X)
#define CAM_FRONT_CR_TG_EN_AS                                   (1<<3U)
#define CAM_FRONT_RG_DVPAS_FIFO_TH_SHIFT                        (4U)
#define CAM_FRONT_RG_DVPAS_FIFO_TH_MASK                         (0xfff<<CAM_FRONT_RG_DVPAS_FIFO_TH_SHIFT)
#define CAM_FRONT_RG_TOTAL_HPIXELS_AS_SHIFT                     (16U)
#define CAM_FRONT_RG_TOTAL_HPIXELS_AS_MASK                      (0xffff<<CAM_FRONT_RG_TOTAL_HPIXELS_AS_SHIFT)
#else
#define CAM_FRONT_RG_DVPAS_DA_ORDER                             (1<<3U)
#define CAM_FRONT_RG_DVPAS_FIFO_TH_SHIFT                        (16U)
#define CAM_FRONT_RG_DVPAS_FIFO_TH_MASK                         (0x7ff<<CAM_FRONT_RG_DVPAS_FIFO_TH_SHIFT)
#endif

#if defined(QCC75X)
/* 0x8 : dvp_as_regb */
#define CAM_FRONT_CR_ACT_PIXEL_WIDTH_AS_SHIFT                   (0U)
#define CAM_FRONT_CR_ACT_PIXEL_WIDTH_AS_MASK                    (0xffff<<CAM_FRONT_CR_ACT_PIXEL_WIDTH_AS_SHIFT)
#define CAM_FRONT_CR_ACT_START_PIXEL_AS_SHIFT                   (16U)
#define CAM_FRONT_CR_ACT_START_PIXEL_AS_MASK                    (0xffff<<CAM_FRONT_CR_ACT_START_PIXEL_AS_SHIFT)

/* 0xC : dvp_as_regc */
#define CAM_FRONT_CR_ACT_LINE_HEIGHT_AS_SHIFT                   (0U)
#define CAM_FRONT_CR_ACT_LINE_HEIGHT_AS_MASK                    (0xffff<<CAM_FRONT_CR_ACT_LINE_HEIGHT_AS_SHIFT)
#define CAM_FRONT_CR_ACT_START_LINE_AS_SHIFT                    (16U)
#define CAM_FRONT_CR_ACT_START_LINE_AS_MASK                     (0xffff<<CAM_FRONT_CR_ACT_START_LINE_AS_SHIFT)

/* 0x10 : dvp_mux_sel_reg */
#define CAM_FRONT_REG_422TO420A_IN_SEL_SHIFT                    (4U)
#define CAM_FRONT_REG_422TO420A_IN_SEL_MASK                     (0x7<<CAM_FRONT_REG_422TO420A_IN_SEL_SHIFT)
#define CAM_FRONT_REG_420TO422A_IN_SEL_SHIFT                    (12U)
#define CAM_FRONT_REG_420TO422A_IN_SEL_MASK                     (0x3<<CAM_FRONT_REG_420TO422A_IN_SEL_SHIFT)
#define CAM_FRONT_REG_B2RA_IN_SEL_SHIFT                         (16U)
#define CAM_FRONT_REG_B2RA_IN_SEL_MASK                          (0x3<<CAM_FRONT_REG_B2RA_IN_SEL_SHIFT)
#define CAM_FRONT_REG_D2XA_IN_SEL_SHIFT                         (24U)
#define CAM_FRONT_REG_D2XA_IN_SEL_MASK                          (0x7<<CAM_FRONT_REG_D2XA_IN_SEL_SHIFT)
#define CAM_FRONT_REG_D2XB_IN_SEL_SHIFT                         (28U)
#define CAM_FRONT_REG_D2XB_IN_SEL_MASK                          (0x7<<CAM_FRONT_REG_D2XB_IN_SEL_SHIFT)

/* 0x14 : dvp_mux_sel_reg2 */
#define CAM_FRONT_REG_D2SA_IN_SEL_SHIFT                         (0U)
#define CAM_FRONT_REG_D2SA_IN_SEL_MASK                          (0x7<<CAM_FRONT_REG_D2SA_IN_SEL_SHIFT)
#define CAM_FRONT_REG_D2SB_IN_SEL_SHIFT                         (4U)
#define CAM_FRONT_REG_D2SB_IN_SEL_MASK                          (0x7<<CAM_FRONT_REG_D2SB_IN_SEL_SHIFT)
#define CAM_FRONT_REG_OSDA_IN_SEL_SHIFT                         (8U)
#define CAM_FRONT_REG_OSDA_IN_SEL_MASK                          (0x7<<CAM_FRONT_REG_OSDA_IN_SEL_SHIFT)
#define CAM_FRONT_REG_DISP_IN_SEL_SHIFT                         (12U)
#define CAM_FRONT_REG_DISP_IN_SEL_MASK                          (0x7<<CAM_FRONT_REG_DISP_IN_SEL_SHIFT)

/* 0x18 : dvp_bt656 */
#define CAM_FRONT_CR_DVP_BT656_EN                               (1<<0U)
#define CAM_FRONT_CR_DVP_BT656_CLK_INV                          (1<<1U)
#define CAM_FRONT_CR_DVP_BT656_YC_INV                           (1<<2U)

/* 0x20 : mm_bus */
#define CAM_FRONT_CR_W_THRE_MM2EXT_SHIFT                        (0U)
#define CAM_FRONT_CR_W_THRE_MM2EXT_MASK                         (0x3<<CAM_FRONT_CR_W_THRE_MM2EXT_SHIFT)

/* 0x40 : mm_misc */
#define CAM_FRONT_CR_R2B_Y_SEL_SHIFT                            (0U)
#define CAM_FRONT_CR_R2B_Y_SEL_MASK                             (0x7<<CAM_FRONT_CR_R2B_Y_SEL_SHIFT)
#define CAM_FRONT_CR_R2B_C_SEL_SHIFT                            (4U)
#define CAM_FRONT_CR_R2B_C_SEL_MASK                             (0x7<<CAM_FRONT_CR_R2B_C_SEL_SHIFT)
#define CAM_FRONT_CR_BLOCK_SRAM_EN                              (1<<31U)

/* 0x100 : mm_misc_rst_0 */
#define CAM_FRONT_SWRST_MM_MISC                                 (1<<0U)
#define CAM_FRONT_SWRST_D2XA                                    (1<<1U)
#define CAM_FRONT_SWRST_D2XB                                    (1<<2U)
#define CAM_FRONT_SWRST_JENC                                    (1<<3U)
#define CAM_FRONT_SWRST_JDEC                                    (1<<4U)
#define CAM_FRONT_SWRST_DTSRC                                   (1<<5U)
#define CAM_FRONT_SWRST_R2B                                     (1<<6U)
#define CAM_FRONT_SWRST_B2R                                     (1<<7U)
#define CAM_FRONT_SWRST_D2SA                                    (1<<8U)
#define CAM_FRONT_SWRST_D2SB                                    (1<<9U)
#define CAM_FRONT_SWRST_OSDA                                    (1<<10U)
#define CAM_FRONT_SWRST_DISP                                    (1<<11U)

/* 0x120 : mm_misc_clk_0 */
#define CAM_FRONT_CGEN_MM                                       (1<<0U)
#endif

#if defined(QCC74X)
/* 0x8 : dvp2bus_src_sel_1 */
#define CAM_FRONT_RG_D2X_DVP_SEL                                (1<<0U)

/* 0xC : snsr_ctrl */
#define CAM_FRONT_RG_SNSR_RST                                   (1<<0U)
#define CAM_FRONT_RG_SNSR_PWDN                                  (1<<1U)

/* 0x10 : emi_misc */
#define CAM_FRONT_REG_X_WTHRE_PB_SHIFT                          (0U)
#define CAM_FRONT_REG_X_WTHRE_PB_MASK                           (0x3<<CAM_FRONT_REG_X_WTHRE_PB_SHIFT)
#define CAM_FRONT_REG_SF_HARB_MODE                              (1<<4U)
#endif

#if !defined(QCC75X)
/* 0x14 : isp_id_yuv */
#define CAM_FRONT_REG_YUV_IDGEN_RST                             (1<<0U)
#define CAM_FRONT_REG_YUV_IDGEN_EDGE                            (1<<1U)
#define CAM_FRONT_REG_YUV_IDGEN_CNT_INCR_SHIFT                  (16U)
#define CAM_FRONT_REG_YUV_IDGEN_CNT_INCR_MASK                   (0xffff<<CAM_FRONT_REG_YUV_IDGEN_CNT_INCR_SHIFT)
#endif

#endif  /* __CAM_FRONT_REG_H__ */
