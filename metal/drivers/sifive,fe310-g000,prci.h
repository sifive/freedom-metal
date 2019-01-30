/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FE310_G000_PRCI_H
#define METAL__DRIVERS__SIFIVE_FE310_G000_PRCI_H

#include <metal/compiler.h>
#include <metal/io.h>

struct __metal_driver_sifive_fe310_g000_prci;

long __metal_driver_sifive_fe310_g000_prci_get_reg(const struct __metal_driver_sifive_fe310_g000_prci *, long offset);
long __metal_driver_sifive_fe310_g000_prci_set_reg(const struct __metal_driver_sifive_fe310_g000_prci *, long offset, long value);

struct __metal_driver_vtable_sifive_fe310_g000_prci {
    long (*get_reg)(const struct __metal_driver_sifive_fe310_g000_prci *, long offset);
    long (*set_reg)(const struct __metal_driver_sifive_fe310_g000_prci *, long offset, long value);
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_fe310_g000_prci) = {
    .get_reg = __metal_driver_sifive_fe310_g000_prci_get_reg,
    .set_reg = __metal_driver_sifive_fe310_g000_prci_set_reg,
};

struct __metal_driver_sifive_fe310_g000_prci {
    const struct __metal_driver_vtable_sifive_fe310_g000_prci *vtable;
    const long base;
    const long size;
};

#endif

