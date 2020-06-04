#ifndef __METAL_DT_PMP__H
#define __METAL_DT_PMP__H

{% include 'template_comment.h' %}

#include <metal/generated/cpu.h>

static uint32_t num_pmp_regions[__METAL_DT_NUM_HARTS] = {
	{% for hart in harts %}
		{{ hart.num_pmp_regions }},
	{% endfor %}
};

#endif /* ! __METAL_DT_PMP__H */