/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__SIFIVE_TIMER_H
#define MEE__DRIVERS__SIFIVE_TIMER_H

#include <mee/compiler.h>
#include <mee/timer.h>

struct __mee_driver_vtable_timer {
    struct mee_timer_vtable vtable;
};

void __mee_driver_sifive_timer_init(struct mee_timer *timer, int timebase);
int  __mee_driver_sifive_timer_get(struct mee_timer *timer, unsigned long *tv);
int  __mee_driver_sifive_timebase_get(struct mee_timer *timer, int *tb);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_timer) = {
    .vtable.init       = __mee_driver_sifive_timer_init,
    .vtable.timer_get  = __mee_driver_sifive_timer_get,
    .vtable.timebase_get  = __mee_driver_sifive_timebase_get,
};

struct __mee_driver_timer {
    struct mee_timer timer;
    const struct mee_timer_vtable *vtable;
    int timebase;
};

#endif
