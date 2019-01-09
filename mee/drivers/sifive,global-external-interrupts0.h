/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__SIFIVE_GLOBAL_EXTERNAL_INTERRUPTS0_H
#define MEE__DRIVERS__SIFIVE_GLOBAL_EXTERNAL_INTERRUPTS0_H

#include <mee/compiler.h>
#include <mee/drivers/riscv,cpu.h>

struct __mee_driver_vtable_sifive_global_external_interrupts0 {
    struct mee_interrupt_vtable global0_vtable;
};

void __mee_driver_sifive_global_external_interrupt_init(struct mee_interrupt *global0);
int __mee_driver_sifive_global_external_interrupt_register(struct mee_interrupt *controller,
                                                           int id, mee_interrupt_handler_t isr,
                                                           void *priv_data);
int __mee_driver_sifive_global_external_interrupt_enable(struct mee_interrupt *controller, int id);
int __mee_driver_sifive_global_external_interrupt_disable(struct mee_interrupt *controller, int id);
int __mee_driver_sifive_global_external_command_request(struct mee_interrupt *clint,
                                                        int command, void *data);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_sifive_global_external_interrupts0) = {
    .global0_vtable.interrupt_init     = __mee_driver_sifive_global_external_interrupt_init,
    .global0_vtable.interrupt_register = __mee_driver_sifive_global_external_interrupt_register,
    .global0_vtable.interrupt_enable   = __mee_driver_sifive_global_external_interrupt_enable,
    .global0_vtable.interrupt_disable  = __mee_driver_sifive_global_external_interrupt_disable,
    .global0_vtable.command_request    = __mee_driver_sifive_global_external_command_request,
};

struct __mee_driver_sifive_global_external_interrupts0 {
    struct mee_interrupt global0;
    const struct __mee_driver_vtable_sifive_global_external_interrupts0 *vtable;
    struct mee_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_lines[];
};


#endif
