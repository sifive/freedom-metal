
#include <metal/interrupt.h>

void metal_riscv_cpu_intc_default_handler();
void metal_riscv_cpu_intc_msip_handler();
void metal_riscv_cpu_intc_mtip_handler();
void metal_riscv_cpu_intc_meip_handler();

/* Interrupt handlers for Local Interrupts */
{% if defined(local_interrupts) %}
  void metal_{{ to_snakecase(local_interrupts.controller) }}_default_handler();
  {% for irq in local_interrupts.irqs %}
    {% if defined(irq.source) %}
      void metal_{{ to_snakecase(irq.source.compatible) }}_source_{{ irq.source.id }}_handler();
    {% else %}
      void metal_{{ to_snakecase(local_interrupts.controller) }}_{{ irq.id }}_handler();
    {% endif %}
  {% endfor %}
{% endif %}

/* Interrupt handlers for External Interrupts */
{% if defined(external_interrupts) %}
  void metal_riscv_plic0_default_handler();
  {% for irq in external_interrupts.irqs %}
    {% if defined(irq.source) %}
      void metal_{{ to_snakecase(irq.source.compatible) }}_source_{{ irq.source.id }}_handler();
    {% else %}
      void metal_riscv_plic0_{{ irq.id }}_handler();
    {% endif %}
  {% endfor %}
{% endif %}
