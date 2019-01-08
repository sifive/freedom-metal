/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__SIFIVE_UART0_H
#define MEE__DRIVERS__SIFIVE_UART0_H

#include <mee/drivers/sifive,gpio0.h>
#include <mee/drivers/riscv,plic0.h>
#include <mee/clock.h>
#include <mee/io.h>
#include <mee/uart.h>
#include <mee/compiler.h>

struct __mee_driver_vtable_sifive_uart0 {
    const struct mee_uart_vtable uart;
};

struct __mee_driver_sifive_uart0;

void __mee_driver_sifive_uart0_init(struct mee_uart *uart, int baud_rate);
int __mee_driver_sifive_uart0_putc(struct mee_uart *uart, unsigned char c);
int __mee_driver_sifive_uart0_getc(struct mee_uart *uart, unsigned char *c);
int __mee_driver_sifive_uart0_get_baud_rate(struct mee_uart *guart);
int __mee_driver_sifive_uart0_set_baud_rate(struct mee_uart *guart, int baud_rate);
struct mee_interrupt* __mee_driver_sifive_uart0_interrupt_controller(struct mee_uart *uart);
void __mee_driver_sifive_uart0_interrupt_init(struct mee_uart *uart);
int __mee_driver_sifive_uart0_get_interrupt_id(struct mee_uart *uart);
int __mee_driver_sifive_uart0_enable_interrupt_id(struct mee_uart *uart, int id);
int __mee_driver_sifive_uart0_disable_interrupt_id(struct mee_uart *uart, int id);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_sifive_uart0) = {
    .uart.init          = __mee_driver_sifive_uart0_init,
    .uart.putc          = __mee_driver_sifive_uart0_putc,
    .uart.getc          = __mee_driver_sifive_uart0_getc,
    .uart.get_baud_rate = __mee_driver_sifive_uart0_get_baud_rate,
    .uart.set_baud_rate = __mee_driver_sifive_uart0_set_baud_rate,
    .uart.controller_interrupt = __mee_driver_sifive_uart0_interrupt_controller,
    .uart.interrupt_init       = __mee_driver_sifive_uart0_interrupt_init,
    .uart.get_interrupt_id     = __mee_driver_sifive_uart0_get_interrupt_id,
    .uart.enable_interrupt_id  = __mee_driver_sifive_uart0_enable_interrupt_id,
    .uart.disable_interrupt_id = __mee_driver_sifive_uart0_disable_interrupt_id,
};

struct __mee_driver_sifive_uart0 {
    struct mee_uart uart;
    const struct __mee_driver_vtable_sifive_uart0 *vtable;
    struct mee_clock *clock;
    const unsigned long control_base;
    const unsigned long control_size;
    unsigned long baud_rate;
    struct __mee_driver_sifive_gpio0 *pinmux;
    const unsigned long pinmux_output_selector;
    const unsigned long pinmux_source_selector;
    struct mee_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_line;
};


#endif
