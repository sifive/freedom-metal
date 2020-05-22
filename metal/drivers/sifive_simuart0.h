/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_SIMUART0_H
#define METAL__DRIVERS__SIFIVE_SIMUART0_H

#include <metal/clock.h>
#include <metal/compiler.h>
#include <metal/drivers/riscv_plic0.h>
#include <metal/drivers/sifive_gpio0.h>
#include <metal/io.h>
#include <metal/uart.h>

struct __metal_driver_sifive_simuart0 {
    unsigned long baud_rate;
};

#endif
