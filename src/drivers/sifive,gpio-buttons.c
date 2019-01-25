/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <string.h>
#include <mee/drivers/riscv,cpu.h>
#include <mee/drivers/sifive,gpio-buttons.h>

int  __mee_driver_button_exist (struct mee_button *button, char *label)
{
    struct __mee_driver_sifive_gpio_button *but = (void *)(button);

    if (strcmp(but->label, label) == 0) {
	return 1;
    }
    return 0;
}

struct mee_interrupt *
__mee_driver_button_interrupt_controller(struct mee_button *button)
{
    struct __mee_driver_sifive_gpio_button *but = (void *)(button);

    return but->interrupt_parent;
}

int __mee_driver_button_get_interrupt_id(struct mee_button *button)
{
    int max_irq;
    struct __mee_driver_sifive_gpio_button *but = (void *)(button);

    if (but->interrupt_parent != NULL) {
        max_irq = _mee_interrupt_command_request(but->interrupt_parent,
                                                MEE_MAX_INTERRUPT_GET, NULL);

	if (but->interrupt_line < max_irq) {
            return _mee_interrupt_command_request(but->interrupt_parent,
                                                 MEE_INDEX_INTERRUPT_GET,
						 (void *)&but->interrupt_line);
	}
    }
    return MEE_INTERRUPT_ID_LCMX;
}
