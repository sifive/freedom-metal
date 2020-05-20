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

/* Weak stubs for when no driver is present */

int metal_pwm_enable(struct metal_pwm *gpwm) __attribute__((weak));
int metal_pwm_enable(struct metal_pwm *gpwm) { return -1; }

int metal_pwm_set_freq(struct metal_pwm *gpwm, unsigned int idx,
                       unsigned int freq) __attribute__((weak));
int metal_pwm_set_freq(struct metal_pwm *gpwm, unsigned int idx,
                       unsigned int freq) {
    return -1;
}

int metal_pwm_set_duty(struct metal_pwm *gpwm, unsigned int idx,
                       unsigned int duty, metal_pwm_phase_correct_t phase_corr)
    __attribute__((weak));
int metal_pwm_set_duty(struct metal_pwm *gpwm, unsigned int idx,
                       unsigned int duty,
                       metal_pwm_phase_correct_t phase_corr) {
    return -1;
}

unsigned int metal_pwm_get_duty(struct metal_pwm *gpwm, unsigned int idx)
    __attribute__((weak));
unsigned int metal_pwm_get_duty(struct metal_pwm *gpwm, unsigned int idx) {
    return 0;
}

unsigned int metal_pwm_get_freq(struct metal_pwm *gpwm, unsigned int idx)
    __attribute__((weak));
unsigned int metal_pwm_get_freq(struct metal_pwm *gpwm, unsigned int idx) {
    return 0;
}

int metal_pwm_trigger(struct metal_pwm *gpwm, unsigned int idx,
                      metal_pwm_run_mode_t mode) __attribute__((weak));
int metal_pwm_trigger(struct metal_pwm *gpwm, unsigned int idx,
                      metal_pwm_run_mode_t mode) {
    return -1;
}

int metal_pwm_stop(struct metal_pwm *gpwm, unsigned int idx)
    __attribute__((weak));
int metal_pwm_stop(struct metal_pwm *gpwm, unsigned int idx) { return -1; }

int metal_pwm_cfg_interrupt(struct metal_pwm *gpwm, metal_pwm_interrupt_t flag)
    __attribute__((weak));
int metal_pwm_cfg_interrupt(struct metal_pwm *gpwm,
                            metal_pwm_interrupt_t flag) {
    return -1;
}

int metal_pwm_clr_interrupt(struct metal_pwm *gpwm, unsigned int idx)
    __attribute__((weak));
int metal_pwm_clr_interrupt(struct metal_pwm *gpwm, unsigned int idx) {
    return -1;
}

struct metal_interrupt *metal_pwm_interrupt_controller(struct metal_pwm *gpwm)
    __attribute__((weak));
struct metal_interrupt *metal_pwm_interrupt_controller(struct metal_pwm *gpwm) {
    return NULL;
}

int metal_pwm_get_interrupt_id(struct metal_pwm *gpwm, unsigned int idx)
    __attribute__((weak));
int metal_pwm_get_interrupt_id(struct metal_pwm *gpwm, unsigned int idx) {
    return -1;
}
