/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__FIXED_CLOCK_H
#define METAL__DRIVERS__FIXED_CLOCK_H

{% include 'template_comment.h' %}

#include <metal/clock.h>

#define __METAL_DT_NUM_FIXED_CLOCKS {{ fixed_clocks|length }}

static const uint64_t dt_clock_rate[__METAL_DT_NUM_FIXED_CLOCKS] = {
	{% for clk in fixed_clocks %}
		{{ clk.clock_frequency[0] }},
	{% endfor %}
};

#endif
