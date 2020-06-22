/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <assert.h>
#include <cpu.h>
#include <metal/drivers/riscv_clint0.h>
#include <metal/drivers/riscv_cpu.h>
#include <metal/drivers/riscv_cpu_intc.h>
#include <metal/drivers/sifive_clic0.h>
#include <metal/generated/riscv_cpu.h>
#include <metal/platform.h>
#include <metal/io.h>
#include <metal/shutdown.h>
#include <stdint.h>

static inline uint32_t get_hartid(struct metal_cpu cpu) {
    return cpu.__hartid;
}

unsigned long long metal_cpu_get_timer(struct metal_cpu cpu) {
    unsigned long long val = 0;

#if __riscv_xlen == 32
    unsigned long hi, hi1, lo;

    do {
        __asm__ volatile("csrr %0, mcycleh" : "=r"(hi));
        __asm__ volatile("csrr %0, mcycle" : "=r"(lo));
        __asm__ volatile("csrr %0, mcycleh" : "=r"(hi1));
        /* hi != hi1 means mcycle overflow during we get value,
         * so we must retry. */
    } while (hi != hi1);

    val = ((unsigned long long)hi << 32) | lo;
#else
    __asm__ volatile("csrr %0, mcycle" : "=r"(val));
#endif

    return val;
}

unsigned long long metal_cpu_get_timebase(struct metal_cpu cpu) {
    return dt_cpu_data[get_hartid(cpu)].timebase;
}


uint64_t metal_cpu_get_mtime(struct metal_cpu cpu) __attribute__((weak));
uint64_t metal_cpu_get_mtime(struct metal_cpu cpu) {
    return 0;
}


int metal_cpu_set_mtimecmp(struct metal_cpu cpu, uint64_t time) __attribute__((weak));
int metal_cpu_set_mtimecmp(struct metal_cpu cpu, uint64_t time) {
    return -1;
}

struct metal_interrupt metal_cpu_timer_interrupt_controller(struct metal_cpu cpu) {
#ifdef METAL_RISCV_CLINT0
    return (struct metal_interrupt) { cpu.__hartid };
#else
#ifdef METAL_SIFIVE_CLIC0
    return (struct metal_interrupt) { cpu.__hartid };
#endif
#endif
    assert(0);
    return (struct metal_interrupt) { 0 };
}

int metal_cpu_timer_get_interrupt_id(struct metal_cpu cpu) {
    return METAL_INTERRUPT_ID_TMR;
}

struct metal_interrupt metal_cpu_software_interrupt_controller(struct metal_cpu cpu) {
#ifdef METAL_RISCV_CLINT0
    return (struct metal_interrupt) { cpu.__hartid };
#else
#ifdef METAL_SIFIVE_CLIC0
    return (struct metal_interrupt) { cpu.__hartid };
#endif
#endif
    assert(0);
    return (struct metal_interrupt) { 0 };
}

int metal_cpu_software_get_interrupt_id(struct metal_cpu cpu) {
    return METAL_INTERRUPT_ID_SW;
}

int metal_cpu_software_set_ipi(struct metal_cpu cpu, int hartid) __attribute__((weak));
int metal_cpu_software_set_ipi(struct metal_cpu cpu, int hartid) {
    return -1;
}

int metal_cpu_software_clear_ipi(struct metal_cpu cpu, int hartid) __attribute__((weak));
int metal_cpu_software_clear_ipi(struct metal_cpu cpu, int hartid) {
    return -1;
}

int metal_cpu_get_msip(struct metal_cpu cpu, int hartid) __attribute__((weak));
int metal_cpu_get_msip(struct metal_cpu cpu, int hartid) {
    return 0;
}

struct metal_interrupt metal_cpu_interrupt_controller(struct metal_cpu cpu) {
    return dt_cpu_data[get_hartid(cpu)].interrupt_controller;
}

int metal_cpu_get_instruction_length(struct metal_cpu cpu, uintptr_t epc) {
    /**
     * Per ISA compressed instruction has last two bits of opcode set.
     * The encoding '00' '01' '10' are used for compressed instruction.
     * Only enconding '11' isn't regarded as compressed instruction (>16b).
     */
    return ((*(unsigned short *)epc & METAL_INSN_LENGTH_MASK) ==
            METAL_INSN_NOT_COMPRESSED)
               ? 4
               : 2;
}

uintptr_t metal_cpu_get_exception_pc(struct metal_cpu cpu) {
    uintptr_t mepc;
    __asm__ volatile("csrr %0, mepc" : "=r"(mepc));
    return mepc;
}

int metal_cpu_set_exception_pc(struct metal_cpu cpu, uintptr_t mepc) {
    __asm__ volatile("csrw mepc, %0" ::"r"(mepc));
    return 0;
}

struct metal_buserror metal_cpu_get_buserror(struct metal_cpu cpu) {
    assert(dt_cpu_data[get_hartid(cpu)].has_buserror);
    return dt_cpu_data[get_hartid(cpu)].buserror;
}
