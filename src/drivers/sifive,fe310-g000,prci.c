/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/drivers/sifive,fe310-g000,prci.h>

long __metal_driver_sifive_fe310_g000_prci_get_reg(const struct __metal_driver_sifive_fe310_g000_prci *prci, long offset) {
    return __METAL_ACCESS_ONCE((__metal_io_u32 *)(prci->base + offset));
}

long __metal_driver_sifive_fe310_g000_prci_set_reg(const struct __metal_driver_sifive_fe310_g000_prci *prci, long offset, long value) {
    return __METAL_ACCESS_ONCE((__metal_io_u32 *)(prci->base + offset)) = value;
}
