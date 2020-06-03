/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/spi.h>

/* Weak stubs for when no driver exists */

int metal_spi_transfer(struct metal_spi spi, struct metal_spi_config *config,
                       size_t len, char *tx_buf, char *rx_buf)
    __attribute__((weak));
int metal_spi_transfer(struct metal_spi spi, struct metal_spi_config *config,
                       size_t len, char *tx_buf, char *rx_buf) {
    return -1;
}

int metal_spi_get_baud_rate(struct metal_spi spi) __attribute__((weak));
int metal_spi_get_baud_rate(struct metal_spi spi) { return -1; }

int metal_spi_set_baud_rate(struct metal_spi spi, int baud_rate)
    __attribute__((weak));
int metal_spi_set_baud_rate(struct metal_spi spi, int baud_rate) {
    return -1;
}

void metal_spi_init(struct metal_spi spi, int baud_rate)
    __attribute__((weak));
void metal_spi_init(struct metal_spi spi, int baud_rate) {}
