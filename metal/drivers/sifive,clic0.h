/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_CLIC0_H
#define METAL__DRIVERS__SIFIVE_CLIC0_H

#include <metal/compiler.h>
#include <metal/drivers/riscv,cpu.h>


/* Per clic-spec, design supports up to 1024 interrup per hart */
#define METAL_CLIC_MAX_INTRS           0x0400
#define METAL_CLIC_CNTRL_BASE          0x0000UL
#define METAL_CLIC_INTR_IP             (METAL_CLIC_CNTRL_BASE)
#define METAL_CLIC_INTR_IE             (METAL_CLIC_INTR_IP + METAL_CLIC_MAX_INTRS) 
#define METAL_CLIC_INTR_CTRL           (METAL_CLIC_INTR_IE + METAL_CLIC_MAX_INTRS)
#define METAL_CLIC_CFG                 (METAL_CLIC_INTR_CTRL + METAL_CLIC_MAX_INTRS)
#define METAL_CLIC_MAX_NMBITS          2
#define METAL_CLIC_MAX_NLBITS          8
#define METAL_CLIC_MAX_NVBITS          1

#define METAL_CLIC_MMODE_OFFSET        0x00800000UL  /* currently support */
#define METAL_CLIC_HSMODE_OFFSET       0x00A00000UL
#define METAL_CLIC_SMODE_OFFSET        0x00C00000UL
#define METAL_CLIC_UMODE_OFFSET        0x00E00000UL

#define METAL_CLIC_CFG_NMBITS_MMODE    0x00
#define METAL_CLIC_CFG_NMBITS_SMODE1   0x20
#define METAL_CLIC_CFG_NMBITS_SMODE2   0x40
#define METAL_CLIC_CFG_NMBITS_MASK     0x60
#define METAL_CLIC_CFG_NLBITS_MASK     0x1E
#define METAL_CLIC_CFG_NVBIT_MASK      0x01

#define METAL_CLIC_ICTRL_SMODE1_MASK   0x7F          /* b8 set imply M-mode */
#define METAL_CLIC_ICTRL_SMODE2_MASK   0x3F          /* b8 set M-mode, b7 clear U-mode */

#define METAL_CLIC_MSIP_HART_OFFSET    0x0000UL
#define METAL_CLIC_MTIMECMP_OFFSET     0x4000UL
#define METAL_CLIC_MTIME_OFFSET        0xBFF8UL

#define METAL_MAX_INTERRUPT_LEVEL      ((1 << METAL_CLIC_MAX_NLBITS) - 1)

struct __metal_driver_vtable_sifive_clic0 {
    struct metal_interrupt_vtable clic_vtable;
};

void __metal_driver_sifive_clic0_init(struct metal_interrupt *clic);
int __metal_driver_sifive_clic0_register(struct metal_interrupt *controller,
                                       int id, metal_interrupt_handler_t isr,
                                       void *priv);
int __metal_driver_sifive_clic0_enable(struct metal_interrupt *controller, int id);
int __metal_driver_sifive_clic0_disable(struct metal_interrupt *controller, int id);
int __metal_driver_sifive_clic0_enable_interrupt_vector(struct metal_interrupt *controller,
                                                      int id, metal_vector_mode mode);
int __metal_driver_sifive_clic0_disable_interrupt_vector(struct metal_interrupt *controller,
                                                       int id);
int __metal_driver_sifive_clic0_command_request(struct metal_interrupt *clic,
                                              int command, void *data);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_clic0) = {
    .clic_vtable.interrupt_init     = __metal_driver_sifive_clic0_init,
    .clic_vtable.interrupt_register = __metal_driver_sifive_clic0_register,
    .clic_vtable.interrupt_enable   = __metal_driver_sifive_clic0_enable,
    .clic_vtable.interrupt_disable  = __metal_driver_sifive_clic0_disable,
    .clic_vtable.interrupt_vector_enable   = __metal_driver_sifive_clic0_enable_interrupt_vector,
    .clic_vtable.interrupt_vector_disable  = __metal_driver_sifive_clic0_disable_interrupt_vector,
    .clic_vtable.command_request    = __metal_driver_sifive_clic0_command_request,
};

#define __METAL_MACHINE_MACROS
#include <metal/machine.h>
struct __metal_driver_sifive_clic0 {
    struct metal_interrupt controller;
    const struct __metal_driver_vtable_sifive_clic0 *vtable;
    const unsigned long control_base;
    const unsigned long control_size;
    int init_done;
    struct metal_interrupt *interrupt_parent;
    const int num_interrupts;
    /* Hardcode max of 3 direct interrupts to core for now, SW, Timer, Ext */
    const int interrupt_lines[3];
    int max_levels;
    int num_subinterrupts;
    int num_intbits;
    int interrupt_controller;
    metal_interrupt_handler_t metal_mtvt_table[__METAL_CLIC_SUBINTERRUPTS];
    __metal_interrupt_data metal_exint_table[__METAL_CLIC_SUBINTERRUPTS];
};
#undef __METAL_MACHINE_MACROS

#endif
