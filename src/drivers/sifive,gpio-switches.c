/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <string.h>
#include <metal/drivers/riscv,cpu.h>
#include <metal/drivers/sifive,gpio-switches.h>

int  __metal_driver_switch_exist (struct metal_switch *flip, char *label)
{
    struct __metal_driver_sifive_gpio_switch *swch = (void *)(flip);

    if (strcmp(swch->label, label) == 0) {
	return 1;
    }
    return 0;
}

struct metal_interrupt *
__metal_driver_switch_interrupt_controller(struct metal_switch *flip)
{
    struct __metal_driver_sifive_gpio_switch *swch = (void *)(flip);

    return swch->interrupt_parent;
}

int __metal_driver_switch_get_interrupt_id(struct metal_switch *flip)
{
    int max_irq;
    struct __metal_driver_sifive_gpio_switch *swch = (void *)(flip);

    if (swch->interrupt_parent != NULL) {
        max_irq = _metal_interrupt_command_request(swch->interrupt_parent,
                                                METAL_MAX_INTERRUPT_GET, NULL);

        if (swch->interrupt_line < max_irq) {
            return _metal_interrupt_command_request(swch->interrupt_parent,
                                                 METAL_INDEX_INTERRUPT_GET,
                                                 (void *)&swch->interrupt_line);
	}
    }
    return METAL_INTERRUPT_ID_LCMX;
}
