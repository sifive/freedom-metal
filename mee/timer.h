/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__TIMER_H
#define MEE__TIMER_H

struct mee_timer;

struct mee_timer_vtable {
    void (*init)(struct mee_timer *timer, int timebase);
    int (*timer_get)(struct mee_timer *timer, unsigned long *value);
    int (*timebase_get)(struct mee_timer *timer, int *value);
};

struct mee_timer {
    const struct mee_timer_vtable *vtable;
};

/* Initializes a TIMER. */
inline void mee_timer_init(struct mee_timer *timer, int timebase) { timer->vtable->init(timer, timebase); }

/* Get and Set to machine time.  These return 0 on success. */
inline int mee_timer_get(struct mee_timer *timer, unsigned long *tv) { return timer->vtable->timer_get(timer, tv); }
inline int mee_timebase_get(struct mee_timer *timer, int *tb) { return timer->vtable->timebase_get(timer, tb); }

#endif
