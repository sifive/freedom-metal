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
	struct metal_interrupt interrupt_parent;
	uint32_t interrupt_id;
} dt_uart_data[__METAL_DT_NUM_UARTS] = {
	{% for uart in sifive_uart0s %}
	{
	    .base_addr = METAL_SIFIVE_UART0_{{ uart.id }}_BASE_ADDR,

	{% if uart.clocks is defined %}
	    /* {{ uart.clocks[0].compatible[0] }} */
	    .clock = (struct metal_clock) { {{ uart.clocks[0].id }} },
	{% endif %}

	{% if uart.pinmux is defined %}
		.has_pinmux = 1,
		.pinmux = (struct metal_gpio) { {{ uart.pinmux[0].id }} },
		.pinmux_output_selector = {{ uart.pinmux[1] }},
		.pinmux_source_selector = {{ uart.pinmux[2] }},
	{% else %}
		.has_pinmux = 0,
	{% endif %}

	{% if uart.interrupt_parent is defined %}
	    /* {{ uart.interrupt_parent[0].compatible[0] }} */
		.interrupt_parent = (struct metal_interrupt) { {{ uart.interrupt_parent[0].id }} },
		.interrupt_id = {{ uart.interrupts[0] }},
	{% endif %}
	},
	{% endfor %}
};

{% if sifive_uart0s[0].clocks is defined %}
{% set driver_string = to_snakecase(sifive_uart0s[0].clocks[0].compatible[0]) %}
{% include 'clock_dispatch.h' %}
{% endif %}

{% if sifive_uart0s[0].interrupt_parent is defined %}
{% set driver_string = to_snakecase(sifive_uart0s[0].interrupt_parent[0].compatible[0]) %}
{% include 'interrupt_dispatch.h' %}
{% endif %}

{% if chosen.stdout_path is defined %}
{% if chosen.stdout_path[0].compatible[0] == "sifive,uart0" %}
#define METAL_STDOUT_SIFIVE_UART0
#define __METAL_DT_STDOUT_UART_HANDLE ((struct metal_uart) { {{ chosen.stdout_path[0].id }} })
#define __METAL_DT_STDOUT_UART_BAUD {{ chosen.stdout_path[1] }}
{% endif %}
{% endif %}

#endif
