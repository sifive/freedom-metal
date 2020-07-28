/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/watchdog.h>

__attribute__((weak)) int metal_watchdog_feed(struct metal_watchdog wdog) {
    return -1;
}

__attribute__((weak)) uint64_t
metal_watchdog_get_rate(struct metal_watchdog wdog) {
    return 0;
}

__attribute__((weak)) uint64_t
metal_watchdog_set_rate(struct metal_watchdog wdog, const uint64_t rate) {
    return 0;
}

__attribute__((weak)) metal_watchdog_count_t
metal_watchdog_get_timeout(struct metal_watchdog wdog) {
    return 0;
}

__attribute__((weak)) metal_watchdog_count_t
metal_watchdog_set_timeout(struct metal_watchdog wdog,
                           const metal_watchdog_count_t timeout) {
    return 0;
}

__attribute__((weak)) int
metal_watchdog_set_result(struct metal_watchdog wdog,
                          const enum metal_watchdog_result result) {
    return -1;
}

__attribute__((weak)) int
metal_watchdog_run(struct metal_watchdog wdog,
                   const enum metal_watchdog_run_option option) {
    return -1;
}

__attribute__((weak)) int
metal_watchdog_enable_interrupt(struct metal_watchdog wdog) {
    return -1;
}

__attribute__((weak)) int
metal_watchdog_disable_interrupt(struct metal_watchdog wdog) {
    return -1;
}

__attribute__((weak)) int
metal_watchdog_clear_interrupt(struct metal_watchdog wdog) {
    return -1;
}
