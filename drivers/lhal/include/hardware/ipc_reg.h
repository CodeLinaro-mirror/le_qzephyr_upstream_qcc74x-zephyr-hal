/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef  __IPC_REG_H__
#define  __IPC_REG_H__

/****************************************************************************
 * Pre-processor Definitions
****************************************************************************/

/* Register offsets *********************************************************/

#define IPC_AP2NP_TRIGGER_OFFSET                                (0x0)/* ap2np_trigger */
#define IPC_NP2AP_RAW_STATUS_OFFSET                             (0x4)/* np2ap_raw_status */
#define IPC_NP2AP_ACK_OFFSET                                    (0x8)/* np2ap_ack */
#define IPC_NP2AP_UNMASK_SET_OFFSET                             (0xC)/* np2ap_unmask_set */
#define IPC_NP2AP_UNMASK_CLEAR_OFFSET                           (0x10)/* np2ap_unamsk_clear */
#define IPC_NP2AP_LINE_SEL_LOW_OFFSET                           (0x14)/* np2ap_line_sel_low */
#define IPC_NP2AP_LINE_SEL_HIGH_OFFSET                          (0x18)/* np2ap_line_sel_high */
#define IPC_NP2AP_STATUS_OFFSET                                 (0x1C)/* np2ap_status */
#define IPC_NP2AP_TRIGGER_OFFSET                                (0x20)/* np2ap_trigger */
#define IPC_AP2NP_RAW_STATUS_OFFSET                             (0x24)/* ap2np_raw_status */
#define IPC_AP2NP_ACK_OFFSET                                    (0x28)/* ap2np_ack */
#define IPC_AP2NP_UNMASK_SET_OFFSET                             (0x2C)/* ap2np_unmask_set */
#define IPC_AP2NP_UNMASK_CLEAR_OFFSET                            (0x30)/* ap2np_umask_clear */
#define IPC_AP2NP_LINE_SEL_LOW_OFFSET                           (0x34)/* ap2np_line_sel_low */
#define IPC_AP2NP_LINE_SEL_HIGH_OFFSET                          (0x38)/* ap2np_line_sel_high */
#define IPC_AP2NP_STATUS_OFFSET                                 (0x3C)/* ap2np_status */

/* Register Bitfield definitions *****************************************************/

/* 0x0 : ap2np_trigger */
#define IPC_AP2NP_TRIGGER_SHIFT                                 (0U)
#define IPC_AP2NP_TRIGGER_MASK                                  (0xffffffff<<IPC_AP2NP_TRIGGER_SHIFT)

/* 0x4 : np2ap_raw_status */
#define IPC_NP2AP_RAW_STATUS_SHIFT                              (0U)
#define IPC_NP2AP_RAW_STATUS_MASK                               (0xffffffff<<IPC_NP2AP_RAW_STATUS_SHIFT)

/* 0x8 : np2ap_ack */
#define IPC_NP2AP_ACK_SHIFT                                     (0U)
#define IPC_NP2AP_ACK_MASK                                      (0xffffffff<<IPC_NP2AP_ACK_SHIFT)

/* 0xC : np2ap_unmask_set */
#define IPC_NP2AP_UNMASK_SET_SHIFT                              (0U)
#define IPC_NP2AP_UNMASK_SET_MASK                               (0xffffffff<<IPC_NP2AP_UNMASK_SET_SHIFT)

/* 0x10 : np2ap_unamsk_clear */
#define IPC_NP2AP_UNAMSK_CLEAR_SHIFT                            (0U)
#define IPC_NP2AP_UNAMSK_CLEAR_MASK                             (0xffffffff<<IPC_NP2AP_UNAMSK_CLEAR_SHIFT)

/* 0x14 : np2ap_line_sel_low */
#define IPC_NP2AP_LINE_SEL_LOW_SHIFT                            (0U)
#define IPC_NP2AP_LINE_SEL_LOW_MASK                             (0xffffffff<<IPC_NP2AP_LINE_SEL_LOW_SHIFT)

/* 0x18 : np2ap_line_sel_high */
#define IPC_NP2AP_LINE_SEL_HIGH_SHIFT                           (0U)
#define IPC_NP2AP_LINE_SEL_HIGH_MASK                            (0xffffffff<<IPC_NP2AP_LINE_SEL_HIGH_SHIFT)

/* 0x1C : np2ap_status */
#define IPC_NP2AP_STATUS_SHIFT                                  (0U)
#define IPC_NP2AP_STATUS_MASK                                   (0xffffffff<<IPC_NP2AP_STATUS_SHIFT)

/* 0x20 : np2ap_trigger */
#define IPC_NP2AP_TRIGGER_SHIFT                                 (0U)
#define IPC_NP2AP_TRIGGER_MASK                                  (0xffffffff<<IPC_NP2AP_TRIGGER_SHIFT)

/* 0x24 : ap2np_raw_status */
#define IPC_AP2NP_RAW_STATUS_SHIFT                              (0U)
#define IPC_AP2NP_RAW_STATUS_MASK                               (0xffffffff<<IPC_AP2NP_RAW_STATUS_SHIFT)

/* 0x28 : ap2np_ack */
#define IPC_AP2NP_ACK_SHIFT                                     (0U)
#define IPC_AP2NP_ACK_MASK                                      (0xffffffff<<IPC_AP2NP_ACK_SHIFT)

/* 0x2C : ap2np_unmask_set */
#define IPC_AP2NP_UNMASK_SET_SHIFT                              (0U)
#define IPC_AP2NP_UNMASK_SET_MASK                               (0xffffffff<<IPC_AP2NP_UNMASK_SET_SHIFT)

/* 0x30 : ap2np_umask_clear */
#define IPC_AP2NP_UMASK_CLEAR_SHIFT                             (0U)
#define IPC_AP2NP_UMASK_CLEAR_MASK                              (0xffffffff<<IPC_AP2NP_UMASK_CLEAR_SHIFT)

/* 0x34 : ap2np_line_sel_low */
#define IPC_AP2NP_LINE_SEL_LOW_SHIFT                            (0U)
#define IPC_AP2NP_LINE_SEL_LOW_MASK                             (0xffffffff<<IPC_AP2NP_LINE_SEL_LOW_SHIFT)

/* 0x38 : ap2np_line_sel_high */
#define IPC_AP2NP_LINE_SEL_HIGH_SHIFT                           (0U)
#define IPC_AP2NP_LINE_SEL_HIGH_MASK                            (0xffffffff<<IPC_AP2NP_LINE_SEL_HIGH_SHIFT)

/* 0x3C : ap2np_status */
#define IPC_AP2NP_STATUS_SHIFT                                  (0U)
#define IPC_AP2NP_STATUS_MASK                                   (0xffffffff<<IPC_AP2NP_STATUS_SHIFT)

#endif  /* __IPC_REG_H__ */
