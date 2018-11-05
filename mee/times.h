/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__TIMES_H
#define MEE__TIMES_H

/* Read the machine cycle counts from csrr register */
int mee_get_cyclecount(unsigned long *mcc);

/*
 Get the machine timebase frequency, so number seconds can be calculate.
 For a timebase-frequency of 1000000 (ie 1MHz), each cycle count is 1s
 */
int mee_get_timebase_frequency(int *timebase);

#endif
