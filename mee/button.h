/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__BUTTON_H
#define MEE__BUTTON_H

#include <mee/interrupt.h>

struct mee_button;

struct mee_button_vtable {
    int (*button_exist)(struct mee_button *button, char *label);
    struct mee_interrupt* (*interrupt_controller)(struct mee_button *button);
    int (*get_interrupt_id)(struct mee_button *button);
};

struct mee_button {
    const struct mee_button_vtable *vtable;
};

struct mee_button* mee_button_get(char *label);
inline struct mee_interrupt*
    mee_button_interrupt_controller(struct mee_button *button) { return button->vtable->interrupt_controller(button); }
inline int mee_button_get_interrupt_id(struct mee_button *button) { return button->vtable->get_interrupt_id(button); }

#endif
