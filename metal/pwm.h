/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__PWM_H
#define METAL__PWM_H

#include <metal/private/metal_private_pwm.h>
#include <stdbool.h>
#include <stdint.h>

/*! @brief Enums for PWM running modes. */
typedef enum {
    METAL_PWM_CONTINUOUS = 0,
    METAL_PWM_ONE_SHOT = 1
} metal_pwm_run_mode_t;

/*! @brief Enums for Phase correct PWM. */
typedef enum {
    METAL_PWM_PHASE_CORRECT_DISABLE = 0,
    METAL_PWM_PHASE_CORRECT_ENABLE = 1,
} metal_pwm_phase_correct_t;

/*! @brief Enums for Interrupts enable/disable. */
typedef enum {
    METAL_PWM_INTERRUPT_DISABLE = 0,
    METAL_PWM_INTERRUPT_ENABLE = 1,
} metal_pwm_interrupt_t;

/*! @brief A handle for a PWM device. */
struct metal_pwm {
    uint32_t __pwm_index;
};

/*! @brief Gets a PWM device handle.
 * @param device_num The index of the desired PWM device.
 * @return A handle to the PWM device */
static inline struct metal_pwm metal_pwm_get_device(uint32_t index) {
    return ((struct metal_pwm){index});
}

/*! @brief Enables PWM operation.
 * @param pwm The handle for the PWM device to initialize.
 * @return 0 If no error.*/
int metal_pwm_enable(struct metal_pwm pwm);

/*! @brief Disables PWM operation.
 * @param pwm The handle for the PWM device to be disabled.
 * @return 0 If no error.*/
int metal_pwm_disable(struct metal_pwm pwm);

/*! @brief Sets frequency in Hz for a given PWM instance.
 * @param pwm PWM device handle.
 * @param idx PWM channel id.
 * @param freq PWM frequency in Hz.
 * @return 0 If no error.*/
int metal_pwm_set_freq(struct metal_pwm pwm, uint32_t idx, uint32_t freq);

/*! @brief Sets duty cycle in percent values [0 - 100] for a given PWM instance.
 * Phase correct mode provides center aligned PWM waveform output.
 * @param pwm PWM device handle.
 * @param idx PWM channel id.
 * @param duty PWM duty cycle value.
 * @param phase_corr Enable / Disable phase correct mode.
 * @return 0 If no error.*/
int metal_pwm_set_duty(struct metal_pwm pwm, uint32_t idx, unsigned int duty,
                       metal_pwm_phase_correct_t phase_corr);

/*! @brief Gets duty cycle in percent values [0 - 100] for a given PWM instance.
 * @param pwm PWM device handle.
 * @param idx PWM channel id.
 * @return PWM duty cycle value.*/
uint32_t metal_pwm_get_duty(struct metal_pwm pwm, uint32_t idx);

/*! @brief Gets frequency in Hz for a given PWM instance.
 * @param pwm PWM device handle.
 * @param idx PWM channel id.
 * @return PWM frequency in Hz.*/
uint32_t metal_pwm_get_freq(struct metal_pwm pwm, uint32_t idx);

/*! @brief Starts a PWM instance in selected run mode (continuous/one shot).
 * @param pwm PWM device handle.
 * @param idx PWM channel id.
 * @return 0 If no error.*/
int metal_pwm_trigger(struct metal_pwm pwm, uint32_t idx,
                      metal_pwm_run_mode_t mode);

/*! @brief Stops a running PWM instance in continuous mode.
 * @param pwm PWM device handle.
 * @param idx PWM channel id.
 * @return 0 If no error.*/
int metal_pwm_stop(struct metal_pwm pwm, uint32_t idx);

/*! @brief Enable or Disable PWM interrupts.
 * @param pwm PWM device handle.
 * @param flag PWM interrupt enable flag.
 * @param idx PWM channel id.
 * @return 0 If no error.*/
int metal_pwm_cfg_interrupt(struct metal_pwm pwm, metal_pwm_interrupt_t flag,
                            uint32_t idx);

/*! @brief Clears pending interrupt flags.
 * @param pwm PWM device handle.
 * @param idx PWM channel id.
 * @return 0 If no error.*/
int metal_pwm_clear_interrupt(struct metal_pwm pwm, uint32_t idx);

#endif
