/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__RISCV_CPU_INTC_H
#define METAL__DRIVERS__RISCV_CPU_INTC_H

#include <metal/interrupt.h>

#define METAL_MTVEC_MODE 0x03
#define METAL_MTVEC_DIRECT 0x00
#define METAL_MTVEC_VECTORED 0x01
#define METAL_MTVEC_CLIC 0x02
#define METAL_MTVEC_CLIC_VECTORED 0x03
#define METAL_MTVEC_CLIC_RESERVED 0x3C

#define METAL_MTVEC_MODE_MASK 0x3F

#if __riscv_xlen == 32
#define RISCV_MCAUSE_INTERRUPT 0x80000000UL
#define RISCV_MCAUSE_CODE_MASK 0x000003FFUL
#else
#define RISCV_MCAUSE_INTERRUPT 0x8000000000000000ULL
#define RISCV_MCAUSE_CODE_MASK 0x00000000000003FFULL
#endif
#define RISCV_MCAUSE_IS_INTERRUPT(mcause) ((mcause) & RISCV_MCAUSE_INTERRUPT)
#define RISCV_MCAUSE_IS_EXCEPTION(mcause) (!(RISCV_MCAUSE_IS_INTERRUPT(mcause))
#define RISCV_MCAUSE_ID(mcause) ((mcause) & RISCV_MCAUSE_CODE_MASK)

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

int
riscv_cpu_intc_set_vector_mode(struct metal_interrupt controller,
                                              metal_vector_mode mode);

metal_vector_mode
riscv_cpu_intc_get_vector_mode(struct metal_interrupt controller);

int riscv_cpu_intc_set_privilege(struct metal_interrupt controller,
                                                metal_intr_priv_mode privilege);

metal_intr_priv_mode
riscv_cpu_intc_get_privilege(struct metal_interrupt controller);

int riscv_cpu_intc_clear(struct metal_interrupt controller,
                                        int id);

int riscv_cpu_intc_set(struct metal_interrupt controller, int id);

int riscv_cpu_intc_enable(struct metal_interrupt controller, int id);

int riscv_cpu_intc_disable(struct metal_interrupt controller, int id);

int riscv_cpu_intc_set_threshold(struct metal_interrupt controller,
                                                unsigned int level);

unsigned int
riscv_cpu_intc_get_threshold(struct metal_interrupt controller);

int riscv_cpu_intc_set_priority(struct metal_interrupt controller,
                                               int id, unsigned int priority);

unsigned int
riscv_cpu_intc_get_priority(struct metal_interrupt controller, int id);

int
riscv_cpu_intc_set_preemptive_level(struct metal_interrupt controller, int id,
                                                   unsigned int level);

unsigned int
riscv_cpu_intc_get_preemptive_level(struct metal_interrupt controller, int id);

int riscv_cpu_intc_vector_enable(struct metal_interrupt controller, int id, metal_vector_mode mode);

int
riscv_cpu_intc_vector_disable(struct metal_interrupt controller, int id);

metal_affinity
riscv_cpu_intc_affinity_enable(struct metal_interrupt controller,
                                              metal_affinity bitmask, int id);

metal_affinity
riscv_cpu_intc_affinity_disable(struct metal_interrupt controller,
                                               metal_affinity bitmask, int id);

metal_affinity
riscv_cpu_intc_affinity_set_threshold(struct metal_interrupt controller,
                                                     metal_affinity bitmask,
                                                     unsigned int level);

unsigned int
riscv_cpu_intc_affinity_get_threshold(struct metal_interrupt controller,
                                                     int context_id);

#endif
