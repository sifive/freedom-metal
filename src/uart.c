/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/uart.h>

extern inline void metal_uart_init(const struct metal_uart *uart, int baud_rate);
extern inline int metal_uart_putc(const struct metal_uart *uart, unsigned char c);
extern inline int metal_uart_getc(const struct metal_uart *uart, unsigned char *c);
extern inline int metal_uart_get_baud_rate(const struct metal_uart *uart);
extern inline int metal_uart_set_baud_rate(const struct metal_uart *uart, int baud_rate);
