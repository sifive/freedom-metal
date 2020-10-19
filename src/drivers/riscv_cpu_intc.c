/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/drivers/riscv_cpu_intc.h>
#include <metal/exception.h>
#include <metal/init.h>
#include <metal/interrupt.h>
#include <metal/io.h>
#include <metal/platform.h>
#include <metal/riscv.h>
#include <stdbool.h>
#include <stdint.h>

#define get_index(intc) ((intc).__interrupt_index)

static bool init_done[__METAL_DT_NUM_HARTS] = {false};

void riscv_cpu_intc_init(struct metal_interrupt intc) {
    assert(get_index(intc) == metal_cpu_get_current_hartid());

    if (!init_done[get_index(intc)]) {
        /*
         * Set the real trap handler if the value of mtvec is equal to
         * early_trap_vector. If mtvec is not equal to early_trap_vector,
         * that means user has own trap handler, then we don't overwrite it.
         */
        uintptr_t mtvec;
        __asm__ volatile("csrr %0, mtvec" : "=r"(mtvec));
        if (mtvec == (uintptr_t)&early_trap_vector) {
#ifdef METAL_SIFIVE_CLIC0
            riscv_cpu_intc_set_vector_mode(intc, METAL_CLIC_DIRECT_MODE);
#else
            riscv_cpu_intc_set_vector_mode(intc, METAL_DIRECT_MODE);
#endif
        }

#ifdef METAL_RISCV_PLIC0
        /* When the PLIC exists, automatically enable the external interrupt
         * line.
         */
        riscv_cpu_intc_enable(intc, METAL_INTERRUPT_ID_EXT);
#endif

        init_done[get_index(intc)] = 1;
    }
}

int riscv_cpu_intc_set_vector_mode(struct metal_interrupt controller,
                                   metal_vector_mode mode) {
    assert(get_index(controller) == metal_cpu_get_current_hartid());
    switch (mode) {
#ifndef METAL_SIFIVE_CLIC0
    default:
    case METAL_CLIC_DIRECT_MODE:
    case METAL_DIRECT_MODE:
        /* Write the trap vector address with the vectored bit unset */
        __asm__ volatile(
            "csrw mtvec, %0" ::"r"((uintptr_t)__metal_trap_vector));
        break;
    case METAL_CLIC_VECTORED_MODE:
    case METAL_VECTORED_MODE:
        /* Write the jump table address with the vectored bit set */
        __asm__ volatile("csrw mtvec, %0" ::"r"(
            (uintptr_t)__metal_vector_table | RISCV_MTVEC_MODE_VECTORED));
        break;
#else
    default:
    case METAL_DIRECT_MODE:
        /* Write the trap vector address with the vectored bit unset */
        __asm__ volatile(
            "csrw mtvec, %0" ::"r"((uintptr_t)__metal_trap_vector));
        break;
    case METAL_VECTORED_MODE:
        /* Write the jump table address with the vectored bit set */
        __asm__ volatile("csrw mtvec, %0" ::"r"(
            (uintptr_t)__metal_vector_table | RISCV_MTVEC_MODE_VECTORED));
        break;
    case METAL_CLIC_DIRECT_MODE:
        __asm__ volatile("csrw mtvec, %0" ::"r"((uintptr_t)__metal_trap_vector |
                                                RISCV_MTVEC_MODE_CLIC_DIRECT));
        __asm__ volatile(
            "csrw 0x307, %0" ::"r"((uintptr_t)__metal_local_interrupt_table));
        break;
    case METAL_CLIC_VECTORED_MODE:
        __asm__ volatile("csrw mtvec, %0" ::"r"(
            (uintptr_t)__metal_trap_vector | RISCV_MTVEC_MODE_CLIC_VECTORED));
        __asm__ volatile(
            "csrw 0x307, %0" ::"r"((uintptr_t)__metal_local_interrupt_table));
        break;
#endif
    }

    return 0;
}

metal_vector_mode
riscv_cpu_intc_get_vector_mode(struct metal_interrupt controller) {
    assert(get_index(controller) == metal_cpu_get_current_hartid());

    uintptr_t mtvec;
    __asm__ volatile("csrr %0, mtvec" : "=r"(mtvec));

    switch (__METAL_GET_FIELD(mtvec, RISCV_MTVEC_MODE_MASK)) {
    default:
    case RISCV_MTVEC_MODE_DIRECT:
        return METAL_DIRECT_MODE;
    case RISCV_MTVEC_MODE_VECTORED:
        return METAL_VECTORED_MODE;
    case RISCV_MTVEC_MODE_CLIC_DIRECT:
        return METAL_CLIC_DIRECT_MODE;
    case RISCV_MTVEC_MODE_CLIC_VECTORED:
        return METAL_CLIC_VECTORED_MODE;
    }
}

int riscv_cpu_intc_enable(struct metal_interrupt controller, int id) {
    assert(get_index(controller) == metal_cpu_get_current_hartid());

    if (id >= __METAL_NUM_LOCAL_INTERRUPTS) {
        return -1;
    }

    switch (id) {
    case METAL_INTERRUPT_ID_SW:
        __asm__ volatile("csrs mie, %0" ::"r"(METAL_MIE_MSIE));
        break;
    case METAL_INTERRUPT_ID_TMR:
        __asm__ volatile("csrs mie, %0" ::"r"(METAL_MIE_MTIE));
        break;
    case METAL_INTERRUPT_ID_EXT:
        __asm__ volatile("csrs mie, %0" ::"r"(METAL_MIE_MEIE));
        break;
    case METAL_INTERRUPT_ID_LC0:
    case METAL_INTERRUPT_ID_LC1:
    case METAL_INTERRUPT_ID_LC2:
    case METAL_INTERRUPT_ID_LC3:
    case METAL_INTERRUPT_ID_LC4:
    case METAL_INTERRUPT_ID_LC5:
    case METAL_INTERRUPT_ID_LC6:
    case METAL_INTERRUPT_ID_LC7:
    case METAL_INTERRUPT_ID_LC8:
    case METAL_INTERRUPT_ID_LC9:
    case METAL_INTERRUPT_ID_LC10:
    case METAL_INTERRUPT_ID_LC11:
    case METAL_INTERRUPT_ID_LC12:
    case METAL_INTERRUPT_ID_LC13:
    case METAL_INTERRUPT_ID_LC14:
    case METAL_INTERRUPT_ID_LC15:
        __asm__ volatile("csrs mie, %0" ::"r"(1 << id));
        break;
    default:
        return -1;
    }
    return 0;
}

int riscv_cpu_intc_disable(struct metal_interrupt controller, int id) {
    assert(get_index(controller) == metal_cpu_get_current_hartid());

    if (id >= __METAL_NUM_LOCAL_INTERRUPTS) {
        return -1;
    }

    switch (id) {
    case METAL_INTERRUPT_ID_SW:
        __asm__ volatile("csrc mie, %0" ::"r"(METAL_MIE_MSIE));
        break;
    case METAL_INTERRUPT_ID_TMR:
        __asm__ volatile("csrc mie, %0" ::"r"(METAL_MIE_MTIE));
        break;
    case METAL_INTERRUPT_ID_EXT:
        __asm__ volatile("csrc mie, %0" ::"r"(METAL_MIE_MEIE));
        break;
    case METAL_INTERRUPT_ID_LC0:
    case METAL_INTERRUPT_ID_LC1:
    case METAL_INTERRUPT_ID_LC2:
    case METAL_INTERRUPT_ID_LC3:
    case METAL_INTERRUPT_ID_LC4:
    case METAL_INTERRUPT_ID_LC5:
    case METAL_INTERRUPT_ID_LC6:
    case METAL_INTERRUPT_ID_LC7:
    case METAL_INTERRUPT_ID_LC8:
    case METAL_INTERRUPT_ID_LC9:
    case METAL_INTERRUPT_ID_LC10:
    case METAL_INTERRUPT_ID_LC11:
    case METAL_INTERRUPT_ID_LC12:
    case METAL_INTERRUPT_ID_LC13:
    case METAL_INTERRUPT_ID_LC14:
    case METAL_INTERRUPT_ID_LC15:
        __asm__ volatile("csrc mie, %0" ::"r"(1 << id));
        break;
    default:
        return -1;
    }
    return 0;
}

int riscv_cpu_intc_set_threshold(struct metal_interrupt controller,
                                 unsigned int level) {
    return -1;
}

unsigned int riscv_cpu_intc_get_threshold(struct metal_interrupt controller) {
    return 0;
}

int riscv_cpu_intc_set_priority(struct metal_interrupt controller, int id,
                                unsigned int priority) {
    return -1;
}

unsigned int riscv_cpu_intc_get_priority(struct metal_interrupt controller,
                                         int id) {
    return 0;
}

int riscv_cpu_intc_vector_enable(struct metal_interrupt controller, int id,
                                 metal_vector_mode mode) {
    switch (riscv_cpu_intc_get_vector_mode(controller)) {
    case METAL_CLIC_VECTORED_MODE:
    case METAL_VECTORED_MODE:
        return 0;
    default:
        return -1;
    }
}

int riscv_cpu_intc_vector_disable(struct metal_interrupt controller, int id) {

    switch (riscv_cpu_intc_get_vector_mode(controller)) {
    case METAL_CLIC_VECTORED_MODE:
    case METAL_VECTORED_MODE:
        return -1;
    default:
        return 0;
    }
}
