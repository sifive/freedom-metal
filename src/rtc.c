/* Copyright 2019 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/rtc.h>

/* Weak stubs for when no driver exists */

uint64_t metal_rtc_get_rate(const struct metal_rtc rtc)
    __attribute__((weak));
uint64_t metal_rtc_get_rate(const struct metal_rtc rtc) { return 0; }

uint64_t metal_rtc_set_rate(const struct metal_rtc rtc,
                            const uint64_t rate) __attribute__((weak));
uint64_t metal_rtc_set_rate(const struct metal_rtc rtc,
                            const uint64_t rate) {
    return 0;
}

uint64_t metal_rtc_get_compare(const struct metal_rtc rtc)
    __attribute__((weak));
uint64_t metal_rtc_get_compare(const struct metal_rtc rtc) { return 0; }

uint64_t metal_rtc_set_compare(const struct metal_rtc rtc,
                               const uint64_t compare) __attribute__((weak));
uint64_t metal_rtc_set_compare(const struct metal_rtc rtc,
                               const uint64_t compare) {
    return 0;
}

uint64_t metal_rtc_get_count(const struct metal_rtc rtc)
    __attribute__((weak));
uint64_t metal_rtc_get_count(const struct metal_rtc rtc) { return 0; }

uint64_t metal_rtc_set_count(const struct metal_rtc rtc,
                             const uint64_t count) __attribute__((weak));
uint64_t metal_rtc_set_count(const struct metal_rtc rtc,
                             const uint64_t count) {
    return 0;
}

int metal_rtc_run(const struct metal_rtc rtc,
                  const enum metal_rtc_run_option option) __attribute__((weak));
int metal_rtc_run(const struct metal_rtc rtc,
                  const enum metal_rtc_run_option option) {
    return -1;
}

struct metal_interrupt *
metal_rtc_get_interrupt(const struct metal_rtc rtc)
    __attribute__((weak));
struct metal_interrupt *
metal_rtc_get_interrupt(const struct metal_rtc rtc) {
    return NULL;
}

int metal_rtc_get_interrupt_id(const struct metal_rtc rtc)
    __attribute__((weak));
int metal_rtc_get_interrupt_id(const struct metal_rtc rtc) { return -1; }
