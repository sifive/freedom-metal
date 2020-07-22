/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__TIME_H
#define METAL__TIME_H

#define _POSIX_MONOTONIC_CLOCK 200809L
#define _POSIX_TIMERS
#include <time.h>

/*!
 * @file time.h
 * @brief API for dealing with time
 */

/*!
 * @brief Get monotonic time in seconds
 * @return time relative to unknown base in seconds
 */
static __inline__ time_t metal_time(void) {
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return tp.tv_sec;
}

#endif
