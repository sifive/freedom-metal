/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_PWM0

#include <metal/clock.h>
#include <metal/cpu.h>
#include <metal/drivers/sifive_pwm0_regs.h>
#include <metal/io.h>
#include <metal/private/metal_private_sifive_pwm0.h>
#include <metal/time.h>

#define METAL_PWMCFG_CMPCENTER(x) (1UL << (PWM_PWMCFG_PWMCMPNCENTER_Pos + x))
#define METAL_PWMCFG_INVERT(x) (1UL << (PWM_PWMCFG_PWMINVERTN_Pos + x))
#define METAL_PWMCFG_GANG(x) (1UL << (PWM_PWMCFG_PWMGANGN_Pos + x))
#define METAL_PWMCFG_CMPIP(x) (1UL << (PWM_PWMCFG_PWMIPN_Pos + x))

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
    PWM_Type *PWM_regs = (PWM_Type *)dt_pwm_data[get_index(pwm)].base_addr;
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
                PWM_regs->PWMCMP[idx] = METAL_PWM_GETCMPVAL(duty);
            }
        }
    }
}

int sifive_pwm0_enable(struct metal_pwm pwm) {
    int ret = METAL_PWM_RET_ERR;

    if (dt_pwm_data[get_index(pwm)].base_addr != 0) {
        PWM_Type *PWM_regs = (PWM_Type *)dt_pwm_data[get_index(pwm)].base_addr;
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
        PWM_regs->PWMCFG = 0;
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

int sifive_pwm0_disable(struct metal_pwm pwm) {
    int ret = METAL_PWM_RET_ERR;

    if (dt_pwm_data[get_index(pwm)].base_addr != 0) {
        PWM_Type *PWM_regs = (PWM_Type *)dt_pwm_data[get_index(pwm)].base_addr;
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
    PWM_Type *PWM_regs = (PWM_Type *)dt_pwm_data[get_index(pwm)].base_addr;
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
        PWM_regs->PWMCMP[0] = count;
        PWM_regs->PWMCFG |= (prescale & 0x0FUL);
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
    PWM_Type *PWM_regs = (PWM_Type *)dt_pwm_data[get_index(pwm)].base_addr;
    uint8_t cmp_count = dt_pwm_data[get_index(pwm)].comparator_count;
    int ret = METAL_PWM_RET_ERR;

    /* Check if duty value is within limits, duty cycle cannot be set for
     * PWMCMP0 */
    if ((idx > 0) && (idx < cmp_count) && (duty <= METAL_PWM_MAXDUTY)) {
        /* Calculate PWM compare count value for given duty cycle */
        PWM_regs->PWMCMP[idx] = METAL_PWM_GETCMPVAL(duty);
        pwm_state[get_index(pwm)].duty[idx] = duty;

        /* Enable / Disable phase correct PWM mode */
        if (phase_corr == METAL_PWM_PHASE_CORRECT_ENABLE) {
            PWM_regs->PWMCFG |= METAL_PWMCFG_CMPCENTER(idx);
        } else {
            PWM_regs->PWMCFG &= ~METAL_PWMCFG_CMPCENTER(idx);
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
    int ret = METAL_PWM_RET_ERR;

    (void)idx; /* Unused parameter,for later use */

    if (dt_pwm_data[get_index(pwm)].base_addr != 0) {
        PWM_Type *PWM_regs = (PWM_Type *)dt_pwm_data[get_index(pwm)].base_addr;

        /* Configure for requested PWM run mode */
        if (mode == METAL_PWM_CONTINUOUS) {
            PWM_regs->PWMCFG |= PWM_PWMCFG_PWMDEGLITCH_Msk |
                                PWM_PWMCFG_PWMZEROCMP_Msk |
                                PWM_PWMCFG_PWMENALWAYS_Msk;
        } else {
            PWM_regs->PWMCFG |= PWM_PWMCFG_PWMDEGLITCH_Msk |
                                PWM_PWMCFG_PWMZEROCMP_Msk |
                                PWM_PWMCFG_PWMONESHOT_Msk;
        }
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

int sifive_pwm0_stop(struct metal_pwm pwm, uint32_t idx) {
    int ret = METAL_PWM_RET_ERR;

    (void)idx; /* Unused parameter,for later use */

    if (dt_pwm_data[get_index(pwm)].base_addr != 0) {
        PWM_Type *PWM_regs = (PWM_Type *)dt_pwm_data[get_index(pwm)].base_addr;

        /* Disable always running mode */
        PWM_regs->PWMCFG &= ~PWM_PWMCFG_PWMENALWAYS_Msk;
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

int sifive_pwm0_cfg_interrupt(struct metal_pwm pwm,
                              metal_pwm_interrupt_t flag) {
    int ret = METAL_PWM_RET_ERR;

    if (dt_pwm_data[get_index(pwm)].base_addr != 0) {
        PWM_Type *PWM_regs = (PWM_Type *)dt_pwm_data[get_index(pwm)].base_addr;

        if (flag == METAL_PWM_INTERRUPT_ENABLE) {
            /* Enable sticky bit, to make sure interrupts are not forgotten */
            PWM_regs->PWMCFG |= PWM_PWMCFG_PWMSTICKY_Msk;
        } else {
            PWM_regs->PWMCFG &= ~PWM_PWMCFG_PWMSTICKY_Msk;
        }
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

int sifive_pwm0_clear_interrupt(struct metal_pwm pwm, uint32_t idx) {
    uint8_t cmp_count = dt_pwm_data[get_index(pwm)].comparator_count;
    int ret = METAL_PWM_RET_ERR;

    if ((dt_pwm_data[get_index(pwm)].base_addr != 0) && (idx < cmp_count)) {
        PWM_Type *PWM_regs = (PWM_Type *)dt_pwm_data[get_index(pwm)].base_addr;
        /* Clear interrupt pending bit for given PWM comparator */
        PWM_regs->PWMCFG &= ~METAL_PWMCFG_CMPIP(idx);
        ret = METAL_PWM_RET_OK;
    }
    return ret;
}

#endif /* METAL_SIFIVE_PWM0 */

typedef int no_empty_translation_units;
