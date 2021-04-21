/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#define METAL_SIFIVE_FLASH
#ifdef METAL_SIFIVE_FLASH

#include <metal/drivers/sifive_flash.h>
#include <metal/drivers/sifive_nb2qspi0.h>
#include <metal/machine.h>
#include <metal/io.h>
#include <metal/time.h>
#include <stdio.h>
#include <metal/qspi.h>


#define FLASH_DEFAULT_DUMMY 8
#define FLASH_DEFAULT_OPCODE FLASH_CMD_QUAD_O_READ //1-1-4 QUAD FAst read

#define PRINT_DEBUG_INFO
#define PRINT_DEBUG_ERROR


//#define AXI_FLASH_BASEADDR		 0x20000000UL

static qspi_static_config_t qcfg;
static qspi_command_config_t cmd_cfg;

/* declaring qspi structure as a static and getting valid handle for qspi in flash_init function */
static struct metal_qspi *qspi;

/* Read Flash status value */
static int flash_status_read(struct metal_flash *pfl) {

	uint32_t read_status = 0;

	qspi_static_config_t *cfg=&qcfg;

	cfg->wrmode	= QSPI_READ;
	cfg->rxlen	= QSPI_8BIT;
	cfg->txlen	= QSPI_8BIT;
	if (pfl->mode == FLASH_MODE_444) {
		cfg->opmode	= (pfl->mode & 0x0F);
		cfg->speedmode	= ((pfl->mode & 0xF0) >> 4);
	} else {
		cfg->opmode	= QSPI_SPI;
		cfg->speedmode	= QSPI_CMD_SERIAL;
	}
	cfg->burstmode	= QSPI_APB_ACCESS;

	metal_qspi_setconfig(qspi,cfg);

	/*set command enable, read enable phase*/
	cmd_cfg.custom_command	= (QSPI_OPCODE_PH_EN | QSPI_RX_DATA_PH_EN);
	cmd_cfg.opcode		= FLASH_CMD_RDSR;

	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);
	
	metal_qspi_execute_cmd(qspi);

	metal_qspi_read(qspi, 0, 4, (uint8_t *)&read_status);
	
	return read_status;
}


static void flash_send_write_en(struct metal_flash *pfl)
{
	qspi_static_config_t *cfg=&qcfg;

	cfg->addrlen	= QSPI_ADDR_24BIT;
	cfg->wrmode	= QSPI_WRITE;
	cfg->rxlen	= QSPI_32BIT;
	cfg->txlen	= QSPI_32BIT;
	if (pfl->mode == FLASH_MODE_444) {
		cfg->opmode	= (pfl->mode & 0x0F);
		cfg->speedmode	= ((pfl->mode & 0xF0) >> 4);
	} else {
		cfg->opmode	= QSPI_SPI;
		cfg->speedmode	= QSPI_CMD_SERIAL;
	}
	cfg->burstmode	= QSPI_APB_ACCESS;

	metal_qspi_setconfig(qspi,cfg);

	/*set command enable*/
	cmd_cfg.custom_command	= QSPI_OPCODE_PH_EN;
	cmd_cfg.opcode		= FLASH_CMD_WRITE_EN;

	metal_qspi_setcommand_params(qspi,&cmd_cfg,cfg);

	metal_qspi_execute_cmd(qspi);


}

/* If we want to write less data use APB write
 *
 * */
static int flash_write(struct metal_flash *pfl,long int dst_addr_offset,size_t length,char *tx_buff)
{

	uint32_t read_status;
	//	struct metal_qspi *qspi =  __metal_driver_sifive_gpio_led_gpio(pfl);

	flash_send_write_en(pfl);

	cmd_cfg.custom_command=QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_TX_DATA_PH_EN;
	cmd_cfg.opcode=QSPI_APB_ACCESS;
	cmd_cfg.addr=dst_addr_offset;
	//cmd_cfg.dummy_stg=0;
	//cmd_cfg.mode_stg=0;
	//cmd_cfg.addr=0;
	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

	metal_qspi_write(qspi,dst_addr_offset,4,tx_buff);

	metal_qspi_execute_cmd(qspi);

	do {
		read_status = flash_status_read(pfl);
	} while ((read_status & 0x1) != 0x1);
	return 0;
}
/*
 *If you want to write full page use continuous bust mode axi
 *
 * */

static int flash_write_page(struct metal_flash *pfl,long int dst_addr_offset,size_t length,char *tx_buff)
{
	uint32_t read_status;
	//	struct metal_qspi *qspi =  __metal_driver_sifive_gpio_led_gpio(pfl);

	//	flash_send_write_en();

	qspi_static_config_t *cfg=&qcfg;

	cfg->addrlen = QSPI_ADDR_24BIT;
	cfg->burstmode = QSPI_SINGLE_BURST;/* set AXI mode */
	cfg->wrmode = QSPI_WRITE;
	cfg->opmode = QSPI_SPI;
	cfg->rxlen = QSPI_32BIT;
	cfg->txlen = QSPI_32BIT;
	metal_qspi_setconfig(qspi,cfg);

	cmd_cfg.custom_command=QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_TX_DATA_PH_EN;
	cmd_cfg.opcode=FLASH_CMD_PAGE_PROGRAM;

	//cmd_cfg.dummy_stg=0;
	//cmd_cfg.mode_stg=0;
	//cmd_cfg.addr=0;

	metal_qspi_setcommand_params(qspi,&cmd_cfg,cfg);

	/*start command*/
	metal_qspi_execute_cmd(qspi);

	uint32_t *axi_addr = (uint32_t*)(METAL_QSPI_AXI_BASE_ADDR + dst_addr_offset);


	uint32_t *dataptr=(uint32_t *)tx_buff;

	for(int i=0;i<length/4;i+=4)
	{
		*axi_addr = *dataptr;

		dataptr++;
		axi_addr++;

		/* wait until write in progress */
		do {
			read_status = flash_status_read(pfl);
		} while ((read_status & 0x1) != 0x1);

	}
	DEBUG_INFO("qspi_axi_write %d bytes written via AXI Write\n", length);

	return 0;
}


static void flash_send_command_read(struct metal_flash *pfl)
{

	qspi_static_config_t *cfg;
	cfg = &qcfg;

	cfg->addrlen = QSPI_ADDR_24BIT;
	cfg->wrmode = QSPI_READ;
	cfg->rxlen = QSPI_32BIT;
	cfg->txlen = QSPI_32BIT;
	cfg->opmode = QSPI_SPI;
	cfg->burstmode = QSPI_APB_ACCESS;
	metal_qspi_setconfig(qspi,cfg);

	cmd_cfg.custom_command= QSPI_RX_DATA_PH_EN | QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN;
	cmd_cfg.opcode= FLASH_CMD_READ_CMD;
	cmd_cfg.dummy_stg = QSPI_SET_DUMMY_DLP(0, 3, 0);

	metal_qspi_setcommand_params(qspi,&cmd_cfg,cfg);

	metal_qspi_execute_cmd(qspi);
}


static int flash_do_data_write(void)
{

	return 0;
}

static int flash_do_data_read(void)
{

	return 0;
}

static int flash_is_busy(void)
{

	return 0;
}

/*********************************************************************************************************************/
int __metal_driver_sifive_flash_init(struct metal_flash *gflash,void *ptr)
{
	unsigned long baud_rate=METAL_FLASH_BAUD;
	struct __metal_driver_sifive_flash *flash = (void *)gflash;

	memset((void *)&qcfg,0,sizeof(qspi_static_config_t));
   	/* Set-up initial parameters */
   	qcfg.addrlen 	= QSPI_ADDR_24BIT;
   	qcfg.rxlen 	= QSPI_32BIT;
   	qcfg.txlen 	= QSPI_32BIT;
   	qcfg.cs_i 	= QSPI_DEVICE_0;
   	qcfg.clk_div 	= QSPI_DIV_4;//TODO 
   	qcfg.wrmode  	= QSPI_READ;
   	qcfg.bit_align 	= QSPI_MSB_FIRST;
   	qcfg.clock_polarity=0;
   	qcfg.opmode	= QSPI_QUAD;
   	qcfg.speedmode 	= QSPI_CMD_ADDR_SERIAL;
   	qcfg.write_prot = QSPI_WRITE_PROT_DIS;
   	qcfg.hold 	= QSPI_HOLD_DIS;
   	qcfg.burstmode 	= QSPI_SINGLE_BURST;
	qcfg.rxdelay	=0;
   	qcfg.rxfinedelay=0;
   	qcfg.rx_dly_half_clk=0;
   	qcfg.rx_phshift_bypass=0;

	qspi = metal_qspi_get_device(0);
	metal_qspi_init(qspi,baud_rate);

	metal_qspi_setconfig(qspi,&qcfg);

	cmd_cfg.custom_command	= (QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_DUMMY_PH_EN | QSPI_RX_DATA_PH_EN);
	cmd_cfg.dummy_stg	= QSPI_SET_DUMMY_DLP(0, FLASH_DEFAULT_DUMMY, 0);
	cmd_cfg.mode_stg	= QSPI_SET_MODE_REG(0, 0x0);
	cmd_cfg.opcode		= FLASH_CMD_QUAD_O_READ;
	
	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);
	
	return 0;
}

/* Implement flash read operation here */
int __metal_driver_sifive_flash_read(struct metal_flash *flash, uint32_t addr, const size_t size, char *rx_buf)
{
	qspi_static_config_t *cfg=&qcfg;

	cfg->addrlen	= QSPI_ADDR_24BIT;
	cfg->rxlen	= QSPI_32BIT;
	cfg->txlen	= QSPI_32BIT;
	cfg->wrmode	= QSPI_READ;
	cfg->opmode	= (flash->mode & 0x0F);
	cfg->speedmode	= ((flash->mode & 0xF0) >> 4);
	cfg->burstmode	= QSPI_SINGLE_BURST;

	metal_qspi_setconfig(qspi,cfg);

	if (flash->opcode == FLASH_CMD_NORMAL_READ)
	{
		/*set command enable, address phase enable, read phase enable*/
		cmd_cfg.custom_command	= (QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_RX_DATA_PH_EN);
		cmd_cfg.opcode		= flash->opcode;
	}
	else
	{
		/*set command enable, address phase enable, dummy phase enable, mode phase enable, read phase enable*/
		
		if (flash->opcode == FLASH_CMD_QUAD_O_READ)
		{
			cmd_cfg.custom_command	= (QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_DUMMY_PH_EN | QSPI_RX_DATA_PH_EN);
		}else 
		{
			cmd_cfg.custom_command	= (QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_DUMMY_PH_EN | QSPI_MODE_BITS_PH_EN | QSPI_RX_DATA_PH_EN);
			cmd_cfg.mode_stg	= QSPI_SET_MODE_REG((flash->mode_count - 1), 0x0);
		}

		cmd_cfg.dummy_stg	= QSPI_SET_DUMMY_DLP(0, (flash->dummy_count - 1), 0);
		cmd_cfg.opcode		= flash->opcode;
	}

	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

	metal_qspi_read(qspi,addr,size,(uint8_t *)rx_buf);

	return 0;
}

/* Implement flash write operation here */
int __metal_driver_sifive_flash_write(struct metal_flash *flash, uint32_t mem_addr_offset, const size_t size, char *tx_buf)
{
	uint32_t read_status=0;
	qspi_static_config_t *cfg = &qcfg;

	uint32_t *dataptr=(uint32_t *)tx_buf;
	uint32_t *memaddr=(uint32_t *)mem_addr_offset;


	for(int i=0;i<size/4;i++)
	{
		flash_send_write_en(flash);

		cfg->addrlen   = QSPI_ADDR_24BIT;
		cfg->rxlen     = QSPI_32BIT;
		cfg->txlen     = QSPI_32BIT;
		cfg->wrmode    = QSPI_WRITE;
		cfg->opmode    = (flash->mode & 0x0F);
		cfg->speedmode = ((flash->mode & 0xF0) >> 4);

		cmd_cfg.opcode   = flash->opcode;

		/*set cmd_en and tx_data, address enable phase*/
		cmd_cfg.custom_command = (QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_TX_DATA_PH_EN);

		cfg->burstmode = QSPI_SINGLE_BURST;/*set AXI mode*/
		metal_qspi_setconfig(qspi,cfg);

		metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

		metal_qspi_write(qspi,memaddr,4,(uint8_t*)dataptr);

		memaddr++;
		dataptr++;

		/* wait until write in progress */
		do {
			read_status = flash_status_read(flash);
		} while ((read_status & 0x1) == 0x1);
	}
	DEBUG_INFO("flash_write %d bytes written via AXI Write\n", i);

	return 0;
}

/*temporarily here instead of bool write_protect, char* write_protect is used (no bool type in freedom-metal) */
int __metal_driver_sifive_flash_write_protect(struct metal_flash *flash, char* write_protect)
{
	qspi_static_config_t *cfg;
	cfg = &qcfg;

	if(write_protect == "true")
		cfg->write_prot = QSPI_WRITE_PROT_EN;

	else
		cfg->write_prot = QSPI_WRITE_PROT_DIS;

	metal_qspi_setconfig(qspi,cfg);
	return 0;
}

int __metal_driver_sifive_flash_erase(struct metal_flash *flash, unsigned int addr, const size_t size)
{
	/* Chip erase
	 * cmd_cfg.opcode = FLASH_CMD_CE
	 * */

	/* Block erase
	 * cmd_cfg.opcode = FLASH_BLOCK_ERASE
	 * */

	int num_of_sectors=((size/FLASH_SECTOR_SIZE)==0)?1:(size/FLASH_SECTOR_SIZE);
	/* Sector erase */

	for(int i=0;i<num_of_sectors;i++)
	{
		uint32_t read_status;
		flash_send_write_en(flash);

		/*	following code is implemented in flash_send_write_en()
		 * qspi_static_config_t *cfg=&qcfg;

		 cfg->addrlen = QSPI_ADDR_24BIT;
		 cfg->wrmode = QSPI_WRITE;
		 cfg->rxlen = QSPI_32BIT;
		 cfg->txlen = QSPI_32BIT;
		 cfg->opmode = QSPI_SPI;
		 cfg->burstmode = QSPI_APB_ACCESS;
		 metal_qspi_setconfig(qspi,cfg);
		 */

		cmd_cfg.opcode = FLASH_CMD_SECTOR_ERASE;

		cmd_cfg.custom_command = QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN;
		cmd_cfg.addr = addr;
		metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

		metal_qspi_execute_cmd(qspi);

		DEBUG_INFO(" Erase flash sector from %x \n", addr);

		/*set cmd_en and address enable phase*/

		do {
			read_status = flash_status_read(flash);
		} while ((read_status & 0x1) == 0x1);

		addr=addr+FLASH_SECTOR_SIZE+1;
	}

	return 0;
}

int __metal_driver_sifive_flash_register_read(struct metal_flash *flash, reg_read_t reg, uint32_t addr_offset, const size_t size, char *data)
{
	qspi_static_config_t *cfg=&qcfg;

	if (reg == FLASH_RDSFDP) {

		cfg->addrlen	= QSPI_ADDR_24BIT;
		cfg->rxlen	= QSPI_32BIT;
		cfg->wrmode	= QSPI_READ;
		if (flash->mode == FLASH_MODE_444) {
			cfg->opmode	= (flash->mode & 0x0F);
			cfg->speedmode	= ((flash->mode & 0xF0) >> 4);
		} else {
			cfg->opmode	= QSPI_SPI;
			cfg->speedmode	= QSPI_CMD_SERIAL;
		}
		cfg->burstmode	= QSPI_SINGLE_BURST;

		metal_qspi_setconfig(qspi,cfg);

		/*set command enable, address phase enable, dummy phase enable, read phase enable*/
		cmd_cfg.custom_command	= (QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_DUMMY_PH_EN | QSPI_RX_DATA_PH_EN);
		cmd_cfg.dummy_stg	= QSPI_SET_DUMMY_DLP(0, (flash->dummy_count - 1), 0);
		cmd_cfg.opcode		= reg;

		metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

		metal_qspi_read(qspi,addr_offset,size,data);

		/* Check SFDP signature */
		if (data[0] == 'S' && data[1] == 'F' && data[2] == 'D' && data[3] == 'P')
		{
			DEBUG_INFO("The device supports SFDP.\n");
			return 0;
		}
		else {
			DEBUG_ERROR("The device does not support SFDP.\n");
			return -1;
		}
	}
	else {
		cfg->wrmode	= QSPI_READ;
		cfg->rxlen	= QSPI_8BIT;
		if (flash->mode == FLASH_MODE_444) {
			cfg->opmode	= (flash->mode & 0x0F);
			cfg->speedmode	= ((flash->mode & 0xF0) >> 4);
		} else {
			cfg->opmode	= QSPI_SPI;
			cfg->speedmode	= QSPI_CMD_SERIAL;
		}
		cfg->burstmode	= QSPI_APB_ACCESS;

		metal_qspi_setconfig(qspi,cfg);

		/*set command enable, read phase enable*/
		cmd_cfg.custom_command	= (QSPI_OPCODE_PH_EN | QSPI_RX_DATA_PH_EN);
		cmd_cfg.opcode		= reg;

		metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

		metal_qspi_execute_cmd(qspi);

		metal_qspi_read(qspi,addr_offset,size,data);

		return 0;
	}
}

int __metal_driver_sifive_flash_register_write(struct metal_flash *flash, reg_write_t reg, uint32_t addr_offset, const size_t size, char *data)
{
	uint8_t read_status = 0;
	qspi_static_config_t *cfg = &qcfg;

	flash_send_write_en(flash);

	cfg->txlen	= QSPI_8BIT;
	cfg->wrmode	= QSPI_WRITE;
	if (flash->mode == FLASH_MODE_444) {
		cfg->opmode	= (flash->mode & 0x0F);
		cfg->speedmode	= ((flash->mode & 0xF0) >> 4);
	} else {
		cfg->opmode	= QSPI_SPI;
		cfg->speedmode	= QSPI_CMD_SERIAL;
	}
	cfg->burstmode	= QSPI_APB_ACCESS;

	metal_qspi_setconfig(qspi,cfg);

	/*set command enable, write phase enable*/
	cmd_cfg.custom_command	= (QSPI_OPCODE_PH_EN | QSPI_TX_DATA_PH_EN);
	cmd_cfg.opcode		= reg;

	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

	metal_qspi_write(qspi,addr_offset,size,data);

	metal_qspi_execute_cmd(qspi);

	do {
		read_status = flash_status_read(flash);
	} while ((read_status & 0x1)== 0x1);

	return 0;
}

int __metal_driver_sifive_flash_qpi_mode(struct metal_flash *flash)
{
	qspi_static_config_t *cfg=&qcfg;

	cfg->addrlen	= QSPI_ADDR_32BIT;
	cfg->rxlen	= QSPI_32BIT;
	cfg->txlen	= QSPI_32BIT;
	cfg->opmode     = (flash->mode & 0x0F);
	cfg->speedmode  = ((flash->mode & 0xF0) >> 4);
	cfg->burstmode	= QSPI_APB_ACCESS;

	metal_qspi_setconfig(qspi,cfg);

	/*set command enable*/
	cmd_cfg.custom_command	= QSPI_OPCODE_PH_EN;
	cmd_cfg.opcode		= flash->opcode;
	cmd_cfg.dummy_stg	= QSPI_SET_DUMMY_DLP(0, (flash->dummy_count - 1), 0);
	cmd_cfg.mode_stg	= QSPI_SET_MODE_REG(flash->dummy_count - 1, 0x0);

	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

	metal_qspi_execute_cmd(qspi);

	return 0;
}

/* Read Manufacturer ID by JEDEC */
int __metal_driver_sifive_flash_manufacturer(struct metal_flash *flash, uint32_t *mfr_id) 
{
	uint32_t jd_mfr_id = 0;

	qspi_static_config_t *cfg=&qcfg;

	cfg->wrmode	= QSPI_READ;
	cfg->rxlen	= QSPI_24BIT;
	cfg->txlen	= QSPI_8BIT;
	if (flash->mode == FLASH_MODE_444) {
		cfg->opmode	= (flash->mode & 0x0F);
		cfg->speedmode	= ((flash->mode & 0xF0) >> 4);
	} else {
		cfg->opmode	= QSPI_SPI;
		cfg->speedmode	= QSPI_CMD_ADDR_SERIAL;
	}
	cfg->burstmode	= QSPI_APB_ACCESS;

	metal_qspi_setconfig(qspi,cfg);

	/*set command enable, read enable phase*/
	cmd_cfg.custom_command	= (QSPI_OPCODE_PH_EN | QSPI_RX_DATA_PH_EN);
	cmd_cfg.opcode		= flash->opcode;

	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

	metal_qspi_execute_cmd(qspi);



	metal_qspi_read(qspi, 0, 1, (uint8_t *)&jd_mfr_id);

	*mfr_id = jd_mfr_id;

	DEBUG_INFO("Manufacturer ID: 0x%x\n", jd_mfr_id);

	switch (jd_mfr_id&0xFF) {

		case SPI_FLASH_MFR_ISSI: 
			DEBUG_INFO("Name of Manufacturer: Integrated Silicon\n");
			break;

		case SPI_FLASH_MFR_MICRON:
			DEBUG_INFO("Name of Manufacturer: Micron\n");
			break;

		case SPI_FLASH_MFR_MACRONIX:
			DEBUG_INFO("Name of Manufacturer: Macronix\n");
			break;

		case SPI_FLASH_MFR_WINBOND:
			DEBUG_INFO("Name of Manufacturer: Winbond\n");
			break;

		case SPI_FLASH_MFR_GIGADEV:
			DEBUG_INFO("Name of Manufacturer: GigaDevice\n");
			break;

		case SPI_FLASH_MFR_SPANSION:
			DEBUG_INFO("Name of Manufacturer: Spansion\n");
			break;

		default:
			break;
	}

	return 0;
}

int __metal_driver_sifive_flash_soft_reset(struct metal_flash *flash)
{
	int retval=0;

	qspi_static_config_t *cfg=&qcfg;

	cfg->addrlen	= QSPI_ADDR_32BIT;
	cfg->rxlen	= QSPI_32BIT;
	cfg->txlen	= QSPI_32BIT;
	if (flash->mode == FLASH_MODE_444) {
		cfg->opmode	= (flash->mode & 0x0F);
		cfg->speedmode	= ((flash->mode & 0xF0) >> 4);
	} else {
		cfg->opmode	= QSPI_SPI;
		cfg->speedmode	= QSPI_CMD_SERIAL;
	}
	cfg->burstmode	= QSPI_APB_ACCESS;

	metal_qspi_setconfig(qspi,cfg);

	/*set command enable*/
	cmd_cfg.custom_command	= (QSPI_OPCODE_PH_EN);
	cmd_cfg.opcode		= FLASH_CMD_RST_EN;

	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

	retval=metal_qspi_execute_cmd(qspi);
	if(retval==0)
	{
		cmd_cfg.opcode		= FLASH_CMD_RST;

		retval=metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);
		if(retval==0)
		{
			retval=metal_qspi_execute_cmd(qspi);
		}
	}
	return retval;
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_flash) = {
	.flash.init		= __metal_driver_sifive_flash_init,
	.flash.write_protect	= __metal_driver_sifive_flash_write_protect,
	.flash.write		= __metal_driver_sifive_flash_write,
	.flash.read		= __metal_driver_sifive_flash_read,
	.flash.erase		= __metal_driver_sifive_flash_erase,
	.flash.register_read	= __metal_driver_sifive_flash_register_read,
	.flash.register_write	= __metal_driver_sifive_flash_register_write,
	.flash.qpi_mode		= __metal_driver_sifive_flash_qpi_mode,
	.flash.get_mfr_id	= __metal_driver_sifive_flash_manufacturer,
	.flash.soft_reset	= __metal_driver_sifive_flash_soft_reset,
};

#endif /*METAL_SIFIVE_FLASH*/
typedef int no_empty_translation_units;


