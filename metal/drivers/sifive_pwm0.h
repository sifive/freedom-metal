/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_PWM0_H
#define METAL__DRIVERS__SIFIVE_PWM0_H

#include <metal/clock.h>
#include <metal/pwm.h>

/* Max possible PWM channel count */
#define METAL_MAX_PWM_CHANNELS 16

struct __metal_driver_sifive_pwm0 {
    unsigned int max_count;
    unsigned int count_val;
    unsigned int freq;
    unsigned int duty[METAL_MAX_PWM_CHANNELS];
    metal_clock_callback pre_rate_change_callback;
    metal_clock_callback post_rate_change_callback;
};

#endif
