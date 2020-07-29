/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__FLASH_H
#define METAL__FLASH_H

#include <metal/interrupt.h>
#include <stdint.h>

#define SPI_FLASH_MAX_ID_LEN 20

#define FLASH_CMD_NORMAL_READ	0x03
#define FLASH_CMD_DUAL_IO_READ	0XBB
#define FLASH_CMD_DUAL_O_READ	0X3B
#define FLASH_CMD_QUAD_IO_READ	0xEB
#define FLASH_CMD_QUAD_O_READ	0X6B
#define FLASH_CMD_QUAD_DDR_READ	0xEE

typedef enum {
	FLASH_RDSR	= 0x05,		/* Status Register */
	FLASH_RDFR	= 0x48,		/* Function Register */
	FLASH_RDSFDP	= 0x5A,		/* Serial Flash Discoverable Parameters (SFDP) */
	FLASH_RDRP	= 0x61,		/* Read Register */
	FLASH_RDERP	= 0x81,		/* Extended Read Register */
	FLASH_RDBR	= 0x16,		/* Bank Address Register */
} reg_read_t;

/*
Note:	All the bits of Function register are OTP while some bits of other registers are non-volatile.
	Once OTP bits are written to “1”, it cannot be modified to “0” any more.
*/

typedef enum {
	FLASH_WRSR	= 0x01,		/* Status Register */
	FLASH_WRFR	= 0x42,		/* Function Register */
	FLASH_SRPNV	= 0x65,		/* Non-volatile and Volatile Read Register */
	FLASH_SRPV	= 0x63,		/* Volatile Read Register */
	FLASH_SERPNV	= 0x85,		/* Non-volatile and Volatile Extended Read Register */
	FLASH_SERPV	= 0x83,		/* Volatile Extended Read Register */
	FLASH_WRBRNV	= 0x18,		/* Non-volatile and Volatile Bank Address Register */
	FLASH_WRBRV	= 0x17,		/* Volatile Bank Address Register */
} reg_write_t;

/*! @brief Enum to select flash operating mode */
typedef enum {
	FLASH_MODE_111	= 0x00,
	FLASH_MODE_112	= 0x01,
	FLASH_MODE_114	= 0x02,
	FLASH_MODE_122	= 0x11,
	FLASH_MODE_144	= 0x12,
	FLASH_MODE_222	= 0x21,
	FLASH_MODE_444	= 0x22,
} flash_mode_t;

struct metal_flash;

struct metal_flash_vtable {
	int (*init)(struct metal_flash *flash,void *ptr);
	int (*read)(struct metal_flash *flash, unsigned int addr, const size_t size, char *rx_buff);
	int (*write)(struct metal_flash *flash, unsigned int addr, const size_t size, char *tx_buff);
	int (*erase)(struct metal_flash *flash, unsigned int start_addr,const size_t size);
	int (*write_protect)(struct metal_flash *flash, char* write_prot);
	int (*get_device_id)(struct metal_flash *flash,unsigned int *pdevice_id);
	int (*register_read)(struct metal_flash *flash, reg_read_t reg, uint32_t addr_offset, const size_t size, char *data);
	int (*register_write)(struct metal_flash *flash, reg_write_t reg, uint32_t addr_offset, const size_t size, char *data);
	int (*soft_reset)(struct metal_flash *flash);
};

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
	unsigned char id[SPI_FLASH_MAX_ID_LEN];
	flash_mode_t mode;
	uint8_t dummy_count;
	uint8_t mode_count;
	uint8_t opcode;
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

//read flash register
__inline__ int metal_flash_register_read(struct metal_flash *flash, reg_read_t reg, uint32_t addr_offset, const size_t size, char *data)
{
	return flash->vtable->register_read(flash, reg, addr_offset, size, data);
}

//write flash register
__inline__ int metal_flash_register_write(struct metal_flash *flash, reg_write_t reg, uint32_t addr_offset, const size_t size, char *data)
{
	return flash->vtable->register_write(flash, reg, addr_offset, size, data);
}

//software reset
__inline__ int metal_flash_soft_reset(struct metal_flash *flash)
{
	return flash->vtable->soft_reset(flash);
}

//get a FLASH handle
struct metal_flash *metal_flash_get_device(const int index);

#endif /* METAL__FLASH_H */
