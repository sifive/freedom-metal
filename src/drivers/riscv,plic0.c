/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/io.h>
#include <metal/shutdown.h>
#include <metal/drivers/riscv,plic0.h>


unsigned int __metal_plic0_claim_interrupt (struct __metal_driver_riscv_plic0 *plic)
{
    return __METAL_ACCESS_ONCE((__metal_io_u32 *)(plic->control_base +
					      METAL_PLIC_CLAIM_OFFSET));
}

void __metal_plic0_complete_interrupt(struct __metal_driver_riscv_plic0 *plic,
				    unsigned int id)
{
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(plic->control_base +
				       METAL_PLIC_CLAIM_OFFSET)) = id;
}

void __metal_plic0_set_threshold(struct __metal_driver_riscv_plic0 *plic,
			       unsigned int threshold)
{
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(plic->control_base +
				       METAL_PLIC_THRESHOLD_OFFSET)) = threshold;
}

void __metal_plic0_set_priority(struct __metal_driver_riscv_plic0 *plic,
			      int id, unsigned int priority)
{
    if ( (plic->max_priority) &&
	 (priority < plic->max_priority) ) {
        __METAL_ACCESS_ONCE((__metal_io_u32 *)(plic->control_base +
					   METAL_PLIC_PRIORITY_OFFSET +
					   (id << METAL_PLIC_SOURCE_PRIORITY_SHIFT))) = priority;
    }
}

void __metal_plic0_enable(struct __metal_driver_riscv_plic0 *plic, int id, int enable)
{
    unsigned int current;
    unsigned long hartid = __metal_myhart_id();

    current = __METAL_ACCESS_ONCE((__metal_io_u32 *)(plic->control_base +
						METAL_PLIC_ENABLE_OFFSET +
						(id >> METAL_PLIC_SOURCE_SHIFT) * 4));
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(plic->control_base +
					METAL_PLIC_ENABLE_OFFSET +
					((id >> METAL_PLIC_SOURCE_SHIFT) * 4))) =
              enable ? (current | (1 << (id & METAL_PLIC_SOURCE_MASK)))
                     : (current & ~(1 << (id & METAL_PLIC_SOURCE_MASK)));
}

void __metal_plic0_default_handler (int id, void *priv) {
    metal_shutdown(300);
}

void __metal_plic0_handler (int id, void *priv)
{
    struct __metal_driver_riscv_plic0 *plic = priv;
    unsigned int idx = __metal_plic0_claim_interrupt(plic);

    if ( (idx < plic->num_interrupts) &&
	 (plic->metal_exint_table[idx]) ) {
	plic->metal_exint_table[idx](idx,
				  plic->metal_exdata_table[idx].exint_data);
    }

    __metal_plic0_complete_interrupt(plic, idx);
}

void __metal_driver_riscv_plic0_init (struct metal_interrupt *controller)
{
    struct __metal_driver_riscv_plic0 *plic = (void *)(controller);

    if ( !plic->init_done ) {
        struct metal_interrupt *intc;

        intc = plic->interrupt_parent;

	/* Initialize ist parent controller, aka cpu_intc. */
	intc->vtable->interrupt_init(intc);

	for (int i = 0; i < plic->num_interrupts; i++) {
	    __metal_plic0_enable(plic, i, METAL_DISABLE);
	    __metal_plic0_set_priority(plic, i, 0);
	    plic->metal_exint_table[i] = NULL;
	    plic->metal_exdata_table[i].sub_int = NULL;
	    plic->metal_exdata_table[i].exint_data = NULL;
	}

	__metal_plic0_set_threshold(plic, 0);

	/* Register plic (ext) interrupt with with parent controller */
        intc->vtable->interrupt_register(intc,
                                         plic->interrupt_line,
                                         NULL, plic);
	/* Register plic handler for dispatching its device interrupts */
	intc->vtable->interrupt_register(intc,
					 plic->interrupt_line,
					 __metal_plic0_handler, plic);
	/* Enable plic (ext) interrupt with with parent controller */
        intc->vtable->interrupt_enable(intc, plic->interrupt_line);
        plic->init_done = 1;
    }
}

int __metal_driver_riscv_plic0_register (struct metal_interrupt *controller,
			               int id, metal_interrupt_handler_t isr,
			               void *priv)
{
    struct __metal_driver_riscv_plic0 *plic = (void *)(controller);

    if (id >= plic->num_interrupts ) {
        return -1;
    }
 
    if (isr) {
        __metal_plic0_set_priority(plic ,id, 2);
	plic->metal_exint_table[id] = isr;
	plic->metal_exdata_table[id].exint_data = priv;
    } else {
        __metal_plic0_set_priority(plic, id, 1);
	plic->metal_exint_table[id] = __metal_plic0_default_handler;
	plic->metal_exdata_table[id].sub_int = priv;
    }

    return 0;
}

int __metal_driver_riscv_plic0_enable (struct metal_interrupt *controller, int id)
{
    struct __metal_driver_riscv_plic0 *plic = (void *)(controller);

    if ( id >= plic->num_interrupts ) {
        return -1;
    }

    __metal_plic0_enable(plic, id, METAL_ENABLE);
    return 0;
}

int __metal_driver_riscv_plic0_disable (struct metal_interrupt *controller, int id)
{
    struct __metal_driver_riscv_plic0 *plic = (void *)(controller);

    if ( id >= plic->num_interrupts ) {
        return -1;
    }
    __metal_plic0_enable(plic, id, METAL_DISABLE);
    return 0;
}
