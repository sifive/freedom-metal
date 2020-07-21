/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_SPI0_H
#define METAL__DRIVERS__SIFIVE_SPI0_H

#include <metal/clock.h>
#include <metal/compiler.h>
#include <metal/io.h>
#include <metal/spi.h>

void sifive_spi0_init(struct metal_spi spi, int baud_rate);

int sifive_spi0_transfer(struct sifive_spi0 spi,
                         struct sifive_spi0_config *config, size_t len,
                         char *tx_buf, char *rx_buf);

int sifive_spi0_get_baud_rate(struct sifive_spi0 spi);

int sifive_spi0_set_baud_rate(struct sifive_spi0 spi, int baud_rate);

#endif
