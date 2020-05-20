/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/switch.h>

struct metal_switch *metal_switch_get(char *label) {
    int i;
    struct metal_switch *flip;

    if ((__METAL_DT_MAX_BUTTONS == 0) || (label == NULL)) {
        return NULL;
    }

    for (i = 0; i < __METAL_DT_MAX_BUTTONS; i++) {
        flip = (struct metal_switch *)__metal_switch_table[i];
        if (metal_switch_has_label(flip, label)) {
            return flip;
        }
    }
    return NULL;
}
