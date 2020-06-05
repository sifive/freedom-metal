/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_RTC0_H
#define METAL__DRIVERS__SIFIVE_RTC0_H

{% include 'template_comment.h' %}

#include <metal/clock.h>
#include <metal/interrupt.h>
#include <metal/machine/platform.h>
#include <metal/rtc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static const struct dt_rtc_data {
	uintptr_t base_addr;
	struct metal_clock clock;
	struct metal_interrupt *interrupt_parent;
	uint32_t interrupt_id;
} dt_rtc_data[__METAL_DT_NUM_RTCS] = {
	{% for rtc in rtcs %}
	{
	    .base_addr = METAL_SIFIVE_RTC0_{{ rtc.id }}_BASE_ADDR,

	    .clock = (struct metal_clock) { {{ rtc.clocks[0].id }} },

		{% if defined(rtc.interrupt_parent) %}
			.interrupt_parent = (struct metal_interrupt *) = {{ metal_interrupt(rtc.interrupt_parent.id) }},
			.interrupt_id = {{ rtc.interrupts[0] }},
		{% else %}
			.interrupt_parent = NULL,
		{% endif %}
	},
	{% endfor %}
};

{% set driver_string = tosnakecase(rtcs[0].clocks[0].compatible[0]) }
{% include 'clock_dispatch.h' %}

#endif
