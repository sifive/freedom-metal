/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/drivers/sifive,test0.h>
#include <metal/io.h>
#include <stdint.h>

#define FINISHER_OFFSET 0

void __metal_driver_sifive_test0_exit(const struct __metal_shutdown *sd, int code)
{
    const struct __metal_driver_sifive_test0 *test = (void *)sd;
    uint32_t out = (code << 16) + (code == 0 ? 0x5555 : 0x3333);
    while (1) {
        __METAL_ACCESS_ONCE((__metal_io_u32 *)(test->base + FINISHER_OFFSET)) = out;
    }
}
