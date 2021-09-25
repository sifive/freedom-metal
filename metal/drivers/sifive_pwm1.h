/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_PWM1_H
#define METAL__DRIVERS__SIFIVE_PWM1_H

#include <metal/clock.h>
#include <metal/pwm.h>

struct __metal_driver_vtable_sifive_pwm1 {
    const struct metal_pwm_vtable pwm;
};

/* Max possible PWM channel count */
#define METAL_MAX_PWM_CHANNELS 16

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_pwm1)

struct __metal_driver_sifive_pwm1 {
    struct metal_pwm pwm;
    unsigned int max_count;
    unsigned int count_val;
    unsigned int freq;
    unsigned int duty[METAL_MAX_PWM_CHANNELS];
    metal_clock_callback pre_rate_change_callback;
    metal_clock_callback post_rate_change_callback;
};

#endif
