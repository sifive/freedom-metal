/* Copyright 2020 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/rtc.h>

__attribute__((weak)) uint64_t metal_rtc_get_rate(struct metal_rtc rtc) {
    return 0;
}

__attribute__((weak)) uint64_t metal_rtc_set_rate(struct metal_rtc rtc,
                                                  const uint64_t rate) {
    return 0;
}

__attribute__((weak)) uint64_t metal_rtc_get_compare(struct metal_rtc rtc) {
    return 0;
}

__attribute__((weak)) uint64_t metal_rtc_set_compare(struct metal_rtc rtc,
                                                     const uint64_t compare) {
    return 0;
}

__attribute__((weak)) uint64_t metal_rtc_get_count(struct metal_rtc rtc) {
    return 0;
}

__attribute__((weak)) uint64_t metal_rtc_set_count(struct metal_rtc rtc,
                                                   const uint64_t count) {
    return 0;
}

__attribute__((weak)) int metal_rtc_enable_interrupt(struct metal_rtc rtc) {
    return -1;
}

__attribute__((weak)) int metal_rtc_disable_interrupt(struct metal_rtc rtc) {
    return -1;
}

__attribute__((weak)) int
metal_rtc_run(struct metal_rtc rtc, const enum metal_rtc_run_option option) {
    return -1;
}
