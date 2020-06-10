/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__RISCV_CPU_INTC_H
#define METAL__DRIVERS__RISCV_CPU_INTC_H

#include <metal/interrupt.h>

#define METAL_MAX_MI 32 /* Per ISA MCause interrupts 32+ are Reserved */
#define METAL_MAX_ME 12 /* Per ISA Exception codes 12+ are Reserved   */
#define METAL_DEFAULT_RTC_FREQ 32768

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
#define RISCV_MCAUSE_INTERRUPT 0x8000000000000000UL
#define RISCV_MCAUSE_CODE_MASK 0x00000000000003FFUL
#endif

#define METAL_MCAUSE_MINHV 0x40000000UL
#define METAL_MCAUSE_MPP 0x30000000UL
#define METAL_MCAUSE_MPIE 0x08000000UL
#define METAL_MCAUSE_MPIL 0x00FF0000UL
#define METAL_MSTATUS_MIE 0x00000008UL
#define METAL_MSTATUS_MPIE 0x00000080UL
#define METAL_MSTATUS_MPP 0x00001800UL
#define METAL_MSTATUS_FS_INIT 0x00002000UL
#define METAL_MSTATUS_FS_CLEAN 0x00004000UL
#define METAL_MSTATUS_FS_DIRTY 0x00006000UL
#define METAL_MSTATUS_MPRV 0x00020000UL
#define METAL_MSTATUS_MXR 0x00080000UL
#define METAL_MINTSTATUS_MIL 0xFF000000UL
#define METAL_MINTSTATUS_SIL 0x0000FF00UL
#define METAL_MINTSTATUS_UIL 0x000000FFUL

#define RISCV_MCAUSE_IS_INTERRUPT(mcause) ((mcause) & RISCV_MCAUSE_INTERRUPT)
#define RISCV_MCAUSE_IS_EXCEPTION(mcause) (!(RISCV_MCAUSE_IS_INTERRUPT(mcause))
#define RISCV_MCAUSE_ID(mcause) ((mcause) & RISCV_MCAUSE_CODE_MASK)

#define METAL_LOCAL_INTR(X) (16 + X)
#define METAL_LOCAL_INTR_EXCEPTION(X) (METAL_MCAUSE_INTR + METAL_LOCAL_INTR(X))
#define METAL_LOCAL_INTR_RESERVE0 1
#define METAL_LOCAL_INTR_RESERVE1 2
#define METAL_LOCAL_INTR_RESERVE2 4
#define METAL_LOCAL_INTR_RESERVE4 16
#define METAL_LOCAL_INTR_RESERVE5 32
#define METAL_LOCAL_INTR_RESERVE6 64
#define METAL_LOCAL_INTERRUPT_TMR 128 /* Bit7 0x080 */
#define METAL_LOCAL_INTR_RESERVE8 256
#define METAL_LOCAL_INTR_RESERVE9 512
#define METAL_LOCAL_INTR_RESERVE10 1024
#define METAL_LOCAL_INTERRUPT_EXT 2048 /* Bit11 0x800 */
/* Bit12 to Bit15 are Reserved */
#define METAL_LOCAL_INTERRUPT(X)                                               \
    (0x10000 << X) /* Bit16+ Start of Custom Local Interrupt */
#define METAL_MIE_INTERRUPT METAL_MSTATUS_MIE

typedef enum {
    METAL_INTERRUPT_ID_BASE,
    METAL_INTERRUPT_ID_SW = (METAL_INTERRUPT_ID_BASE + 3),
    METAL_INTERRUPT_ID_TMR = (METAL_INTERRUPT_ID_BASE + 7),
    METAL_INTERRUPT_ID_EXT = (METAL_INTERRUPT_ID_BASE + 11),
    METAL_INTERRUPT_ID_CSW = (METAL_INTERRUPT_ID_BASE + 12),
    METAL_INTERRUPT_ID_LC0 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(0)),
    METAL_INTERRUPT_ID_LC1 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(1)),
    METAL_INTERRUPT_ID_LC2 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(2)),
    METAL_INTERRUPT_ID_LC3 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(3)),
    METAL_INTERRUPT_ID_LC4 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(4)),
    METAL_INTERRUPT_ID_LC5 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(5)),
    METAL_INTERRUPT_ID_LC6 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(6)),
    METAL_INTERRUPT_ID_LC7 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(7)),
    METAL_INTERRUPT_ID_LC8 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(8)),
    METAL_INTERRUPT_ID_LC9 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(9)),
    METAL_INTERRUPT_ID_LC10 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(10)),
    METAL_INTERRUPT_ID_LC11 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(11)),
    METAL_INTERRUPT_ID_LC12 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(12)),
    METAL_INTERRUPT_ID_LC13 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(13)),
    METAL_INTERRUPT_ID_LC14 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(14)),
    METAL_INTERRUPT_ID_LC15 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(15)),
    METAL_INTERRUPT_ID_LCMX,
    METAL_INTERRUPT_ID_GL0 = METAL_INTERRUPT_ID_LCMX,
    METAL_INTERRUPT_ID_GLMX = (RISCV_MCAUSE_CODE_MASK + 1),
    METAL_INTERRUPT_ID_BEU = 128,
} metal_interrupt_id_e;

typedef enum {
    METAL_IAM_EXCEPTION_CODE = 0,      /* Instruction address misaligned */
    METAL_IAF_EXCEPTION_CODE = 1,      /* Instruction access fault */
    METAL_II_EXCEPTION_CODE = 2,       /* Illegal instruction */
    METAL_BREAK_EXCEPTION_CODE = 3,    /* Breakpoint */
    METAL_LAM_EXCEPTION_CODE = 4,      /* Load address misaligned */
    METAL_LAF_EXCEPTION_CODE = 5,      /* Load access fault */
    METAL_SAMOAM_EXCEPTION_CODE = 6,   /* Store/AMO address misaligned */
    METAL_SAMOAF_EXCEPTION_CODE = 7,   /* Store/AMO access fault */
    METAL_ECALL_U_EXCEPTION_CODE = 8,  /* Environment call from U-mode */
    METAL_R9_EXCEPTION_CODE = 9,       /* Reserved */
    METAL_R10_EXCEPTION_CODE = 10,     /* Reserved */
    METAL_ECALL_M_EXCEPTION_CODE = 11, /* Environment call from M-mode */
    METAL_MAX_EXCEPTION_CODE = 12,
} metal_exception_code_e;

void __metal_driver_riscv_cpu_intc_init(struct metal_interrupt controller);

int
__metal_driver_riscv_cpu_intc_set_vector_mode(struct metal_interrupt controller,
                                              metal_vector_mode mode);

metal_vector_mode
__metal_driver_riscv_cpu_intc_get_vector_mode(struct metal_interrupt controller);

int __metal_driver_riscv_cpu_intc_set_privilege(struct metal_interrupt controller,
                                                metal_intr_priv_mode privilege);

metal_intr_priv_mode
__metal_driver_riscv_cpu_intc_get_privilege(struct metal_interrupt controller);

int __metal_driver_riscv_cpu_intc_clear(struct metal_interrupt controller,
                                        int id);

int __metal_driver_riscv_cpu_intc_set(struct metal_interrupt controller, int id);

int
__metal_driver_riscv_cpu_intc_register_handler(struct metal_interrupt controller, int id,
                                               metal_interrupt_handler_t handler,
                                               void *priv_data);

int __metal_driver_riscv_cpu_intc_register_vector_handler(
    struct metal_interrupt controller, int id,
    metal_interrupt_vector_handler_t handler, void *priv_data);

int __metal_driver_riscv_cpu_intc_enable(struct metal_interrupt controller, int id);

int __metal_driver_riscv_cpu_intc_disable(struct metal_interrupt controller, int id);

int __metal_driver_riscv_cpu_intc_set_threshold(struct metal_interrupt controller,
                                                unsigned int level);

unsigned int
__metal_driver_riscv_cpu_intc_get_threshold(struct metal_interrupt controller);

int __metal_driver_riscv_cpu_intc_set_priority(struct metal_interrupt controller,
                                               int id, unsigned int priority);

unsigned int
__metal_driver_riscv_cpu_intc_get_priority(struct metal_interrupt controller, int id);

int
__metal_driver_riscv_cpu_intc_set_preemptive_level(struct metal_interrupt controller, int id,
                                                   unsigned int level);

unsigned int
__metal_driver_riscv_cpu_intc_get_preemptive_level(struct metal_interrupt controller, int id);

int __metal_driver_riscv_cpu_intc_vector_enable(struct metal_interrupt controller, int id);

int
__metal_driver_riscv_cpu_intc_vector_disable(struct metal_interrupt controller, int id);

metal_affinity
__metal_driver_riscv_cpu_intc_affinity_enable(struct metal_interrupt controller,
                                              metal_affinity bitmask, int id);

metal_affinity
__metal_driver_riscv_cpu_intc_affinity_disable(struct metal_interrupt controller,
                                               metal_affinity bitmask, int id);

metal_affinity
__metal_driver_riscv_cpu_intc_affinity_set_threshold(struct metal_interrupt controller,
                                                     metal_affinity bitmask,
                                                     unsigned int level);

unsigned int
__metal_driver_riscv_cpu_intc_affinity_get_threshold(struct metal_interrupt controller,
                                                     int context_id);

#endif