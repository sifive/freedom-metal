/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/drivers/riscv_cpu_intc.h>
#include <metal/generated/cpu.h>
#include <metal/init.h>
#include <metal/interrupt.h>
#include <metal/interrupt_handlers.h>
#include <metal/shutdown.h>
#include <stdbool.h>
#include <stdint.h>

static struct intc_state {
    bool init_done;
    metal_exception_handler_t exception_table[METAL_MAX_ME];
} intc_state[__METAL_DT_NUM_HARTS] = {
    {
        .init_done = false,
    },
};

static uint32_t get_index(struct metal_interrupt intc) {
    return intc.__interrupt_index;
}

/* MIE CSR Manipulation */

static void __metal_interrupt_global_enable(void) {
    uintptr_t m;
    __asm__ volatile("csrrs %0, mstatus, %1"
                     : "=r"(m)
                     : "r"(METAL_MIE_INTERRUPT));
}

static void __metal_interrupt_global_disable(void) {
    uintptr_t m;
    __asm__ volatile("csrrc %0, mstatus, %1"
                     : "=r"(m)
                     : "r"(METAL_MIE_INTERRUPT));
}

static void __metal_interrupt_software_enable(void) {
    uintptr_t m;
    __asm__ volatile("csrrs %0, mie, %1"
                     : "=r"(m)
                     : "r"(METAL_LOCAL_INTERRUPT_SW));
}

static void __metal_interrupt_software_disable(void) {
    uintptr_t m;
    __asm__ volatile("csrrc %0, mie, %1"
                     : "=r"(m)
                     : "r"(METAL_LOCAL_INTERRUPT_SW));
}

static void __metal_interrupt_timer_enable(void) {
    uintptr_t m;
    __asm__ volatile("csrrs %0, mie, %1"
                     : "=r"(m)
                     : "r"(METAL_LOCAL_INTERRUPT_TMR));
}

static void __metal_interrupt_timer_disable(void) {
    uintptr_t m;
    __asm__ volatile("csrrc %0, mie, %1"
                     : "=r"(m)
                     : "r"(METAL_LOCAL_INTERRUPT_TMR));
}

static void __metal_interrupt_external_enable(void) {
    uintptr_t m;
    __asm__ volatile("csrrs %0, mie, %1"
                     : "=r"(m)
                     : "r"(METAL_LOCAL_INTERRUPT_EXT));
}

static void __metal_interrupt_external_disable(void) {
    unsigned long m;
    __asm__ volatile("csrrc %0, mie, %1"
                     : "=r"(m)
                     : "r"(METAL_LOCAL_INTERRUPT_EXT));
}

static void __metal_interrupt_local_enable(int id) {
    uintptr_t b = 1 << id;
    uintptr_t m;
    __asm__ volatile("csrrs %0, mie, %1" : "=r"(m) : "r"(b));
}

static void __metal_interrupt_local_disable(int id) {
    uintptr_t b = 1 << id;
    uintptr_t m;
    __asm__ volatile("csrrc %0, mie, %1" : "=r"(m) : "r"(b));
}

/* Default handlers */

void __metal_default_exception_handler(struct metal_cpu cpu, int ecode) {
    metal_shutdown(100);
}

void metal_riscv_cpu_intc_default_handler() {
    metal_shutdown(200);
}

/* The metal_interrupt_vector_handler() function can be redefined. */
void __attribute__((weak, interrupt)) metal_interrupt_vector_handler(void) {
    metal_shutdown(300);
}

void metal_riscv_cpu_intc_msip_handler() __attribute__((weak, interrupt));
void metal_riscv_cpu_intc_msip_handler() {
    uintptr_t mcause;
    __asm__ volatile("csrr %0, mcause" : "=r"(mcause));

    uint32_t hartid = metal_cpu_get_current_hartid();
    struct metal_cpu cpu = metal_cpu_get(hartid);
    intc_state[hartid].exception_table[RISCV_MCAUSE_ID(mcause)](cpu, 3);
}

void metal_riscv_cpu_intc_mtip_handler() __attribute__((weak, interrupt));
void metal_riscv_cpu_intc_mtip_handler() {
    struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());
    unsigned long long time = metal_cpu_get_mtime(cpu);

    /* Set a 10 cycle timer */
    metal_cpu_set_mtimecmp(cpu, time + 10);
}

void __metal_default_beu_handler(int id, void *priv) {}

void __metal_exception_handler(void) __attribute__((interrupt, aligned(128)));
void __metal_exception_handler(void) {
    uint32_t hartid = metal_cpu_get_current_hartid();

    uintptr_t mcause, mepc, mtval;
    __asm__ volatile("csrr %0, mcause" : "=r"(mcause));
    __asm__ volatile("csrr %0, mepc" : "=r"(mepc));
    __asm__ volatile("csrr %0, mtval" : "=r"(mtval));

    int id = RISCV_MCAUSE_ID(mcause);

    if (RISCV_MCAUSE_IS_INTERRUPT(mcause)) {
        __metal_vector_table[id + 1]();
    } else {
        intc_state[hartid].exception_table[id](metal_cpu_get(hartid), id);
    }
}

/* Exception API */

int metal_cpu_exception_register(struct metal_cpu cpu, int ecode,
                                 metal_exception_handler_t isr) {
    struct metal_interrupt intc = (struct metal_interrupt) { cpu.__hartid };
    if ((ecode < METAL_MAX_EXCEPTION_CODE) && isr) {
        intc_state[get_index(intc)].exception_table[ecode] = isr;
        return 0;
    }
    return -1;
}

/* Interrupt API */

extern void early_trap_vector(void);

void __metal_driver_riscv_cpu_intc_interrupt_init(
    struct metal_interrupt intc) {

    if (!intc_state[get_index(intc)].init_done) {
        /*
         * Set the real trap handler if the value of mtvec is equal to
         * early_trap_vector. If mtvec is not equal to early_trap_vector,
         * that means user has own trap handler, then we don't overwrite it.
         */
        uintptr_t mtvec;
        __asm__ volatile("csrr %0, mtvec" : "=r"(mtvec));
        if (mtvec == (uintptr_t)&early_trap_vector) {
            __metal_driver_riscv_cpu_intc_set_vector_mode(intc, METAL_DIRECT_MODE);
        }

        intc_state[get_index(intc)].init_done = 1;
    }
}

METAL_CONSTRUCTOR(riscv_cpu_intc_init) {
    for (int i = 0; i < __METAL_DT_NUM_HARTS; i++) {
        struct metal_interrupt intc = (struct metal_interrupt) { i };
        __metal_driver_riscv_cpu_intc_interrupt_init(intc);
    }
}

int __metal_driver_riscv_cpu_intc_set_vector_mode(
    struct metal_interrupt controller, metal_vector_mode mode) {

    uintptr_t mtvec;
    __asm__ volatile("csrr %0, mtvec" : "=r"(mtvec));
    
    /* Zero the mtvec CLIC config bits */
    mtvec &= ~(METAL_MTVEC_CLIC_VECTORED | METAL_MTVEC_CLIC_RESERVED);

    switch (mode) {
    default:
    case METAL_DIRECT_MODE:
        /* Write the trap vector address with the vectored bit unset */
        __asm__ volatile(
            "csrw mtvec, %0" ::"r"((uintptr_t)__metal_exception_handler & ~METAL_MTVEC_CLIC_VECTORED));
        break;
    }

    return 0;
}

metal_vector_mode __metal_driver_riscv_cpu_intc_get_vector_mode(
    struct metal_interrupt controller) {
    uintptr_t mtvec;
    __asm__ volatile("csrr %0, mtvec" : "=r"(mtvec));

    /* TODO */

    return METAL_DIRECT_MODE;
}

int __metal_driver_riscv_cpu_intc_set_privilege(struct metal_interrupt controller,
                                                metal_intr_priv_mode privilege) {
    if (privilege == METAL_INTR_PRIV_M_MODE) {
        return 0;
    }
    return -1;
}

metal_intr_priv_mode
__metal_driver_riscv_cpu_intc_get_privilege(struct metal_interrupt controller) {
    return METAL_INTR_PRIV_M_MODE;
}

int __metal_driver_riscv_cpu_intc_clear(struct metal_interrupt controller,
                                        int id) {
    return -1;
}

int __metal_driver_riscv_cpu_intc_set(struct metal_interrupt controller, int id) {
    return -1;
}

int __metal_driver_riscv_cpu_intc_register_vector_handler(
    struct metal_interrupt controller, int id,
    metal_interrupt_vector_handler_t handler, void *priv_data) {
    return -1;
}

int __metal_driver_riscv_cpu_intc_interrupt_enable(
    struct metal_interrupt controller, int id) {
    switch (id) {
    case METAL_INTERRUPT_ID_BASE:
        __metal_interrupt_global_enable();
        break;
    case METAL_INTERRUPT_ID_SW:
        __metal_interrupt_software_enable();
        break;
    case METAL_INTERRUPT_ID_TMR:
        __metal_interrupt_timer_enable();
        break;
    case METAL_INTERRUPT_ID_EXT:
        __metal_interrupt_external_enable();
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
        __metal_interrupt_local_enable(id);
        break;
    default:
        return -1;
    }
    return 0;
}

int __metal_driver_riscv_cpu_intc_interrupt_disable(
    struct metal_interrupt controller, int id) {
    switch (id) {
    case METAL_INTERRUPT_ID_BASE:
        __metal_interrupt_global_disable();
        break;
    case METAL_INTERRUPT_ID_SW:
        __metal_interrupt_software_disable();
        break;
    case METAL_INTERRUPT_ID_TMR:
        __metal_interrupt_timer_disable();
        break;
    case METAL_INTERRUPT_ID_EXT:
        __metal_interrupt_external_disable();
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
        __metal_interrupt_local_disable(id);
        break;
    default:
        return -1;
    }
    return 0;
}

int __metal_driver_riscv_cpu_intc_set_threshold(struct metal_interrupt controller,
                                                unsigned int level) {
    return -1;
}

unsigned int
__metal_driver_riscv_cpu_intc_get_threshold(struct metal_interrupt controller) {
    return 0;
}

int __metal_driver_riscv_cpu_intc_set_priority(struct metal_interrupt controller,
                                               int id, unsigned int priority) {
    return -1;
}

unsigned int
__metal_driver_riscv_cpu_intc_get_priority(struct metal_interrupt controller, int id) {
    return 0;
}

int
__metal_driver_riscv_cpu_intc_set_preemptive_level(struct metal_interrupt controller, int id,
                                                   unsigned int level) {
    return -1;
}

unsigned int
__metal_driver_riscv_cpu_intc_get_preemptive_level(struct metal_interrupt controller, int id) {
    return 0;
}

int __metal_driver_riscv_cpu_intc_vector_enable(
    struct metal_interrupt intc, int id, metal_vector_mode mode) {

    return __metal_driver_riscv_cpu_intc_set_vector_mode(intc, mode);
}

int
__metal_driver_riscv_cpu_intc_vector_disable(
    struct metal_interrupt controller, int id) {
    
    return __metal_driver_riscv_cpu_intc_set_vector_mode(controller, METAL_DIRECT_MODE);
}

metal_affinity
__metal_driver_riscv_cpu_intc_affinity_enable(struct metal_interrupt controller,
                                              metal_affinity bitmask, int id) {
    /* Return a 1 for each hart in the bitmask, representing failure. */
    return (metal_affinity) { (1 << __METAL_DT_NUM_HARTS) - 1 };
}

metal_affinity
__metal_driver_riscv_cpu_intc_affinity_disable(struct metal_interrupt controller,
                                               metal_affinity bitmask, int id) {
    return (metal_affinity) { (1 << __METAL_DT_NUM_HARTS) - 1 };
}

metal_affinity
__metal_driver_riscv_cpu_intc_affinity_set_threshold(struct metal_interrupt controller,
                                                     metal_affinity bitmask,
                                                     unsigned int level) {
    return (metal_affinity) { (1 << __METAL_DT_NUM_HARTS) - 1 };
}

unsigned int
__metal_driver_riscv_cpu_intc_affinity_get_threshold(struct metal_interrupt controller,
                                                     int context_id) {
    return 0;
}
