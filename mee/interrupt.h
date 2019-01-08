/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__INTERRUPT_H
#define MEE__INTERRUPT_H

#include <stddef.h>

typedef void (*mee_interrupt_handler_t) (int, void *);

struct mee_interrupt;

struct mee_interrupt_vtable {
    void (*interrupt_init)(struct mee_interrupt *controller);
    int (*interrupt_register)(struct mee_interrupt *controller, int id,
			      mee_interrupt_handler_t isr, void *priv_data);
    int (*interrupt_enable)(struct mee_interrupt *controller, int id);
    int (*interrupt_disable)(struct mee_interrupt *controller, int id);
    int (*command_request)(struct mee_interrupt *controller, int cmd, void *data);
};

struct mee_interrupt {
    const struct mee_interrupt_vtable *vtable;
};

/* Initialize a given interrupt controller */
inline void mee_interrupt_init(struct mee_interrupt *controller)
{
    return controller->vtable->interrupt_init(controller);
}


/* Registering an interrupt handler for a given interrupt controller and id */
inline int mee_interrupt_register_handler(struct mee_interrupt *controller,
                                          int id,
                                          mee_interrupt_handler_t handler,
                                          void *priv_data)
{
    return controller->vtable->interrupt_register(controller, id, handler, priv_data);
}

/* Enable an interrupt for a given controller and id */
inline int mee_interrupt_enable(struct mee_interrupt *controller, int id)
{
    return controller->vtable->interrupt_enable(controller, id);
}

/* Disable an interrupt for a given controller and id */
inline int mee_interrupt_disable(struct mee_interrupt *controller, int id)
{
    return controller->vtable->interrupt_disable(controller, id);
}

/* Utilities function to controll, manages devices via a given interrupt controller */
inline int mee_interrupt_command_request(struct mee_interrupt *controller,
					 int cmd, void *data)
{
    return controller->vtable->command_request(controller, cmd, data);
}

#endif
