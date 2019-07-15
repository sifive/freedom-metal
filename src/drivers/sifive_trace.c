/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_TRACE

#include <metal/drivers/sifive_trace.h>
#include <metal/machine.h>

#define TRACE_REG(base, offset)   (((unsigned long)base + offset))
#define TRACE_REGB(base, offset)  (__METAL_ACCESS_ONCE((__metal_io_u8  *)TRACE_REG(base, offset)))
#define TRACE_REGW(base, offset)  (__METAL_ACCESS_ONCE((__metal_io_u32 *)TRACE_REG(base, offset)))

int __metal_driver_sifive_trace_putc(struct metal_uart *trace, unsigned char c)
{
    long base = __metal_driver_sifive_trace_base(trace);
    return 0;
}

int __metal_driver_sifive_trace_getc(struct metal_uart *trace, unsigned char *c)
{
    long base = __metal_driver_sifive_trace_base(trace);
    return 0;
}

void __metal_driver_sifive_trace_init(struct metal_uart *trace, int baud_rate)
{
    long base = __metal_driver_sifive_trace_base(trace);
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_trace) = {
    .uart.init          = __metal_driver_sifive_trace_init,
    .uart.putc          = __metal_driver_sifive_trace_putc,
    .uart.getc          = __metal_driver_sifive_trace_getc,

    .uart.get_baud_rate = NULL,
    .uart.set_baud_rate = NULL,

    .uart.controller_interrupt = NULL,
    .uart.get_interrupt_id     = NULL,
};

#endif /* METAL_SIFIVE_TRACE */
