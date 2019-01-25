/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <string.h>
#include <mee/drivers/riscv,cpu.h>
#include <mee/drivers/sifive,gpio-switches.h>

int  __mee_driver_switch_exist (struct mee_switch *flip, char *label)
{
    struct __mee_driver_sifive_gpio_switch *swch = (void *)(flip);

    if (strcmp(swch->label, label) == 0) {
	return 1;
    }
    return 0;
}

struct mee_interrupt *
__mee_driver_switch_interrupt_controller(struct mee_switch *flip)
{
    struct __mee_driver_sifive_gpio_switch *swch = (void *)(flip);

    return swch->interrupt_parent;
}

int __mee_driver_switch_get_interrupt_id(struct mee_switch *flip)
{
    int max_irq;
    struct __mee_driver_sifive_gpio_switch *swch = (void *)(flip);

    if (swch->interrupt_parent != NULL) {
        max_irq = _mee_interrupt_command_request(swch->interrupt_parent,
                                                MEE_MAX_INTERRUPT_GET, NULL);

        if (swch->interrupt_line < max_irq) {
            return _mee_interrupt_command_request(swch->interrupt_parent,
                                                 MEE_INDEX_INTERRUPT_GET,
                                                 (void *)&swch->interrupt_line);
	}
    }
    return MEE_INTERRUPT_ID_LCMX;
}
