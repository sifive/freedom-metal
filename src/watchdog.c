/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/watchdog.h>

struct metal_watchdog *metal_watchdog_get_device(const int index) {
    if (index > __METAL_DT_MAX_WDOGS) {
        return NULL;
    }

    return (struct metal_watchdog *)__metal_wdog_table[index];
}
