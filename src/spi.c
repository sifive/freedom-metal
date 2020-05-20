/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/spi.h>

struct metal_spi *metal_spi_get_device(unsigned int device_num) {
#if __METAL_DT_MAX_SPIS > 0
    if (device_num < __METAL_DT_MAX_SPIS) {
        return (struct metal_spi *)__metal_spi_table[device_num];
    }
#endif

    return NULL;
}

/* Weak stubs for when no driver exists */

int metal_spi_transfer(struct metal_spi *gspi, struct metal_spi_config *config,
                       size_t len, char *tx_buf, char *rx_buf)
    __attribute__((weak));
int metal_spi_transfer(struct metal_spi *gspi, struct metal_spi_config *config,
                       size_t len, char *tx_buf, char *rx_buf) {
    return -1;
}

int metal_spi_get_baud_rate(struct metal_spi *gspi) __attribute__((weak));
int metal_spi_get_baud_rate(struct metal_spi *gspi) { return -1; }

int metal_spi_set_baud_rate(struct metal_spi *gspi, int baud_rate)
    __attribute__((weak));
int metal_spi_set_baud_rate(struct metal_spi *gspi, int baud_rate) {
    return -1;
}

void metal_spi_init(struct metal_spi *gspi, int baud_rate)
    __attribute__((weak));
void metal_spi_init(struct metal_spi *gspi, int baud_rate) {}
