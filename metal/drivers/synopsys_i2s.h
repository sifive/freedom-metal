#ifndef METAL__DRIVERS__SYNOPSYS_I2S_H
#define METAL__DRIVERS__SYNOPSYS_I2S_H

#include<metal/compiler.h>
#include<metal/drivers/riscv_plic0.h>
#include<metal/i2s.h>
#include <metal/clock.h>

struct __metal_driver_vtable_synopsys_i2s
{
	const struct metal_i2s_vtable i2s;
};

struct __metal_driver_synopsys_i2s;

__METAL_DECLARE_VTABLE(__metal_driver_vtable_synopsys_i2s)


struct __metal_driver_synopsys_i2s
{

 struct metal_i2s i2s;
 
};

#endif