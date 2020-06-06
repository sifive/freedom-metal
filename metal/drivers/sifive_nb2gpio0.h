#ifndef METAL__DRIVERS__SIFIVE_NB2GPIO0_H
#define METAL__DRIVERS__SIFIVE_NB2GPIO0_H

#include <metal/compiler.h>
#include <metal/gpio.h>

struct __metal_driver_vtable_sifive_nb2gpio0 {
    const struct __metal_gpio_vtable gpio;
};

struct __metal_driver_sifive_nb2gpio0;

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_nb2gpio0)

struct __metal_driver_sifive_nb2gpio0 {
	struct metal_gpio gpio;
	unsigned int pin;
};

#endif
