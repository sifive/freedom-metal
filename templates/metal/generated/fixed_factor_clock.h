/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__FIXED_FACTOR_CLOCK_H
#define METAL__DRIVERS__FIXED_FACTOR_CLOCK_H

{% include 'template_comment.h' %}

{% if fixed_factor_clocks is defined %}

#include <metal/clock.h>

#define __METAL_DT_NUM_FIXED_FACTOR_CLOCKS {{ fixed_factor_clocks|length }}

static const struct dt_fixed_factor_clock_data {
	uint32_t mul;
	uint32_t div;
	struct metal_clock ref;
} dt_clock_data[__METAL_DT_NUM_FIXED_FACTOR_CLOCKS] = {
	{% for clk in fixed_factor_clocks %}
		.mul = {{ clk.clock_mult }},
		.div = {{ clk.clock_div }},
		.ref = (struct metal_clock) { {{ clk.clocks[0].id }} }
	{% endfor %}
};

{% set driver_string = to_snakecase(fixed_factor_clocks[0].clocks[0].compatible[0]) %}
{% include 'clock_dispatch.h' %}

{% endif %}

#endif

