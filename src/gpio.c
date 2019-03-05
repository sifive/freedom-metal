/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/gpio.h>

extern inline int metal_gpio_disable_input(const struct metal_gpio *gpio, int pin);
extern inline int metal_gpio_enable_output(const struct metal_gpio *gpio, int pin);
extern inline int metal_gpio_set_pin(const struct metal_gpio *, int pin, int value);
extern inline int metal_gpio_get_pin(const struct metal_gpio *, int pin);
extern inline int metal_gpio_clear_pin(const struct metal_gpio *, int pin);
extern inline int metal_gpio_toggle_pin(const struct metal_gpio *, int pin);
extern inline int metal_gpio_enable_pinmux(const struct metal_gpio *, int pin, int io_function);

struct metal_gpio *metal_gpio_get_device(int device_num)
{
    if(dev_num > __MEE_DT_MAX_GPIOS) {
	return NULL;
    }

    return __metal_gpio_table[dev_num];
}
