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

#define SD_HOST_CLK 200000000
#define SD_ADDR_SECTOR_MODE 1
#define DEVICE_BLOCK_SIZE 0x200

#define SWR (1 << 0)
#define EMMC_LEGACY_MODE 0x01
#define ENABLE_FLAGS 0xFFFFFFFF
#define CLEAR_FLAGS 0xFFFFFFFF
#define BOOT_PARTITION_1_ENABLE 3
#define BOOT_PARTITION_ACCESS 0
#define GO_PRE_IDLE_STATE 0xF0F0F0F0
#define BOOT_INITIATION 0xFFFFFFFA
#define ERROR_STATUS_MASK 0xFFFF8000
#define BIT_WIDTH_MASK 0x0F
#define ERASE_VAL 0x00000000

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

#define METAL_EMMC_REG(offset)   (((unsigned long)emmc_control_base + offset))
#define METAL_EMMC_REGB(offset)  (__METAL_ACCESS_ONCE((__metal_io_u8  *)METAL_EMMC_REG(offset)))
#define METAL_EMMC_REGW(offset)  (__METAL_ACCESS_ONCE((__metal_io_u32 *)METAL_EMMC_REG(offset)))

static unsigned long emmc_control_base=0;
static eMMCRequest_t input_cmd;


static void emmc_host_reset()
{
	uint32_t timeout=0;
	METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_HRS00) = SWR ;
	while ((METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_HRS00) & SWR) == 1)  //wait for HRS0.0 (SWR)= 0
	{
		if(timeout > MAX_COUNT){
			printf("Exit from function \"static void emmc_host_reset()\" due to timeout");
			exit(1);
		}
		else
			timeout++;
	}
}

static void emmc_host_set_clock(unsigned int freq)
{
	uint32_t sdclkfs, dtcvval;
	uint32_t timeout=0;
	if(freq) {
		sdclkfs = (SD_HOST_CLK / 2000) / freq;
	} else {
		sdclkfs = 0;
	}

	dtcvval = 0xE;
	// set clock
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS11) = 0;  // disable SDCE
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS11) = (dtcvval << DTCV) | (sdclkfs << SDCFSL) | (1 << ICE); // ICE (internal clock enable)
	while ((METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS11) & (1 << ICS)) == 0) // wait for ICS (internal clock stable)
	{
		if(timeout > MAX_COUNT){
			printf("Exit from function \"static void emmc_host_set_clock()\" due to timeout");
			exit(1);
		}
		else
			timeout++;
	}
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS11) = (dtcvval << DTCV) | (sdclkfs << SDCFSL) | (1 << ICE) | (1 << SDCE); // enable SD clock (SRS11.SDCE)
}


static void emmc_host_initialization()
{
	uint32_t timeout=0;
	printf("\n Doing eMMC Reset");
	// Software reset
	emmc_host_reset();

	// set Host controller to eMMC legacy mode
	METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_HRS06) = EMMC_LEGACY_MODE;

	// sd card detection
	while ((METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS09) & (1 << CIns)) == 0)  //wait for SRS9.CI = 1
		{
			if(timeout > MAX_COUNT){
				printf("Exit from function \"static void emmc_host_initialization()\" due to timeout");
				exit(1);
			}
			else
				timeout++;
		}
	//eMMC card reset
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS10) = (7 << BVS) | (1 << BP); //BVS - SD Bus Voltage Select,  BP - SD Bus Power for VDD1
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS10) = (7 << BVS) | (0 << BP);

	//enable supply voltage
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS10) = (7 << BVS) | (1 << BP);  // BVS = 7, BP = 1, BP2 only in UHS2 mode

	//sarting SDclk
	emmc_host_set_clock( 398);		//clock freq. set

	// enable flags
	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS13) = ENABLE_FLAGS;		//all flags enable
}


static int emmc_host_send_cmd(eMMCRequest_t *input_cmd)
{
	uint32_t cmd=0;
	uint32_t crccet_val=0;
	uint32_t timeout=0;

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
	

	if(input_cmd->response_type==EMMC_RESPONSE_R1B)
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
	}

	printf("Sending eMMC Command CMD%d\n arg=%x",input_cmd->cmd, input_cmd->arg);

	METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS03) = cmd;  // execute command


	do {
		input_cmd->status = METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS12);
			if(timeout > MAX_COUNT) {
			printf("Exit from function \"static int emmc_host_send_cmd()\" due to timeout");
			exit(1);
		}
			else
				timeout++;
	} while ((input_cmd->status & ((1 << CC) | (1 << EINT)) ) == 0); // exit loop when CC (command complete) flag is set to 1

	printf("EMMC:CMD%d complete status %x\n",input_cmd->cmd,input_cmd->status);
	input_cmd->status &= ERROR_STATUS_MASK;

	if(input_cmd->response_type !=EMMC_RESPONSE_NO_RESP)
	{
		input_cmd->cmd_response[0]=METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS04);
		input_cmd->cmd_response[1]=METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS05);
		input_cmd->cmd_response[2]=METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS06);
		input_cmd->cmd_response[3]=METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS07);

	printf("Response received %x %x %x %x\n",input_cmd->cmd_response[0],input_cmd->cmd_response[1],input_cmd->cmd_response[2],input_cmd->cmd_response[3]);
	}

	return input_cmd->status;
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
	
	printf("CID received %x %x %x %x\n",input_cmd.cmd_response[0],input_cmd.cmd_response[1],input_cmd.cmd_response[1],input_cmd.cmd_response[3]);


	uint32_t manufacturerId = (uint8_t)((input_cmd.cmd_response[3] >> 16) & 0xFFU);
        uint32_t oemApplicationId = (uint16_t)(input_cmd.cmd_response[3] & 0xFFFFU);

	uint32_t name[10];
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


	printf("\n manufacturerId %x\n",manufacturerId); 
	printf("\n Name %s\n",name); 
	printf("\n productRevision %x\n",productRevision); 
	printf("\n productSn %x\n",productSn); 
	printf("\n manufacturingDate %x\n",manufacturingDate); 

}


static int emmc_card_init(unsigned int low_voltage_en)
{
	uint32_t timeout=0;
	uint32_t _status;
	uint32_t _count_delay;

	printf("emmc_card_init\n");
	// CMD0 - reset all cards to IDLE state
	//eMMCRequest_t input_cmd;
	
	input_cmd.cmd=EMMC_CMD0;
	input_cmd.arg=0;
	input_cmd.response_type=EMMC_RESPONSE_NO_RESP;
	input_cmd.data_present=0;
	emmc_host_send_cmd(&input_cmd);

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
		emmc_host_send_cmd(&input_cmd);  // voltage and address mode


		//sd_emmc_host_send_cmd( (1 << CI) | (2<<RTS), _status, 0);  // voltage and address mode
		 _status =input_cmd.cmd_response[0];
		if(timeout > MAX_COUNT) {
			printf("Exit from function \"static int emmc_card_init()\" due to timeout");
			exit(1);
		}
		else
			timeout++;
	} while ((_status & 0x80000000) == 0);

	emmc_read_cid();

	// CMD3 - send RCA to eMMC card and go to STBY
	uint32_t sd_card_rca = 0x12;
	input_cmd.cmd=EMMC_CMD3;
	input_cmd.arg=sd_card_rca<<16;
	input_cmd.response_type=EMMC_RESPONSE_R1;
	input_cmd.data_present=0;
	emmc_host_send_cmd(&input_cmd);  

	// -- initialization finished --
	return 0;
}


static void emmc_card_ext_csd_write( unsigned int index, unsigned int val)
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



static int emmc_select_card (uint8_t card_rca)
{
	//eMMCRequest_t input_cmd;
	input_cmd.cmd=EMMC_CMD7;
	if (card_rca == 0x000){

		input_cmd.arg=0;
		input_cmd.response_type=EMMC_RESPONSE_NO_RESP;
		input_cmd.data_present=0;
		emmc_host_send_cmd(&input_cmd);

		// no response from the deselected Card
		//sd_emmc_host_send_cmd((7 << CI) | (0 << CRCCE) | (0 << RTS), 0x0000, 0);
		return 1;
	}
	else{
		//  R1/R1b response from the selected Card
		input_cmd.arg=card_rca << 16;
		input_cmd.response_type=EMMC_RESPONSE_R1;
		printf("card selected with RCA %x \n",card_rca );
	}
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

//-----------------------------------------------------------------------------
static void process_databuffer_read(eMMCRequest_t* request)
{
    uint32_t data_tocopy = calc_data_tocopy(request);
    uint32_t timeout1=0;
    uint32_t timeout2=0;

    request->dataRemaining -= data_tocopy;

    uint8_t *tempdataptr=request->dataptr;

    while(data_tocopy > 0U) 
    {
        uint32_t data = METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS08);
        uint8_t byte_count = 4;
	if(timeout1 > MAX_COUNT) {
		printf("Exit from function \"static void process_databuffer_read()\" due to timeout1");
		exit(1);
	}
	else
		timeout1++;
        while(data_tocopy && byte_count) {
            *((uint8_t*)tempdataptr) = data & 0xFF;
            data >>= 8;
            byte_count--;
            data_tocopy--;
	    tempdataptr++;
	    if(timeout2 > MAX_COUNT) {
		    printf("Exit from function \"static void process_databuffer_read()\" due to timeout2");
		    exit(1);
	    }
	    else
		    timeout2++;
        }
    }
}



int __metal_driver_sifive_nb2emmc_boot(struct metal_emmc *emmc)
{
	emmc_control_base=(uintptr_t)__metal_driver_sifive_nb2emmc_base(emmc);
	
	return 0;
}


int __metal_driver_sifive_nb2emmc_init(struct metal_emmc *emmc,void *ptr)
{
	
	emmc_control_base=(uintptr_t)__metal_driver_sifive_nb2emmc_base(emmc);

	emmc_host_initialization();


	emmc_card_init(1);

	emmc_select_card (0x12);
		
	//emmc_host_set_bit_width(4);
#if 0	
	emmc_card_ext_csd_write( 179, (1 << BOOT_PARTITION_1_ENABLE) | (1 << BOOT_PARTITION_ACCESS));	// Boot partition 1 enable

	input_cmd.cmd=EMMC_CMD0;
	input_cmd.arg=GO_PRE_IDLE_STATE;
	input_cmd.response_type=EMMC_RESPONSE_NO_RESP;
	input_cmd.data_present=0;

	emmc_host_send_cmd(&input_cmd);// (EMMC_CMD0 << CI) | (0 << CRCCE) | (0 << RTS), GO_PRE_IDLE_STATE);		// pre-idle state
	
	input_cmd.cmd=EMMC_CMD0;
	input_cmd.arg=BOOT_INITIATION;
	input_cmd.response_type=EMMC_RESPONSE_NO_RESP;
	input_cmd.data_present=0;

	emmc_host_send_cmd(&input_cmd);// (EMMC_CMD0 << CI) | (0 << CRCCE) | (0 << RTS), BOOT_INITIATION);		// initiate alternative boot operation
#endif
	return 0;
}



int __metal_driver_sifive_nb2emmc_read_block(struct metal_emmc *emmc, long int addr, const size_t len, char *rx_buff)
{
	//uint32_t temp_data[DEVICE_BLOCK_SIZE / 4];

	uint32_t status;
	uint32_t timeout1=0;
	uint32_t timeout2=0;

	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS01) = DEVICE_BLOCK_SIZE;

	emmc_set_block_size(DEVICE_BLOCK_SIZE);


	//eMMCRequest_t input_cmd;
	input_cmd.cmd=EMMC_CMD17;
	input_cmd.arg=addr;
	input_cmd.response_type=EMMC_RESPONSE_R1;
	input_cmd.data_present=1;
	input_cmd.data_direction=EMMC_TRANSFER_READ;

	emmc_host_send_cmd(&input_cmd);//((EMMC_CMD17 << CI) | (1 << DPS) | (2 << CRCCE) | (1 << DTDS) | (2 << RTS)), addr);

	while( ((METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS09) >> BRE) & 0x01) == 0 ) // wait for BRE(buffer read enable)
	{
		if(timeout1 > MAX_COUNT) {
			printf("Exit from function \"__metal_driver_sifive_nb2emmc_read_block()\" due to timeout1");
			exit(1);
		}
		else
			timeout1++;
	}

		input_cmd.dataptr=rx_buff;
		input_cmd.dataRemaining=len;
		input_cmd.blocklen=DEVICE_BLOCK_SIZE;

		process_databuffer_read(&input_cmd);

		do
		{
			status = METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS12);
			if(timeout2 > MAX_COUNT) {
				printf("Exit from function \"__metal_driver_sifive_nb2emmc_read_block()\" due to timeout2");
				exit(1);
			}
			else
				timeout2++;
		} while ((status & (1 << TC)) == 0); //to check complition of transmission

	return 0;
}

int __metal_driver_sifive_nb2emmc_write_block(struct metal_emmc *emmc,long int addr, const size_t len, char *tx_buff)
{
	emmc_control_base=(uintptr_t)__metal_driver_sifive_nb2emmc_base(emmc);
	uint32_t timeout1=0;
	uint32_t timeout2=0;
	unsigned int status;

	METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS01) = DEVICE_BLOCK_SIZE;

	emmc_set_block_size( DEVICE_BLOCK_SIZE);


	//eMMCRequest_t input_cmd;
	input_cmd.cmd=EMMC_CMD24;
	input_cmd.cmd=addr;
	input_cmd.response_type=EMMC_RESPONSE_R1;

	//emmc_host_send_cmd( )//((24 << CI) | (1 << DPS) | (2 << CRCCE) | (1 << DTDS) | (2 << RTS)), addr);

	while( ((METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS09) >> BWE) & 0x01) == 0 ) // wait for BWE (buffer write enable)
	{
		if(timeout1 > MAX_COUNT) {
			printf("Exit from function \"__metal_driver_sifive_nb2emmc_write_block()\" due to timeout1");
			exit(1);
		}
		else
			timeout1++;
	}
	for (int i = 0; i < DEVICE_BLOCK_SIZE / 4; i=i+4){
		METAL_EMMC_REGW(METAL_SIFIVE_NB2EMMC_SRS08) = (uint32_t)tx_buff[i];
	}

	do {
		status = METAL_EMMC_REGW( METAL_SIFIVE_NB2EMMC_SRS12);
		if(timeout2 > MAX_COUNT) {
			printf("Exit from function \"__metal_driver_sifive_nb2emmc_write_block()\" due to timeout2");
			exit(1);
		}
		else
			timeout2++;
	} while ((status & (1 << TC)) == 0);

	return 0;
}


int __metal_driver_sifive_nb2emmc_erase_block(struct metal_emmc *emmc, long int start_addr, long int end_addr)
{

	//eMMCRequest_t input_cmd;
	
	input_cmd.cmd=EMMC_CMD35;
	input_cmd.cmd=start_addr;
	input_cmd.response_type=EMMC_RESPONSE_R1;

	emmc_host_send_cmd(&input_cmd);//(EMMC_CMD35 << CI) |  (2 << CRCCE) |  (2 << RTS), start_addr);
	
	input_cmd.cmd=EMMC_CMD36;
	input_cmd.cmd=end_addr;
	input_cmd.response_type=EMMC_RESPONSE_R1;
	
	emmc_host_send_cmd(&input_cmd);//(EMMC_CMD36 << CI) |  (2 << CRCCE) |  (2 << RTS), end_addr);
	
	input_cmd.cmd=EMMC_CMD38;
	input_cmd.cmd=start_addr;
	input_cmd.response_type=EMMC_RESPONSE_R1B;
	
	emmc_host_send_cmd(&input_cmd);//(38 << CI) |  (3 << CRCCE) |  (3 << RTS), ERASE_VAL);

	return 0;
}


__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_nb2emmc) = {
	.emmc.boot = __metal_driver_sifive_nb2emmc_boot,
	.emmc.init = __metal_driver_sifive_nb2emmc_init,
	.emmc.read_block = __metal_driver_sifive_nb2emmc_read_block,
	.emmc.write_block = __metal_driver_sifive_nb2emmc_write_block,
	.emmc.erase_block = __metal_driver_sifive_nb2emmc_erase_block,
};

#endif //SIFIVE_NB2_EMMC

typedef int no_empty_translation_units;
