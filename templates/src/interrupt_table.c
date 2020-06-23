/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/interrupt_handlers.h>

#ifndef METAL_HLIC_VECTORED

{% if local_interrupts is defined %}
#define __METAL_NUM_LOCAL_INTERRUPTS {{ 16 + local_interrupts.irqs|length }}
{% else %}
#define __METAL_NUM_LOCAL_INTERRUPTS 16
{% endif %}

metal_interrupt_vector_handler_t __metal_vector_table[__METAL_NUM_LOCAL_INTERRUPTS] = {
    __metal_exception_handler,
    metal_riscv_cpu_intc_default_handler,
    metal_riscv_cpu_intc_default_handler,
    metal_riscv_cpu_intc_default_handler,
    metal_riscv_cpu_intc_msip_handler,
    metal_riscv_cpu_intc_default_handler,
    metal_riscv_cpu_intc_default_handler,
    metal_riscv_cpu_intc_default_handler,
    metal_riscv_cpu_intc_mtip_handler,
{% if external_interrupts is defined %}
    metal_riscv_plic0_source_0_handler,
{% else %}
    metal_riscv_cpu_intc_meip_handler,
{% endif %}
    metal_riscv_cpu_intc_default_handler,
    metal_riscv_cpu_intc_default_handler,
    metal_riscv_cpu_intc_default_handler,
    metal_riscv_cpu_intc_default_handler,
{% if local_interrupts is defined %}
{% for irq in local_interrupts.irqs %}
    {% if irq.source is defined %}
    metal_{{ to_snakecase(irq.source.compatible) }}_source_{{ irq.source.id }}_handler,
    {% else %}
    metal_{{ local_interrupts.controller }}_{{ irq.id }}_handler,
    {% endif %}
{% endfor %}
{% endif %}
};

#endif /* ! METAL_HLIC_VECTORED */

{% if global_interrupts is defined %}

#define __METAL_NUM_GLOBAL_INTERRUPTS {{ global_interrupts.irqs|length }}

metal_interrupt_handler_t __metal_global_interrupt_table[__METAL_NUM_GLOBAL_INTERRUPTS] = {
{% for irq in global_interrupts.irqs %}
    {% if irq.source is defined %}
    metal_{{ to_snakecase(irq.source.compatible) }}_source_{{ irq.source.id }}_handler,
    {% else %}
    metal_{{ global_interrupts.controller }}_{{ irq.id }}_handler,
    {% endif %}
{% endfor %}
};

{% else %}

#define __METAL_NUM_GLOBAL_INTERRUPTS 0

{% endif %}
