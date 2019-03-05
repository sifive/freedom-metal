/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/interrupt.h>
#include <metal/machine.h>

extern inline void metal_interrupt_init(const struct metal_interrupt *controller);

extern inline int metal_interrupt_register_handler(const struct metal_interrupt *controller,
						 int id,
						 metal_interrupt_handler_t handler,
						 void *priv);

extern inline int metal_interrupt_enable(const struct metal_interrupt *controller, int id);

extern inline int metal_interrupt_disable(const struct metal_interrupt *controller, int id);

extern inline int metal_interrupt_vector_enable(const struct metal_interrupt *controller,
                                                     int id, metal_vector_mode mode);

extern inline int metal_interrupt_vector_disable(const struct metal_interrupt *controller, int id);

extern inline int _metal_interrupt_command_request(const struct metal_interrupt *controller,
                                         	int cmd, void *data);
