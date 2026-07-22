/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_EMAC_H
#define _QC7XX_EMAC_H

#include "qc7xx_emac_common.h"

/* Buffer hardware descriptor table:
 * |---------------------|-------------------|---------------------------|
 * |   Address field     |       Length      |         Attribute         |
 * |---------------------|-------------------|---------------------------|
 * |63                 32|31               16|15                       00|
 * |---------------------|-------------------|---------------------------|
 * |   32-bit address    |   16-bit length   | 16-bit control and status |
 * |---------------------|-------------------|---------------------------|
 *
*/
struct qc7xx_emac_hw_buff_desc_s {
    uint16_t attribute; /* The control and status field. */
    uint16_t length;    /* The length field. */
    uint32_t address;   /* The address field. */
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif