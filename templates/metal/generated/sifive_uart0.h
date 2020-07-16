/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_UART0_H
#define METAL__DRIVERS__SIFIVE_UART0_H

{% include 'template_comment.h' %}

#include <metal/clock.h>
#include <metal/generated/uart.h>
#include <metal/gpio.h>
#include <metal/interrupt.h>
#include <metal/machine/platform.h>
#include <stdbool.h>
#include <stddef.h>

static const struct dt_uart_data {
	uintptr_t base_addr;
	struct metal_clock clock;
	bool has_pinmux;
	struct metal_gpio pinmux;
	uint32_t pinmux_output_selector;
	uint32_t pinmux_source_selector;
	struct metal_interrupt *interrupt_parent;
	uint32_t interrupt_id;
} dt_uart_data[__METAL_DT_NUM_UARTS] = {
	{% for uart in uarts %}
	{
	    .base_addr = METAL_SIFIVE_UART0_{{ uart.id }}_BASE_ADDR,

	    .clock = (struct metal_clock) { {{ uart.clocks[0].id }} },

	    .has_pinmux = {{ defined(uart.pinmux) ? 1 : 0 }},
	    {% if defined(uart.pinmux) %}
		    .pinmux = (struct metal_gpio) { {{ uart.pinmux.id }} },
			.pinmux_output_selector = {{ uart.pinmux.output_selector }},
			.pinmux_source_selector = {{ uart.pinmux.source_selector }},
		{% endif %}

		{% if defined(uart.interrupt_parent) %}
			.interrupt_parent = (struct metal_interrupt *) = {{ metal_interrupt(uart.interrupt_parent.id) }},
			.interrupt_id = {{ uart.interrupts[0] }},
		{% else %}
			.interrupt_parent = NULL,
		{% endif %}
	},
	{% endfor %}
};

{% set driver_string = tosnakecase(uarts[0].clocks[0].compatible[0]) }
{% include 'clock_dispatch.h' %}

#endif
