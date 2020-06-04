#ifndef __METAL_DT_SIFIVE_BUSERROR0__H
#define __METAL_DT_SIFIVE_BUSERROR0__H

{% include 'template_comment.h' %}

#include <metal/interrupt.h>

#define __METAL_DT_NUM_BEUS {{ num_beus }}

static struct dt_beu_data {
	uintptr_t base_addr;
	struct metal_interrupt *interrupt_parent;
	uint32_t interrupt_id;
} dt_beu_data[__METAL_DT_NUM_BEUS] = {
	{% for beu in beus %}
	{
		.base_addr = {{ beu.base_addr }},
		.interrupt_parent = (struct metal_interrupt *) {{ metal_interrupt(beu.interrupt_parent.id) }},
		.interrupt_id = {{ beu.interrupts[0] }},
	}
	{% endfor %}
}

#endif /* ! __METAL_DT_SIFIVE_BUSERROR0__H */