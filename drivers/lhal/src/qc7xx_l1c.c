/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_l1c.h"
#include "qc7xx_core.h"

#if (defined(QCC74X) || defined(QCC75X)) && !defined(CPU_LP)
#include "csi_core.h"
void qc7xx_l1c_icache_enable(void)
{
#ifdef romapi_qc7xx_l1c_icache_enable
    romapi_qc7xx_l1c_icache_enable();
#else
    csi_icache_enable();
#endif
}

void qc7xx_l1c_icache_disable(void)
{
#ifdef romapi_qc7xx_l1c_icache_disable
    romapi_qc7xx_l1c_icache_disable();
#else
    csi_icache_disable();
#endif
}

ATTR_TCM_SECTION void qc7xx_l1c_icache_invalid_all(void)
{
#ifdef romapi_qc7xx_l1c_icache_invalid_all
    romapi_qc7xx_l1c_icache_invalid_all();
#else
    csi_icache_invalid();
#endif
}

ATTR_TCM_SECTION void qc7xx_l1c_icache_invalid_range(void *addr, uint32_t size)
{
#ifdef romapi_qc7xx_l1c_icache_invalid_range
    romapi_qc7xx_l1c_icache_invalid_range(addr, size);
#else
    if (qc7xx_check_cache_addr(addr)) {
        csi_icache_invalid_range(addr, size);
    }
#endif
}

void qc7xx_l1c_dcache_enable(void)
{
#ifdef romapi_qc7xx_l1c_dcache_enable
    romapi_qc7xx_l1c_dcache_enable();
#else
    csi_dcache_enable();
#endif
}

void qc7xx_l1c_dcache_disable(void)
{
#ifdef romapi_qc7xx_l1c_dcache_disable
    romapi_qc7xx_l1c_dcache_disable();
#else
    csi_dcache_disable();
#endif
}

ATTR_TCM_SECTION void qc7xx_l1c_dcache_clean_all(void)
{
#ifdef romapi_qc7xx_l1c_dcache_clean_all
    romapi_qc7xx_l1c_dcache_clean_all();
#else
    csi_dcache_clean();
#endif
}

ATTR_TCM_SECTION void qc7xx_l1c_dcache_invalidate_all(void)
{
#ifdef romapi_qc7xx_l1c_dcache_invalidate_all
    romapi_qc7xx_l1c_dcache_invalidate_all();
#else
    csi_dcache_invalid();
#endif
}

ATTR_TCM_SECTION void qc7xx_l1c_dcache_clean_invalidate_all(void)
{
#ifdef romapi_qc7xx_l1c_dcache_clean_invalidate_all
    romapi_qc7xx_l1c_dcache_clean_invalidate_all();
#else
    csi_dcache_clean_invalid();
#endif
}

ATTR_TCM_SECTION void qc7xx_l1c_dcache_clean_range(void *addr, uint32_t size)
{
#ifdef romapi_qc7xx_l1c_dcache_clean_range
    romapi_qc7xx_l1c_dcache_clean_range(addr, size);
#else
    if (qc7xx_check_cache_addr(addr)) {
        csi_dcache_clean_range(addr, size);
    }
#endif
}

ATTR_TCM_SECTION void qc7xx_l1c_dcache_invalidate_range(void *addr, uint32_t size)
{
#ifdef romapi_qc7xx_l1c_dcache_invalidate_range
    romapi_qc7xx_l1c_dcache_invalidate_range(addr, size);
#else
    if (qc7xx_check_cache_addr(addr)) {
        csi_dcache_invalid_range(addr, size);
    }
#endif
}

ATTR_TCM_SECTION void qc7xx_l1c_dcache_clean_invalidate_range(void *addr, uint32_t size)
{
#ifdef romapi_qc7xx_l1c_dcache_clean_invalidate_range
    romapi_qc7xx_l1c_dcache_clean_invalidate_range(addr, size);
#else
    if (qc7xx_check_cache_addr(addr)) {
        csi_dcache_clean_invalid_range(addr, size);
    }
#endif
}
#else

void qc7xx_l1c_icache_enable(void)
{
}

void qc7xx_l1c_icache_disable(void)
{
}

void qc7xx_l1c_icache_invalid_all(void)
{
}

void qc7xx_l1c_icache_invalid_range(void *addr, uint32_t size)
{
}

void qc7xx_l1c_dcache_enable(void)
{
}

void qc7xx_l1c_dcache_disable(void)
{
}

void qc7xx_l1c_dcache_clean_all(void)
{
}

void qc7xx_l1c_dcache_invalidate_all(void)
{
}

void qc7xx_l1c_dcache_clean_invalidate_all(void)
{
}

void qc7xx_l1c_dcache_clean_range(void *addr, uint32_t size)
{
}

ATTR_TCM_SECTION void qc7xx_l1c_dcache_invalidate_range(void *addr, uint32_t size)
{
}

ATTR_TCM_SECTION void qc7xx_l1c_dcache_clean_invalidate_range(void *addr, uint32_t size)
{
}

/****************************************************************************/ /**
 * @brief  Get hit count
 *
 * @param  hit_count_low: hit count low 32 bits pointer
 * @param  hit_count_high: hit count high 32 bits pointer
 *
 * @return None
 *
*******************************************************************************/
__WEAK
void ATTR_TCM_SECTION qc7xx_l1c_hit_count_get(uint32_t *hit_count_low, uint32_t *hit_count_high)
{
    *hit_count_low = getreg32(0x40009000 + 0x4);
    *hit_count_high = getreg32(0x40009000 + 0x8);
}

/****************************************************************************/ /**
 * @brief  Get miss count
 *
 * @param  None
 *
 * @return Miss count
 *
*******************************************************************************/
__WEAK
uint32_t ATTR_TCM_SECTION qc7xx_l1c_miss_count_get(void)
{
    return getreg32(0x40009000 + 0xC);
}

#endif