/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_sec_gmac.h"
#include "hardware/sec_eng_reg.h"

void qc7xx_sec_gmac_le_enable(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_sec_gmac_le_enable
    romapi_qc7xx_sec_gmac_le_enable(dev);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;

    regval = getreg32(reg_base + SEC_ENG_SE_GMAC_0_CTRL_0_OFFSET);
    regval &= ~SEC_ENG_SE_GMAC_0_T_ENDIAN;
    regval &= ~SEC_ENG_SE_GMAC_0_H_ENDIAN;
    regval &= ~SEC_ENG_SE_GMAC_0_X_ENDIAN;
    putreg32(regval, reg_base + SEC_ENG_SE_GMAC_0_CTRL_0_OFFSET);
#endif
}

void qc7xx_sec_gmac_link_enable(struct qc7xx_device_s *dev, uint8_t enable)
{
#ifdef romapi_qc7xx_sec_gmac_link_enable
    romapi_qc7xx_sec_gmac_link_enable(dev, enable);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;

    regval = getreg32(reg_base + SEC_ENG_SE_GMAC_0_CTRL_0_OFFSET);

    if (enable) {
        regval |= SEC_ENG_SE_GMAC_0_EN;
    } else {
        regval &= ~SEC_ENG_SE_GMAC_0_EN;
    }
    putreg32(regval, reg_base + SEC_ENG_SE_GMAC_0_CTRL_0_OFFSET);
#endif
}

int qc7xx_sec_gmac_link_work(struct qc7xx_device_s *dev, uint32_t addr, const uint8_t *in, uint32_t len, uint8_t *out)
{
#ifdef romapi_qc7xx_sec_gmac_link_work
    return romapi_qc7xx_sec_gmac_link_work(dev, addr, in, len, out);
#else
    uint32_t regval;
    uint32_t reg_base;
    uint64_t start_time;

    reg_base = dev->reg_base;

    /* Link address should word align */
    if ((addr & 0x03) != 0 || len % 16 != 0) {
        return -EINVAL;
    }

    /* Set link address */
    putreg32(addr, reg_base + SEC_ENG_SE_GMAC_0_LCA_OFFSET);

    /* Change source buffer address */
    *(uint32_t *)(uintptr_t)(addr + 4) = (uint32_t)(uintptr_t)in;

    /* Set data length */
    *((uint16_t *)(uintptr_t)addr + 1) = len / 16;

    /* Start gmac engine and wait finishing */
    regval = getreg32(reg_base + SEC_ENG_SE_GMAC_0_CTRL_0_OFFSET);
    regval |= SEC_ENG_SE_GMAC_0_TRIG_1T;
    putreg32(regval, reg_base + SEC_ENG_SE_GMAC_0_CTRL_0_OFFSET);

    start_time = qc7xx_mtimer_get_time_ms();
    while (getreg32(reg_base + SEC_ENG_SE_GMAC_0_CTRL_0_OFFSET) & SEC_ENG_SE_GMAC_0_BUSY) {
        if ((qc7xx_mtimer_get_time_ms() - start_time) > 100) {
            return -ETIMEDOUT;
        }
    }

    /* Get result */
    arch_memcpy_fast(out, (uint8_t *)(uintptr_t)(addr + 0x18), 16);

    return 0;
#endif
}

void qc7xx_group0_request_gmac_access(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_group0_request_gmac_access
    romapi_qc7xx_group0_request_gmac_access(dev);
#else
    uint32_t regval;
    uint32_t reg_base;

    reg_base = dev->reg_base;

    regval = getreg32(reg_base + SEC_ENG_SE_CTRL_PROT_RD_OFFSET);
    if (((regval >> 10) & 0x03) == 0x03) {
        putreg32(0x02, reg_base + SEC_ENG_SE_GMAC_0_CTRL_PROT_OFFSET);

        regval = getreg32(reg_base + SEC_ENG_SE_CTRL_PROT_RD_OFFSET);
        if (((regval >> 10) & 0x03) == 0x01) {
        }
    }
#endif
}

void qc7xx_group0_release_gmac_access(struct qc7xx_device_s *dev)
{
#ifdef romapi_qc7xx_group0_release_gmac_access
    romapi_qc7xx_group0_release_gmac_access(dev);
#else
    uint32_t reg_base;

    reg_base = dev->reg_base;

    putreg32(0x06, reg_base + SEC_ENG_SE_GMAC_0_CTRL_PROT_OFFSET);
#endif
}
