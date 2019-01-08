/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/cpu.h>
#include <mee/machine.h>

struct mee_cpu* mee_cpu_get(int hartid)
{
    if (hartid < __MEE_DT_MAX_HARTS) {
        return &(__mee_cpu_table[hartid].cpu);
    }   
    return NULL;
}

extern inline int mee_cpu_get_timer(struct mee_cpu *cpu, int hartid, unsigned long long *tv);

extern inline int mee_cpu_get_timebase(struct mee_cpu *cpu, unsigned long long *tb);

extern inline unsigned long long mee_cpu_get_mtime(struct mee_cpu *cpu);

extern inline int mee_cpu_set_mtimecmp(struct mee_cpu *cpu, unsigned long long time);

extern inline struct mee_interrupt* mee_cpu_timer_interrupt_controller(struct mee_cpu *cpu);

extern inline int mee_cpu_timer_get_interrupt_id(struct mee_cpu *cpu);

extern inline struct mee_interrupt* mee_cpu_software_interrupt_controller(struct mee_cpu *cpu);

extern inline int mee_cpu_software_get_interrupt_id(struct mee_cpu *cpu);

extern inline int mee_cpu_software_set_ipi(struct mee_cpu *cpu, int hartid);

extern inline int mee_cpu_software_clear_ipi(struct mee_cpu *cpu, int hartid);

extern inline int mee_cpu_get_msip(struct mee_cpu *cpu, int hartid);

extern inline struct mee_interrupt* mee_cpu_interrupt_controller(struct mee_cpu *cpu);

extern inline int mee_cpu_exception_register(struct mee_cpu *cpu, int ecode, mee_exception_handler_t handler);

extern inline unsigned long mee_cpu_get_exception_frame_pointer(struct mee_cpu *cpu);

extern inline int mee_cpu_set_exception_frame_pointer(struct mee_cpu *cpu, unsigned long epc);
