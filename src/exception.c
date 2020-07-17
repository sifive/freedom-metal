/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/exception.h>
#include <metal/interrupt.h>
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
        __metal_exception_handler(mcause);
    } else {
        __metal_interrupt_handler(mcause);
    }
}

void __metal_exception_handler(riscv_xlen_t mcause) {
    const int id = RISCV_MCAUSE_ID(mcause);
    const struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());
    __metal_exception_table[id](cpu, id);
}

void __metal_interrupt_handler(riscv_xlen_t mcause) {
    const int id = RISCV_MCAUSE_ID(mcause);
    __metal_local_interrupt_table[id]();
}
