/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__SIFIVE_EXTERNAL_INTERRUPTS0_H
#define MEE__DRIVERS__SIFIVE_EXTERNAL_INTERRUPTS0_H

#include <mee/compiler.h>
#include <mee/drivers/riscv,cpu.h>

struct __mee_driver_vtable_sifive_local_external_interrupts0 {
    struct mee_interrupt_vtable local0_vtable;
};

void __mee_driver_sifive_local_external_interrupt_init(struct mee_interrupt *local0);
int __mee_driver_sifive_local_external_interrupt_register(struct mee_interrupt *controller,
                                                          int id, mee_interrupt_handler_t isr,
                                                          void *priv);
int __mee_driver_sifive_local_external_interrupt_enable(struct mee_interrupt *controller, int id);
int __mee_driver_sifive_local_external_interrupt_disable(struct mee_interrupt *controller, int id);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_sifive_local_external_interrupts0) = {
    .local0_vtable.interrupt_init     = __mee_driver_sifive_local_external_interrupt_init,
    .local0_vtable.interrupt_register = __mee_driver_sifive_local_external_interrupt_register,
    .local0_vtable.interrupt_enable   = __mee_driver_sifive_local_external_interrupt_enable,
    .local0_vtable.interrupt_disable  = __mee_driver_sifive_local_external_interrupt_disable,
};

struct __mee_driver_sifive_local_external_interrupts0 {
    struct mee_interrupt local0;
    const struct __mee_driver_vtable_sifive_local_external_interrupts0 *vtable;
    struct mee_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_lines[];
};


#endif
