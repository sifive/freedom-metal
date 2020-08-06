/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FLASH_H
#define METAL__DRIVERS__SIFIVE_FLASH_H

#include <metal/io.h>
#include <metal/compiler.h>
#include <metal/clock.h>
#include <metal/interrupt.h>
#include <metal/flash.h>


#define SPI_FLASH_MAXID_LEN			20

#define FLASH_CMD_PAGE_PROGRAM			0x2
#define FLASH_CMD_ID_READ			0x9F	/* Read JEDEC ID */

#define FLASH_CMD_WRSR				0x01	/* Write status register */
#define FLASH_CMD_WRDI				0x04	/* Write disable */
#define FLASH_CMD_RDSR				0x05	/* Read status register */
#define FLASH_CMD_WRITE_EN			0x6

#define FLASH_CMD_READ_CMD			0x3
#define FLASH_CMD_RDCR_CMD			0x35
#define FLASH_CMD_DDR_QUAD_READ_CMD		0xEE

#define FLASH_CMD_QUAD_READ_CMD			0xEC
#define FLASH_CMD_QUAD_WRITE			0x32
#define FLASH_CMD_DUAL_READ			0XBB

#define FLASH_CMD_CE				0xC7	/* Chip erase */
#define FLASH_CMD_SECTOR_ERASE			0x20	/*sector erase*/
#define FLASH_BLOCK_ERASE			0xD8	/*block  erase*/
//#define FLASH_SECTOR_ERASE			0xD7	/*section erase*/

#define FLASH_CMD_RST_EN                        0x66
#define FLASH_CMD_RST                           0x99

#define FLASH_CMD_RDJDID			0x9F

struct spi_flash_config {
	unsigned char id[SPI_FLASH_MAXID_LEN];
	unsigned int  page_size;
	unsigned int  sector_size;
	unsigned int  n_sectors;
};

struct __metal_driver_vtable_sifive_flash {
    const struct metal_flash_vtable flash;
};

struct __metal_driver_sifive_flash;

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_flash)

struct __metal_driver_sifive_flash {
    const struct metal_flash flash;
};


#endif
