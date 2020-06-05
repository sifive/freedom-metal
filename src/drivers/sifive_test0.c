/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_TEST0

#include <metal/io.h>
#include <stdint.h>

void metal_shutdown(int code) __attribute__((noreturn));
void metal_shutdown(int code) {
    uintptr_t base = METAL_SIFIVE_TEST0_0_BASE_ADDR;
    uint32_t out = (code << 16) + (code == 0 ? 0x5555 : 0x3333);
    while (1) {
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_TEST0_FINISHER_OFFSET)) = out;
    }
}

#endif /* METAL_SIFIVE_TEST0 */

typedef int no_empty_translation_units;
