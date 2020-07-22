/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_I2C0_H
#define METAL__DRIVERS__SIFIVE_I2C0_H

#include <metal/clock.h>
#include <metal/i2c.h>

void sifive_i2c0_init(struct metal_i2c i2c, unsigned int baud_rate,
                      metal_i2c_mode_t mode);

int sifive_i2c0_get_baud_rate(struct metal_i2c i2c);

int sifive_i2c0_set_baud_rate(struct metal_i2c i2c, unsigned int baud_rate);

int sifive_i2c0_write(struct metal_i2c i2c, unsigned int addr, unsigned int len,
                      unsigned char buf[], metal_i2c_stop_bit_t stop_bit);

int sifive_i2c0_read(struct metal_i2c i2c, unsigned int addr, unsigned int len,
                     unsigned char buf[], metal_i2c_stop_bit_t stop_bit);

int sifive_i2c0_transfer(struct metal_i2c i2c, unsigned int addr,
                         unsigned char txbuf[], unsigned int txlen,
                         unsigned char rxbuf[], unsigned int rxlen);
#endif
