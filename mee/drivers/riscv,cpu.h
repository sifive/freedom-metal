/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__RISCV_CPU_H
#define MEE__DRIVERS__RISCV_CPU_H

#include <mee/compiler.h>
#include <mee/timer.h>

struct __mee_driver_vtable_cpu {
    struct mee_timer_vtable timer_vtable;
};

void __mee_driver_riscv_cpu_timer_init(struct mee_timer *timer, unsigned long long timebase);
int  __mee_driver_riscv_cpu_timer_get(struct mee_timer *timer, int hartid, unsigned long long *tv);
int  __mee_driver_riscv_cpu_timebase_get(struct mee_timer *timer, unsigned long long *tb);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_cpu) = {
    .timer_vtable.timer_init    = __mee_driver_riscv_cpu_timer_init,
    .timer_vtable.timer_get     = __mee_driver_riscv_cpu_timer_get,
    .timer_vtable.timebase_get  = __mee_driver_riscv_cpu_timebase_get,
};

struct __mee_driver_cpu {
    struct mee_timer timer;
    const struct mee_timer_vtable *timer_vtable;
    int timebase;
};

#endif
