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

#define AXI_FLASH_BASEADDR		 0x20000000UL

static qspi_static_config_t qcfg;
static qspi_command_config_t cmd_cfg;

/* declaring qspi structure as a static and getting valid handle for qspi in flash_init function */
static struct metal_qspi *qspi;


/* Read Flash status value */
static int flash_status_read(void) {

	uint32_t read_status;

	qspi_static_config_t *cfg=&qcfg;

	cfg->wrmode = QSPI_READ;
	cfg->rxlen = QSPI_8BIT;
	cfg->txlen = QSPI_8BIT;
	cfg->opmode = QSPI_SPI;
	cfg->burstmode = QSPI_APB_ACCESS;
	metal_qspi_setconfig(qspi,cfg);

	/*set command enable, read enable phase*/
	cmd_cfg.custom_command=QSPI_RX_DATA_PH_EN | QSPI_OPCODE_PH_EN;
	cmd_cfg.opcode=FLASH_CMD_RDSR;
	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);
	metal_qspi_execute_cmd(qspi);

	metal_qspi_read(qspi,0,4,(uint8_t *)&read_status);
	
	return read_status;
}


static void flash_send_write_en(struct metal_flash *pfl)
{
	//	struct metal_qspi *qspi =  __metal_driver_sifive_gpio_led_gpio(pfl);


	qspi_static_config_t *cfg=&qcfg;
	cfg->addrlen = QSPI_ADDR_24BIT;
	cfg->wrmode = QSPI_WRITE;
	cfg->rxlen = QSPI_32BIT;
	cfg->txlen = QSPI_32BIT;
	cfg->opmode = QSPI_SPI;
	cfg->burstmode = QSPI_APB_ACCESS;
	metal_qspi_setconfig(qspi,cfg);

	//cmd_cfg.addr=0;
	//cmd_cfg.dummy_stg=0;
	//cmd_cfg.mode_stg=0;
	cmd_cfg.opcode=FLASH_CMD_WRITE_EN;
	cmd_cfg.custom_command=QSPI_OPCODE_PH_EN;
	metal_qspi_setcommand_params(qspi,&cmd_cfg,cfg);

	metal_qspi_execute_cmd(qspi);
}

/* If we want to write less data use APB write
 *
 * */
static int flash_write(struct metal_flash *pfl)
{

	uint32_t read_status;
	//	struct metal_qspi *qspi =  __metal_driver_sifive_gpio_led_gpio(pfl);

	flash_send_write_en(pfl);

	cmd_cfg.custom_command=QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_TX_DATA_PH_EN;
	cmd_cfg.opcode=QSPI_SINGLE_BURST;
	//cmd_cfg.dummy_stg=0;
	//cmd_cfg.mode_stg=0;
	//cmd_cfg.addr=0;
	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);
	metal_qspi_execute_cmd(qspi);

	do {
		read_status = flash_status_read();
	} while ((read_status & 0x1) == 0x1);
	return 0;
}
/*
 *If you want to write full page use continuous bust mode axi
 *
 * */

static int flash_write_page(long int dst_addr_offset,size_t length,char *tx_buff)
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

	uint32_t *axi_addr = (uint32_t*)(AXI_FLASH_BASEADDR + dst_addr_offset);


	uint32_t *dataptr=(uint32_t *)tx_buff;

	for(int i=0;i<length/4;i+=4)
	{
		*axi_addr = *dataptr;

		dataptr++;
		axi_addr++;

		/* wait until write in progress */
		do {
			read_status = flash_status_read();
		} while ((read_status & 0x1) == 0x1);

	}
	printf("qspi_axi_write %d bytes written via AXI Write\n", length);

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
	unsigned long baud_rate=100000;
	struct __metal_driver_sifive_flash *flash = (void *)gflash;


	/* Set-up initial parameters */
   	qcfg.addrlen = QSPI_ADDR_24BIT;
   	qcfg.rxlen = QSPI_32BIT;
   	qcfg.txlen = QSPI_32BIT;
   	qcfg.cs_i = QSPI_DEVICE_0;
   	qcfg.clk_div = QSPI_DIV_1;
   	qcfg.bit_align = QSPI_MSB_FIRST;
   	qcfg.speedmode = QSPI_CMD_ADDR_SERIAL;
   	qcfg.write_prot = QSPI_WRITE_PROT_DIS;
   	qcfg.hold = QSPI_HOLD_DIS;
   	qcfg.burstmode = QSPI_APB_ACCESS;

	qspi = metal_qspi_get_device(0);
	metal_qspi_init(qspi,baud_rate);
	return 0;
}

/* Implement flash read operation here */
int __metal_driver_sifive_flash_read(struct metal_flash *flash, uint32_t addr, const size_t size, char *rx_buf)
{
	uint32_t *axi_addr = addr;
	qspi_static_config_t *cfg=&qcfg;

	cfg->addrlen   = QSPI_ADDR_32BIT;
	cfg->rxlen     = QSPI_32BIT;
	cfg->txlen     = QSPI_32BIT;
	cfg->wrmode    = QSPI_READ;
	cfg->opmode    = QSPI_SPI;
	cfg->speedmode = QSPI_CMD_SERIAL;
	cfg->burstmode = QSPI_SINGLE_BURST;

	metal_qspi_setconfig(qspi,cfg);

	/*set command enable, address enable phase,dummy phase enable,mode phase enable,read enable phase*/

	cmd_cfg.custom_command = (QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_DUMMY_PH_EN 
			| QSPI_MODE_BITS_PH_EN | QSPI_RX_DATA_PH_EN);

	cmd_cfg.dummy_stg = QSPI_SET_DUMMY_DLP(0 , 3 , 0);
	cmd_cfg.mode_stg  = QSPI_SET_MODE_REG(0x1 , 0x0);
	cmd_cfg.opcode    = FLASH_CMD_QUAD_READ_CMD;

	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

	metal_qspi_read(qspi,addr,size,(uint8_t *)rx_buf);

	return size;
}

/* Implement flash write operation here */
int __metal_driver_sifive_flash_write(struct metal_flash *flash, uint32_t mem_addr_offset, const size_t size, char *tx_buf)
{
	uint32_t read_status;
	qspi_static_config_t *cfg = &qcfg;

	flash_send_write_en(flash);

	cmd_cfg.opcode = FLASH_CMD_PAGE_PROGRAM;
	/*set cmd_en and tx_data, address enable phase*/
	cmd_cfg.custom_command = (QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_TX_DATA_PH_EN);

	cfg->burstmode = QSPI_SINGLE_BURST;/*set AXI mode*/
	metal_qspi_setconfig(qspi,cfg);

	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);


	for(int i=0;i<size/4;i+=4)
	{	
		metal_qspi_write(qspi,(mem_addr_offset+i),4,(tx_buf+i));

		/* wait until write in progress */
		do {
			read_status = flash_status_read();
		} while ((read_status & 0x1) == 0x1);
	}
	printf("\nflash_write %d bytes written via AXI Write\n", size);

	return size;
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

/* Read JEDEC ID */
int __metal_driver_sifive_flash_get_device_id(struct metal_flash *flash, unsigned int *pdevice_id)
{
	uint32_t flash_id;
	qspi_static_config_t *cfg;

	cfg = &qcfg;

	cfg->rxlen = QSPI_32BIT;
	cfg->txlen = QSPI_32BIT;
	cfg->wrmode = QSPI_READ;
	cfg->opmode = QSPI_SPI;
	cfg->burstmode = QSPI_APB_ACCESS;
	metal_qspi_setconfig(qspi,cfg);

	cmd_cfg.opcode=FLASH_CMD_ID_READ;
	cmd_cfg.custom_command=QSPI_RX_DATA_PH_EN | QSPI_OPCODE_PH_EN;
	metal_qspi_setcommand_params(qspi,&cmd_cfg,&qcfg);

	metal_qspi_execute_cmd(qspi);

	for (volatile int i = 0; i < 5; i++);

	metal_qspi_read(qspi,0,4,(uint8_t *)&flash_id);
	*pdevice_id=flash_id;

	printf("Flash ID: %x \n", flash_id);
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

	/* Sector erase */
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

	printf(" Erase flash sector from %x \n", addr);

	/*set cmd_en and address enable phase*/

	do {
		read_status = flash_status_read();
	} while ((read_status & 0x1) == 0x1);

	return 0;
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_flash) = {
	.flash.init 	 	  = __metal_driver_sifive_flash_init,
	.flash.get_device_id      = __metal_driver_sifive_flash_get_device_id,
	.flash.write_protect      = __metal_driver_sifive_flash_write_protect,
	.flash.write	      	  = __metal_driver_sifive_flash_write,
	.flash.read  		  = __metal_driver_sifive_flash_read,
	.flash.erase  		  = __metal_driver_sifive_flash_erase,
};

#endif /*METAL_SIFIVE_FLASH*/
typedef int no_empty_translation_units;

#if 0
/* Flash read in AXI QUAD DDR mode */
void flash_axi_quad_ddr_read(uint32_t src_addr,uint8_t *rxdata,uint32_t length) {

	uint32_t *axi_addr = (uint32_t*) (AXI_FLASH_BASEADDR+src_addr);
	qspi_config *cfg;
	cfg = qspi_getconfig();

	cfg->addrlen = QSPI_ADDR_32BIT;
	cfg->rxlen = QSPI_32BIT;
	cfg->txlen = QSPI_32BIT;
	cfg->wrmode = QSPI_READ;
	cfg->opmode = QSPI_QUAD_DDR;
	cfg->speedmode = QSPI_CMD_SERIAL;
	cfg->burstmode = QSPI_SINGLE_BURST;
	qspi_setconfig(cfg);

	/*set command enable, address enable phase,dummy phase enable,
	 * mode phase enable,read enable phase*/
	qspi_set_custom_cmd(
			QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_DUMMY_PH_EN
			| QSPI_MODE_BITS_PH_EN | QSPI_RX_DATA_PH_EN);

	qspi_set_mode_reg(0x1, 0x00);

	qspi_set_dummy_dlp(1, 5, 0);

	qspi_set_opcode(DDR_QUAD_READ_CMD);

	/* Read length/4 locations */
	for (int i = 0; i < length; i+=4) {
		printf("QSPI AXI QUAD DDR Read at address %x:%x \n", axi_addr, *axi_addr);
		*(rxdata+i)=*axi_addr;
		axi_addr++;
	}
}

/* Flash read in AXI QUAD mode */
void flash_axi_quad_read(uint32_t src_addr,uint8_t *rxdata,uint32_t length) {

	uint32_t *axi_addr = (uint32_t*) (AXI_FLASH_BASEADDR+src_addr);
	qspi_config *cfg;

	cfg = qspi_getconfig();
	cfg->addrlen = QSPI_ADDR_32BIT;
	cfg->rxlen = QSPI_32BIT;
	cfg->txlen = QSPI_32BIT;
	cfg->wrmode = QSPI_READ;
	cfg->opmode = QSPI_QUAD;
	cfg->speedmode = QSPI_CMD_SERIAL;
	cfg->burstmode = QSPI_SINGLE_BURST;

	qspi_setconfig(cfg);

	/*set command enable, address enable phase,dummy phase enable,
	 * mode phase enable,read enable phase*/
	qspi_set_custom_cmd(
			QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_DUMMY_PH_EN
			| QSPI_MODE_BITS_PH_EN | QSPI_RX_DATA_PH_EN);

	qspi_set_mode_reg(0x1, 0x00);

	qspi_set_dummy_dlp(0, 3, 0);

	qspi_set_opcode(QUAD_READ_CMD);

	/* Read length/4 locations */
	for (int i = 0; i < length; i+=4) {
		printf("QSPI AXI QUAD DDR Read at address %x:%x \n", axi_addr, *axi_addr);
		*(rxdata+i)=*axi_addr;
		axi_addr++;
	}
}

/* Read Flash ID */
static uint32_t flash_id_read(void) {


}



/* Flash write in AXI QUAD mode */
static void flash_apb_quad_write(uint32_t dst_addr_offset,uint8_t *txdata,uint32_t length) {

	qspi_config *cfg;
	uint32_t tempaddr = (AXI_FLASH_BASEADDR+dst_addr_offset);
	uint32_t read_status, cnt;

	for (cnt = 0; cnt < length; cnt+=4) {

		cfg = qspi_getconfig();

		cfg->addrlen = QSPI_ADDR_24BIT;
		cfg->wrmode = QSPI_WRITE;
		cfg->opmode = QSPI_QUAD;
		cfg->rxlen = QSPI_32BIT;
		cfg->txlen = QSPI_32BIT;
		cfg->burstmode = QSPI_APB_ACCESS;
		cfg->speedmode = QSPI_CMD_ADDR_SERIAL;
		qspi_setconfig(cfg);

		qspi_set_opcode(WRITE_EN);
		/*set command enable*/
		qspi_set_custom_cmd(QSPI_OPCODE_PH_EN);
		/*start command*/
		qspi_apb_rw_trigger();

		qspi_set_opcode(QUAD_WRITE);
		qspi_apb_setaddr(tempaddr);

		/*set cmd_en and tx_data, address enable phase*/
		qspi_set_custom_cmd(
				QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_TX_DATA_PH_EN);
		qspi_apb_write((uint32_t)*(txdata+cnt));
		tempaddr += 4;
		/*start command*/
		qspi_apb_rw_trigger();

		do {
			read_status = flash_status_read();
		} while ((read_status & 0x1) == 0x1);
	}

	printf("%d bytes written via APB QUAD Write, \n", cnt * 4);
}

/* Flash write in XIAPB QUAD mode */
static void flash_axi_quad_write(uint32_t dst_addr_offset,uint8_t *txdata,uint32_t length) {

	qspi_config *cfg;
	uint32_t tempaddr = (AXI_FLASH_BASEADDR+dst_addr_offset);
	uint32_t read_status, cnt;

	uint32_t *axi_addr = (uint32_t*)(AXI_FLASH_BASEADDR+dst_addr_offset);

	for (cnt = 0; cnt < length; cnt+=4)
	{

		cfg = qspi_getconfig();

		cfg->addrlen = QSPI_ADDR_24BIT;
		cfg->burstmode = QSPI_APB_ACCESS;
		cfg->wrmode = QSPI_WRITE;
		cfg->opmode = QSPI_QUAD;
		cfg->rxlen = QSPI_32BIT;
		cfg->txlen = QSPI_32BIT;
		cfg->speedmode = QSPI_CMD_ADDR_SERIAL;
		qspi_setconfig(cfg);

		qspi_set_opcode(WRITE_EN);
		/*set command enable*/
		qspi_set_custom_cmd(QSPI_OPCODE_PH_EN);
		/*start command*/
		qspi_apb_rw_trigger();

		qspi_set_opcode(QUAD_WRITE);

		/*set cmd_en and tx_data, address enable phase*/
		qspi_set_custom_cmd(
				QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_TX_DATA_PH_EN);

		cfg->burstmode = QSPI_SINGLE_BURST;
		qspi_setconfig(cfg); /* set AXI mode */

		*axi_addr = *(txdata+cnt);

		axi_addr++;

		/* wait unit write in progress */
		do {
			read_status = flash_status_read();
		} while ((read_status & 0x1) == 0x1);
	}

	printf("%d bytes written via AXI QUAD Write, pattern: 0xBEECEEXX \n", cnt * 4);
}


/*
 * SDR WRITE
 *
 * */
static void flash_apb_write(uint32_t dst_addr_offset,uint8_t *txdata,uint32_t length) {

	uint32_t read_status, cnt;
	uint32_t tempaddr = (AXI_FLASH_BASEADDR+dst_addr_offset);

	qspi_config *cfg;
	cfg = qspi_getconfig();

	for (int cnt = 0; cnt < length; cnt+=4)
	{

		cfg->addrlen = QSPI_ADDR_24BIT;
		cfg->wrmode = QSPI_WRITE;
		cfg->rxlen = QSPI_32BIT;
		cfg->txlen = QSPI_32BIT;
		cfg->opmode = QSPI_SPI;
		cfg->burstmode = QSPI_APB_ACCESS;
		qspi_setconfig(cfg);

		qspi_set_opcode(WRITE_EN);


		/*set command enable*/
		qspi_set_custom_cmd(QSPI_OPCODE_PH_EN);

		/*start command*/
		qspi_apb_rw_trigger();

		qspi_set_opcode(PAGE_PROGRAM_3BYTE_ADDR);


		qspi_apb_setaddr(dst_addr_offset+cnt);
		/*set cmd_en and tx_data, address enable phase*/
		qspi_set_custom_cmd(
				QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_TX_DATA_PH_EN);

		qspi_apb_write(*(txdata+cnt));
		/*start command*/
		qspi_apb_rw_trigger();

		do {
			read_status = flash_status_read();
		} while ((read_status & 0x1) == 0x1);
	}

	printf("%d bytes written via APB SPI mode Write, pattern: 0xCAFFE9XX \n", cnt * 4);
}

static void flash_erase(uint32_t dst_addr,uint32_t length) {


}

/**
 * APB SDR READ
 */
static void flash_apb_read(uint32_t dst_addr_offset,uint8_t *rxdata,uint32_t length) {

	uint32_t cnt;

	qspi_config *cfg;
	cfg = qspi_getconfig();

	cfg->addrlen = QSPI_ADDR_24BIT;
	cfg->wrmode = QSPI_READ;
	cfg->rxlen = QSPI_32BIT;
	cfg->txlen = QSPI_32BIT;
	cfg->opmode = QSPI_SPI;
	cfg->burstmode = QSPI_APB_ACCESS;
	qspi_setconfig(cfg);

	/*set cmd_en and rx_data,address enable phase*/
	qspi_set_custom_cmd(
			QSPI_RX_DATA_PH_EN | QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN);


	/* Read length/4 locations */
	for (int i = 0; i < length; i+=4)
	{
		qspi_set_opcode(READ_CMD);
		qspi_apb_setaddr(dst_addr_offset+i);
		qspi_set_dummy_dlp(0, 3, 0);
		/*start command*/
		qspi_apb_rw_trigger();

		for (volatile int i = 0; i < 5; i++)
			;

		qspi_apb_read((uint32_t *)(rxdata+i));
		printf("APB DUAL mode Read at %x: %x \n", dst_addr_offset+i, rxdata[i]);
	}

}

/**
 *
 *
 */
static void flash_apb_dual_read(uint32_t dst_addr_offset,uint8_t *rxdata,uint32_t length) {

	uint32_t cnt=0;

	qspi_config *cfg;
	cfg = qspi_getconfig();

	cfg->addrlen = QSPI_ADDR_24BIT;
	cfg->wrmode = QSPI_READ;
	cfg->rxlen = QSPI_32BIT;
	cfg->txlen = QSPI_32BIT;
	cfg->opmode = QSPI_DUAL;
	cfg->speedmode = QSPI_CMD_SERIAL;
	cfg->burstmode = QSPI_APB_ACCESS;
	qspi_setconfig(cfg);

	/*set cmd_en and rx_data, dummy phase,address enable phase*/
	qspi_set_custom_cmd(
			QSPI_RX_DATA_PH_EN | QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN
			| QSPI_DUMMY_PH_EN);

	/* Read length/4 locations */
	for (int i = 0; i < length; i+=4)
	{
		qspi_set_opcode(DUAL_READ);
		qspi_apb_setaddr(dst_addr_offset+i);
		qspi_set_dummy_dlp(0, 3, 0);
		/*start command*/
		qspi_apb_rw_trigger();

		for (volatile int i = 0; i < 5; i++)
			;

		qspi_apb_read((uint32_t *)(rxdata+i));
		printf("APB DUAL mode Read at %x: %x \n", dst_addr_offset+i, rxdata[i]);
	}
}

/**
 *SDR mode AXI write
 *
 */
void flash_axi_write(uint32_t dst_addr_offset,uint8_t *txdata,uint32_t length)
{
	uint32_t read_status, cnt;
	uint32_t *axi_addr = (uint32_t*)(AXI_FLASH_BASEADDR+dst_addr_offset);

	qspi_config *cfg;
	cfg = qspi_getconfig();

	cfg->addrlen = QSPI_ADDR_24BIT;
	cfg->burstmode = QSPI_APB_ACCESS;
	cfg->wrmode = QSPI_WRITE;
	cfg->opmode = QSPI_SPI;
	cfg->rxlen = QSPI_32BIT;
	cfg->txlen = QSPI_32BIT;
	qspi_setconfig(cfg);

	qspi_set_opcode(WRITE_EN);
	/*set command enable*/
	qspi_set_custom_cmd(QSPI_OPCODE_PH_EN);
	/*start command*/
	qspi_apb_rw_trigger();

	qspi_set_opcode(PAGE_PROGRAM_3BYTE_ADDR);

	/*set cmd_en and tx_data, address enable phase*/
	qspi_set_custom_cmd(
			QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_TX_DATA_PH_EN);

	cfg->burstmode = QSPI_SINGLE_BURST;
	qspi_setconfig(cfg); /* set AXI mode */


	uint32_t *dataptr=(uint32_t *)txdata;

	for(int i=0;i<length;i+=4)
	{
		*axi_addr = *dataptr;

		dataptr++;
		axi_addr++;

		/* wait until write in progress */
		do {
			read_status = flash_status_read();
		} while ((read_status & 0x1) == 0x1);

	}
	printf("qspi_axi_write %d bytes written via AXI Write\n", length);
}


/**
 *SDR mode AXI write
 *
 */
void flash_axi_read(uint32_t dst_addr_offset,uint8_t *rxdata,uint32_t length)
{
	uint32_t *axi_addr = (uint32_t*) (AXI_FLASH_BASEADDR+dst_addr_offset);
	qspi_config *cfg;

	cfg = qspi_getconfig();
	cfg->addrlen = QSPI_ADDR_32BIT;
	cfg->rxlen = QSPI_32BIT;
	cfg->txlen = QSPI_32BIT;
	cfg->wrmode = QSPI_READ;
	cfg->opmode = QSPI_SPI;
	cfg->speedmode = QSPI_CMD_SERIAL;
	cfg->burstmode = QSPI_SINGLE_BURST;

	qspi_setconfig(cfg);

	/*set command enable, address enable phase,dummy phase enable,
	 * mode phase enable,read enable phase*/
	qspi_set_custom_cmd(
			QSPI_OPCODE_PH_EN | QSPI_ADDR_PH_EN | QSPI_DUMMY_PH_EN
			| QSPI_MODE_BITS_PH_EN | QSPI_RX_DATA_PH_EN);

	qspi_set_mode_reg(0x1, 0x00);

	qspi_set_dummy_dlp(0, 3, 0);

	qspi_set_opcode(QUAD_READ_CMD);

	/* Read length/4 locations */
	for (int i = 0; i < length; i+=4) {
		printf("QSPI AXI QUAD DDR Read at address %x:%x \n", axi_addr, *axi_addr);
		*(rxdata+i)=*axi_addr;
		axi_addr++;
	}
}
#endif
