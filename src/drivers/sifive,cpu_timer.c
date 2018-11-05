/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/drivers/sifive,timer.h>

int __mee_driver_sifive_timer_get(struct mee_timer *timer, unsigned long *tv)
{
    struct __mee_driver_timer *mee_timer = (void *)(timer);
    unsigned long val = 0;

    #if __riscv_xlen == 32
        unsigned int hi, hi1, lo;

        asm volatile ("csrr %0, mcycleh" : "=r"(hi));
        asm volatile ("csrr %0, mcycle" : "=r"(lo));
        asm volatile ("csrr %0, mcycleh" : "=r"(hi1));
	if (hi == hi1) {
	    val = ((unsigned long)hi << 32) | lo;
        }
    #else
        asm volatile ("csrr %0, mcycle" : "=r"(val));
    #endif

    *tv = val;
    return 0;
}

int __mee_driver_sifive_timebase_get(struct mee_timer *timer, int *tb)
{
    struct __mee_driver_timer *mee_timer = (void *)(timer);

    if (mee_timer->timebase) {
        *tb = mee_timer->timebase;
	return 0;
    } else {
        return -1;
    }
}

void __mee_driver_sifive_timer_init(struct mee_timer *timer, int timebase)
{
    struct __mee_driver_timer *mee_timer = (void *)(timer);

    (mee_timer->timebase != timebase) ? mee_timer->timebase = timebase : mee_timer->timebase;
}
