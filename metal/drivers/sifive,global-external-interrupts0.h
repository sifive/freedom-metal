/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GLOBAL_EXTERNAL_INTERRUPTS0_H
#define METAL__DRIVERS__SIFIVE_GLOBAL_EXTERNAL_INTERRUPTS0_H

#include <metal/compiler.h>
#include <metal/drivers/riscv,cpu.h>

struct __metal_driver_vtable_sifive_global_external_interrupts0 {
    struct metal_interrupt_vtable global0_vtable;
};

void __metal_driver_sifive_global_external_interrupt_init(struct metal_interrupt *global0);
int __metal_driver_sifive_global_external_interrupt_register(struct metal_interrupt *controller,
                                                           int id, metal_interrupt_handler_t isr,
                                                           void *priv_data);
int __metal_driver_sifive_global_external_interrupt_enable(struct metal_interrupt *controller, int id);
int __metal_driver_sifive_global_external_interrupt_disable(struct metal_interrupt *controller, int id);
int __metal_driver_sifive_global_external_command_request(struct metal_interrupt *clint,
                                                        int command, void *data);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_global_external_interrupts0) = {
    .global0_vtable.interrupt_init     = __metal_driver_sifive_global_external_interrupt_init,
    .global0_vtable.interrupt_register = __metal_driver_sifive_global_external_interrupt_register,
    .global0_vtable.interrupt_enable   = __metal_driver_sifive_global_external_interrupt_enable,
    .global0_vtable.interrupt_disable  = __metal_driver_sifive_global_external_interrupt_disable,
    .global0_vtable.command_request    = __metal_driver_sifive_global_external_command_request,
};

struct __metal_driver_sifive_global_external_interrupts0 {
    struct metal_interrupt irc;
    const struct __metal_driver_vtable_sifive_global_external_interrupts0 *vtable;
    int init_done;
    struct metal_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_lines[];
};


#endif
