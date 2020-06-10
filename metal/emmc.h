/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__EMMC_H
#define METAL__EMMC_H

#include<stdint.h>
#include <metal/interrupt.h>

struct metal_emmc;

struct metal_emmc_vtable {
	
	int (*init)(struct metal_emmc *emmc, void *ptr);
	int (*read_block)(struct metal_emmc *emmc, long int addr, const size_t len, char *rx_buff);
	int (*write_block)(struct metal_emmc *emmc, long int addr, const size_t len, char *tx_buff);
	int (*erase_block)(struct metal_emmc *emmc, long int start_addr, long int end_addr);

	int (*boot)(struct metal_emmc *emmc,uint32_t bootpartion,uint8_t *rx_data,uint8_t size);
	int (*get_boot_partition_size)(struct metal_emmc *emmc,uint32_t bootpartion,uint32_t *size);
	int (*set_boot_write_protect)(struct metal_emmc *emmc,uint32_t bootpartion,uint32_t wp);
};

//@brief Handle for a EMMC
struct metal_emmc {
  const struct metal_emmc_vtable *vtable;
};



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

//for EMMC boot mode
inline int metal_emmc_boot(struct metal_emmc *emmc,uint32_t bootpartion,uint8_t *rx_data,uint8_t size)
{
	return emmc->vtable->boot(emmc,bootpartion,rx_data, size);
}

inline	int metal_emmc_get_boot_partition_size(struct metal_emmc *emmc,uint32_t bootpartion,uint32_t *size)
{
	return emmc->vtable->get_boot_partition_size(emmc,bootpartion,size);
}

inline	int metal_emmc_set_boot_write_protect(struct metal_emmc *emmc,uint32_t bootpartion,uint32_t wp)
{
	return emmc->vtable->set_boot_write_protect(emmc,bootpartion,wp);
}


//get a EMMC handle
struct metal_emmc *metal_emmc_get_device(const int index);

#endif /* METAL__EMMC_H */

