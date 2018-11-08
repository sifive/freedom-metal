/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <sys/time.h>
#include <sys/times.h>
#include <mee/timer.h>
#include <mee/machine.h>

#if defined(__MEE_DT_MAX_HARTS)

/* This implementation serves as a small shim that interfaces with the first
 * timer on a system. */
int mee_timer_get_cyclecount(int hartid, unsigned long long *mcc)
{
    if (hartid < __MEE_DT_MAX_HARTS) {
       return mee_timer_get(&(__MEE_DT_CPU_HANDLE(hartid)->timer), hartid, mcc);
       //return mee_timer_get(__MEE_DT_TIMER_HANDLE(hartid), hartid, mcc);
    }	
    return -1;
}

int mee_timer_get_timebase_frequency(int hartid, unsigned long long *timebase)
{
    if (hartid < __MEE_DT_MAX_HARTS) {
       return mee_timebase_get(&(__MEE_DT_CPU_HANDLE(hartid)->timer), timebase);
       //return mee_timebase_get(__MEE_DT_TIMER_HANDLE(hartid), timebase);
    } 
    return -1;
}

static void mee_gettime_init(void) __attribute__((constructor));
static void mee_gettime_init(void)
{
    mee_timer_init(&(__MEE_DT_CPU_HANDLE(0)->timer), __MEE_DT_TIMEBASE_FREQUENCY_cpu_0);
    //mee_timer_init(__MEE_DT_TIMER_HANDLE(0), __MEE_DT_TIMEBASE_FREQUENCY_cpu_0);
}
#else
/* This implementation of gettimeofday doesn't actually do anything, it's just there to
 * provide a shim and return 0 so we can ensure that everything can link to _gettimeofday.
 */
int nop_cyclecount(int id, unsigned long long *c) __attribute__((section(".text.mee.nop.cyclecount")));
int nop_cyclecount(int id, unsigned long long *c) { return -1; }
int nop_timebase(unsigned long long *t) __attribute__((section(".text.mee.nop.timebase")));
int nop_timebase(unsigned long long *t) { return -1; }
int mee_timer_get_cyclecount(int hartid, unsigned long long *c) __attribute__((weak, alias("nop_cyclecount")));
#warning "There is no default timer device, mee_get_cyclecount() will always return cyclecount -1."
int mee_get_timebase_frequency(unsigned long long *t) __attribute__((weak, alias("nop_timebase")));
#warning "There is no default timer device, mee_get_timebase_frequency() will always return timebase -1."
#endif
