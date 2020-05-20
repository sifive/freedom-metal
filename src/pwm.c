/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/pwm.h>

struct metal_pwm *metal_pwm_get_device(unsigned int device_num) {
#if __METAL_DT_MAX_PWMS > 0
    if (device_num < __METAL_DT_MAX_PWMS) {
        return (struct metal_pwm *)__metal_pwm_table[device_num];
    }
#endif
    return NULL;
}
