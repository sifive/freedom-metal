/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/button.h>
#include <mee/machine.h>

struct mee_button* mee_button_get (char *label)
{
    int i;
    struct mee_button *button;

    if ((__MEE_DT_MAX_BUTTONS == 0) || (label == NULL)) {
        return NULL;
    }

    for (i = 0; i < __MEE_DT_MAX_BUTTONS; i++) {
        button = (struct mee_button*)__mee_button_table[i];
        if (button->vtable->button_exist(button, label)) {
            return button;
        }
    }
    return NULL;
}

extern inline struct mee_interrupt*
    mee_button_interrupt_controller(struct mee_button *button);
extern inline int mee_button_get_interrupt_id(struct mee_button *button);
