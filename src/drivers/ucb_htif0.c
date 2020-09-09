/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_UCB_HTIF0

#include <metal/drivers/ucb_htif0.h>
#include <metal/private/metal_private_ucb_htif0.h>
#include <metal/tty.h>
#include <stddef.h>
#include <stdint.h>

#define FINISHER_OFFSET 0

volatile uint64_t fromhost __attribute__((aligned(4096)));
volatile uint64_t tohost __attribute__((aligned(4096)));

#if __riscv_xlen == 64
#define TOHOST_CMD(dev, cmd, payload)                                          \
    (((uint64_t)(dev) << 56) | ((uint64_t)(cmd) << 48) | (uint64_t)(payload))
#else
#define TOHOST_CMD(dev, cmd, payload)                                          \
    ({                                                                         \
        if ((dev) || (cmd))                                                    \
            __builtin_trap();                                                  \
        (payload);                                                             \
    })
#endif
#define FROMHOST_DEV(fromhost_value) ((uint64_t)(fromhost_value) >> 56)
#define FROMHOST_CMD(fromhost_value) ((uint64_t)(fromhost_value) << 8 >> 56)
#define FROMHOST_DATA(fromhost_value) ((uint64_t)(fromhost_value) << 16 >> 16)

static void __check_fromhost(void) {
    uint64_t fh = fromhost;
    if (!fh)
        return;
    fromhost = 0;
}

static void __set_tohost(uintptr_t dev, uintptr_t cmd, uintptr_t data) {
    while (tohost)
        __check_fromhost();
    tohost = TOHOST_CMD(dev, cmd, data);
}

static void do_tohost_fromhost(uintptr_t dev, uintptr_t cmd, uintptr_t data) {
    __set_tohost(dev, cmd, data);

    while (1) {
        uint64_t fh = fromhost;
        if (fh) {
            if (FROMHOST_DEV(fh) == dev && FROMHOST_CMD(fh) == cmd) {
                fromhost = 0;
                break;
            }
            __check_fromhost();
        }
    }
}

int ucb_htif0_putc(int c) {
    volatile uint64_t magic_mem[8];
    magic_mem[0] = 64; // SYS_write
    magic_mem[1] = 1;
    magic_mem[2] = (uintptr_t)&c;
    magic_mem[3] = 1;

    do_tohost_fromhost(0, 0, (uintptr_t)magic_mem);

    return 0;
}

#ifdef METAL_STDOUT_UCB_HTIF0
int metal_tty_putc(int c) __attribute__((alias("ucb_htif0_putc")));
#endif /* METAL_STDOUT_UCB_HTIF0 */

void metal_shutdown(int code) {
    volatile uint64_t magic_mem[8];
    magic_mem[0] = 93; // SYS_exit
    magic_mem[1] = code;
    magic_mem[2] = 0;
    magic_mem[3] = 0;

    do_tohost_fromhost(0, 0, (uintptr_t)magic_mem);

    while (1) {
        // loop forever
    }
}

#endif /* METAL_UCB_HTIF0 */

typedef int no_empty_translation_units;
