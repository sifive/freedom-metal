/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FE310_G000_HFXOSC_CLOCK_H
#define METAL__DRIVERS__SIFIVE_FE310_G000_HFXOSC_CLOCK_H

{% include 'template_comment.h' %}

#include <metal/clock.h>

#define __METAL_DT_NUM_SIFIVE_FE310_G000_HFXOSC_CLOCKS {{ sifive_fe310_g000_hfxoscs|length }}

static const struct dt_sifive_fe310_g000_hfxosc_clock_data {
	uintptr_t base;
	struct metal_clock ref;
} dt_clock_data[__METAL_DT_NUM_SIFIVE_FE310_G000_HFXOSC_CLOCKS] = {
	{% for clk in sifive_fe310_g000_hfxoscs %}
	{
		.base = {{ clk.regs_by_name['config'] }},
		.ref = (struct metal_clock) { {{ clk.clocks[0].id }} },
	},
	{% endfor %}
};

{% set driver_string = to_snakecase(sifive_fe310_g000_hfxoscs[0].clocks[0].compatible[0]) %} 
{% include 'clock_dispatch.h' %}

#endif
