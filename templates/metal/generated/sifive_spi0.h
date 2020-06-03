/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_SPI0_H
#define METAL__DRIVERS__SIFIVE_SPI0_H

{% include 'template_comment.h' %}

#include <metal/clock.h>
#include <metal/generated/spi.h>
#include <metal/gpio.h>
#include <metal/interrupt.h>
#include <metal/machine/platform.h>
#include <stdbool.h>
#include <stddef.h>

static const struct dt_spi_data {
	uintptr_t base_addr;
	struct metal_clock *clock;
	bool has_pinmux;
	struct metal_gpio pinmux;
	uint32_t pinmux_output_selector;
	uint32_t pinmux_source_selector;
	struct metal_interrupt *interrupt_parent;
	uint32_t interrupt_id;
} dt_spi_data[__METAL_DT_NUM_spiS] = {
	{% for spi in spis %}
	{
	    .base_addr = METAL_SIFIVE_spi0_{{ spi.id }}_BASE_ADDR,

	    {% if defined(spi.clocks) %}
		    .clock = (struct metal_clock *) {{ metal_clock(spi.clocks[0].id) }},
	    {% else %}
		    .clock = NULL,
	    {% endif %}

	    .has_pinmux = {{ defined(spi.pinmux) ? 1 : 0 }},
	    {% if defined(spi.pinmux) %}
		    .pinmux = (struct metal_gpio) { {{ spi.pinmux.id }} },
			.pinmux_output_selector = {{ spi.pinmux.output_selector }},
			.pinmux_source_selector = {{ spi.pinmux.source_selector }},
		{% endif %}

		{% if defined(spi.interrupt_parent) %}
			.interrupt_parent = (struct metal_interrupt *) = {{ metal_interrupt(spi.interrupt_parent.id) }},
			.interrupt_id = {{ spi.interrupts[0] }},
		{% else %}
			.interrupt_parent = NULL,
		{% endif %}
	},
	{% endfor %}
};

#endif
