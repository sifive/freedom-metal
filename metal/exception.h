/* Copyright 2020 SiFive, Inc */
/* Copyright © 2020 Keith Packard */
/*
 * Licensed under the Apache License, Version 2.0 (the “License”); you
 * may not use this file except in compliance with the License. A copy
 * of this license can be found in the LICENSE.Apache2 file included
 * with this software. You may also obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/riscv.h>
#include <stdint.h>

/*!
 * @brief At first init, the installed trap vector in mtvec
 */
void early_trap_vector(void);

/*!
 * @brief The default trap vector used by Freedom Metal
 */
void __metal_trap_vector(void) __attribute__((interrupt, aligned(128)));

/*!
 * @brief The trap vector table used by riscv,cpu-intc hardware vectoring
 */
void __metal_vector_table(void);

/*!
 * @brief Calls the appropriate Freedom Metal exception handler
 */
void __metal_exception_handler(riscv_xlen_t mcause);

/*!
 * @brief Calls the appropriate Freedom Metal interrupt handler
 */
void __metal_interrupt_handler(riscv_xlen_t mcause);
