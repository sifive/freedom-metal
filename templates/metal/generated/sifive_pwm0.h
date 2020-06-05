/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_PWM0_H
#define METAL__DRIVERS__SIFIVE_PWM0_H

{% include 'template_comment.h' %}

#include <metal/clock.h>
#include <metal/interrupt.h>
#include <metal/machine/platform.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Max possible PWM channel count */
#define METAL_MAX_PWM_CHANNELS 16

static const struct dt_pwm_data {
	uintptr_t base_addr;
	uint32_t num_channels;
	struct metal_clock clock;
	bool has_pinmux;
	struct metal_gpio pinmux;
	uint32_t pinmux_output_selector;
	uint32_t pinmux_source_selector;
	struct metal_interrupt *interrupt_parent;
	uint32_t interrupt_id;
} dt_pwm_data[__METAL_DT_NUM_PWMS] = {
	{% for pwm in pwms %}
	{
	    .base_addr = METAL_SIFIVE_PWM0_{{ pwm.id }}_BASE_ADDR,

	    .num_channels = {{ pwm.sifive_ncomparators[0] }},

	    .clock = (struct metal_clock) { {{ pwm.clocks[0].id }} },

	    .has_pinmux = {{ defined(pwm.pinmux) ? 1 : 0 }},
	    {% if defined(pwm.pinmux) %}
		    .pinmux = (struct metal_gpio) { {{ pwm.pinmux.id }} },
			.pinmux_output_selector = {{ pwm.pinmux.output_selector }},
			.pinmux_source_selector = {{ pwm.pinmux.source_selector }},
		{% endif %}

		{% if defined(pwm.interrupt_parent) %}
			.interrupt_parent = (struct metal_interrupt *) = {{ metal_interrupt(pwm.interrupt_parent.id) }},
			.interrupt_id = {{ pwm.interrupts[0] }},
		{% else %}
			.interrupt_parent = NULL,
		{% endif %}
	},
	{% endfor %}
}

{% set driver_string = tosnakecase(pwms[0].clocks[0].compatible[0]) }
{% include 'clock_dispatch.h' %}

#endif
