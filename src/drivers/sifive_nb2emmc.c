/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#define SIFIVE_NB2_EMMC
#ifdef SIFIVE_NB2_EMMC

#include <metal/machine/platform.h>
#include <metal/drivers/sifive_nb2emmc.h>
#include <metal/machine.h>
#include <limits.h>
#include <metal/io.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_COUNT 10000
#define EMMC_RCA 0x1000


#define SD_HOST_CLK 200000000
#define SD_ADDR_SECTOR_MODE 1


#define SWR (1 << 0)
#define EMMC_LEGACY_MODE 0x01
#define ENABLE_FLAGS 0xFFFFFFFF
#define CLEAR_FLAGS 0xFFFFFFFF
#define BOOT_PARTITION_1_ENABLE 3
#define BOOT_PARTITION_ACCESS 0
#define GO_PRE_IDLE_STATE 0xF0F0F0F0
#define GO_IDLE_STATE 0x0


#define BOOT_INITIATION 0xFFFFFFFA
#define ERROR_STATUS_MASK 0xFFFF8000
#define BIT_WIDTH_MASK 0x0F
#define ERASE_VAL 0x00000000



#define MMC_EXCSD_CQ_SUPPORT              308U
/// CMD Queuing Depth
#define MMC_EXCSD_CQ_DEPTH                307U
/// Boot information (supported transmission modes)
#define MMC_EXCSD_BOOT_INFO               228U
/// Boot partition size
#define MMC_EXCSD_BOOT_SIZE_MULTI         226U
/// I/O Driver Strength
#define MMC_EXCSD_I_O_DRIVER_STRENGTH     197U
/// Device type
#define MMC_EXCSD_DEVICE_TYPE             196U
/// MMC card power class
#define MMC_EXCSD_POWER_CLASS             187U
/// High speed interface timing
#define MMC_EXCSD_HS_TIMING               185U
///field informs whether device supports Enhanced Strobe
#define MMC_EXCSD_ES_SUPPORTED            184U
/// Bus width mode
#define MMC_EXCSD_BUS_WIDTH               183U
/// Partition configuration
#define MMC_EXCSD_BOOT_PART_CONFIG        179U
/// Boot config protection
#define MMC_EXCSD_BOOT_CONFIG_PROT        178U
/// Boot bus Conditions
#define MMC_EXCSD_BOOT_BUS_COND           177U
/// Command Queue Mode Enable
#define MMC_EXCSD_CQ_MODE_EN              15U
//@}
//


/// select partitions no access to boot partition (default)
#define MMC_EXCSD_BOOTPART_ACCESS_NONE    (0U << 0)
/// select partitions to access to R/W boot partition 1
#define MMC_EXCSD_BOOTPART_ACCESS_BP_1    (1U << 0)
/// select partitions to access to R/W boot partition 2
#define MMC_EXCSD_BOOTPART_ACCESS_BP_2    (2U << 0)
///R/W Replay Protected Memory Block (RPMB)
#define MMC_EXCSD_BOOTPART_ACCESS_RPMB    (3U << 0)
/// select partitions to access to to General Purpose partition 1
#define MMC_EXCSD_BOOTPART_ACCESS_GP_1    (4U << 0)
/// select partitions to access to to General Purpose partition 2
#define MMC_EXCSD_BOOTPART_ACCESS_GP_2    (5U << 0)
/// select partitions to access to to General Purpose partition 3
#define MMC_EXCSD_BOOTPART_ACCESS_GP_3    (6U << 0)
/// select partitions to access to to General Purpose partition 4
#define MMC_EXCSD_BOOTPART_ACCESS_GP_4    (7U << 0)
/// select partitions to access mask
#define MMC_EXCSD_BOOTPART_ACCESS_MASK    (7U << 0)
#define MMC_EXCSD_BOOTPART_CFG_BOOT_EN_MASK   (7U << 3)


/// Device not boot enabled (default)
#define MMC_EXCSD_BOOTPART_CFG_BOOT_DISABLE   (0U << 3)
/// Boot partition 1 enabled for boot
#define MMC_EXCSD_BOOTPART_CFG_BOOT1_EN       (1U << 3)
/// Boot partition 2 enabled for boot
#define MMC_EXCSD_BOOTPART_CFG_BOOT2_EN       (2U << 3)
/// User area enabled for boot
#define MMC_EXCSD_BOOTPART_CFG_BOOTUSR_EN     (7U << 3)
/// boot partition selection mask
#define MMC_EXCSD_BOOTPART_CFG_BOOT_EN_MASK   (7U << 3)

///Boot acknowledge sent during boot operation Bit
#define MMC_EXCSD_BOOTPART_CFG_BOOT_ACK       (1U << 6)


/// macro gets one byte from dword
#define GetByte(dword, byte_nr)     (((dword) >> ((byte_nr) * 8U)) & 0xFFU)



/*
   Host Register Set
#define REG_HRS00 0x0000
#define REG_HRS06 0x0018

Slot Register Set
#define REG_SRS01 0x0204
#define REG_SRS02 0x0208
#define REG_SRS03 0x020C
#define REG_SRS08 0x0220
#define REG_SRS09 0x0224
#define REG_SRS10 0x0228
#define REG_SRS11 0x022c
#define REG_SRS12 0x0230
#define REG_SRS13 0x0234
#define REG_SRS14 0x0238
*/

//SRS01

#define BCCT  16

// SRS10
#define LEDC	0
#define DTW	1
#define HSE	2
#define DMASEL	3
#define EDTW	5
#define CDTL	6
#define CDSS	7
#define BP	8
#define BVS	9
#define BP2	12

// SRS 11
#define ICE	0
#define ICS	1
#define SDCE	2
#define CGS	5
#define SDCFSH	6
#define SDCFSL	8
#define DTCV	16
#define SRFA	24
#define SRCMD	25
#define SRDAT	26


// SRS 12, 13, 14
#define CC       0
#define TC       1
#define BGE      2
#define DMAINT     3
#define BWR      4
#define BRR      5
#define CIN      6
#define CR       7
#define CINT     8
//#define INT_A    9
//#define INT_B   10
//#define INT_C   11
//#define RTNGE   12
#define CQINT   14
#define EINT    15
#define ECT     16
#define ECCRC   17
#define ECEB    18
#define ECI     19
#define EDT     20
#define EDCRC   21
#define EDEB    22
#define ECL     23
#define EAC    24
#define EADMA   25
#define ERSP    27

// SRS03
#define DMAE     0   /* DMA enable */
#define BCE      1   /* Block count enable */
#define ACE      2
#define DTDS     4
#define MSBS     5
#define RECT     6  /*response type R1/R5*/
#define RECE     7  /*response check en*/
#define RID      8  /*response interupt disable*/
#define RTS     16  /* Response type */
#define CRCCE   19  /* CRC check enable */
#define CICE    20
#define DPS     21
#define CT      22
#define CI      24  /* Command index */

// SRS09
#define CICMD    0
#define CIDAT    1
#define DLA      2
#define RTR      3
#define DATSL2   4
#define WTA      8
#define RTA      9
#define BWE     10  /* Buffer write enable */
#define BRE     11  /* Buffer read enable */
#define CIns    16  /* Card Inserted */
#define CSS     17
#define CDSL    18
#define WPSL    19
#define DATSL1  20
#define CMDSL1  24

#define METAL_EMMC_REG(offset)   (((unsigned long long)emmc_control_base + offset))
#define METAL_EMMC_REGB(offset)  (__METAL_ACCESS_ONCE((__metal_io_u8  *)METAL_EMMC_REG(offset)))
#define METAL_EMMC_REGW(offset)  (__METAL_ACCESS_ONCE((__metal_io_u32 *)METAL_EMMC_REG(offset)))

#define METAL_EMMC_IOMUX_REGW(ADDR)	 (__METAL_ACCESS_ONCE((__metal_io_u32 *)ADDR))

#define SCR_RESET_BASE_ADDR     	0x4F0011000UL
#define SCR_REG_BASE_ADDR       	0x4F0010000UL
#define SCR_IOMUX_HSSS_CFG_BASE_ADDR    0x301500000UL

#define PCSS_SCR_EMMC_SDIO_NIU_RESET    		( SCR_RESET_BASE_ADDR + 0x0048 )
#define PCSS_SCR_EMMC_SDIO_RESET        		( SCR_REG_BASE_ADDR + 0x00B4 )

#define HSSS_SCR_IOMUX_CONFIG_EMMC0_CLK                 ( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x005C )
#define HSSS_SCR_IOMUX_CONFIG_EMMC0_CMD                 ( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x0060 )
#define HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT0                ( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x0064 )
#define HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT1                ( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x0068 )
#define HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT2                ( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x006C )
#define HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT3                ( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x0070 )
#define HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT4                ( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x0074 )
#define HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT5                ( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x0078 )
#define HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT6                ( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x007C )
#define HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT7                ( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x0080 )
#define HSSS_SCR_IOMUX_CONFIG_EMMC0_DS			( SCR_IOMUX_HSSS_CFG_BASE_ADDR   + 0x0084 )

static unsigned long long emmc_control_base=0;
static eMMCRequest_t input_cmd;

uint32_t g_data_buffer[128]; //512 bytes


static inline uint8_t GET_BYTE_FROM_BUFFER(const void* buffer, uintptr_t byteNumber)
{
    return ((uint8_t)GetByte((*(uint32_t*)((uintptr_t)buffer + (byteNumber & ~3UL))), (byteNumber & 3UL)));
}

/*
static bool conditional_wait(volatile unsigned long reg,uint32_t val, bool condition)
{
	uint32_t val=0;
	do{
		val=METAL_EMMC_REGW(reg)__METAL_ACCESS_ONCE((__metal_io_u32 *)reg);

	}while(val & SWR== condition);
}
*/
static void emmc_iomux_config(void)
{
	METAL_EMMC_IOMUX_REGW(PCSS_SCR_EMMC_SDIO_NIU_RESET) = 0X49; //Deassert EMMC/SDIO NIU Reset
	METAL_EMMC_IOMUX_REGW(PCSS_SCR_EMMC_SDIO_RESET) = 0X1FF;   //Deassert EMMC/SDIO Reset
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_CLK) = 0x5500;
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_CMD) = 0x5500;
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT0) = 0x5500;
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT1) = 0x5500;
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT2) = 0x5500;
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT3) = 0x5500;
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT4) = 0x5500;
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT5) = 0x5500;
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT6) = 0x5500;
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_DAT7) = 0x5500;
	METAL_EMMC_IOMUX_REGW(HSSS_SCR_IOMUX_CONFIG_EMMC0_DS) = 0x5500;
}

static void emmc_host_reset()
{
	uint32_t timeout=0;
	METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_HRS00) = SWR ;

	//conditional_wait(METAL_SIFIVE_NB2EMMC_HRS00,SWR,true);

	
	while ((METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_HRS00) & SWR) == 1);  //wait for HRS0.0 (SWR)= 0
}

static void emmc_host_set_clock(unsigned int freq)
{
	uint32_t sdclkfs, dtcvval;

	if(freq) {
		sdclkfs = (SD_HOST_CLK / 2000) / freq;
	} else {
		sdclkfs = 0;
	}

	dtcvval = 0xE;
	// set clock
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS11) = 0;  // disable SDCE
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS11) = (dtcvval << DTCV) | (sdclkfs << SDCFSL) | (1 << ICE); // ICE (internal clock enable)


	while ((METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS11) & (1 << ICS)) == 0); // wait for ICS (internal clock stable)
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS11) = (dtcvval << DTCV) | (sdclkfs << SDCFSL) | (1 << ICE) | (1 << SDCE); // enable SD clock (SRS11.SDCE)
}


static void emmc_host_initialization()
{
	printf("Doing eMMC Reset\n");
	// Software reset
	emmc_host_reset();

	// set Host controller to eMMC legacy mode
	METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_HRS06) = EMMC_LEGACY_MODE;

	// sd card detection
	while ((METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS09) & (1 << CIns)) == 0);  //wait for SRS9.CI = 1

	//eMMC card reset
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS10) = (7 << BVS) | (1 << BP); //BVS - SD Bus Voltage Select,  BP - SD Bus Power for VDD1
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS10) = (7 << BVS) | (0 << BP);

	//enable supply voltage
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS10) = (7 << BVS) | (1 << BP);  // BVS = 7, BP = 1, BP2 only in UHS2 mode

	//starting SDclk
	emmc_host_set_clock( 398);		//clock freq. set

	//enable flags
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS13) = ENABLE_FLAGS;		//all flags enable
}


//-----------------------------------------------------------------------------
static uint32_t calc_data_tocopy(const eMMCRequest_t* request)
{
	uint32_t data_tocopy;

	if (request->dataRemaining < (request->blocklen) ){
		data_tocopy = request->dataRemaining;
	} else {
		data_tocopy = request->blocklen;
	}

	return (data_tocopy);
}

//-----------------------------------------------------------------------------
static void process_databuffer_read(eMMCRequest_t* request)
{
	uint32_t data_tocopy = calc_data_tocopy(request);

	request->dataRemaining -= data_tocopy;

	uint8_t *tempdataptr=request->dataptrpos;


	while(data_tocopy > 0U) 
	{
		uint32_t data = METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS08);
		uint8_t byte_count = 4;
	
	//	data_tocopy-=4;	
		

		while(data_tocopy && byte_count) {
			*((uint8_t*)tempdataptr) = data & 0xFF;
			data >>= 8;
			byte_count--;
			data_tocopy--;
			tempdataptr++;
			request->dataptrpos++;
		}
	}
}

static void process_databuffer_write(eMMCRequest_t* request)
{
	uint32_t data_tocopy = calc_data_tocopy(request);

	request->dataRemaining -= data_tocopy;

	uint8_t *tempdataptr=request->dataptrpos;

	while(data_tocopy > 0U)
	{
		uint32_t data = 0;
		uint8_t byte_count = 0;

		while(data_tocopy && byte_count<4) {
			//data |= (uint32_t)*((uint8_t*)request->dataptrpos) << (8 * byte_count);
			data |= (uint32_t)*((uint8_t*)tempdataptr) << (8 * byte_count);
			byte_count++;
			data_tocopy--;
			//request->dataptrpos = (uint8_t*)request->dataptrpos + 1;
			request->dataptrpos++;
		}
		METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS08) = data;
	}
}

static int emmc_host_send_cmd(eMMCRequest_t *input_cmd)
{
	uint32_t cmd=0;
	uint32_t crccet_val=0;
	int retval=0;

	input_cmd->cmd_response[0]=0;
	input_cmd->cmd_response[1]=0;
	input_cmd->cmd_response[2]=0;	
	input_cmd->cmd_response[3]=0;

	// Clear all Flags
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS12) = CLEAR_FLAGS;

	//Set command argument
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS02) = input_cmd->arg;

	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS14) = 0x00000000;  // disable interrupts


	cmd=(input_cmd->cmd << CI) | (input_cmd->response_type << RTS);


	//if((input_cmd->response_type==EMMC_RESPONSE_R1B) ||(input_cmd->response_type==EMMC_RESPONSE_R1))
	if((input_cmd->response_type==EMMC_RESPONSE_R1B))
	{
		crccet_val=3;
	}else
	{
		crccet_val=0;
	}

	cmd  |= crccet_val<< CRCCE;

	if(input_cmd->data_present)
	{
		cmd |=((1 << DPS) | (input_cmd->data_direction << DTDS));

		if(input_cmd->blockcnt>1)
		{
			cmd |= ((1<<MSBS)|(1<<BCE));
		}
		METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS01) = input_cmd->blocklen|(input_cmd->blockcnt<<BCCT);
	}

	printf("Sending eMMC Command CMD%d\n arg=%x",input_cmd->cmd, input_cmd->arg);

	METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS03) = cmd;  // execute command


	do {
		input_cmd->status = METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS12);

	} while ((input_cmd->status & ((1 << CC) | (1 << EINT)) ) == 0); // exit loop when CC (command complete) flag is set to 1

	printf("EMMC:CMD%d complete status %x\n",input_cmd->cmd,input_cmd->status);



	if((input_cmd->status & (1<<EINT))!=0)
		retval=-1;
	else
		retval=0;

	input_cmd->status &= ERROR_STATUS_MASK;


	if(input_cmd->response_type !=EMMC_RESPONSE_NO_RESP)
	{
		input_cmd->cmd_response[0]=METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS04);
		input_cmd->cmd_response[1]=METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS05);
		input_cmd->cmd_response[2]=METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS06);
		input_cmd->cmd_response[3]=METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS07);

		printf("Response received %x %x %x %x\n",input_cmd->cmd_response[0],input_cmd->cmd_response[1],input_cmd->cmd_response[2],input_cmd->cmd_response[3]);
	}

	return retval;
}


/**
 *
 * 
 */
static int emmc_select_card (uint32_t card_rca)
{
	int retval=0;
	//eMMCRequest_t input_cmd;
	input_cmd.cmd=EMMC_CMD7;
	if (card_rca == 0x000){

		input_cmd.arg=0;
		input_cmd.response_type=EMMC_RESPONSE_NO_RESP;
		input_cmd.data_present=0;

		// no response from the deselected Card
		//sd_emmc_host_send_cmd((7 << CI) | (0 << CRCCE) | (0 << RTS), 0x0000, 0);
	}
	else{
		//  R1/R1b response from the selected Card
		input_cmd.arg=card_rca << 16;
		input_cmd.response_type=EMMC_RESPONSE_R1;
		input_cmd.data_present=0;
		printf("card selected with RCA %x \n",card_rca );
	}
	retval=emmc_host_send_cmd(&input_cmd);

	return retval;	
}


static void emmc_read_cid()
{
	printf("Reading eMMC CID: CMD2\n");

	//eMMCRequest_t input_cmd;
	input_cmd.cmd=EMMC_CMD2;
	input_cmd.arg=0;
	input_cmd.response_type=EMMC_RESPONSE_R2;
	input_cmd.data_present=0;

	emmc_host_send_cmd(&input_cmd);

	printf("CID received %x %x %x %x\n",input_cmd.cmd_response[0],input_cmd.cmd_response[1],input_cmd.cmd_response[2],input_cmd.cmd_response[3]);


	uint32_t manufacturerId = (uint8_t)((input_cmd.cmd_response[3] >> 16) & 0xFFU);
	uint32_t oemApplicationId = (uint16_t)(input_cmd.cmd_response[3] & 0xFFFFU);

	uint32_t name[20];
	name[0] = (uint8_t)((input_cmd.cmd_response[1] >> 24) & 0xFFU);
	for (int i = 0U; i < 4U; i++) {
		uint8_t nameElement = 0U;
		const uint8_t shift = i * 8U;
		if (shift < 32U) {
			nameElement = (uint8_t)((input_cmd.cmd_response[2] >> shift) & 0xFFU);
		}
		name[i + 1U] = nameElement;
	}

	uint32_t productRevision = (uint8_t)((input_cmd.cmd_response[1] >> 16) & 0xFFU);

	uint32_t productSn = ((input_cmd.cmd_response[0] >> 16) & 0xFFFFU)
		| ((input_cmd.cmd_response[1] & 0xFFFFU) << 16);

	uint32_t manufacturingDate = (uint16_t)((input_cmd.cmd_response[0]) & 0xFFFU);

	printf(" manufacturerId %x\n",manufacturerId); 
	printf(" Name %s\n",name); 
	printf(" productRevision %x\n",productRevision); 
	printf(" productSn %x\n",productSn); 
	printf(" manufacturingDate %x\n",manufacturingDate); 
}


static int emmc_card_init(unsigned int low_voltage_en)
{

	int retval=0;
	uint32_t _status;
	uint32_t _count_delay;

	printf("emmc_card_init\n");
	// CMD0 - reset all cards to IDLE state
	//eMMCRequest_t input_cmd;

	input_cmd.cmd=EMMC_CMD0;
	input_cmd.arg=0;
	input_cmd.response_type=EMMC_RESPONSE_NO_RESP;
	input_cmd.data_present=0;
	retval=emmc_host_send_cmd(&input_cmd);

	// CMD1 - send operational condition and go to READY state
	do {
		_status = 0x00ff8000; // 2.7-3.6V
		if (low_voltage_en == 1) _status |= (1 << 7);  // 1.8V
		_status |= (SD_ADDR_SECTOR_MODE << 30); // byte or sector address mode
		_status |=(1 << 31);

		input_cmd.cmd=EMMC_CMD1;
		input_cmd.arg=_status;
		input_cmd.response_type=EMMC_RESPONSE_R1;
		input_cmd.data_present=0;
		retval=emmc_host_send_cmd(&input_cmd);  // voltage and address mode


		//sd_emmc_host_send_cmd( (1 << CI) | (2<<RTS), _status, 0);  // voltage and address mode
		_status =input_cmd.cmd_response[0];
	} while ((_status & 0x80000000) == 0);

	emmc_read_cid();

	// CMD3 - send RCA to eMMC card and go to STBY
	uint32_t sd_card_rca = EMMC_RCA;
	input_cmd.cmd=EMMC_CMD3;
	input_cmd.arg=sd_card_rca<<16;
	input_cmd.response_type=EMMC_RESPONSE_R1;
	input_cmd.data_present=0;
	
	retval=emmc_host_send_cmd(&input_cmd);

	if(EMMC_RCA==((input_cmd.cmd_response[0]>>16) &0xFFFFU))
	 {
		printf("RAC set %x\n",((input_cmd.cmd_response[0]>>16) &0xFFFFU));
         }

	// -- initialization finished --
	return 0;
}


static int emmc_card_ext_csd_write( unsigned int index, unsigned int val)
{

	int retval=0;
	unsigned int status;
	long int arg;
	uint32_t timeout=0;


	arg = ((val << 8) | (index << 16) | (3 << 24));

	//MMCRequest_t input_cmd;
	input_cmd.cmd=EMMC_CMD6;
	input_cmd.arg=arg;
	input_cmd.response_type=EMMC_RESPONSE_R1B;
	input_cmd.data_present=0;

	retval=emmc_host_send_cmd(&input_cmd);//(EMMC_CMD6 << CI) | (3 << CRCCE) | (3 << RTS), arg);

	do 
	{
		status = METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS12);
		if(timeout > MAX_COUNT) {
			printf("Exit from function \"static void emmc_card_ext_csd_write()\" due to timeout");
			exit(1);
		}
		else
			timeout++;
	} while ((status & (1 << TC)) == 0);

	return retval;
}


static int  emmc_card_ext_csd_read(uint8_t *rx_data)
{
	int retval=0;
	unsigned int status;
	long int arg;
	uint32_t timeout=0;

	printf("Reading eMMC CSD EXT\n");

	//emmc_select_card(EMMC_RCA);

	input_cmd.cmd=EMMC_CMD8;
	input_cmd.arg=0;
	input_cmd.response_type=EMMC_RESPONSE_R1;
	input_cmd.data_present=1;
	input_cmd.data_direction=EMMC_TRANSFER_READ;

	input_cmd.dataptr=rx_data;
	input_cmd.dataptrpos=rx_data;
	input_cmd.dataRemaining=512;
	input_cmd.blockcnt=1;
	input_cmd.blocklen=512;

	retval=emmc_host_send_cmd(&input_cmd);


	while( ((METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS09) >> BRE) & 0x01) == 0 );
	// wait for BRE(buffer read enable)


	do
	{
		while(((METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS12) >> BRR) & 0x01) == 0 );
		process_databuffer_read(&input_cmd);
		status = METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS12);
	} while ((status & (1 << TC)) == 0);


	return retval;
}

static void emmc_card_csd_write( unsigned int index, unsigned int val)
{
	unsigned int status;
	long int arg;
	uint32_t timeout=0;

	arg = ((val << 8) | (index << 16) | (3 << 24));

	//MMCRequest_t input_cmd;
	input_cmd.cmd=EMMC_CMD6;
	input_cmd.arg=arg;
	input_cmd.response_type=EMMC_RESPONSE_R1B;
	input_cmd.data_present=0;

	emmc_host_send_cmd(&input_cmd);//(EMMC_CMD6 << CI) | (3 << CRCCE) | (3 << RTS), arg);

	do {
		status = METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS12);
		if(timeout > MAX_COUNT) {
			printf("Exit from function \"static void emmc_card_ext_csd_write()\" due to timeout");
			exit(1);
		}
		else
			timeout++;
	} while ((status & (1 << TC)) == 0);
}


static void emmc_card_csd_read(uint8_t *rx_data)
{
	unsigned int status;
	long int arg;
	uint32_t timeout=0;

	printf("Reading eMMC CSD : CMD9\n");

	emmc_select_card(EMMC_RCA);

	input_cmd.cmd=EMMC_CMD9;
	input_cmd.arg=EMMC_RCA<<16;
	input_cmd.response_type=EMMC_RESPONSE_R2;
	input_cmd.data_present=0;

	emmc_host_send_cmd(&input_cmd);

	uint32_t *dataptr=(uint32_t *)rx_data;
	dataptr[0]=input_cmd.cmd_response[0];
	dataptr[1]=input_cmd.cmd_response[1];
	dataptr[2]=input_cmd.cmd_response[2];
	dataptr[3]=input_cmd.cmd_response[3];
}


static void emmc_set_block_size( long int size)
{
	//eMMCRequest_t input_cmd;
	input_cmd.cmd=EMMC_CMD16;
	input_cmd.arg=size;
	input_cmd.response_type=EMMC_RESPONSE_R1;
	input_cmd.data_present=0;

	emmc_host_send_cmd(&input_cmd);//(EMMC_CMD16 << CI) | (2 << CRCCE) | (2 << RTS), size);
}


static int emmc_host_set_bit_width( char bit_width)
{
	unsigned int status, _bit_width;
	long int val;
	_bit_width = _bit_width & BIT_WIDTH_MASK;

	status = METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS10);
	status &= ~((1 << DTW) | (1 << EDTW));

	switch (bit_width) {
		case 0x01: bit_width = 0x00; break;  // bus width 1b
		case 0x04: bit_width = 0x01; status |= (1 << DTW);  break;  // bus width 4b
		case 0x08: bit_width = 0x02; status |= (1 << EDTW) | (1 << DTW); break;  // bus width 8b
		default: /*printf("ERROR: wrong bit width selected \n");*/ return 1;
	}

	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS10) = status;

	val = ((bit_width << 8) | (183 << 16) | (3 << 24));

	//eMMCRequest_t input_cmd;
	input_cmd.cmd=EMMC_CMD6;
	input_cmd.arg=val;
	input_cmd.response_type=EMMC_RESPONSE_R1B;
	input_cmd.data_present=0;

	emmc_host_send_cmd(&input_cmd);//(EMMC_CMD6 << CI) | (3 << CRCCE) | (3 << RTS), val);

	return 0;
}







static void emmc_toggle_sleep()
{
	printf("\n eMMC Sending: CMD5");

	uint32_t arg=0;
	//eMMCRequest_t input_cmd;
	input_cmd.cmd=EMMC_CMD5;
	input_cmd.arg=0;
	input_cmd.response_type=EMMC_RESPONSE_R1;
	input_cmd.data_present=0;

	emmc_host_send_cmd(&input_cmd);


}





//--------------------------------eMMC Boot---------------------------------------------


static int GetPartitionAccessConfig(eMMC_ParitionAccess_t partition, uint8_t* partConfig)
{
    int status =0;
    uint8_t partCfg;

    switch(partition) {
    case EMMC_ACCCESS_NONE:
        partCfg = MMC_EXCSD_BOOTPART_ACCESS_NONE;
        break;
    case EMMC_ACCCESS_BOOT_1:
        partCfg = MMC_EXCSD_BOOTPART_ACCESS_BP_1;
        break;
    case EMMC_ACCCESS_BOOT_2:
        partCfg = MMC_EXCSD_BOOTPART_ACCESS_BP_2;
        break;
    case EMMC_ACCCESS_GENERAL_PURP_1:
        partCfg = MMC_EXCSD_BOOTPART_ACCESS_GP_1;
        break;
    case EMMC_ACCCESS_GENERAL_PURP_2:
        partCfg = MMC_EXCSD_BOOTPART_ACCESS_GP_2;
        break;
    case EMMC_ACCCESS_GENERAL_PURP_3:
        partCfg = MMC_EXCSD_BOOTPART_ACCESS_GP_3;
        break;
    case EMMC_ACCCESS_GENERAL_PURP_4:
        partCfg = MMC_EXCSD_BOOTPART_ACCESS_GP_4;
        break;
    case EMMC_ACCCESS_RPMB:
        partCfg = MMC_EXCSD_BOOTPART_ACCESS_RPMB;
        break;
    default:
        status = -1;
        break;
    }

    if (status == 0) {
        *partConfig = partCfg;
    }

    return (status);
}




static int emmc_set_bootpartition_access(eMMC_Parition_t partitionen,eMMC_ParitionAccess_t access, bool boot_ack)
{
	uint8_t PartConfigAccess=0;
	uint8_t PartConfigEn=0;
	int retval=0;

	emmc_select_card(EMMC_RCA);

	switch(partitionen) {
		case EMMC_PAR_NONE:
			PartConfigEn = MMC_EXCSD_BOOTPART_CFG_BOOT_DISABLE;
			break;
		case EMMC_PAR_BOOT_1:
			PartConfigEn = MMC_EXCSD_BOOTPART_CFG_BOOT1_EN;
			break;
		case EMMC_PAR_BOOT_2:
			PartConfigEn = MMC_EXCSD_BOOTPART_CFG_BOOT2_EN;
			break;
		case EMMC_PAR_USER:
			PartConfigEn = MMC_EXCSD_BOOTPART_CFG_BOOTUSR_EN;
			break;
		default:
			break;
	}

	PartConfigAccess=access;
	
	uint8_t ByteNew = ((boot_ack&1)<<6)| (PartConfigEn)| (PartConfigAccess&7UL) ;

	retval=emmc_card_ext_csd_write(MMC_EXCSD_BOOT_PART_CONFIG,ByteNew);	// Boot partition 1 enable

	emmc_select_card(0);
	return retval;
}

static int emmc_get_partition_access(eMMC_Parition_t *partition,eMMC_ParitionAccess_t *access)
{
	int retval=0;
	
	emmc_select_card(EMMC_RCA);


	retval=emmc_card_ext_csd_read((uint8_t*)g_data_buffer);


	printf("########### BYTE 228 %d\n",GET_BYTE_FROM_BUFFER(g_data_buffer,228));
	uint8_t bootcfg=GET_BYTE_FROM_BUFFER(g_data_buffer,MMC_EXCSD_BOOT_PART_CONFIG);//179

	
	uint8_t boot_partition_en=((bootcfg>>3)&0x7);	
	uint8_t boot_partition_access=(bootcfg&0x7);	

	switch(boot_partition_en) {
		case 0:
			*partition = EMMC_PAR_NONE;
			break;
		case 1:
			*partition = EMMC_PAR_BOOT_1;
			break;
		case 2:
			*partition = EMMC_PAR_BOOT_2;
			break;
		case 7:
			*partition = EMMC_PAR_USER;
			break;
		default:
			break;
	}

	*access=(eMMC_ParitionAccess_t)boot_partition_access;

	emmc_select_card(0);
	return retval;
}




int __metal_driver_sifive_nb2emmc_boot(struct metal_emmc *emmc,uint8_t *rx_data,uint32_t size)
{

	int retval=0;
	unsigned int status=0;
	emmc_control_base=(uintptr_t)__metal_driver_sifive_nb2emmc_base(emmc);

	input_cmd.cmd=EMMC_CMD0;
	input_cmd.arg=GO_PRE_IDLE_STATE;
	input_cmd.response_type=EMMC_RESPONSE_NO_RESP;
	input_cmd.data_present=0;
	retval=emmc_host_send_cmd(&input_cmd);// (EMMC_CMD0 << CI) | (0 << CRCCE) | (0 << RTS), GO_PRE_IDLE_STATE);	// pre-idle state

	//add logic to wait for 74 clock cycles
	for(volatile int i=0;i<0x100;i++);
	if(retval==0)
	{
		input_cmd.cmd=EMMC_CMD0;
		input_cmd.arg=BOOT_INITIATION;
		input_cmd.response_type=EMMC_RESPONSE_NO_RESP;
		input_cmd.data_present=1;
		input_cmd.data_direction=EMMC_TRANSFER_READ;
		input_cmd.dataRemaining=size;
		input_cmd.blockcnt=size/DEVICE_BLOCK_SIZE;
		input_cmd.blocklen=DEVICE_BLOCK_SIZE;
		input_cmd.dataptr=rx_data;
		input_cmd.dataptrpos=rx_data;

		retval=emmc_host_send_cmd(&input_cmd);

		uint32_t *dptr=input_cmd.dataptr;
		if(retval==0)
		{
			do
			{
			// wait for BRE(buffer read enable)
			while(((METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS09) >> BRE) & 0x01) == 0 );
			process_databuffer_read(&input_cmd);
			status = METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS12);
			} while ((status & (1 << TC)) == 0);
		}
		else{

		printf("EMMC:CMD%d Failed status %x\n",input_cmd.cmd,input_cmd.status);
		}
	}else
	{

		printf("EMMC:CMD%d Failed status %x\n",input_cmd.cmd,input_cmd.status);
	}

	return retval;
}


int __metal_driver_sifive_nb2emmc_init(struct metal_emmc *emmc,void *ptr)
{

	int retval=0;
	emmc_control_base=(uintptr_t)__metal_driver_sifive_nb2emmc_base(emmc);

	emmc->deviceblocklen=DEVICE_BLOCK_SIZE;

	/*IOMUX CONFIG FOR EMMC */
	emmc_iomux_config();

	emmc_host_initialization();

	retval=emmc_card_init(1);

	/*
	   input_cmd.cmd=EMMC_CMD0;
	   input_cmd.arg=GO_IDLE_STATE;
	   input_cmd.response_type=EMMC_RESPONSE_NO_RESP;
	   input_cmd.data_present=0;
	   retval=emmc_host_send_cmd(&input_cmd);	//idle state
	   */
#if 0
	//TO Be Remvoed
	for(int i=0;i<128;i++)
		g_data_buffer[i]=0;

	emmc_card_csd_read((uint8_t*)g_data_buffer);
	printf("CSD %x %x %x %x\n",g_data_buffer[0],g_data_buffer[1],g_data_buffer[2],g_data_buffer[3]) ;


	//	emmc_host_set_bit_width(METAL_EMMC_BIT_WIDTH);

	for(int i=0;i<128;i++)
		g_data_buffer[i]=0;

	emmc_card_ext_csd_read((uint8_t*)g_data_buffer);

	for(int i=0;i<128;i++)
		g_data_buffer[i]=0;

	for(int i=0;i<128;i=i+4)
		printf("CSD_EXT %x %x %x %x\n",g_data_buffer[i],g_data_buffer[i+1],g_data_buffer[i+2],g_data_buffer[i+3]) ;
#endif

	return retval;
}



int __metal_driver_sifive_nb2emmc_read_block(struct metal_emmc *emmc, long int addr, const size_t len, char *rx_buff)
{
	int retval=0;
	uint32_t status=0;
	uint32_t timeout1=0;
	uint32_t timeout2=0;

	emmc_select_card(EMMC_RCA);

	METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS01) = emmc->deviceblocklen;


	emmc_set_block_size(emmc->deviceblocklen);

	input_cmd.cmd=EMMC_CMD18;
	input_cmd.arg=addr;
	input_cmd.response_type=EMMC_RESPONSE_R1;
	input_cmd.data_present=1;
	input_cmd.data_direction=EMMC_TRANSFER_READ;
	input_cmd.dataptr=rx_buff;
	input_cmd.dataptrpos=rx_buff;
	input_cmd.dataRemaining=len;
	input_cmd.blocklen=emmc->deviceblocklen;
	input_cmd.blockcnt=len/emmc->deviceblocklen;

	retval=emmc_host_send_cmd(&input_cmd);//((EMMC_CMD17 << CI) | (1 << DPS) | (2 << CRCCE) | (1 << DTDS) | (2 << RTS)), addr);
	if(retval==0)
	{
		do
		{
		// wait for BRE(buffer read enable)
		while(((METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS09) >> BRE) & 0x01) == 0 );
		process_databuffer_read(&input_cmd);
		status = METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS12);
		} while ((status & (1 << TC)) == 0);
	}else{

		printf("EMMC:CMD%d Failed status %x\n",input_cmd.cmd,input_cmd.status);
	}

	emmc_select_card(0);

	return retval;
}

int __metal_driver_sifive_nb2emmc_write_block(struct metal_emmc *emmc,long int addr, const size_t len, char *tx_buff)
{
	int retval=0;
	emmc_control_base=(uintptr_t)__metal_driver_sifive_nb2emmc_base(emmc);
	uint32_t timeout1=0;
	uint32_t timeout2=0;
	unsigned int status=0;

	emmc_select_card(EMMC_RCA);

	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS01) = emmc->deviceblocklen;

	emmc_set_block_size(emmc->deviceblocklen);

	input_cmd.cmd=EMMC_CMD25;
	input_cmd.arg=addr;
	input_cmd.response_type=EMMC_RESPONSE_R1;
	input_cmd.data_direction=EMMC_TRANSFER_WRITE;
	input_cmd.data_present=1;
	input_cmd.dataptr=tx_buff;
	input_cmd.dataptrpos=tx_buff;
	input_cmd.dataRemaining=len;
	input_cmd.blocklen=emmc->deviceblocklen;
	input_cmd.blockcnt=len/emmc->deviceblocklen;

	emmc_host_send_cmd(&input_cmd);
	if(retval==0)
	{
		do
		{
		/*wait for BWE(buffer write enable)*/
		while(((METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS09) >> BWE) & 0x01) == 0 );
		process_databuffer_write(&input_cmd);
		status = METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS12);
		} while ((status & (1 << TC)) == 0);
	}else{

		printf("EMMC:CMD%d Failed status %x\n",input_cmd.cmd,input_cmd.status);
	}

	emmc_select_card(0);

	return retval;
}


int __metal_driver_sifive_nb2emmc_erase_block(struct metal_emmc *emmc, long int start_addr, long int end_addr)
{
	int retval=0;
	input_cmd.cmd=EMMC_CMD35;
	input_cmd.cmd=start_addr;
	input_cmd.response_type=EMMC_RESPONSE_R1;

	emmc_host_send_cmd(&input_cmd);//(EMMC_CMD35 << CI) |  (2 << CRCCE) |  (2 << RTS), start_addr);
	if(retval!=0)
	{
		printf("EMMC:CMD%d Failed status %x\n",input_cmd.cmd,input_cmd.status);
	}else
	{
		input_cmd.cmd=EMMC_CMD36;
		input_cmd.cmd=end_addr;
		input_cmd.response_type=EMMC_RESPONSE_R1;
		emmc_host_send_cmd(&input_cmd);//(EMMC_CMD36 << CI) |  (2 << CRCCE) |  (2 << RTS), end_addr);
		if(retval!=0)
		{
			printf("EMMC:CMD%d Failed status %x\n",input_cmd.cmd,input_cmd.status);

		}else
		{

			input_cmd.cmd=EMMC_CMD38;
			input_cmd.cmd=start_addr;
			input_cmd.response_type=EMMC_RESPONSE_R1B;
			emmc_host_send_cmd(&input_cmd);//(38 << CI) |  (3 << CRCCE) |  (3 << RTS), ERASE_VAL)
			if(retval!=0)
			{
				printf("EMMC:CMD%d Failed status %x\n",input_cmd.cmd,input_cmd.status);
			}
		}
	}
	return retval;
}


int __metal_driver_sifive_nb2emmc_set_partition(struct metal_emmc *emmc,eMMC_Parition_t partition, eMMC_ParitionAccess_t access)
{
	return  emmc_set_bootpartition_access(partition,access,false);
}

int __metal_driver_sifive_nb2emmc_get_partition(struct metal_emmc *emmc,eMMC_Parition_t *partition, eMMC_ParitionAccess_t *access)
{
	emmc_control_base=(uintptr_t)__metal_driver_sifive_nb2emmc_base(emmc);
	return emmc_get_partition_access(partition,access);
}




__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_nb2emmc) = {
	.emmc.boot = __metal_driver_sifive_nb2emmc_boot,
	.emmc.init = __metal_driver_sifive_nb2emmc_init,
	.emmc.read_block = __metal_driver_sifive_nb2emmc_read_block,
	.emmc.write_block = __metal_driver_sifive_nb2emmc_write_block,
	.emmc.erase_block = __metal_driver_sifive_nb2emmc_erase_block,
	.emmc.get_partition=__metal_driver_sifive_nb2emmc_get_partition,
	.emmc.set_partition=__metal_driver_sifive_nb2emmc_set_partition,
};

#endif //SIFIVE_NB2_EMMC

typedef int no_empty_translation_units;
