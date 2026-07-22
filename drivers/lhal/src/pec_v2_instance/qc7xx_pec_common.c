/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "qc7xx_pec_v2.h"
#include "qc7xx_pec_common.h"

int qc7xx_pec_memory_size_check(struct qc7xx_device_s *dev, uint8_t start, uint16_t size, uint8_t *actual_start)
{
    uint8_t mem;

    mem = start;
    if (mem + size > PEC_MEMORY_SIZE) {
        return PEC_STS_MEMORY_LACK;
    }
    *actual_start = mem;
    qc7xx_pec_mem_store(dev, start);

    return PEC_STS_OK;
}
