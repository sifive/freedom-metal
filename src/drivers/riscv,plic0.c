/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/io.h>
#include <mee/shutdown.h>
#include <mee/drivers/riscv,plic0.h>


unsigned int __mee_plic0_claim_interrupt (struct __mee_driver_riscv_plic0 *plic)
{
    return __MEE_ACCESS_ONCE((__mee_io_u32 *)(plic->control_base +
					      MEE_PLIC_CLAIM_OFFSET));
}

void __mee_plic0_complete_interrupt(struct __mee_driver_riscv_plic0 *plic,
				    unsigned int id)
{
    __MEE_ACCESS_ONCE((__mee_io_u32 *)(plic->control_base +
				       MEE_PLIC_CLAIM_OFFSET)) = id;
}

void __mee_plic0_set_threshold(struct __mee_driver_riscv_plic0 *plic,
			       unsigned int threshold)
{
    __MEE_ACCESS_ONCE((__mee_io_u32 *)(plic->control_base +
				       MEE_PLIC_THRESHOLD_OFFSET)) = threshold;
}

void __mee_plic0_set_priority(struct __mee_driver_riscv_plic0 *plic,
			      int id, unsigned int priority)
{
    if ( (plic->max_priority) &&
	 (priority < plic->max_priority) ) {
        __MEE_ACCESS_ONCE((__mee_io_u32 *)(plic->control_base +
					   MEE_PLIC_PRIORITY_OFFSET +
					   (id << MEE_PLIC_SOURCE_PRIORITY_SHIFT))) = priority;
    }
}

void __mee_plic0_enable(struct __mee_driver_riscv_plic0 *plic, int id, int enable)
{
    unsigned char current;
    unsigned long hartid = __mee_myhart_id();

    current = __MEE_ACCESS_ONCE((__mee_io_u8 *)(plic->control_base +
						MEE_PLIC_ENABLE_OFFSET +
						(id >> MEE_PLIC_SOURCE_BYTE_SHIFT)));
    __MEE_ACCESS_ONCE((__mee_io_u32 *)(plic->control_base +
				       MEE_PLIC_ENABLE_OFFSET +
				       (id >> MEE_PLIC_SOURCE_BYTE_SHIFT))) =
              enable ? (current | (1 << (id & MEE_PLIC_SOURCE_BYTE_MASK)))
                     : (current & ~(1 << (id & MEE_PLIC_SOURCE_BYTE_MASK)));
}

void __mee_plic0_default_handler (int id, void *priv) {
    mee_shutdown(300);
}

void __mee_plic0_handler (int id, void *priv)
{
    struct __mee_driver_riscv_plic0 *plic = priv;
    unsigned int idx = __mee_plic0_claim_interrupt(plic);

    if ( (idx <= plic->num_interrupts) &&
	 (plic->mee_exint_table[idx].handler) ) {
	plic->mee_exint_table[idx].handler(idx,
				  plic->mee_exint_table[idx].exint_data);
    }

    __mee_plic0_complete_interrupt(plic, idx);
}

void __mee_driver_riscv_plic0_init (struct mee_interrupt *controller)
{
    struct __mee_driver_riscv_plic0 *plic = (void *)(controller);

    if ( !plic->init_done ) {
        struct mee_interrupt *intc;

        intc = plic->interrupt_parent;

	/* Initialize ist parent controller, aka cpu_intc. */
	intc->vtable->interrupt_init(intc);

	for (int i = 0; i <= (plic->num_interrupts); i++) {
	    __mee_plic0_enable(plic, i, MEE_DISABLE);
	    __mee_plic0_set_priority(plic, i, 0);
	    plic->mee_exint_table[i].handler = NULL;
	    plic->mee_exint_table[i].sub_int = NULL;
	    plic->mee_exint_table[i].exint_data = NULL;
	}

	__mee_plic0_set_threshold(plic, 0);

	/* Register plic (ext) interrupt with with parent controller */
        intc->vtable->interrupt_register(intc,
                                         plic->interrupt_line,
                                         NULL, plic);
	/* Register plic handler for dispatching its device interrupts  */
	intc->vtable->interrupt_register(intc,
					 plic->interrupt_line,
					 __mee_plic0_handler, plic);
        plic->init_done = 1;
    }
}

int __mee_driver_riscv_plic0_register (struct mee_interrupt *controller,
			               int id, mee_interrupt_handler_t isr,
			               void *priv)
{
    struct __mee_driver_riscv_plic0 *plic = (void *)(controller);
    unsigned int idx = id - MEE_INTERRUPT_ID_GL0;

    if ( idx > plic->num_interrupts ) {
        return -1;
    }
 
    if (isr) {
        __mee_plic0_enable(plic, idx, MEE_ENABLE);
        __mee_plic0_set_priority(plic ,idx, 2);
	plic->mee_exint_table[idx].handler = isr;
	plic->mee_exint_table[idx].exint_data = priv;
    } else {
        __mee_plic0_enable(plic, idx, MEE_ENABLE);
        __mee_plic0_set_priority(plic, idx, 1);
	plic->mee_exint_table[idx].handler = __mee_plic0_default_handler;
	plic->mee_exint_table[idx].sub_int = priv;
    }

    return 0;
}

int __mee_driver_riscv_plic0_enable (struct mee_interrupt *controller, int id)
{
    struct __mee_driver_riscv_plic0 *plic = (void *)(controller);
    unsigned int idx = id - MEE_INTERRUPT_ID_GL0;

    if ( idx > plic->num_interrupts ) {
        return -1;
    }

    __mee_plic0_enable(plic, idx, MEE_ENABLE);
    return 0;
}

int __mee_driver_riscv_plic0_disable (struct mee_interrupt *controller, int id)
{
    struct __mee_driver_riscv_plic0 *plic = (void *)(controller);
    unsigned int idx = id - MEE_INTERRUPT_ID_GL0;

    if ( idx > plic->num_interrupts ) {
        return -1;
    }
    __mee_plic0_enable(plic, idx, MEE_DISABLE);
    return 0;
}
