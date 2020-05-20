/* Copyright 2019 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/rtc.h>

#include <stddef.h>

struct metal_rtc *metal_rtc_get_device(int index) {
#ifdef __METAL_DT_MAX_RTCS
    if (index < __METAL_DT_MAX_RTCS) {
        return (struct metal_rtc *)__metal_rtc_table[index];
    }
#endif
    return NULL;
}
