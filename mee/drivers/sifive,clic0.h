/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__SIFIVE_CLIC0_H
#define MEE__DRIVERS__SIFIVE_CLIC0_H

#include <mee/compiler.h>
#include <mee/drivers/riscv,cpu.h>

#define MEE_CLIC_MSIP_HART_OFFSET    0x0000UL
#define MEE_CLIC_MTIMECMP_OFFSET     0x4000UL
#define MEE_CLIC_MTIME_OFFSET        0xBFF8UL

struct __mee_driver_vtable_sifive_clic0 {
    struct mee_interrupt_vtable clic_vtable;
};

void __mee_driver_sifive_clic0_init(struct mee_interrupt *clic);
int __mee_driver_sifive_clic0_register(struct mee_interrupt *controller,
                                       int id, mee_interrupt_handler_t isr,
                                       void *priv);
int __mee_driver_sifive_clic0_enable(struct mee_interrupt *controller, int id);
int __mee_driver_sifive_clic0_disable(struct mee_interrupt *controller, int id);
int __mee_driver_sifive_clic0_command_request(struct mee_interrupt *clic,
                                              int command, void *data);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_sifive_clic0) = {
    .clic_vtable.interrupt_init     = __mee_driver_sifive_clic0_init,
    .clic_vtable.interrupt_register = __mee_driver_sifive_clic0_register,
    .clic_vtable.interrupt_enable   = __mee_driver_sifive_clic0_enable,
    .clic_vtable.interrupt_disable  = __mee_driver_sifive_clic0_disable,
    .clic_vtable.command_request    = __mee_driver_sifive_clic0_command_request,
};

struct __mee_driver_sifive_clic0 {
    struct mee_interrupt controller;
    const struct __mee_driver_vtable_sifive_clic0 *vtable;
    const unsigned long control_base;
    const unsigned long control_size;
    int init_done;
    struct mee_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_lines[3];
    int max_levels;
    int num_subinterrupts;
    int interrupt_controller;
    __mee_interrupt_data mee_exint_table[];
};

#define MEE_PLIC_SOURCE_BYTE_MASK       0x7
#define MEE_PLIC_SOURCE_BYTE_SHIFT      3
#define MEE_PLIC_SOURCE_PRIORITY_SHIFT  2
#define MEE_PLIC_SOURCE_PENDING_SHIFT   0
#define MEE_PLIC_HART_ENABLE_SHIFT      7
#define MEE_PLIC_HART_THRESHOLD_SHIFT   12
#define MEE_PLIC_HART_CLAIM_SHIFT       12

#define MEE_PLIC_PRIORITY_OFFSET        0x0000UL
#define MEE_PLIC_ENABLE_OFFSET          0x2000UL
#define MEE_PLIC_THRESHOLD_OFFSET       0x200000UL
#define MEE_PLIC_CLAIM_OFFSET           0x200004UL

#endif
