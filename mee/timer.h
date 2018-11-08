/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__TIMER_H
#define MEE__TIMER_H

/* Read the machine cycle counts from csrr register */
int mee_timer_get_cyclecount(int hartid, unsigned long long *cyclecount);

/* Get the machine timebase frequency, so number seconds can be calculate. */
int mee_timer_get_timebase_frequency(int hartid, unsigned long long *timebase);

struct mee_timer;

struct mee_timer_vtable {
  void (*timer_init)(struct mee_timer *timer, unsigned long long timebase);
  int (*timer_get)(struct mee_timer *timer, int hartid, unsigned long long *value);
  int (*timebase_get)(struct mee_timer *timer, unsigned long long *value);
};

struct mee_timer {
    const struct mee_timer_vtable *vtable;
};

/* Initializes a TIMER. */
inline void mee_timer_init(struct mee_timer *timer, unsigned long long timebase)
{ timer->vtable->timer_init(timer, timebase); }

/* Get and Set to machine time.  These return 0 on success. */
inline int mee_timer_get(struct mee_timer *timer, int hartid, unsigned long long *tv)
{ return timer->vtable->timer_get(timer, hartid, tv); }
inline int mee_timebase_get(struct mee_timer *timer, unsigned long long *tb)
{ return timer->vtable->timebase_get(timer, tb); }

#endif
