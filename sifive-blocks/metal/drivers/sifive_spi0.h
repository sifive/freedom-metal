/* Copyright 2018 SiFive, Inc */
/* Copyright © 2020 Keith Packard */
/*
 * Licensed under the Apache License, Version 2.0 (the “License”); you
 * may not use this file except in compliance with the License. A copy
 * of this license can be found in the LICENSE.Apache2 file included
 * with this software. You may also obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
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
