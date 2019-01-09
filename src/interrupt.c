/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/interrupt.h>
#include <mee/machine.h>

extern inline void mee_interrupt_init(struct mee_interrupt *controller);

extern inline int mee_interrupt_register_handler(struct mee_interrupt *controller,
						 int id,
						 mee_interrupt_handler_t handler,
						 void *priv);

extern inline int mee_interrupt_enable(struct mee_interrupt *controller, int id);

extern inline int mee_interrupt_disable(struct mee_interrupt *controller, int id);

extern inline int mee_interrupt_command_request(struct mee_interrupt *controller,
                                         	int cmd, void *data);
