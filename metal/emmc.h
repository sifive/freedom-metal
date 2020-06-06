/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__EMMC_H
#define METAL__EMMC_H

#include <metal/interrupt.h>

struct metal_emmc;

struct metal_emmc_vtable {
	int (*boot)(struct metal_emmc *emmc);
	int (*init)(struct metal_emmc *emmc, void *ptr);
	int (*read_block)(struct metal_emmc *emmc, long int addr, const size_t len, char *rx_buff);
	int (*write_block)(struct metal_emmc *emmc, long int addr, const size_t len, char *tx_buff);
	int (*erase_block)(struct metal_emmc *emmc, long int start_addr, long int end_addr);
};

//@brief Handle for a EMMC
struct metal_emmc {
  const struct metal_emmc_vtable *vtable;
};

//for EMMC boot mode
inline int metal_emmc_boot(struct metal_emmc *emmc)
{
	return emmc->vtable->boot(emmc);
}

// initialization of EMMC
inline int metal_emmc_init(struct metal_emmc *emmc, void *ptr)
{
	return emmc->vtable->init(emmc,ptr);
}

//read a block of size len starting from addr
inline int metal_emmc_read_block(struct metal_emmc *emmc, long int addr, const size_t len, char *rx_buff)
{
	return emmc->vtable->read_block(emmc,addr,len,rx_buff);
}

//write to the block of size len starting from addr
inline int metal_emmc_write_block(struct metal_emmc *emmc, long int addr, const size_t len, char *tx_buff)
{
	return emmc->vtable->write_block(emmc,addr,len,tx_buff);
}

//erase block from start_addr to end_addr
inline int metal_emmc_erase_block(struct metal_emmc *emmc, long int start_addr, long int end_addr)
{
	return emmc->vtable->erase_block(emmc,start_addr,end_addr);
}

//get a EMMC handle
struct metal_emmc *metal_emmc_get_device(const int index);

#endif /* METAL__EMMC_H */

