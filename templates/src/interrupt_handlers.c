/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/interrupt_handlers.h>

{% if local_interrupts is defined %}
{% for irq in local_interrupts.irqs %}
void metal_{{ to_snakecase(irq.source.compatible) }}_source_{{ irq.source.id }}_handler() __attribute((interrupt, weak));
void metal_{{ to_snakecase(irq.source.compatible) }}_source_{{ irq.source.id }}_handler() {}

{% endfor %}
{% endif %}

{% if global_interrupts is defined %}
{% for irq in global_interrupts.irqs %}
void metal_{{ to_snakecase(irq.source.compatible) }}_source_{{ irq.source.id }}_handler() __attribute((weak));
void metal_{{ to_snakecase(irq.source.compatible) }}_source_{{ irq.source.id }}_handler() {}
{% endfor %}
{% endif %}
