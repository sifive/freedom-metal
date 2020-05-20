/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/i2c.h>
#include <metal/machine.h>

struct metal_i2c *metal_i2c_get_device(unsigned int device_num) {
#if __METAL_DT_MAX_I2CS > 0
    if (device_num < __METAL_DT_MAX_I2CS) {
        return (struct metal_i2c *)__metal_i2c_table[device_num];
    }
#endif
    return NULL;
}
