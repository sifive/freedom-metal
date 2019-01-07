/* Copyright 2018 SiFive, Inc */

/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__CPU_H
#define MEE__CPU_H

#include <mee/interrupt.h>

typedef void (*mee_exception_handler_t) (int ecode);

struct mee_cpu;

struct mee_cpu_vtable {
    int (*timer_get)(struct mee_cpu *cpu, int hartid, unsigned long long *value);
    int (*timebase_get)(struct mee_cpu *cpu, unsigned long long *value);
    unsigned long long (*mtime_get)(struct mee_cpu *cpu);
    int (*mtimecmp_set)(struct mee_cpu *cpu, unsigned long long time);
    struct mee_interrupt* (*tmr_controller_interrupt)(struct mee_cpu *cpu);
    int (*get_tmr_interrupt_id)(struct mee_cpu *cpu);
    struct mee_interrupt* (*sw_controller_interrupt)(struct mee_cpu *cpu);
    int (*get_sw_interrupt_id)(struct mee_cpu *cpu);
    int (*set_sw_ipi)(struct mee_cpu *cpu, int hartid);
    int (*clear_sw_ipi)(struct mee_cpu *cpu, int hartid);
    int (*get_msip)(struct mee_cpu *cpu, int hartid);
    struct mee_interrupt* (*controller_interrupt)(struct mee_cpu *cpu);
    int (*exception_register)(struct mee_cpu *cpu, int ecode, mee_exception_handler_t handler);
    unsigned long (*get_frame_pointer)(struct mee_cpu *cpu);
    int (*set_frame_pointer)(struct mee_cpu *cpu, unsigned long epc);
};

struct mee_cpu {
    const struct mee_cpu_vtable *vtable;
};

struct mee_cpu* mee_cpu_get(int hartid);

inline int mee_cpu_get_timer(struct mee_cpu *cpu, int hartid, unsigned long long *tv)
{ return cpu->vtable->timer_get(cpu, hartid, tv); }

inline int mee_cpu_get_timebase(struct mee_cpu *cpu, unsigned long long *tb)
{ return cpu->vtable->timebase_get(cpu, tb); }

inline unsigned long long mee_cpu_get_mtime(struct mee_cpu *cpu)
{ return cpu->vtable->mtime_get(cpu); }

inline int mee_cpu_set_mtimecmp(struct mee_cpu *cpu, unsigned long long time)
{ return cpu->vtable->mtimecmp_set(cpu, time); }

inline struct mee_interrupt* mee_cpu_timer_interrupt_controller(struct mee_cpu *cpu)
{ return cpu->vtable->tmr_controller_interrupt(cpu); }

inline int mee_cpu_timer_get_interrupt_id(struct mee_cpu *cpu)
{ return cpu->vtable->get_tmr_interrupt_id(cpu); }

inline struct mee_interrupt* mee_cpu_software_interrupt_controller(struct mee_cpu *cpu)
{ return cpu->vtable->sw_controller_interrupt(cpu); }

inline int mee_cpu_software_get_interrupt_id(struct mee_cpu *cpu)
{ return cpu->vtable->get_sw_interrupt_id(cpu); }

inline int mee_cpu_software_set_ipi(struct mee_cpu *cpu, int hartid)
{ return cpu->vtable->set_sw_ipi(cpu, hartid); }

inline int mee_cpu_software_clear_ipi(struct mee_cpu *cpu, int hartid)
{ return cpu->vtable->clear_sw_ipi(cpu, hartid); }

inline int mee_cpu_get_msip(struct mee_cpu *cpu, int hartid)
{ return cpu->vtable->get_msip(cpu, hartid); }

inline struct mee_interrupt* mee_cpu_interrupt_controller(struct mee_cpu *cpu)
{ return cpu->vtable->controller_interrupt(cpu); }

inline int mee_cpu_exception_register(struct mee_cpu *cpu, int ecode, mee_exception_handler_t handler)
{ return cpu->vtable->exception_register(cpu, ecode, handler); }

inline unsigned long mee_cpu_get_exception_frame_pointer(struct mee_cpu *cpu)
{ return cpu->vtable->get_frame_pointer(cpu); }

inline int mee_cpu_set_exception_frame_pointer(struct mee_cpu *cpu, unsigned long epc)
{ return cpu->vtable->set_frame_pointer(cpu, epc); }

#endif
