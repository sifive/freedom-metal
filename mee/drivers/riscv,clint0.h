/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__RISCV_CLINT0_H
#define MEE__DRIVERS__RISCV_CLINT0_H

#include <mee/compiler.h>
#include <mee/drivers/riscv,cpu.h>

#define MEE_CLINT_MSIP_HART_OFFSET    0x0000UL
#define MEE_CLINT_MTIMECMP_OFFSET     0x4000UL
#define MEE_CLINT_MTIME_OFFSET        0xBFF8UL

struct __mee_driver_vtable_riscv_clint0 {
    struct mee_interrupt_vtable clint_vtable;
};

void __mee_driver_riscv_clint0_init(struct mee_interrupt *clint);
int __mee_driver_riscv_clint0_register(struct mee_interrupt *controller,
                                       int id, mee_interrupt_handler_t isr,
                                       void *priv);
int __mee_driver_riscv_clint0_enable(struct mee_interrupt *controller, int id);
int __mee_driver_riscv_clint0_disable(struct mee_interrupt *controller, int id);
int __mee_driver_riscv_clint0_command_request(struct mee_interrupt *clint,
                                              int command, void *data);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_riscv_clint0) = {
    .clint_vtable.interrupt_init     = __mee_driver_riscv_clint0_init,
    .clint_vtable.interrupt_register = __mee_driver_riscv_clint0_register,
    .clint_vtable.interrupt_enable   = __mee_driver_riscv_clint0_enable,
    .clint_vtable.interrupt_disable  = __mee_driver_riscv_clint0_disable,
    .clint_vtable.command_request    = __mee_driver_riscv_clint0_command_request,
};

struct __mee_driver_riscv_clint0 {
    struct mee_interrupt controller;
    const struct __mee_driver_vtable_riscv_clint0 *vtable;
    const unsigned long control_base;
    const unsigned long control_size;
    int init_done;
    struct mee_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_lines[];
};

#endif
