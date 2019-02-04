/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__RISCV_CLINT0_H
#define METAL__DRIVERS__RISCV_CLINT0_H

#include <metal/compiler.h>
#include <metal/drivers/riscv,cpu.h>

#define METAL_CLINT_MSIP_HART_OFFSET    0x0000UL
#define METAL_CLINT_MTIMECMP_OFFSET     0x4000UL
#define METAL_CLINT_MTIME_OFFSET        0xBFF8UL

struct __metal_driver_vtable_riscv_clint0 {
    struct metal_interrupt_vtable clint_vtable;
};

void __metal_driver_riscv_clint0_init(struct metal_interrupt *clint);
int __metal_driver_riscv_clint0_register(struct metal_interrupt *controller,
                                       int id, metal_interrupt_handler_t isr,
                                       void *priv);
int __metal_driver_riscv_clint0_enable(struct metal_interrupt *controller, int id);
int __metal_driver_riscv_clint0_disable(struct metal_interrupt *controller, int id);
int __metal_driver_riscv_clint0_command_request(struct metal_interrupt *clint,
                                              int command, void *data);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_riscv_clint0) = {
    .clint_vtable.interrupt_init     = __metal_driver_riscv_clint0_init,
    .clint_vtable.interrupt_register = __metal_driver_riscv_clint0_register,
    .clint_vtable.interrupt_enable   = __metal_driver_riscv_clint0_enable,
    .clint_vtable.interrupt_disable  = __metal_driver_riscv_clint0_disable,
    .clint_vtable.command_request    = __metal_driver_riscv_clint0_command_request,
};

struct __metal_driver_riscv_clint0 {
    struct metal_interrupt controller;
    const struct __metal_driver_vtable_riscv_clint0 *vtable;
    const unsigned long control_base;
    const unsigned long control_size;
    int init_done;
    struct metal_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_lines[];
};

#endif
