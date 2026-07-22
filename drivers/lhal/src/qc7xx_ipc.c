/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_ipc.h"
#include "hardware/ipc_reg.h"

void qc7xx_ipc_init(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_ipc_init
    return romapi_qc7xx_ipc_init(dev);
#else
    /* clear all ipc bits and mask all int */
    if (dev->sub_idx) {
        putreg32(IPC_BITS_ALL, dev->reg_base + IPC_NP2AP_ACK_OFFSET);
        putreg32(IPC_BITS_ALL, dev->reg_base + IPC_NP2AP_UNMASK_CLEAR_OFFSET);
    } else {
        putreg32(IPC_BITS_ALL, dev->reg_base + IPC_AP2NP_ACK_OFFSET);
        putreg32(IPC_BITS_ALL, dev->reg_base + IPC_AP2NP_UNMASK_CLEAR_OFFSET);
    }

#endif
}

void qc7xx_ipc_deinit(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_ipc_deinit
    return romapi_qc7xx_ipc_deinit(dev);
#else
    /* clear all ipc bits and mask all int */
    if (dev->sub_idx) {
        putreg32(IPC_BITS_ALL, dev->reg_base + IPC_NP2AP_ACK_OFFSET);
        putreg32(IPC_BITS_ALL, dev->reg_base + IPC_NP2AP_UNMASK_CLEAR_OFFSET);
    } else {
        putreg32(IPC_BITS_ALL, dev->reg_base + IPC_AP2NP_ACK_OFFSET);
        putreg32(IPC_BITS_ALL, dev->reg_base + IPC_AP2NP_UNMASK_CLEAR_OFFSET);
    }
#endif
}

void qc7xx_ipc_int_mask(struct qc7xx_device_s *dev, uint32_t ipc_bits)
{
#ifdef romapi_qc7xx_ipc_int_mask
    return romapi_qc7xx_ipc_int_mask(dev, ipc_bits);
#else
    if (dev->sub_idx) {
        putreg32(ipc_bits, dev->reg_base + IPC_NP2AP_UNMASK_CLEAR_OFFSET);
    } else {
        putreg32(ipc_bits, dev->reg_base + IPC_AP2NP_UNMASK_CLEAR_OFFSET);
    }
#endif
}

void qc7xx_ipc_int_unmask(struct qc7xx_device_s *dev, uint32_t ipc_bits)
{
#ifdef romapi_qc7xx_ipc_int_unmask
    return romapi_qc7xx_ipc_int_unmask(dev, ipc_bits);
#else
    if (dev->sub_idx) {
        putreg32(ipc_bits, dev->reg_base + IPC_NP2AP_UNMASK_SET_OFFSET);
    } else {
        putreg32(ipc_bits, dev->reg_base + IPC_AP2NP_UNMASK_SET_OFFSET);
    }
#endif
}

void qc7xx_ipc_trig(struct qc7xx_device_s *dev, uint32_t ipc_bits)
{
#ifdef romapi_qc7xx_ipc_trig
    return romapi_qc7xx_ipc_trig(dev, ipc_bits);
#else
    if (dev->sub_idx) {
        putreg32(ipc_bits, dev->reg_base + IPC_NP2AP_TRIGGER_OFFSET);
    } else {
        putreg32(ipc_bits, dev->reg_base + IPC_AP2NP_TRIGGER_OFFSET);
    }
#endif
}

void qc7xx_ipc_clear(struct qc7xx_device_s *dev, uint32_t ipc_bits)
{
#ifdef romapi_qc7xx_ipc_clear
    return romapi_qc7xx_ipc_clear(dev, ipc_bits);
#else
    if (dev->sub_idx) {
        putreg32(ipc_bits, dev->reg_base + IPC_NP2AP_ACK_OFFSET);
    } else {
        putreg32(ipc_bits, dev->reg_base + IPC_AP2NP_ACK_OFFSET);
    }
#endif
}

uint32_t qc7xx_ipc_get_sta(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_ipc_get_sta
    return romapi_qc7xx_ipc_get_sta(dev);
#else
    uint32_t regval;
    if (dev->sub_idx) {
        regval = getreg32(dev->reg_base + IPC_NP2AP_RAW_STATUS_OFFSET);
    } else {
        regval = getreg32(dev->reg_base + IPC_AP2NP_RAW_STATUS_OFFSET);
    }
    return regval;
#endif
}

uint32_t qc7xx_ipc_get_intsta(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_ipc_get_intsta
    return romapi_qc7xx_ipc_get_intsta(dev);
#else
    uint32_t regval;
    if (dev->sub_idx) {
        regval = getreg32(dev->reg_base + IPC_NP2AP_STATUS_OFFSET);
    } else {
        regval = getreg32(dev->reg_base + IPC_AP2NP_STATUS_OFFSET);
    }
    return regval;
#endif
}
