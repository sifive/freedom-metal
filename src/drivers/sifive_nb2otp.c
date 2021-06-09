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


#define OPT_WRITE_CONFIG 0x104C2704
#define OPT_READ_CONFIG  0x104C2700
#define SCR_REG_BASE_ADDR       		0x4F0010000UL


#define FUSE_BASE_ADDR 0x4F0004000UL


static int __metal_sifive_otp_read(uint64_t otp_ctrl_base, uint64_t addr, const size_t len, uint8_t *rx_buff, int nbits,uint32_t mask)
{
	uint32_t value=0;
	uint32_t pos=0;

	for(int i=0;i<32;i++)
	{
		if(((mask>>i)&1)==1) 
		{	
			pos=i;
			break;
		}
	}


	OTP_REGW(SCR_REG_BASE_ADDR, METAL_SIFIVE_NB2OTP_PCSS_SCR_OTP_2) = OPT_READ_CONFIG;
	value=OTP_REGW(FUSE_BASE_ADDR, (addr&0xFFF));
	
	DEBUG_PRINT(" opt read %x val %x mask %x\n",addr,value,mask);
	value &= mask;
	value=value>>pos;
	*((uint32_t *)rx_buff) = value;

	return 0;
}


static int __metal_sifive_otp_write(uint64_t otp_ctrl_base,uint64_t addr, const size_t len, uint8_t *tx_buff, int nbits,uint32_t mask)
{
	uint32_t pos=0;
	uint32_t read_val=0;
	uint32_t write_data=*((uint32_t *)tx_buff);

	for(int i=0;i<32;i++)
	{
		if(((mask>>i)&1)==1) 
		{	
			pos=i;
			break;
		}
	}
	/*First Read value*/	
	OTP_REGW(SCR_REG_BASE_ADDR, METAL_SIFIVE_NB2OTP_PCSS_SCR_OTP_2) = OPT_READ_CONFIG;
	read_val=OTP_REGW(FUSE_BASE_ADDR, (addr&0xFFF));
	
	//read_val = (*(uint32_t*)addr);

	read_val &= ~mask;


	write_data=(write_data<<pos);
	write_data&=mask;

	write_data |= read_val;

	OTP_REGW(SCR_REG_BASE_ADDR, METAL_SIFIVE_NB2OTP_PCSS_SCR_OTP_2) = OPT_WRITE_CONFIG;
	OTP_REGW(FUSE_BASE_ADDR, (addr&0xFFF))= write_data;
	return 0;
}


static int __metal_sifive_otp_read_block(uint64_t otp_ctrl_base, uint64_t addr, const size_t len, uint8_t *rx_buff, int nbits)
{
	uint32_t mask;
	uint32_t value=0;
	uint32_t bits=nbits;
	uint32_t nwords=nbits/32;
	uint32_t nremaining_bits=nbits%32;

	OTP_REGW(SCR_REG_BASE_ADDR, METAL_SIFIVE_NB2OTP_PCSS_SCR_OTP_2) = OPT_READ_CONFIG;
	do
	{
		mask=0;
		if(bits < 32)
			mask = (1 << bits) - 1;
		else
			mask = 0xFFFFFFFF;

		value=OTP_REGW(FUSE_BASE_ADDR, (addr&0xFFF));
		value &= mask;
		DEBUG_PRINT(" opt read %x val %x mask %x\n",addr,value,mask);
		*((uint32_t*)rx_buff) = value;
		rx_buff += 4;
		addr +=	4;
		bits -= 32;
	}while(bits>0);

	return 0;
}


static int __metal_sifive_otp_write_block(uintptr_t otp_ctrl_base,uint64_t addr, const size_t len, uint8_t *tx_buff, int nbits)
{
	uint32_t mask;
	uint32_t value=0;
	uint32_t bits=nbits;

	OTP_REGW(SCR_REG_BASE_ADDR, METAL_SIFIVE_NB2OTP_PCSS_SCR_OTP_2) = OPT_WRITE_CONFIG;
	do{
		mask=0;
		if(bits < 32)
			mask = (1 << bits) - 1;
		else
			mask = 0xFFFFFFFF;

		value  = *((uint32_t *)tx_buff);
		value &= mask;
	    	OTP_REGW(FUSE_BASE_ADDR, (addr&0xFFF))=value;
		tx_buff += 4;
		bits  -=   32;
		addr  +=   4;

	}while(bits>0);
	return 0;
}




int __metal_driver_sifive_nb2otp_init(struct metal_otp *otp,void *ptr)
{
	int retval=0;
	otp_control_base=(uintptr_t)__metal_driver_sifive_nb2otp_base(otp);
	return retval;
}


int __metal_driver_sifive_nb2otp_read(struct metal_otp *otp, long int addr, const size_t len, uint8_t *rx_buff, int bits,uint32_t mask)
{
	int retval;
	otp_control_base=(uintptr_t)__metal_driver_sifive_nb2otp_base(otp);

	retval = __metal_sifive_otp_read(otp_control_base, addr, len, rx_buff, bits,mask);
	return retval;
}

int __metal_driver_sifive_nb2otp_write(struct metal_otp *otp,long int addr, const size_t len, uint8_t *tx_buff, int bits,uint32_t mask)
{
	int retval;
	otp_control_base=(uintptr_t)__metal_driver_sifive_nb2otp_base(otp);

	retval = __metal_sifive_otp_write(otp_control_base, addr, len, tx_buff, bits,mask);
	return retval;
}

int __metal_driver_sifive_nb2otp_read_block(struct metal_otp *otp, long int addr, const size_t len, uint8_t *rx_buff, int bits)
{
	int retval;
	otp_control_base=(uintptr_t)__metal_driver_sifive_nb2otp_base(otp);

	retval = __metal_sifive_otp_read_block(otp_control_base, addr, len, rx_buff, bits);
	return retval;
}

int __metal_driver_sifive_nb2otp_write_block(struct metal_otp *otp,long int addr, const size_t len, uint8_t *tx_buff, int bits)
{
	int retval;
	otp_control_base=(uintptr_t)__metal_driver_sifive_nb2otp_base(otp);

	retval = __metal_sifive_otp_write_block(otp_control_base, addr, len, tx_buff, bits);
	return retval;
}


__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_nb2otp) = {
	.otp.init = __metal_driver_sifive_nb2otp_init,
	.otp.read = __metal_driver_sifive_nb2otp_read,
	.otp.write = __metal_driver_sifive_nb2otp_write,
	.otp.read_block = __metal_driver_sifive_nb2otp_read_block,
	.otp.write_block = __metal_driver_sifive_nb2otp_write_block,
};

#endif //SIFIVE_NB2_OTP
