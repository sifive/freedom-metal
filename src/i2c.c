/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/i2c.h>

/* Weak stubs for when no driver exists */

void metal_i2c_init(struct metal_i2c i2c, unsigned int baud_rate,
                    metal_i2c_mode_t mode) __attribute__((weak));
void metal_i2c_init(struct metal_i2c i2c, unsigned int baud_rate,
                    metal_i2c_mode_t mode) {}

int metal_i2c_get_baud_rate(struct metal_i2c i2c) __attribute__((weak));
int metal_i2c_get_baud_rate(struct metal_i2c i2c) { return -1; }

int metal_i2c_set_baud_rate(struct metal_i2c i2c, unsigned int baud_rate)
    __attribute__((weak));
int metal_i2c_set_baud_rate(struct metal_i2c i2c, unsigned int baud_rate) {
    return -1;
}

int metal_i2c_write_addr(unsigned long base, unsigned int addr,
                         unsigned char rw_flag) __attribute__((weak));
int metal_i2c_write_addr(unsigned long base, unsigned int addr,
                         unsigned char rw_flag) {
    return -1;
}

int metal_i2c_write(struct metal_i2c i2c, unsigned int addr, unsigned int len,
                    unsigned char buf[], metal_i2c_stop_bit_t stop_bit)
    __attribute__((weak));
int metal_i2c_write(struct metal_i2c i2c, unsigned int addr, unsigned int len,
                    unsigned char buf[], metal_i2c_stop_bit_t stop_bit) {
    return -1;
}

int metal_i2c_read(struct metal_i2c i2c, unsigned int addr, unsigned int len,
                   unsigned char buf[], metal_i2c_stop_bit_t stop_bit)
    __attribute__((weak));
int metal_i2c_read(struct metal_i2c i2c, unsigned int addr, unsigned int len,
                   unsigned char buf[], metal_i2c_stop_bit_t stop_bit) {
    return -1;
}

int metal_i2c_transfer(struct metal_i2c i2c, unsigned int addr,
                       unsigned char txbuf[], unsigned int txlen,
                       unsigned char rxbuf[], unsigned int rxlen)
    __attribute__((weak));
int metal_i2c_transfer(struct metal_i2c i2c, unsigned int addr,
                       unsigned char txbuf[], unsigned int txlen,
                       unsigned char rxbuf[], unsigned int rxlen) {
    return -1;
}
