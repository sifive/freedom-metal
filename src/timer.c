/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/timer.h>

extern inline void mee_timer_init(struct mee_timer *timer, int timebase);
extern inline int mee_timer_get(struct mee_timer *timer, unsigned long *tv);
extern inline int mee_timebase_get(struct mee_timer *timer, int *tb);
