#ifndef METAL__DRIVERS__SYNOPSIS_I2C_H
#define METAL__DRIVERS__SYNOPSIS_I2C_H

#include<metal/compiler.h>
#include<metal/drivers/riscv_plic0.h>
#include<metal/i2c.h>
#include <metal/clock.h>

struct __metal_driver_vtable_synopsys_i2c
{
	const struct metal_i2c_vtable i2c;
};

struct __metal_driver_synopsys_i2c;

__METAL_DECLARE_VTABLE(__metal_driver_vtable_synopsys_i2c)

struct __metal_driver_synopsys_i2c
{

 struct metal_i2c i2c;
 
};

#endif
