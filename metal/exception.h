/* Copyright 2020 SiFive, Inc */
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
