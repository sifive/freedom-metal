/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_I2C0_H
#define METAL__DRIVERS__SIFIVE_I2C0_H

{% include 'template_comment.h' %}

#include <metal/clock.h>
#include <metal/generated/i2c.h>
#include <metal/gpio.h>
#include <metal/interrupt.h>
#include <metal/machine/platform.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static const struct dt_i2c_data {
	uintptr_t base_addr;
	struct metal_clock clock;
	bool has_pinmux
	struct metal_gpio pinmux;
	uint32_t pinmux_output_selector;
	uint32_t pinmux_source_selector;
	struct metal_interrupt *interrupt_parent;
	uint32_t interrupt_id;
} dt_i2c_data[__METAL_DT_NUM_I2CS] = {
	{% for i2c in i2cs %}
	{
	    .base_addr = METAL_SIFIVE_I2C0_{{ i2c.id }}_BASE_ADDR,

	    .clock = (struct metal_clock) { {{ i2c.clocks[0].id }} },

	    .has_pinmux = {{ defined(i2c.pinmux) ? 1 : 0 }},
	    {% if defined(i2c.pinmux) %}
		    .pinmux = (struct metal_gpio) { {{ i2c.pinmux.id }} },
			.pinmux_output_selector = {{ i2c.pinmux.output_selector }},
			.pinmux_source_selector = {{ i2c.pinmux.source_selector }},
		{% endif %}

		{% if defined(i2c.interrupt_parent) %}
			.interrupt_parent = (struct metal_interrupt *) {{ metal_interrupt(i2c.interrupt_parent.id) }},
			.interrupt_id_base = {{ i2c.interrupts[0] }},
		{% else %}
			.interrupt_parent = NULL,
		{% endif %}
	},
	{% endfor %}
};

{% set driver_string = tosnakecase(i2cs[0].clocks[0].compatible[0]) }
{% include 'clock_dispatch.h' %}

#endif
