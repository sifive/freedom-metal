/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <sys/time.h>
#include <sys/times.h>
#include <mee/timer.h>
#include <mee/machine.h>

#if defined(__MEE_DT_TIMER_HANDLE)
/* This implementation serves as a small shim that interfaces with the first
 * timer on a system. */
int mee_get_cyclecount(unsigned long *mcc)
{
    return mee_timer_get(__MEE_DT_TIMER_HANDLE, mcc);
}

int mee_get_timebase_frequency(int *timebase)
{
    return mee_timebase_get(__MEE_DT_TIMER_HANDLE, timebase);
}

#ifndef __MEE_DT_TIMEBASE_FREQUENCY
#define __MEE_DT_TIMEBASE_FREQUENCY 1000000
#endif

static void mee_gettime_init(void) __attribute__((constructor));
static void mee_gettime_init(void)
{
    mee_timer_init(__MEE_DT_TIMER_HANDLE, __MEE_DT_TIMEBASE_FREQUENCY);
}
#else
/* This implementation of gettimeofday doesn't actually do anything, it's just there to
 * provide a shim and return 0 so we can ensure that everything can link to _gettimeofday.
 */
int nop_cyclecount(unsigned long *c) __attribute__((section(".text.mee.nop.cyclecount")));
int nop_cyclecount(unsigned long *c) { return 0; }
int nop_timebase(int *t) __attribute__((section(".text.mee.nop.timebase")));
int nop_timebase(int *t) { return 0; }
int mee_get_cyclecount(unsigned long *mcc) __attribute__((weak, alias("nop_cyclecount")));
#warning "There is no default timer device, mee_get_cyclecount() will always return 0."
int mee_get_timebase_frequency(int *tbf) __attribute__((weak, alias("nop_timebase")));
#warning "There is no default timer device, mee_get_timebase_frequency() will always return 0."
#endif
