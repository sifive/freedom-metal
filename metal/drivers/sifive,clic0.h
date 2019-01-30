/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__SIFIVE_CLIC0_H
#define MEE__DRIVERS__SIFIVE_CLIC0_H

#include <mee/compiler.h>
#include <mee/drivers/riscv,cpu.h>


/* Per clic-spec, design supports up to 1024 interrup per hart */
#define MEE_CLIC_MAX_INTRS           0x0400
#define MEE_CLIC_CNTRL_BASE          0x0000UL
#define MEE_CLIC_INTR_IP             (MEE_CLIC_CNTRL_BASE)
#define MEE_CLIC_INTR_IE             (MEE_CLIC_INTR_IP + MEE_CLIC_MAX_INTRS) 
#define MEE_CLIC_INTR_CTRL           (MEE_CLIC_INTR_IE + MEE_CLIC_MAX_INTRS)
#define MEE_CLIC_CFG                 (MEE_CLIC_INTR_CTRL + MEE_CLIC_MAX_INTRS)
#define MEE_CLIC_MAX_NMBITS          2
#define MEE_CLIC_MAX_NLBITS          8
#define MEE_CLIC_MAX_NVBITS          1

#define MEE_CLIC_MMODE_OFFSET        0x00800000UL  /* currently support */
#define MEE_CLIC_HSMODE_OFFSET       0x00A00000UL
#define MEE_CLIC_SMODE_OFFSET        0x00C00000UL
#define MEE_CLIC_UMODE_OFFSET        0x00E00000UL

#define MEE_CLIC_CFG_NMBITS_MMODE    0x00
#define MEE_CLIC_CFG_NMBITS_SMODE1   0x20
#define MEE_CLIC_CFG_NMBITS_SMODE2   0x40
#define MEE_CLIC_CFG_NMBITS_MASK     0x60
#define MEE_CLIC_CFG_NLBITS_MASK     0x1E
#define MEE_CLIC_CFG_NVBIT_MASK      0x01

#define MEE_CLIC_ICTRL_SMODE1_MASK   0x7F          /* b8 set imply M-mode */
#define MEE_CLIC_ICTRL_SMODE2_MASK   0x3F          /* b8 set M-mode, b7 clear U-mode */

#define MEE_CLIC_MSIP_HART_OFFSET    0x0000UL
#define MEE_CLIC_MTIMECMP_OFFSET     0x4000UL
#define MEE_CLIC_MTIME_OFFSET        0xBFF8UL

#define MEE_MAX_INTERRUPT_LEVEL      ((1 << MEE_CLIC_MAX_NLBITS) - 1)

struct __mee_driver_vtable_sifive_clic0 {
    struct mee_interrupt_vtable clic_vtable;
};

void __mee_driver_sifive_clic0_init(struct mee_interrupt *clic);
int __mee_driver_sifive_clic0_register(struct mee_interrupt *controller,
                                       int id, mee_interrupt_handler_t isr,
                                       void *priv);
int __mee_driver_sifive_clic0_enable(struct mee_interrupt *controller, int id);
int __mee_driver_sifive_clic0_disable(struct mee_interrupt *controller, int id);
int __mee_driver_sifive_clic0_enable_interrupt_vector(struct mee_interrupt *controller,
                                                      int id, mee_vector_mode mode);
int __mee_driver_sifive_clic0_disable_interrupt_vector(struct mee_interrupt *controller,
                                                       int id);
int __mee_driver_sifive_clic0_command_request(struct mee_interrupt *clic,
                                              int command, void *data);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_sifive_clic0) = {
    .clic_vtable.interrupt_init     = __mee_driver_sifive_clic0_init,
    .clic_vtable.interrupt_register = __mee_driver_sifive_clic0_register,
    .clic_vtable.interrupt_enable   = __mee_driver_sifive_clic0_enable,
    .clic_vtable.interrupt_disable  = __mee_driver_sifive_clic0_disable,
    .clic_vtable.interrupt_vector_enable   = __mee_driver_sifive_clic0_enable_interrupt_vector,
    .clic_vtable.interrupt_vector_disable  = __mee_driver_sifive_clic0_disable_interrupt_vector,
    .clic_vtable.command_request    = __mee_driver_sifive_clic0_command_request,
};

#define __MEE_MACHINE_MACROS
#include <mee/machine.h>
struct __mee_driver_sifive_clic0 {
    struct mee_interrupt controller;
    const struct __mee_driver_vtable_sifive_clic0 *vtable;
    const unsigned long control_base;
    const unsigned long control_size;
    int init_done;
    struct mee_interrupt *interrupt_parent;
    const int num_interrupts;
    /* Hardcode max of 3 direct interrupts to core for now, SW, Timer, Ext */
    const int interrupt_lines[3];
    int max_levels;
    int num_subinterrupts;
    int num_intbits;
    int interrupt_controller;
    mee_interrupt_handler_t mee_mtvt_table[__MEE_CLIC_SUBINTERRUPTS];
    __mee_interrupt_data mee_exint_table[__MEE_CLIC_SUBINTERRUPTS];
};
#undef __MEE_MACHINE_MACROS

#endif
