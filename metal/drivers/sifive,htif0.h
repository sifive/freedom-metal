/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_HTIF0_H
#define METAL__DRIVERS__SIFIVE_HTIF0_H

#include <metal/compiler.h>
#include <metal/shutdown.h>
#include <metal/uart.h>

struct __metal_driver_vtable_sifive_htif0_shutdown {
    const struct __metal_shutdown_vtable shutdown;
};

struct __metal_driver_vtable_sifive_htif0_uart {
    const struct metal_uart_vtable uart;
};

struct __metal_driver_sifive_htif0;

void __metal_driver_sifive_htif0_exit(const struct __metal_shutdown *test, int code) __attribute__((noreturn));

void __metal_driver_sifive_htif0_init(struct metal_uart *uart, int baud_rate);
int __metal_driver_sifive_htif0_putc(struct metal_uart *uart, unsigned char c);
int __metal_driver_sifive_htif0_getc(struct metal_uart *uart, unsigned char *c);
int __metal_driver_sifive_htif0_get_baud_rate(struct metal_uart *guart);
int __metal_driver_sifive_htif0_set_baud_rate(struct metal_uart *guart, int baud_rate);
struct metal_interrupt* __metal_driver_sifive_htif0_interrupt_controller(struct metal_uart *uart);
int __metal_driver_sifive_htif0_get_interrupt_id(struct metal_uart *uart);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_htif0_shutdown) = {
    .shutdown.exit       = &__metal_driver_sifive_htif0_exit,
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_htif0_uart) = {
    .uart.init          = __metal_driver_sifive_htif0_init,
    .uart.putc          = __metal_driver_sifive_htif0_putc,
    .uart.getc          = __metal_driver_sifive_htif0_getc,
    .uart.get_baud_rate = __metal_driver_sifive_htif0_get_baud_rate,
    .uart.set_baud_rate = __metal_driver_sifive_htif0_set_baud_rate,
    .uart.controller_interrupt = __metal_driver_sifive_htif0_interrupt_controller,
    .uart.get_interrupt_id     = __metal_driver_sifive_htif0_get_interrupt_id,
};

struct __metal_driver_sifive_htif0_shutdown {
    struct __metal_shutdown shutdown;
    const struct __metal_driver_vtable_sifive_htif0_shutdown *vtable;
};

struct __metal_driver_sifive_htif0_uart {
    struct metal_uart uart;
    const struct __metal_driver_vtable_sifive_htif0_uart *vtable;
};

#endif
