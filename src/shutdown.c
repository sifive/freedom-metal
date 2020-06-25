/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>
#include <metal/shutdown.h>

#if !defined(__METAL_HAS_SHUTDOWN)
#pragma message(                                                               \
    "There is no defined shutdown mechanism, metal_shutdown() will spin.")
#endif

/* Weak stub for when there's no driver */

void metal_shutdown(int code) __attribute__((weak, noreturn));
void metal_shutdown(int code) {
    while (1) {
        __asm__ volatile("nop");
    }
}
