/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <string.h>
#include <metal/drivers/riscv,cpu.h>
#include <metal/drivers/sifive,gpio-buttons.h>

int  __metal_driver_button_exist (struct metal_button *button, char *label)
{
    struct __metal_driver_sifive_gpio_button *but = (void *)(button);

    if (strcmp(but->label, label) == 0) {
	return 1;
    }
    return 0;
}

struct metal_interrupt *
__metal_driver_button_interrupt_controller(struct metal_button *button)
{
    struct __metal_driver_sifive_gpio_button *but = (void *)(button);

    return but->interrupt_parent;
}

int __metal_driver_button_get_interrupt_id(struct metal_button *button)
{
    int max_irq;
    struct __metal_driver_sifive_gpio_button *but = (void *)(button);

    if (but->interrupt_parent != NULL) {
        max_irq = _metal_interrupt_command_request(but->interrupt_parent,
                                                METAL_MAX_INTERRUPT_GET, NULL);

	if (but->interrupt_line < max_irq) {
            return _metal_interrupt_command_request(but->interrupt_parent,
                                                 METAL_INDEX_INTERRUPT_GET,
						 (void *)&but->interrupt_line);
	}
    }
    return METAL_INTERRUPT_ID_LCMX;
}
