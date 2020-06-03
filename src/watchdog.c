/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/watchdog.h>

/* Weak stubs for when no driver exists */

int metal_watchdog_feed(const struct metal_watchdog wdog)
    __attribute__((weak));
int metal_watchdog_feed(const struct metal_watchdog wdog) { return -1; }

long int metal_watchdog_get_rate(const struct metal_watchdog wdog)
    __attribute__((weak));
long int metal_watchdog_get_rate(const struct metal_watchdog wdog) {
    return -1;
}

long int metal_watchdog_set_rate(const struct metal_watchdog wdog,
                                 const long int rate) __attribute__((weak));
long int metal_watchdog_set_rate(const struct metal_watchdog wdog,
                                 const long int rate) {
    return -1;
}

long int metal_watchdog_get_timeout(const struct metal_watchdog wdog)
    __attribute__((weak));
long int metal_watchdog_get_timeout(const struct metal_watchdog wdog) {
    return -1;
}

long int metal_watchdog_set_timeout(const struct metal_watchdog wdog,
                                    const long int timeout)
    __attribute__((weak));
long int metal_watchdog_set_timeout(const struct metal_watchdog wdog,
                                    const long int timeout) {
    return -1;
}

int metal_watchdog_set_result(const struct metal_watchdog wdog,
                              const enum metal_watchdog_result result)
    __attribute__((weak));
int metal_watchdog_set_result(const struct metal_watchdog wdog,
                              const enum metal_watchdog_result result) {
    return -1;
}

int metal_watchdog_run(const struct metal_watchdog wdog,
                       const enum metal_watchdog_run_option option)
    __attribute__((weak));
int metal_watchdog_run(const struct metal_watchdog wdog,
                       const enum metal_watchdog_run_option option) {
    return -1;
}

struct metal_interrupt *
metal_watchdog_get_interrupt(const struct metal_watchdog wdog)
    __attribute__((weak));
struct metal_interrupt *
metal_watchdog_get_interrupt(const struct metal_watchdog wdog) {
    return NULL;
}

int metal_watchdog_get_interrupt_id(const struct metal_watchdog wdog)
    __attribute__((weak));
int metal_watchdog_get_interrupt_id(const struct metal_watchdog wdog) {
    return -1;
}

int metal_watchdog_clear_interrupt(const struct metal_watchdog wdog)
    __attribute__((weak));
int metal_watchdog_clear_interrupt(const struct metal_watchdog wdog) {
    return -1;
}
