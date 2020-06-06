/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__FLASH_H
#define METAL__FLASH_H

#include <metal/interrupt.h>
#include <stdint.h>
#define SPI_FLASH_MAX_ID_LEN 20


struct metal_flash;

struct metal_flash_vtable {
	int (*init)(struct metal_flash *flash,void *ptr);
	int (*read)(struct metal_flash *flash, unsigned int addr, const size_t size, char *rx_buff);
	int (*write)(struct metal_flash *flash, unsigned int addr, const size_t size, char *tx_buff);
	int (*erase)(struct metal_flash *flash, unsigned int start_addr,const size_t size);
	int (*write_protect)(struct metal_flash *flash, char* write_prot);
	int (*get_device_id)(struct metal_flash *flash,unsigned int *pdevice_id);
};

 typedef enum metal_flash_access_mode{
        METAL_FLASH_SDR,
        METAL_FLASH_SDR_DDR,
        METAL_FLASH_DUAL,
        METAL_FLASH_DUAL_DDR,
        METAL_FLASH_QUAD,
        METAL_FLASH_QUAD_DDR,
    } metal_flash_access_mode_t;


struct flash_config {
//	uint8_t id[SPI_NOR_MAX_ID_LEN];
    uint8_t id[SPI_FLASH_MAX_ID_LEN];
	uint32_t page_size;
	uint32_t sector_size;
	uint32_t n_sectors;
};


//@brief Handle for a FLASH
struct metal_flash {
  const struct metal_flash_vtable *vtable;
//unsigned char id[SPI_NOR_MAX_ID_LEN];
  unsigned char id[SPI_FLASH_MAX_ID_LEN];
  metal_flash_access_mode_t mode;
};

// initialization of FLASH
__inline__ int metal_flash_init(struct metal_flash *flash,void *ptr)
{
	return flash->vtable->init(flash,ptr);
}


//read a block of size len starting from addr
__inline__ int metal_flash_read(struct metal_flash *flash, unsigned int addr, const size_t size, char *rx_buff)
{
	return flash->vtable->read(flash,addr,size,rx_buff);
}

//write to the block of size len starting from addr
__inline__ int metal_flash_write(struct metal_flash *flash, unsigned int addr, const size_t size, char *tx_buff)
{
	return flash->vtable->write(flash,addr,size,tx_buff);
}


__inline__ int metal_flash_erase(struct metal_flash *flash, unsigned int addr, const size_t size)
{
	return flash->vtable->erase(flash,addr,size);
}
//write_protect block from start_addr to end_addr
__inline__ int metal_flash_write_protect(struct metal_flash *flash, char* write_prot)
{
	return flash->vtable->write_protect(flash,write_prot);
}

//get_device_id
__inline__ int metal_flash_get_device_id(struct metal_flash *flash, unsigned int *pdevice_id)
{
	return flash->vtable->get_device_id(flash,pdevice_id);
}

//get a FLASH handle
struct metal_flash *metal_flash_get_device(const int index);

#endif /* METAL__FLASH_H */
