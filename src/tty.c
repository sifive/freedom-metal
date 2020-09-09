/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/tty.h>
#include <metal/uart.h>

/* This implementation of putc doesn't actually do anything, it's just there to
 * provide a shim that eats all the characters so we can ensure that everything
 * can link to metal_tty_putc. */
int nop_putc(int c) __attribute__((section(".text.metal.nop.putc")));
// Use a customizable NOP hint instruction so that a post-processor parser can
// look for this instruction, and use the value in a0 as the character to be
// printed.
int nop_putc(int c) {
    // The ABI states that c will be passed in a0. However, under an LTO
    // (link-time-optimizer), it may choose to optimize in ways that would
    // break this assumption.  We want to ensure that the passed argument is
    // truly in a0, for easier post-processing, and so there is a single
    // 32-bit opcode to match against.
    // So explicitly ensure that the argument is placed into a0 first.
    __asm__ volatile("mv a0, %0; slli x0,a0,0x11" ::"r"(c));
    return -1;
}

int nop_getc(int *c) { return -1; }

int metal_tty_putc(int c) __attribute__((weak, alias("nop_putc")));
int metal_tty_getc(int *c) __attribute__((weak, alias("nop_getc")));
