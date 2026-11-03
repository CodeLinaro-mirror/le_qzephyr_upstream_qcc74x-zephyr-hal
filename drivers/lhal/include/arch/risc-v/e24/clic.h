/*
 * Copyright 2016 SiFive, Inc.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause
 * Not a contribution.
 */

// See LICENSE for license details.

#ifndef _SIFIVE_CLIC_H
#define _SIFIVE_CLIC_H

#define CLIC_CTRL_BASE  0x02000000UL
#define CLIC_HART0_BASE 0x02800000UL

#define CLIC_MSIP_OFFSET     0x0000
#define CLIC_MTIMECMP_OFFSET 0x4000
#define CLIC_MTIME_OFFSET    0xBFF8
#define CLIC_INTIP_OFFSET    0x000
#define CLIC_INTIE_OFFSET    0x400
#define CLIC_INTCFG_OFFSET   0x800
#define CLIC_CFG_OFFSET      0xc00

#endif /* _SIFIVE_CLIC_H */
