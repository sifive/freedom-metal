/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/io.h>
#include <metal/shutdown.h>
#include <metal/drivers/sifive,global-external-interrupts0.h>

void __metal_driver_sifive_global_external_interrupt_init(struct metal_interrupt *controller)
{
    struct __metal_driver_sifive_global_external_interrupts0 *global0;

    global0 = (struct __metal_driver_sifive_global_external_interrupts0 *)(controller);
    if ( !global0->init_done ) {
        struct metal_interrupt *intc = global0->interrupt_parent;

	if (intc) {
	    intc->vtable->interrupt_init(intc);
	    /* Register its interrupts with with parent controller */
            for (int i = 0; i < global0->num_interrupts; i++) {
	    	intc->vtable->interrupt_register(intc,
						global0->interrupt_lines[i],
						NULL, global0);
	    }
            global0->init_done = 1;
	}
    }
}

int __metal_driver_sifive_global_external_interrupt_register(struct metal_interrupt *controller,
                                                           int id, metal_interrupt_handler_t isr,
                                                           void *priv)
{
    int rc = -1;
    struct __metal_driver_sifive_global_external_interrupts0 *global0 =
                              (struct __metal_driver_sifive_global_external_interrupts0 *)(controller);

    if (id != 0) {
        struct metal_interrupt *intc = global0->interrupt_parent;

        /* Enable its interrupts with parent controller */
        if (intc) {
            rc = intc->vtable->interrupt_register(intc, id, isr, priv);
        }
    }
    return rc;
}

int __metal_driver_sifive_global_external_interrupt_enable(struct metal_interrupt *controller, int id)
{
    int rc = -1;
    struct __metal_driver_sifive_global_external_interrupts0 *global0 =
                              (struct __metal_driver_sifive_global_external_interrupts0 *)(controller);

    if (id != 0) {
        struct metal_interrupt *intc = global0->interrupt_parent;

        /* Enable its interrupts with parent controller */
        if (intc) {
            rc = intc->vtable->interrupt_enable(intc, id);
        }
    }
    return rc;
}

int __metal_driver_sifive_global_external_interrupt_disable(struct metal_interrupt *controller, int id)
{
    int rc = -1;
    struct __metal_driver_sifive_global_external_interrupts0 *global0 =
                              (struct __metal_driver_sifive_global_external_interrupts0 *)(controller);

    if (id != 0) {
        struct metal_interrupt *intc = global0->interrupt_parent;

        /* Enable its interrupts with parent controller */
        if (intc) {
            rc = intc->vtable->interrupt_disable(intc, id);
        }
    }
    return rc;
}

int __metal_driver_sifive_global_external_command_request (struct metal_interrupt *controller,
                                                         int command, void *data)
{
    int index;
    int rc = -1;
    struct __metal_driver_sifive_global_external_interrupts0 *global0 =
                              (struct __metal_driver_sifive_global_external_interrupts0 *)(controller);

    switch (command) {
    case METAL_MAX_INTERRUPT_GET:
        rc = global0->num_interrupts;
        break;
    case METAL_INDEX_INTERRUPT_GET:
        rc = 0;
        if (data) {
            index = *(int *)data;
            rc = global0->interrupt_lines[index];
        }
        break;
    default:
        break;
    }

    return rc;
}
