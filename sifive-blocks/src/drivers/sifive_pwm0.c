/* Copyright 2020 SiFive, Inc */
/* Copyright © 2020 Keith Packard */
/*
 * Licensed under the Apache License, Version 2.0 (the “License”); you
 * may not use this file except in compliance with the License. A copy
 * of this license can be found in the LICENSE.Apache2 file included
 * with this software. You may also obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_PWM0
#include <metal/clock.h>
#include <metal/cpu.h>
#include <metal/io.h>
#include <metal/private/metal_private_sifive_pwm0.h>
#include <metal/time.h>

/* Register fields */
#define METAL_PWMCFG_STICKY (1UL << 8)
#define METAL_PWMCFG_ZEROCMP (1UL << 9)
#define METAL_PWMCFG_DEGLITCH (1UL << 10)
#define METAL_PWMCFG_ENALWAYS (1UL << 12)
#define METAL_PWMCFG_ENONESHOT (1UL << 13)
#define METAL_PWMCFG_CMPCENTER(x) (1UL << (16 + x))
#define METAL_PWMCFG_CMPIP(x) (1UL << (28 + x))
#define METAL_SIFIVE_PWM0_PWMCMP(x) (METAL_SIFIVE_PWM0_PWMCMP0 + (x * 4))

/* Macros to access registers */
#define METAL_PWM_REG(offset) ((base + offset))
#define METAL_PWM_REGW(offset)                                                 \
    (__METAL_ACCESS_ONCE((__metal_io_u32 *)METAL_PWM_REG(offset)))

/* Macro to get PWM compare count */
#define METAL_PWM_GETCMPVAL(duty)                                              \
    (duty * pwm_state[get_index(pwm)].count_val) / 100U
/* Max duty cycle value */
#define METAL_PWM_MAXDUTY 100UL
/* Max pre-scalar value */
#define METAL_PWM_MAXPRESCAL 15UL

/* Check endianess */
#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error *** Unsupported endianess ***
#endif

#if (METAL_MAX_PWM0_NCMP > METAL_MAX_PWM_CHANNELS)
#error *** METAL_MAX_PWM_CHANNELS exceeded ***
#endif

/* Return values */
#define METAL_PWM_RET_OK 0
#define METAL_PWM_RET_ERR -1

static struct {
    uint32_t max_count;
    uint32_t count_val;
    uint32_t freq;
    uint32_t duty[METAL_MAX_PWM_CHANNELS];
} pwm_state[__METAL_DT_NUM_PWMS];

#define get_index(pwm) ((pwm).__pwm_index)

void _sifive_pwm0_pre_rate_change_callback(uint32_t id) {
    struct metal_pwm pwm = metal_pwm_get_device(id);
    /* Disable active PWM instance. */
    sifive_pwm0_stop(pwm, 0);
}

void _sifive_pwm0_post_rate_change_callback(uint32_t id) {
    struct metal_pwm pwm = metal_pwm_get_device(id);
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;
    uint8_t cmp_count = dt_pwm_data[get_index(pwm)].comparator_count;
    uint8_t idx = 0;
    uint32_t duty;

    /* Check if PWM frequency was set */
    if (pwm_state[get_index(pwm)].freq != 0) {
        /* Set frequency after clock rate change */
        sifive_pwm0_set_freq(pwm, 0, pwm_state[get_index(pwm)].freq);

        /* Set duty cycle after clock rate change */
        while (++idx < cmp_count) {
            duty = pwm_state[get_index(pwm)].duty[idx];
            if (duty != 0) {
                METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCMP(idx)) =
                    METAL_PWM_GETCMPVAL(duty);
            }
        }
    }
}

int sifive_pwm0_enable(struct metal_pwm pwm) {
    int ret = METAL_PWM_RET_ERR;
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;

    if (base != 0) {

        int has_pinmux = dt_pwm_data[get_index(pwm)].has_pinmux;
        if (has_pinmux) {
            struct metal_gpio pinmux = dt_pwm_data[get_index(pwm)].pinmux;
            uint32_t output_sel =
                dt_pwm_data[get_index(pwm)].pinmux_output_selector;
            uint32_t source_sel =
                dt_pwm_data[get_index(pwm)].pinmux_source_selector;
            metal_gpio_enable_pinmux(pinmux, output_sel, source_sel);
        }

        /* Initialize default values */
        pwm_state[get_index(pwm)].max_count =
            (1UL << dt_pwm_data[get_index(pwm)].compare_width) - 1;
        pwm_state[get_index(pwm)].freq = 0;
        pwm_state[get_index(pwm)].count_val = 0;
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) = 0;
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

int sifive_pwm0_disable(struct metal_pwm pwm) {
    int ret = METAL_PWM_RET_ERR;
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;

    if (base != 0) {

        int has_pinmux = dt_pwm_data[get_index(pwm)].has_pinmux;
        if (has_pinmux) {
            /* Disable PWM I/O pins */
            struct metal_gpio pinmux = dt_pwm_data[get_index(pwm)].pinmux;
            uint32_t source_sel =
                dt_pwm_data[get_index(pwm)].pinmux_source_selector;
            metal_gpio_disable_pinmux(pinmux, source_sel);
        }

        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

int sifive_pwm0_set_freq(struct metal_pwm pwm, uint32_t idx, uint32_t freq) {
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;
    uint8_t cmp_count = dt_pwm_data[get_index(pwm)].comparator_count;
    struct metal_clock clock = dt_pwm_data[get_index(pwm)].clock;
    unsigned int count;
    unsigned int prescale = 0;
    int ret = METAL_PWM_RET_ERR;

    if (idx < cmp_count) {
        long clock_rate = metal_clock_get_rate_hz(clock);

        /* Calculate count value for given PWM frequency */
        do {
            count = (clock_rate / (1UL << prescale)) / freq;
        } while ((count > pwm_state[get_index(pwm)].max_count) &&
                 (prescale++ < METAL_PWM_MAXPRESCAL));

        pwm_state[get_index(pwm)].freq =
            (clock_rate / (1UL << prescale)) / count;
        pwm_state[get_index(pwm)].count_val = --count;

        /* Update values into registers */
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCMP0) = count;
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) |= (prescale & 0x0FUL);
        ret = METAL_PWM_RET_OK;

#if defined(METAL_PWM_DEBUG)
        printf("PWM requested freq:%u set freq:%u \n", freq,
               pwm_state[get_index(pwm)].freq);
        printf("CPU Clk:%u Prescale:%u Count:%u \n", clock_rate, prescale,
               count);
#endif
    }
    return ret;
}

int sifive_pwm0_set_duty(struct metal_pwm pwm, uint32_t idx, unsigned int duty,
                         metal_pwm_phase_correct_t phase_corr) {
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;
    uint8_t cmp_count = dt_pwm_data[get_index(pwm)].comparator_count;
    int ret = METAL_PWM_RET_ERR;

    /* Check if duty value is within limits, duty cycle cannot be set for
     * PWMCMP0 */
    if ((idx > 0) && (idx < cmp_count) && (duty <= METAL_PWM_MAXDUTY)) {
        /* Calculate PWM compare count value for given duty cycle */
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCMP(idx)) =
            METAL_PWM_GETCMPVAL(duty);
        pwm_state[get_index(pwm)].duty[idx] = duty;

        /* Enable / Disable phase correct PWM mode */
        if (phase_corr == METAL_PWM_PHASE_CORRECT_ENABLE) {
            METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) |=
                METAL_PWMCFG_CMPCENTER(idx);
        } else {
            METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) &=
                ~METAL_PWMCFG_CMPCENTER(idx);
        }
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

uint32_t sifive_pwm0_get_duty(struct metal_pwm pwm, uint32_t idx) {
    uint8_t cmp_count = dt_pwm_data[get_index(pwm)].comparator_count;
    uint32_t duty = 0;

    /* Check for valid parameters and get configured duty cycle value */
    if ((idx > 0) && (idx < cmp_count)) {
        duty = pwm_state[get_index(pwm)].duty[idx];
    }
    return duty;
}

uint32_t sifive_pwm0_get_freq(struct metal_pwm pwm, uint32_t idx) {
    (void)idx; /* Unused parameter, no support for per channel frequency */

    /* Check for valid parameters and get configured PWM frequency value */
    return pwm_state[get_index(pwm)].freq;
}

int sifive_pwm0_trigger(struct metal_pwm pwm, uint32_t idx,
                        metal_pwm_run_mode_t mode) {
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;
    int ret = METAL_PWM_RET_ERR;

    (void)idx; /* Unused parameter,for later use */

    if (base != 0) {
        /* Configure for requested PWM run mode */
        if (mode == METAL_PWM_CONTINUOUS) {
            METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) |= METAL_PWMCFG_DEGLITCH |
                                                        METAL_PWMCFG_ZEROCMP |
                                                        METAL_PWMCFG_ENALWAYS;
        } else {
            METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) |= METAL_PWMCFG_DEGLITCH |
                                                        METAL_PWMCFG_ZEROCMP |
                                                        METAL_PWMCFG_ENONESHOT;
        }
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

int sifive_pwm0_stop(struct metal_pwm pwm, uint32_t idx) {
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;
    int ret = METAL_PWM_RET_ERR;

    (void)idx; /* Unused parameter,for later use */

    if (base != 0) {
        /* Disable always running mode */
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) &= ~METAL_PWMCFG_ENALWAYS;
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

int sifive_pwm0_cfg_interrupt(struct metal_pwm pwm,
                              metal_pwm_interrupt_t flag) {
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;
    int ret = METAL_PWM_RET_ERR;

    if (base != 0) {
        if (flag == METAL_PWM_INTERRUPT_ENABLE) {
            /* Enable sticky bit, to make sure interrupts are not forgotten */
            METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) |= METAL_PWMCFG_STICKY;
        } else {
            METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) &= ~METAL_PWMCFG_STICKY;
        }
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

int sifive_pwm0_clear_interrupt(struct metal_pwm pwm, uint32_t idx) {
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;
    uint8_t cmp_count = dt_pwm_data[get_index(pwm)].comparator_count;
    int ret = METAL_PWM_RET_ERR;

    if ((base != 0) && (idx < cmp_count)) {
        /* Clear interrupt pending bit for given PWM comparator */
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) &= ~METAL_PWMCFG_CMPIP(idx);
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

#endif /* METAL_SIFIVE_PWM0 */

typedef int no_empty_translation_units;
