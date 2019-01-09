/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__SWITCH_H
#define MEE__SWITCH_H

#include <mee/interrupt.h>

struct mee_switch;

struct mee_switch_vtable {
    int (*switch_exist)(struct mee_switch *flip, char *label);
    struct mee_interrupt* (*interrupt_controller)(struct mee_switch *flip);
    int (*get_interrupt_id)(struct mee_switch *flip);
};

struct mee_switch {
    const struct mee_switch_vtable *vtable;
};

struct mee_switch* mee_switch_get(char *label);
inline struct mee_interrupt*
    mee_switch_interrupt_controller(struct mee_switch *flip) { return flip->vtable->interrupt_controller(flip); }
inline int mee_switch_get_interrupt_id(struct mee_switch *flip) { return flip->vtable->get_interrupt_id(flip); }

#endif
