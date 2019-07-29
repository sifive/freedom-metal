/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/interrupt.h>
#include <metal/machine.h>

extern __inline__ void metal_interrupt_init(struct metal_interrupt *controller);

extern __inline__ int metal_interrupt_register_handler(struct metal_interrupt *controller,
						 int id,
						 metal_interrupt_handler_t handler,
						 void *priv);

extern __inline__ int metal_interrupt_enable(struct metal_interrupt *controller, int id);

extern __inline__ int metal_interrupt_disable(struct metal_interrupt *controller, int id);

extern __inline__ int metal_interrupt_vector_enable(struct metal_interrupt *controller,
                                                     int id, metal_vector_mode mode);

extern __inline__ int metal_interrupt_vector_disable(struct metal_interrupt *controller, int id);

extern __inline__ int _metal_interrupt_command_request(struct metal_interrupt *controller,
                                         	int cmd, void *data);
