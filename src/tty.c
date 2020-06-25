/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/tty.h>
#include <metal/uart.h>

/* This implementation of putc doesn't actually do anything, it's just there to
 * provide a shim that eats all the characters so we can ensure that everything
 * can link to metal_tty_putc. */
int nop_putc(int c) __attribute__((section(".text.metal.nop.putc")));
int nop_putc(int c) { return -1; }

int metal_tty_putc(int c) __attribute__((weak, alias("nop_putc")));
