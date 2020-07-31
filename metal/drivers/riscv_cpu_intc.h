/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__RISCV_CPU_INTC_H
#define METAL__DRIVERS__RISCV_CPU_INTC_H

#include <metal/interrupt.h>

#define METAL_MSTATUS_MIE 0x00000008UL

#define METAL_MIE_MSIE 0x00000008UL
#define METAL_MIE_MTIE 0x00000080UL
#define METAL_MIE_MEIE 0x00000800UL

#define METAL_INTERRUPT_ID_SW 3
#define METAL_INTERRUPT_ID_TMR 7
#define METAL_INTERRUPT_ID_EXT 11
#define METAL_INTERRUPT_ID_LC0 16
#define METAL_INTERRUPT_ID_LC1 17
#define METAL_INTERRUPT_ID_LC2 18
#define METAL_INTERRUPT_ID_LC3 19
#define METAL_INTERRUPT_ID_LC4 20
#define METAL_INTERRUPT_ID_LC5 21
#define METAL_INTERRUPT_ID_LC6 22
#define METAL_INTERRUPT_ID_LC7 23
#define METAL_INTERRUPT_ID_LC8 24
#define METAL_INTERRUPT_ID_LC9 25
#define METAL_INTERRUPT_ID_LC10 26
#define METAL_INTERRUPT_ID_LC11 27
#define METAL_INTERRUPT_ID_LC12 28
#define METAL_INTERRUPT_ID_LC13 29
#define METAL_INTERRUPT_ID_LC14 30
#define METAL_INTERRUPT_ID_LC15 31
#define METAL_INTERRUPT_ID_BEU 128

void riscv_cpu_intc_init(struct metal_interrupt controller);

int riscv_cpu_intc_set_vector_mode(struct metal_interrupt controller,
                                   metal_vector_mode mode);

metal_vector_mode
riscv_cpu_intc_get_vector_mode(struct metal_interrupt controller);

int riscv_cpu_intc_clear(struct metal_interrupt controller, int id);

int riscv_cpu_intc_set(struct metal_interrupt controller, int id);

int riscv_cpu_intc_enable(struct metal_interrupt controller, int id);

int riscv_cpu_intc_disable(struct metal_interrupt controller, int id);

int riscv_cpu_intc_set_threshold(struct metal_interrupt controller,
                                 unsigned int level);

unsigned int riscv_cpu_intc_get_threshold(struct metal_interrupt controller);

int riscv_cpu_intc_set_priority(struct metal_interrupt controller, int id,
                                unsigned int priority);

unsigned int riscv_cpu_intc_get_priority(struct metal_interrupt controller,
                                         int id);

int riscv_cpu_intc_set_preemptive_level(struct metal_interrupt controller,
                                        int id, unsigned int level);

unsigned int
riscv_cpu_intc_get_preemptive_level(struct metal_interrupt controller, int id);

int riscv_cpu_intc_vector_enable(struct metal_interrupt controller, int id,
                                 metal_vector_mode mode);

int riscv_cpu_intc_vector_disable(struct metal_interrupt controller, int id);

#endif
