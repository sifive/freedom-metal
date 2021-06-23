/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_PRCI0_H
#define METAL__DRIVERS__SIFIVE_PRCI0_H

#include <metal/compiler.h>
#include <metal/prci.h>

struct __metal_driver_vtable_sifive_prci0 {
    const struct metal_prci_vtable prci;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_prci0)

struct __metal_driver_sifive_prci0 {
    struct metal_prci prci;
    int init_done;
};

#endif /* METAL__DRIVERS__SIFIVE_PRCI0_H */
