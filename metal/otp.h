/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__OTP_H
#define METAL__OTP_H

#include <stdint.h>
#include <stdbool.h>
#include <metal/interrupt.h>

struct metal_otp;

struct metal_otp_vtable {
	int (*init)(struct metal_otp *otp, void *ptr);
	int (*read)(struct metal_otp *otp, long int addr, const size_t len, uint8_t *rx_buff, int bits,uint32_t mask);
	int (*write)(struct metal_otp *otp, long int addr, const size_t len, uint8_t *tx_buff, int bits,uint32_t mask);
	int (*read_block)(struct metal_otp *otp, long int addr, const size_t len, uint8_t *rx_buff, int bits);
        int (*write_block)(struct metal_otp *otp, long int addr, const size_t len, uint8_t *tx_buff, int bits);

};

//@brief Handle for a OTP
struct metal_otp {
  const struct metal_otp_vtable *vtable;
};

// initialization of OTP
inline int metal_otp_init(struct metal_otp *otp, void *ptr)
{
	return otp->vtable->init(otp,ptr);
}

//read a word at addr
inline int metal_otp_read(struct metal_otp *otp, long int addr, const size_t len, uint8_t *rx_buff, int bits,uint32_t mask)
{
	return otp->vtable->read(otp,addr,len,rx_buff,bits,mask);
}

//write word at addr
inline int metal_otp_write(struct metal_otp *otp, long int addr, const size_t len, uint8_t *tx_buff, int bits,uint32_t mask)
{
	return otp->vtable->write(otp,addr,len,tx_buff,bits,mask);
}

//read a block of size len starting from addr
inline int metal_otp_read_block(struct metal_otp *otp, long int addr, const size_t len, uint8_t *rx_buff, int bits)
{
	return otp->vtable->read_block(otp,addr,len,rx_buff,bits);
}

//write to the block of size len starting from addr
inline int metal_otp_write_block(struct metal_otp *otp, long int addr, const size_t len, uint8_t *tx_buff, int bits)
{
	return otp->vtable->write_block(otp,addr,len,tx_buff,bits);
}




//get a OTP handle
struct metal_otp *metal_otp_get_device(const int index);

#endif /* METAL__OTP_H */
