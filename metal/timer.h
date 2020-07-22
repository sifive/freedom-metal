/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__TIMER_H
#define METAL__TIMER_H

#include <metal/cpu.h>

/*!
 * @file timer.h
 * @brief API for reading and manipulating the machine timer
 */

/*!
 * @brief Read the machine cycle count
 * @param hartid The hart ID to read the cycle count of
 * @param cyclecount The variable to hold the value
 * @return 0 upon success
 */
static __inline__ int
metal_timer_get_cyclecount(unsigned int hartid,
                           unsigned long long *cyclecount) {
    *cyclecount = metal_cpu_get_timer(metal_cpu_get(hartid));
    return 0;
}

/*!
 * @brief Get the machine timebase frequency
 * @param hartid The hart ID to read the timebase of
 * @param timebase The variable to hold the value
 * @return 0 upon success
 */
static __inline__ int
metal_timer_get_timebase_frequency(unsigned int hartid,
                                   unsigned long long *timebase) {
    *timebase = metal_cpu_get_timebase(metal_cpu_get(hartid));
    return 0;
}

#endif
