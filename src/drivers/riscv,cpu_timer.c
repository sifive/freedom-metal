/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/drivers/riscv,cpu.h>

int __mee_driver_riscv_cpu_timer_get(struct mee_timer *timer,
				     int hartid, unsigned long long *tv)
{
    struct __mee_driver_cpu *mee_timer = (void *)(timer);
    unsigned long long val = 0;
    int myhart;

    asm volatile ("csrr %0, mhartid" : "=r"(myhart));
    if (myhart == hartid) {
    #if __riscv_xlen == 32
        unsigned long hi, hi1, lo;

        asm volatile ("csrr %0, mcycleh" : "=r"(hi));
        asm volatile ("csrr %0, mcycle" : "=r"(lo));
        asm volatile ("csrr %0, mcycleh" : "=r"(hi1));
	if (hi == hi1) {
	    val = ((unsigned long long)hi << 32) | lo;
        }
    #else
        asm volatile ("csrr %0, mcycle" : "=r"(val));
    #endif

	*tv = val;
	return 0;
    }
    return -1;
}

int __mee_driver_riscv_cpu_timebase_get(struct mee_timer *timer,
					unsigned long long *tb)
{
    struct __mee_driver_cpu *mee_timer = (void *)(timer);

    if (mee_timer && tb) {
        *tb = mee_timer->timebase;
	return 0;
    }
    return -1;
}

void __mee_driver_riscv_cpu_timer_init(struct mee_timer *timer,
				       unsigned long long timebase)
{
    struct __mee_driver_cpu *mee_timer = (void *)(timer);

    if (mee_timer) {
        (mee_timer->timebase != timebase) ?
            mee_timer->timebase = timebase : mee_timer->timebase;
    }
}
