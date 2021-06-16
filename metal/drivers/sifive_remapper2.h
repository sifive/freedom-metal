/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_REMAPPER2_H
#define METAL__DRIVERS__SIFIVE_REMAPPER2_H

#include <metal/compiler.h>
#include <metal/remapper.h>

struct __metal_driver_vtable_sifive_remapper2 {
    const struct metal_remapper_vtable remapper;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_remapper2)

struct __metal_driver_sifive_remapper2 {
    struct metal_remapper remapper;
};

#endif /* METAL__DRIVERS__SIFIVE_REMAPPER2_H */
