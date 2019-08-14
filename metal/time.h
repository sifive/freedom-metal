/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__TIME_H
#define METAL__TIME_H

#include <time.h>

#include <sys/types.h>

/*!
 * @file time.h
 * @brief API for dealing with time
 */

struct timeval {
    time_t tv_sec;
    suseconds_t tv_usec;
};

int metal_gettimeofday(struct timeval *tp, void *tzp);

time_t metal_time(void);

#endif
