/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_sec_pka.h"
#include "hardware/sec_eng_reg.h"

void qc7xx_pka_init(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_pka_init
    romapi_qc7xx_pka_init(dev);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;

    putreg32(0, reg_base + SEC_ENG_SE_PKA_0_CTRL_0_OFFSET);

    /* Enable sec pka engine in big-endian mode to match Bouffalo SDK. */
    putreg32(SEC_ENG_SE_PKA_0_EN, reg_base + SEC_ENG_SE_PKA_0_CTRL_0_OFFSET);

    regval = getreg32(reg_base + SEC_ENG_SE_PKA_0_CTRL_0_OFFSET);
    regval |= SEC_ENG_SE_PKA_0_ENDIAN;
    putreg32(regval, reg_base + SEC_ENG_SE_PKA_0_CTRL_0_OFFSET);
#endif
}

void qc7xx_pka_deinit(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_pka_deinit
    romapi_qc7xx_pka_deinit(dev);
#else
    putreg32(0, dev->reg_base + SEC_ENG_SE_PKA_0_CTRL_0_OFFSET);
#endif
}
