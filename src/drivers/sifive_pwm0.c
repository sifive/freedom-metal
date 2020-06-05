/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef METAL_SIFIVE_PWM0

#include <metal/generated/sifive_pwm0.h>
#include <metal/io.h>
#include <stdio.h>

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
#define METAL_PWM_GETCMPVAL(duty) (duty * pwm_state[index].count_val) / 100U
/* Max duty cycle value */
#define METAL_PWM_MAXDUTY 100UL
/* Max pre-scalar value */
#define METAL_PWM_MAXPRESCALE 15UL

/* Check endianess */
#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error *** Unsupported endianess ***
#endif

/* Return values */
#define METAL_PWM_RET_OK 0
#define METAL_PWM_RET_ERR -1

static struct {
    unsigned int max_count;
    unsigned int count_val;
    unsigned int freq;
    unsigned int duty[METAL_MAX_PWM_CHANNELS];
    metal_clock_callback pre_rate_change_callback;
    metal_clock_callback post_rate_change_callback;
} pwm_state[__METAL_DT_NUM_PWMS];

static uint32_t get_index(struct metal_pwm pwm) {
    return pwm.__pwm_index;
}

static void pre_rate_change_callback(void *priv) {
    struct metal_pwm pwm = priv;
    /* Disable active PWM instance. */
    metal_pwm_stop(pwm, 0);
}

static void post_rate_change_callback(void *priv) {
    struct metal_pwm pwm = priv;
    uint32_t index = get_index(pwm)
    uintptr_t base = dt_pwm_data[index].base_addr;
    uint32_t num_channels = dt_pwm_data[index].num_channels;

    /* Check if PWM frequency was set */
    if (pwm_state[index].freq != 0) {
        /* Set frequency after clock rate change */
        metal_pwm_set_freq(pwm, 0, pwm_state[index].freq);

        /* Set duty cycle after clock rate change */
        uint32_t channel;
        while (++channel < num_channels) {
            uint32_t duty = pwm_state[index].duty[channel];
            if (duty != 0) {
                METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCMP(channel)) =
                    METAL_PWM_GETCMPVAL(duty);
            }
        }
    }
}

int metal_pwm_enable(struct metal_pwm pwm) {
    uint32_t index = get_index(pwm);
    uintptr_t base = dt_pwm_data[index].base_addr;

    bool has_pinmux = dt_pwm_data[index].has_pinmux;
    if (has_pinmux) {
        struct metal_gpio pinmux = dt_pwm_data[index].pinmux;
        uint32_t output_sel = dt_pwm_data[index].pinmux_output_selector;
        uint32_t source_sel = dt_pwm_data[index].pinmux_source_selector;
        metal_gpio_enable_pinmux(pinmux, output_sel, source_sel);
    }

    /* Initialize default values */
    pwm_state[index].max_count =
        (1UL << __metal_driver_sifive_pwm0_compare_width(pwm)) - 1;
    pwm_state[index].freq = 0;
    pwm_state[index].count_val = 0;
    METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) = 0;

    return METAL_PWM_RET_OK;
}

int metal_pwm_disable(struct metal_pwm pwm) {
    uint32_t index = get_index(pwm);
    uintptr_t base = dt_pwm_data[index].base_addr;

    bool has_pinmux = dt_pwm_data[index].has_pinmux;
    if (has_pinmux) {
        struct metal_gpio pinmux = dt_pwm_data[index].pinmux;
        uint32_t output_sel = dt_pwm_data[index].pinmux_output_selector;
        uint32_t source_sel = dt_pwm_data[index].pinmux_source_selector;
        metal_gpio_disable_pinmux(pinmux, output_sel, source_sel);
    }

    return METAL_PWM_RET_OK;
}

int metal_pwm_set_freq(struct metal_pwm pwm, unsigned int channel,
                       unsigned int freq) {
    uint32_t index = get_index(pwm);
    uintptr_t base = dt_pwm_data[index].base_addr;
    uint32_t num_channels = dt_pwm_data[index].num_channels;
    
    int ret = METAL_PWM_RET_ERR;

    struct metal_clock clock = dt_metal_data[index].clock;
    if (channel < num_channels) {
        /* Register clock rate change call-backs */
        if (pwm_state[index].freq == 0) {
            metal_clock_callback_t pre_cb = &pwm_state[index].pre_rate_change_callback;
            metal_clock_callback_t post_cb = &pwm_state[index].post_rate_change_callback;

            pre_cb->callback = &pre_rate_change_callback_func;
            pre_cb->priv = pwm;
            dt_clock_register_pre_rate_change_callback(clock, pre_cb);

            post_cb->callback = &post_rate_change_callback_func;
            post_cb->priv = pwm;
            dt_clock_register_post_rate_change_callback(clock, post_cb);
        }

        uint64_t clock_rate = dt_clock_get_rate_hz(clock);

        /* Calculate count value for given PWM frequency */
        uint64_t count;
        uint32_t prescale = 0;
        do {
            count = (clock_rate / (1UL << prescale)) / freq;
        } while ((count > pwm_state[index].max_count) &&
                 (prescale++ < METAL_PWM_MAXPRESCALE));

        pwm_state[index].freq = (clock_rate / (1UL << prescale)) / count;
        pwm_state[index].count_val = --count;

        /* Update values into registers */
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCMP0) = count;
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) |= (prescale & 0x0FUL);

        ret = METAL_PWM_RET_OK;

#if defined(METAL_PWM_DEBUG)
        printf("PWM requested freq:%u set freq:%u \n", freq, pwm_state[index].freq);
        printf("CPU Clk:%u Prescale:%u Count:%u \n", clock_rate, prescale,
               count);
#endif
    }

    return ret;
}

int metal_pwm_set_duty(struct metal_pwm pwm, unsigned int channel,
                       unsigned int duty,
                       metal_pwm_phase_correct_t phase_corr) {
    uint32_t index = get_index(pwm);
    uintptr_t base = dt_pwm_data[index].base_addr;
    uint32_t num_channels = dt_pwm_data[index].num_channels;

    int ret = METAL_PWM_RET_ERR;

    /* Check if duty value is within limits, duty cycle cannot be set for
     * PWMCMP0 */
    if ((channel > 0) && (channel < num_channels) && (duty <= METAL_PWM_MAXDUTY)) {
        /* Calculate PWM compare count value for given duty cycle */
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCMP(channel)) =
            METAL_PWM_GETCMPVAL(duty);
        pwm_state[index].duty[channel] = duty;

        /* Enable / Disable phase correct PWM mode */
        if (phase_corr == METAL_PWM_PHASE_CORRECT_ENABLE) {
            METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) |=
                METAL_PWMCFG_CMPCENTER(channel);
        } else {
            METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) &=
                ~METAL_PWMCFG_CMPCENTER(channel);
        }
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

unsigned int metal_pwm_get_duty(struct metal_pwm pwm, unsigned int channel) {
    uint32_t index = get_index(pwm);
    uintptr_t base = dt_pwm_data[index].base_addr;
    uint32_t num_channels = dt_pwm_data[index].num_channels;

    /* Check for valid parameters and get configured duty cycle value */
    if ((channel > 0) && (channel < num_channels)) {
        return pwm_state[index].duty[channel];
    }
    return 0;
}

unsigned int metal_pwm_get_freq(struct metal_pwm pwm, unsigned int channel) {
    return pwm_state[get_index(pwm)].freq;
}

int metal_pwm_trigger(struct metal_pwm pwm, unsigned int channel,
                      metal_pwm_run_mode_t mode) {
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;

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

    return METAL_PWM_RET_OK;
}

int metal_pwm_stop(struct metal_pwm pwm, unsigned int channel) {
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;

    /* Disable always running mode */
    METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) &= ~METAL_PWMCFG_ENALWAYS;

    return METAL_PWM_RET_OK;
}

int metal_pwm_cfg_interrupt(struct metal_pwm pwm,
                            metal_pwm_interrupt_t flag) {
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;

    if (flag == METAL_PWM_INTERRUPT_ENABLE) {
        /* Enable sticky bit, to make sure interrupts are not forgotten */
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) |= METAL_PWMCFG_STICKY;
    } else {
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) &= ~METAL_PWMCFG_STICKY;
    }

    return METAL_PWM_RET_OK;
}

int metal_pwm_clr_interrupt(struct metal_pwm pwm, unsigned int channel) {
    uintptr_t base = dt_pwm_data[get_index(pwm)].base_addr;
    unsigned int num_channels = dt_pwm_data[index].num_channels;
    int ret = METAL_PWM_RET_ERR;

    if (channel < num_channels) {
        /* Clear interrupt pending bit for given PWM comparator */
        METAL_PWM_REGW(METAL_SIFIVE_PWM0_PWMCFG) &= ~METAL_PWMCFG_CMPIP(channel);
        ret = METAL_PWM_RET_OK;
    }

    return ret;
}

struct metal_interrupt *metal_pwm_interrupt_controller(struct metal_pwm pwm) {
    return dt_pwm_data[get_index(pwm)].interrupt_parent;
}

int metal_pwm_get_interrupt_id(struct metal_pwm pwm, unsigned int channel) {
    return dt_pwm_data[get_index(pwm)].interrupt_id;
}

#endif /* METAL_SIFIVE_PWM0 */

typedef int no_empty_translation_units;
