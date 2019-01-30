/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/io.h>
#include <metal/drivers/riscv,clint0.h>

unsigned long long __metal_clint0_mtime_get (struct __metal_driver_riscv_clint0 *clint)
{
    __metal_io_u32 lo, hi;

    /* Guard against rollover when reading */
    do {
	hi = __METAL_ACCESS_ONCE((__metal_io_u32 *)(clint->control_base + METAL_CLINT_MTIME_OFFSET + 4));
	lo = __METAL_ACCESS_ONCE((__metal_io_u32 *)(clint->control_base + METAL_CLINT_MTIME_OFFSET));
    } while (__METAL_ACCESS_ONCE((__metal_io_u32 *)(clint->control_base + METAL_CLINT_MTIME_OFFSET + 4)) != hi);

    return (((unsigned long long)hi) << 32) | lo;
}

int __metal_clint0_mtime_set (struct __metal_driver_riscv_clint0 *clint, unsigned long long time)
{   
    /* Per spec, the RISC-V MTIME/MTIMECMP registers are 64 bit,
     * and are NOT internally latched for multiword transfers.
     * Need to be careful about sequencing to avoid triggering
     * spurious interrupts: For that set the high word to a max
     * value first.
     */
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(clint->control_base + METAL_CLINT_MTIMECMP_OFFSET + 4)) = 0xFFFFFFFF;
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(clint->control_base + METAL_CLINT_MTIMECMP_OFFSET)) = (__metal_io_u32)time;
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(clint->control_base + METAL_CLINT_MTIMECMP_OFFSET + 4)) = (__metal_io_u32)(time >> 32);
    return 0;
}

void __metal_driver_riscv_clint0_init (struct metal_interrupt *controller)
{
    struct __metal_driver_riscv_clint0 *clint =
                              (struct __metal_driver_riscv_clint0 *)(controller);

    if ( !clint->init_done ) {
        struct metal_interrupt *intc = clint->interrupt_parent;

	/* Register its interrupts with with parent controller, aka sw and timerto its default isr */
        for (int i = 0; i < clint->num_interrupts; i++) {
            intc->vtable->interrupt_register(intc,
					     clint->interrupt_lines[i],
					     NULL, clint);
	}
	clint->init_done = 1;
    }	
}

int __metal_driver_riscv_clint0_register (struct metal_interrupt *controller,
                                        int id, metal_interrupt_handler_t isr,
                                        void *priv)
{
    int rc = -1;
    struct __metal_driver_riscv_clint0 *clint =
                              (struct __metal_driver_riscv_clint0 *)(controller);
    struct metal_interrupt *intc = clint->interrupt_parent;

    /* Register its interrupts with parent controller */
    if (intc) {
        rc = intc->vtable->interrupt_register(intc, id, isr, priv);
    }
    return rc;
}

int __metal_driver_riscv_clint0_enable (struct metal_interrupt *controller, int id)
{
    int rc = -1;
    struct __metal_driver_riscv_clint0 *clint =
                              (struct __metal_driver_riscv_clint0 *)(controller);

    if ( id ) {
        struct metal_interrupt *intc = clint->interrupt_parent;
        
        /* Enable its interrupts with parent controller */
        if (intc) {
            rc = intc->vtable->interrupt_enable(intc, id);
        }
    }
}

int __metal_driver_riscv_clint0_disable (struct metal_interrupt *controller, int id)
{
    int rc = -1;
    struct __metal_driver_riscv_clint0 *clint =
                              (struct __metal_driver_riscv_clint0 *)(controller);

    if ( id ) {
        struct metal_interrupt *intc = clint->interrupt_parent;
        
        /* Disable its interrupts with parent controller */
        if (intc) {
            rc = intc->vtable->interrupt_disable(intc, id);
        }
    }
}

int __metal_driver_riscv_clint0_command_request (struct metal_interrupt *controller,
                                               int command, void *data)
{
    int hartid;
    int rc = -1;
    struct __metal_driver_riscv_clint0 *clint =
                              (struct __metal_driver_riscv_clint0 *)(controller);

    switch (command) {
    case METAL_TIMER_MTIME_GET:
        if (data) {
	    *(unsigned long long *)data = __metal_clint0_mtime_get(clint);
            rc = 0;
        }
        break;
    case METAL_TIMER_MTIME_SET:
        if (data) {
	    __metal_clint0_mtime_set(clint, *(unsigned long long *)data);
            rc = 0;
        }
        break;
    case METAL_SOFTWARE_IPI_CLEAR:
	if (data) {
	    hartid = *(int *)data;
            __METAL_ACCESS_ONCE((__metal_io_u32 *)(clint->control_base +
					       (hartid * 4))) = METAL_DISABLE;
            rc = 0;
        }
        break;
    case METAL_SOFTWARE_IPI_SET:
	if (data) {
	    hartid = *(int *)data;
            __METAL_ACCESS_ONCE((__metal_io_u32 *)(clint->control_base +
					       (hartid * 4))) = METAL_ENABLE;
            rc = 0;
        }
        break;
    case METAL_SOFTWARE_MSIP_GET:
        rc = 0;
	if (data) {
	    hartid = *(int *)data;
            rc = __METAL_ACCESS_ONCE((__metal_io_u32 *)(clint->control_base +
						    (hartid * 4)));
        }
        break;
    default:
	break;
    }

    return rc;
}
