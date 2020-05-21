/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_TEST0

#include <metal/machine.h>

#include <stdint.h>

#include <metal/drivers/sifive_test0.h>
#include <metal/io.h>

void metal_shutdown(int code) __attribute__((noreturn));
void metal_shutdown(int code) {
    long base = __metal_driver_sifive_test0_base(
        (struct __metal_shutdown *)__METAL_DT_SHUTDOWN_HANDLE);
    uint32_t out = (code << 16) + (code == 0 ? 0x5555 : 0x3333);
    while (1) {
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_TEST0_FINISHER_OFFSET)) = out;
    }
}

#endif /* METAL_SIFIVE_TEST0 */

typedef int no_empty_translation_units;
