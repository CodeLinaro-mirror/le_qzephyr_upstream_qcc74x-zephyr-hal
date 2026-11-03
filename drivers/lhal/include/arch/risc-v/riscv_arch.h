/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause
 * Not a contribution.
 */

#ifndef _RISCV_ARCH_H
#define _RISCV_ARCH_H

#define getreg8(a)                         (*(volatile uint8_t *)(uintptr_t)(a))
#define putreg8(v, a)                      (*(volatile uint8_t *)(uintptr_t)(a) = (v))
#define getreg16(a)                        (*(volatile uint16_t *)(uintptr_t)(a))
#define putreg16(v, a)                     (*(volatile uint16_t *)(uintptr_t)(a) = (v))
#define getreg32(a)                        (*(volatile uint32_t *)(uintptr_t)(a))
#define putreg32(v, a)                     (*(volatile uint32_t *)(uintptr_t)(a) = (v))
// #define getreg64(a)                        (*(volatile uint64_t *)(a))
// #define putreg64(v, a)                     (*(volatile uint64_t *)(a) = (v))
// #define modifyreg32(a, clearbits, setbits) putreg32((getreg32(a) & (~clearbits)) | setbits, a)

#endif
