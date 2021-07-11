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

#include <metal/exception.h>
#include <metal/interrupt.h>
#include <metal/platform.h>
#include <metal/riscv.h>
#include <stdint.h>

void __metal_trap_vector(void) {
    riscv_xlen_t mcause;
    __asm__ volatile("csrr %0, mcause" : "=r"(mcause));

#ifdef DEBUG
    /* Reading these CSRs for a debug build is helpful to figure out what
     * is going wrong, but is not necessary for any actual functionality.
     */
    riscv_xlen_t mepc, mtval;
    __asm__ volatile("csrr %0, mepc" : "=r"(mepc));
    __asm__ volatile("csrr %0, mtval" : "=r"(mtval));
#endif

    if (RISCV_MCAUSE_IS_INTERRUPT(mcause)) {
        __metal_interrupt_handler(mcause);
    } else {
        __metal_exception_handler(mcause);
    }
}

void __metal_exception_handler(riscv_xlen_t mcause) {
    const int id = RISCV_MCAUSE_ID(mcause);
    const struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());

    __metal_exception_table[id](cpu, id);
}

void __metal_interrupt_handler(riscv_xlen_t mcause) {
    const int id = RISCV_MCAUSE_ID(mcause);

#ifdef METAL_SIFIVE_BUSERROR0
    /* The SiFive Bus Error Unit has a hard-coded local interrupt id of 128 */
    if (id == 128) {
        metal_sifive_buserror0_source_0_handler();
        return;
    }
#endif

    __metal_local_interrupt_table[id]();
}
