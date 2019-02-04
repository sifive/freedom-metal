/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_TEST0_H
#define METAL__DRIVERS__SIFIVE_TEST0_H

#include <metal/compiler.h>
#include <metal/shutdown.h>

struct __metal_driver_vtable_sifive_test0 {
    const struct __metal_shutdown_vtable shutdown;
};

struct __metal_driver_sifive_test0;

void __metal_driver_sifive_test0_exit(const struct __metal_shutdown *test, int code) __attribute__((noreturn));

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_test0) = {
    .shutdown.exit       = &__metal_driver_sifive_test0_exit,
};

struct __metal_driver_sifive_test0 {
    struct __metal_shutdown shutdown;
    const struct __metal_driver_vtable_sifive_test0 *vtable;
    const unsigned long base;
    const unsigned long size;
};


#endif
