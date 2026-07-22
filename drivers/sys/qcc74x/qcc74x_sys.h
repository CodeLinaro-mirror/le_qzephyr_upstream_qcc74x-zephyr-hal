/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __QCC74X_SYS_H__
#define __QCC74X_SYS_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    QC7XX_RST_POWER_OFF = 0,
    QC7XX_RST_HARDWARE_WATCHDOG,
    QC7XX_RST_FATAL_EXCEPTION,
    QC7XX_RST_SOFTWARE_WATCHDOG,
    QC7XX_RST_SOFTWARE,
    QC7XX_RST_HBN,
    QC7XX_RST_BOD,
} QC7XX_RST_REASON_E;

QC7XX_RST_REASON_E qc7xx_sys_rstinfo_get(void);
int qc7xx_sys_rstinfo_set(QC7XX_RST_REASON_E val);
int qc7xx_sys_rstinfo_getsting(char *info);
char *qc7xx_sys_rstinfo_getstring(void);
void qc7xx_sys_rstinfo_init(void);
int qc7xx_sys_reset_por(void);
void qc7xx_sys_reset_system(void);
void qc7xx_sys_reset_system_from_interface(void);
int qc7xx_sys_isxipaddr(uint32_t addr);
int qc7xx_sys_init(void);

void qc7xx_cpu_sysmap_init(bool dcache_preload_en, bool dcache_amr_en);

#endif
