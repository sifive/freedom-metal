/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_WDOG0_H
#define METAL__DRIVERS__SIFIVE_WDOG0_H

{% include 'template_comment.h' %}

#include <metal/clock.h>
#include <metal/generated/watchdog.h>
#include <metal/interrupt.h>
#include <metal/machine/platform.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static const struct dt_wdog_data {
	uintptr_t base_addr;
	struct metal_clock clock;
	bool has_pinmux;
	uint32_t interrupt_id;
} dt_wdog_data[__METAL_DT_NUM_WDOGS] = {
	{% for wdog in wdogs %}
	{
	    .base_addr = METAL_SIFIVE_WDOG0_{{ wdog.id }}_BASE_ADDR,

	    .clock = (struct metal_clock) { {{ wdog.clocks[0].id }} },

		{% if defined(wdog.interrupt_parent) %}
			.interrupt_parent = (struct metal_interrupt *) = {{ metal_interrupt(wdog.interrupt_parent.id) }},
			.interrupt_id = {{ wdog.interrupts[0] }},
		{% else %}
			.interrupt_parent = NULL,
		{% endif %}
	},
	{% endfor %}
};

{% set driver_string = tosnakecase(wdogs[0].clocks[0].compatible[0]) }
{% include 'clock_dispatch.h' %}

#endif
