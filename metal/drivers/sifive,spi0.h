/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_SPI0_H
#define METAL__DRIVERS__SIFIVE_SPI0_H

#include <metal/drivers/sifive,gpio0.h>
#include <metal/clock.h>
#include <metal/compiler.h>
#include <metal/io.h>
#include <metal/spi.h>

struct __metal_driver_vtable_sifive_spi0 {
    const struct metal_spi_vtable spi;
};

struct __metal_driver_sifive_spi0;

void __metal_driver_sifive_spi0_init(struct metal_spi *spi, int baud_rate);
int __metal_driver_sifive_spi0_transfer(struct metal_spi *spi, struct metal_spi_config *config, size_t len, char *tx_buf, char *rx_buf);
int __metal_driver_sifive_spi0_get_baud_rate(struct metal_spi *spi);
int __metal_driver_sifive_spi0_set_baud_rate(struct metal_spi *spi, int baud_rate);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_spi0) = {
    .spi.init          = __metal_driver_sifive_spi0_init,
    .spi.transfer      = __metal_driver_sifive_spi0_transfer,
    .spi.get_baud_rate = __metal_driver_sifive_spi0_get_baud_rate,
    .spi.set_baud_rate = __metal_driver_sifive_spi0_set_baud_rate,
};

struct __metal_driver_sifive_spi0 {
    struct metal_spi spi;
    const struct __metal_driver_vtable_sifive_spi0 *vtable;
    struct metal_clock *clock;
    const unsigned long control_base;
    const unsigned long control_size;
    unsigned long baud_rate;
    struct __metal_driver_sifive_gpio0 *pinmux;
    const unsigned long pinmux_output_selector;
    const unsigned long pinmux_source_selector;
};

#endif
