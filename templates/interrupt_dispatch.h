
#include <metal/drivers/{{ driver_string }}.h>

#define dt_interrupt_init(intc) __metal_driver_{{ driver_string }}_init((intc))

#define dt_interrupt_set_vector_mode(intc, mode) __metal_driver_{{ driver_string }}_set_vector_mode((intc), (mode))
#define dt_interrupt_get_vector_mode(intc) __metal_driver_{{ driver_string }}_get_vector_mode((intc))

#define dt_interrupt_set_privilege(intc, privilege) __metal_driver_{{ driver_string }}_set_privilege((intc), (privilege))
#define dt_interrupt_get_privilege(intc) __metal_driver_{{ driver_string }}_get_privilege((intc))

#define dt_interrupt_clear(intc) __metal_driver_{{ driver_string }}_clear((intc))
#define dt_interrupt_set(intc) __metal_driver_{{ driver_string }}_set((intc))

#define dt_interrupt_register_handler(intc, id, handler, priv_data) __metal_driver_{{ driver_string }}_register_handler((intc), (id), (handler), (priv_data))
#define dt_interrupt_register_vector_handler(intc, id, handler, priv_data) __metal_driver_{{ driver_string }}_register_vector_handler((intc), (id), (handler), (priv_data))

#define dt_interrupt_enable(intc, id) __metal_driver_{{ driver_string }}_enable((intc), (id))
#define dt_interrupt_disable(intc, id) __metal_driver_{{ driver_string }}_disable((intc), (id))

#define dt_interrupt_set_threshold(intc, level) __metal_driver_{{ driver_string }}_set_threshold((intc), (level))
#define dt_interrupt_get_threshold(intc) __metal_driver_{{ driver_string }}_get_threshold((intc))

#define dt_interrupt_set_priority(intc, id, prio) __metal_driver_{{ driver_string }}_set_priority((intc), (id), (prio))
#define dt_interrupt_get_priority(intc, id) __metal_driver_{{ driver_string }}_get_priority((intc), (id))

#define dt_interrupt_set_preemptive_level(intc, id, level) __metal_driver_{{ driver_string }}_set_preemptive_level((intc), (id), (level))
#define dt_interrupt_get_preemptive_level(intc, id) __metal_driver_{{ driver_string }}_get_preemptive_level((intc), (id))

#define dt_interrupt_vector_enable(intc, id) __metal_driver_{{ driver_string }}_vector_enable((intc), (id))
#define dt_interrupt_vector_disable(intc, id) __metal_driver_{{ driver_string }}_vector_disable((intc), (id))

#define dt_interrupt_affinity_enable(intc, bitmask, id) __metal_driver_{{ driver_string }}_affinity_enable((intc), (bitmask), (id))
#define dt_interrupt_affinity_disable(intc, bitmask, id) __metal_driver_{{ driver_string }}_affinity_disable((intc), (bitmask), (id))

#define dt_interrupt_affinity_set_threshold(intc, bitmask, level) __metal_driver_{{ driver_string }}_affinity_set_threshold((intc), (bitmask), (level))
#define dt_interrupt_affinity_get_threshold(intc, context_id) __metal_driver_{{ driver_string }}_affinity_get_threshold((intc), (context_id))

