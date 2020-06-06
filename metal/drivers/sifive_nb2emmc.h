/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_NB2EMMC_H
#define METAL__DRIVERS__SIFIVE_NB2EMMC_H

#include <metal/io.h>
#include <metal/compiler.h>
#include <metal/emmc.h>
#include <metal/clock.h>
#include <metal/interrupt.h>



//---------------------------------------------------------------------------
/// @name Commands definitions
//---------------------------------------------------------------------------
//@{
/// Reset SD card, go to idle state
#define EMMC_CMD0    0U
/// Asks the card, in idle state, to send its Operating Conditions Register contents in the response on the EMMC_CMD line.
#define EMMC_CMD1    1U
/// Asks the card to send its CID number on the EMMC_CMD line
#define EMMC_CMD2    2U
/// Assigns relative address to the card
#define EMMC_CMD3    3U
/// Programs the DSR of the card
#define EMMC_CMD4    4U
/// It is used to inquire about the voltage range needed by the I/O card
#define EMMC_CMD5    5U
/// Switch function command it is used to switch or expand memory card functions
#define EMMC_CMD6    6U
/// Command toggles a card between the stand-by and transfer states or between the programming and disconnect states.
#define EMMC_CMD7    7U
/// Sends SD Memory Card interface condition, which includes host supply voltage information and asks the card whether card supports voltage.
#define EMMC_CMD8    8U
/// Addressed card sends its card-specific data (CSD) on the EMMC_CMD line.
#define EMMC_CMD9    9U
/// Addressed card sends its card identification (CID) on EMMC_CMD the line.
#define EMMC_CMD10   10U
/// Reads data stream from the card, starting at the given address, until a STOP_TRANSMISSION follows. (MMC card only)
#define EMMC_CMD11   11U
/// Forces the card to stop transmission
#define EMMC_CMD12   12U
/// Addressed card sends its status register.
#define EMMC_CMD13   13U
/// A host reads the reversed bus testing data pattern from a card. (MMC card only)
#define EMMC_CMD14   14U
/// Sets the card to inactive state
#define EMMC_CMD15   15U
/// Setss the block length of the SD/EMMC card
#define EMMC_CMD16   16U
/// Reads a block of the size selected by the SET_BLOCKLEN command.
#define EMMC_CMD17   17U
/// Continuously transfers data blocks from card to host until interrupted by a stop command,
#define EMMC_CMD18   18U
/// A host sends the bus test data pattern to a card. (MMC card only)
#define EMMC_CMD19   19U
///  Writes a data stream from the host, starting at the given address, until a STOP_TRANSMISSION follows. (MMC card only)
#define EMMC_CMD20   20U
///  send tuning block (MMC card only)
#define EMMC_CMD21   21U
/// Defines the number of blocks which are going to be transferred in the immediately succeeding multiple block read or write command.(MMC card only)
#define EMMC_CMD23   23U
/// Writes a block of the size selected by the SET_BLOCKLEN command.
#define EMMC_CMD24   24U
/// Continuously writes blocks of data until a STOP_TRANSMISSION follows or the requested number of block received.
#define EMMC_CMD25   25U
/// Programming of the programmable bits of the CSD.
#define EMMC_CMD27   27U
/// If the card has write protection features, this command sets the write protection bit of the addressed group.
#define EMMC_CMD28   28U
/// If the card provides write protection features, this command clears the write protection bit of the addressed group.
#define EMMC_CMD29   29U
/// Ask the card to send the status of the write protection. (If card support write protection)
#define EMMC_CMD30   30U
/// Sets the address of the first write block to be erased.
#define EMMC_CMD32   32U
/// Sets the address of the last write block of the continuous range to be erased.
#define EMMC_CMD33   33U
/// Sets the address of the first write block to be erased for MMC device
#define EMMC_CMD35   35U
/// Sets the address of the last write block of the continuous range to be erased for MMC device
#define EMMC_CMD36   36U
/// Erases all previously selected write blocks.
#define EMMC_CMD38   38U
/// Used to set/reset the password or lock/unlock the card. The size of the data block is set by the SET_BLOCK_LEN command.
#define EMMC_CMD42   42U
/// Access a single register within the total 128K of register space
#define EMMC_CMD52   52U
/// Extended access a single register within the total 128K of register space
#define EMMC_CMD53   53U
/// Commmand informs that the next command is an application specific command rather than a standard command
#define EMMC_CMD55   55U
/// Used either to transfer a data block to the card or to get a data block from the card for general purpose/application specific commands.
#define EMMC_CMD56   56U
/// Read OCR register in SPI mode
#define EMMC_CMD58   58U
/// Turns the CRC option on or off in SPI mode
#define EMMC_CMD59   59U
/// Set the data bus width.
#define EMMC_ACMD6   6U
/// Read SD card status
#define EMMC_ACMD13  13U
/// Command is used to get the number of the written (without errors) write blocks. Responds with 32bit+CRC data block.
#define EMMC_ACMD22  22U
/// Set the number of write blocks to be preerased before writing
#define EMMC_ACMD23  23U
/// Sends host capacity support information (HCS) and asks the accessed card
/// to send its operating condition register (OCR) content in the response on the CMD line.
#define EMMC_ACMD41  41U
/// Connect[1]/Disconnect[0] the 50 KOhm pull-up resistor on CD/DAT3 (pin 1) of the card.
#define EMMC_ACMD42  42U
/// Reads the SD Configuration Register (SCR).
#define EMMC_ACMD51  51U
/// EMMC Command queueing task management
#define EMMC_CMD48   48U
//}@
//

typedef enum
{
    EMMC_RESPONSE_NO_RESP = 0U,
    EMMC_RESPONSE_R2 = 1U,		//136 bits
    EMMC_RESPONSE_R1 = 2U,		//48 bits
    EMMC_RESPONSE_R1B = 3U,		//48 bits
} Emmc_ResponseType;


typedef enum
{
    EMMC_TRANSFER_READ = 1U,
    EMMC_TRANSFER_WRITE = 0U
} eMMC_DataDirection;


/** Structure describes a request which should be executed by host. */
typedef struct eMMCRequest
{
    /** Buffer for command response */
    unsigned int cmd_response[4];

     /** Request type */
    Emmc_ResponseType response_type;
        
    /** Request status */
    unsigned int   status;
    
    unsigned int   busy_checkflags;
    
    
    unsigned int cmd;
    
    unsigned int arg;

    unsigned int data_present;

    eMMC_DataDirection data_direction;

    unsigned int blocklen;

    unsigned int dataRemaining;

    unsigned char  *dataptr;


}eMMCRequest_t;



struct __metal_driver_vtable_sifive_nb2emmc {
    const struct metal_emmc_vtable emmc;
};

struct __metal_driver_sifive_nb2emmc;

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_nb2emmc);

struct __metal_driver_sifive_nb2emmc {
    const struct metal_emmc emmc;
};

#endif
