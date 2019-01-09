/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/switch.h>
#include <mee/machine.h>

struct mee_switch* mee_switch_get (char *label)
{
    int i;
    struct mee_switch *flip;

    if ((__MEE_DT_MAX_BUTTONS == 0) || (label == NULL)) {
        return NULL;
    }

    for (i = 0; i < __MEE_DT_MAX_BUTTONS; i++) {
        flip = (struct mee_switch*)__mee_switch_table[i];
        if (flip->vtable->switch_exist(flip, label)) {
            return flip;
        }
    }
    return NULL;
}

extern inline struct mee_interrupt*
    mee_switch_interrupt_controller(struct mee_switch *flip);
extern inline int mee_switch_get_interrupt_id(struct mee_switch *flip);
