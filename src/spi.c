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
