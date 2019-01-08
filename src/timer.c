/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <sys/time.h>
#include <sys/times.h>
#include <mee/cpu.h>
#include <mee/timer.h>
#include <mee/machine.h>

#if defined(__MEE_DT_MAX_HARTS)
/* This implementation serves as a small shim that interfaces with the first
 * timer on a system. */
int mee_timer_get_cyclecount(int hartid, unsigned long long *mcc)
{
    struct mee_cpu *cpu = mee_cpu_get(hartid);

    if ( cpu ) {
       return mee_cpu_get_timer(cpu, hartid, mcc);
    }	
    return -1;
}

int mee_timer_get_timebase_frequency(int hartid, unsigned long long *timebase)
{
    struct mee_cpu *cpu = mee_cpu_get(hartid);

    if ( cpu ) {
       return mee_cpu_get_timebase(cpu, timebase);
    } 
    return -1;
}

int mee_timer_get_machine_time(int hartid)
{
    struct mee_cpu *cpu = mee_cpu_get(hartid);
       
    if ( cpu ) {
       return mee_cpu_get_mtime(cpu);
    }
    return 0;
}

int mee_timer_set_machine_time(int hartid, unsigned long long time)
{
    struct mee_cpu *cpu = mee_cpu_get(hartid);

    if ( cpu ) {
       return mee_cpu_set_mtimecmp(cpu, time);
    }
    return -1;
}

#else

/* This implementation of gettimeofday doesn't actually do anything, it's just there to
 * provide a shim and return 0 so we can ensure that everything can link to _gettimeofday.
 */
int nop_cyclecount(int id, unsigned long long *c) __attribute__((section(".text.mee.nop.cyclecount")));
int nop_cyclecount(int id, unsigned long long *c) { return -1; }
int nop_timebase(unsigned long long *t) __attribute__((section(".text.mee.nop.timebase")));
int nop_timebase(unsigned long long *t) { return -1; }
int nop_tick(int second) __attribute__((section(".text.mee.nop.tick")));
int nop_tick(int second) { return -1; }
int mee_timer_get_cyclecount(int hartid, unsigned long long *c) __attribute__((weak, alias("nop_cyclecount")))
{
#warning "There is no default timer device, mee_timer_get_cyclecount() will always return cyclecount -1."
}
int mee_timer_get_timebase_frequency(unsigned long long *t) __attribute__((weak, alias("nop_timebase")))
{
#warning "There is no default timer device, mee_timer_get_timebase_frequency() will always return timebase -1."
}
int mee_timer_set_tick(int second) __attribute__((weak, alias("nop_tick")))
{
#warning "There is no default timer device, mee_timer_set_tick) will always return -1."
}

#endif

