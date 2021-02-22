/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#define SIFIVE_NB2_OTP
#ifdef  SIFIVE_NB2_OTP

#include <metal/machine/platform.h>
#include <metal/drivers/sifive_nb2otp.h>
#include <metal/machine.h>
#include <limits.h>
#include <metal/io.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define OTP_REGW(base,offset)   (__METAL_ACCESS_ONCE((uint32_t *)((unsigned long long)base + offset)))
static unsigned long long otp_control_base=0;


static int __metal_sifive_otp_read(struct metal_otp *otp, long int addr, const size_t len, uint8_t *rx_buff, int bits)
{
	otp_control_base=(uintptr_t)__metal_driver_sifive_nb2otp_base(otp);
	uint32_t mask;
	uint32_t value=0;
	
	for(int i=0;i<(len/4) && (bits>0);++i)
	{
		mask=0;
		if(bits < 32)
			mask = (1 << bits) - 1;
		else
			mask = 0xFFFFFFFF;
		OTP_REGW(otp_control_base, METAL_SIFIVE_NB2OTP_PCSS_SCR_OTP_2) = 0x104C2700;
		value = OTP_REGW(otp_control_base, addr);
		value &= mask;
		*((uint32_t *)rx_buff) = value;
		rx_buff += 4;
	        bits -= 32;	
	}
	return 0;
}


static int __metal_sifive_otp_write(struct metal_otp *otp,long int addr, const size_t len, uint8_t *tx_buff, int bits)
{
	otp_control_base=(uintptr_t)__metal_driver_sifive_nb2otp_base(otp);
	int mask;
	uint32_t value=0;

	for(int i=0;i<len/4;++i)
	{
		mask=0;
		if(bits < 32)
			mask = (1 << bits) - 1;
		else
			mask = 0xFFFFFFFF;
		
		value = *((uint32_t *)tx_buff);
		value &= mask;
		OTP_REGW(otp_control_base, METAL_SIFIVE_NB2OTP_PCSS_SCR_OTP_2) = 0x104C2704;
		OTP_REGW(otp_control_base, addr) = value;
		tx_buff += 4;
		bits -= 32;
	}
	return 0;
}


int __metal_driver_sifive_nb2otp_init(struct metal_otp *otp,void *ptr)
{
	int retval=0;
	otp_control_base=(uintptr_t)__metal_driver_sifive_nb2otp_base(otp);

	return retval;
}


int __metal_driver_sifive_nb2otp_read(struct metal_otp *otp, long int addr, const size_t len, uint8_t *rx_buff, int bits)
{
	int retval;
	otp_control_base=(uintptr_t)__metal_driver_sifive_nb2otp_base(otp);
	
	retval = __metal_sifive_otp_read(otp, addr, len, rx_buff, bits);
	return retval;
}

int __metal_driver_sifive_nb2otp_write(struct metal_otp *otp,long int addr, const size_t len, uint8_t *tx_buff, int bits)
{
	int retval;
	otp_control_base=(uintptr_t)__metal_driver_sifive_nb2otp_base(otp);
	retval = __metal_sifive_otp_write(otp, addr, len, tx_buff, bits);
	return retval;
}


__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_nb2otp) = {
	.otp.init = __metal_driver_sifive_nb2otp_init,
	.otp.read = __metal_driver_sifive_nb2otp_read,
	.otp.write = __metal_driver_sifive_nb2otp_write,
};

#endif //SIFIVE_NB2_OTP
