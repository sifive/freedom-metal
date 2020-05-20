/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/button.h>
#include <metal/machine.h>

struct metal_button *metal_button_get(char *label) {
#if __METAL_DT_MAX_BUTTONS > 0
    int i;
    struct metal_button *button;

    if ((__METAL_DT_MAX_BUTTONS == 0) || (label == NULL)) {
        return NULL;
    }

    for (i = 0; i < __METAL_DT_MAX_BUTTONS; i++) {
        button = (struct metal_button *)__metal_button_table[i];
        if (metal_button_has_label(button, label)) {
            return button;
        }
    }
    return NULL;
#else
    return NULL;
#endif
}
