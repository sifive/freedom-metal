/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__TIMER_H
#define MEE__TIMER_H

/* Read the machine cycle counts from csrr register */
int mee_timer_get_cyclecount(int hartid, unsigned long long *cyclecount);

/* Get the machine timebase frequency, so number seconds can be calculate. */
int mee_timer_get_timebase_frequency(int hartid, unsigned long long *timebase);

/* Set the machine timer tick interval in second. */
int mee_timer_set_tick(int hartid, int second);

#endif
