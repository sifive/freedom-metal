/*
 * Copyright © 2019 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#include <metal/tty.h>
#include <stdio.h>

static int metal_putc(char c, FILE *file) {
    (void)file;
    metal_tty_putc(c);
    return c;
}

static int metal_getc(FILE *file) {
    int c;
    (void)file;
    metal_tty_getc(&c);
    return c;
}

static int metal_flush(FILE *file) { return 0; }

static FILE __stdio =
    FDEV_SETUP_STREAM(metal_putc, metal_getc, metal_flush, _FDEV_SETUP_RW);

FILE *const __iob[3] = {&__stdio, &__stdio, &__stdio};
