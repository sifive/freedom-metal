/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_CCACHE0_H
#define METAL__DRIVERS__SIFIVE_CCACHE0_H

{% include 'template_comment.h' %}

#include <metal/generated/cache.h>
#include <metal/machine/platform.h>

static const struct dt_cache_data {
	uintptr_t base_addr;
} dt_cache_data[__METAL_DT_NUM_CACHES] = {
	{% for cache in caches %}
	{
	    .base_addr = METAL_{{ todefine(cache.compatible[0]) }}_{{ cache.id }}_BASE_ADDR,
	},
	{% endfor %}
};

#endif
