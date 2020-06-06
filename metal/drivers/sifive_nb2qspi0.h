/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_NB2QSPI_H
#define METAL__DRIVERS__SIFIVE_NB2QSPI_H

#include <metal/io.h>
#include <metal/compiler.h>
#include <metal/clock.h>
#include <metal/interrupt.h>
#include <metal/qspi.h>


/*Masks to use with qspi_clr/get_interrupt_stat */
/* QSPI transmit burst complete interrupt mask */
#define QSPI_INTR_MASK_TX_BURST_OK		(1UL<<5)
/* APB transmit complete interrupt mask */
#define QSPI_INTR_MASK_APB_TX_OK		(1UL<<4)
/* APB receive complete interrupt mask */
#define QSPI_INTR_MASK_APB_RX_OK		(1UL<<3)
/* AXI Transmit FIFO empty interrupt mask */
#define QSPI_INTR_MASK_AXI_TX_FIFO_EMPTY	(1UL<<2)
/* AXI Receive FIFO Full interrupt mask */
#define QSPI_INTR_MASK_AXI_RX_FIFO_FULL		(1UL<<1)
/* QSPI Interrupt clear*/
#define QSPI_INTR_MASK_INTERRUPT_CLR		(1UL<<0)

/*Masks to use with qspi_set_custom_cmd */
/* Enables the Tx data transmit phase */
#define QSPI_TX_DATA_PH_EN      (1UL<<6)
/* Enables the Rx data transmit phase */
#define QSPI_RX_DATA_PH_EN      (1UL<<5)
/* Enables DLP phase in the operation */
#define QSPI_DLP_PH_EN          (1UL<<4)
/* Enables dummy phase in the operation */
#define QSPI_DUMMY_PH_EN        (1UL<<3)
/* Enables mode bits phase in the operation */
#define QSPI_MODE_BITS_PH_EN    (1UL<<2)
/* Enables address phase in the operation */
#define QSPI_ADDR_PH_EN         (1UL<<1)
/* Enables the opcode phase in the operation */
#define QSPI_OPCODE_PH_EN       (1UL<<0)





/* APIs to access QSPI controller registers */
#define QSPI_SET_DUMMY_DLP(dlp_count,dummy_count,dummy_pattern)\
	(((dlp_count & 0x0F)<<12)  | ((dummy_count & 0x0F)<<8) | (dummy_pattern & 0xFF));


#define QSPI_SET_MODE_REG(mode_count, mode_pattern)\
	(((mode_count & 0x07)<<8) | (mode_pattern & 0xFF));


#define QSPI_SET_BURST_CTRL_FLAGS(sckflag,txflag)\
	((sckflag & 0x01) <<1) | (txflag & 0x01);



/*! @brief Enums to transmitted address */
typedef enum {
    QSPI_ADDR_24BIT = 0, QSPI_ADDR_32BIT = 1,
} qspi_addr_24_32_t;

/*! @brief Enums of tx and rx data length */
typedef enum {
    /* In AXI mode, length must be set to 32bit only */
    QSPI_8BIT = 0, QSPI_16BIT = 1, QSPI_24BIT = 2, QSPI_32BIT = 3,
} qspi_length;

/*! @brief Enums to select active chip */
typedef enum {
    QSPI_NO_DEVICE = 0,
    QSPI_DEVICE_0 = 1,
    QSPI_DEVICE_1 = 2,
    QSPI_DEVICE_BOTH = 3,
/* 'QSPI_DEVICE_BOTH' is valid only for write operations */
} qspi_cs_i;

/*! @brief Enums to select MSB/LSB */
typedef enum {
    QSPI_MSB_FIRST = 0, QSPI_LSB_FIRST = 1,
} qspi_bit_align;

/*! @brief Enums to select spi mode */
typedef enum {
    QSPI_SPI = 0, QSPI_DUAL = 1, QSPI_QUAD = 2, QSPI_QUAD_DDR = 3,
} qspi_opmode;

/*! @brief Enums to select speed mode */
typedef enum {
    QSPI_CMD_ADDR_SERIAL = 0, QSPI_CMD_SERIAL = 1, QSPI_NONE_SERIAL = 2,
} qspi_speedmode;

/*! @brief Enums to select write protection enable/disable */
typedef enum {
    QSPI_WRITE_PROT_EN = 0, QSPI_WRITE_PROT_DIS = 1,
} qspi_write_prot;

/*! @brief Enums to select hold enable/disable */
typedef enum {
    QSPI_HOLD_EN = 0, QSPI_HOLD_DIS = 1,
} qspi_hold;

/*! @brief Enums to select write/read operation */
typedef enum {
    QSPI_WRITE = 0, QSPI_READ = 1,
} qspi_wrmode;

/*! @brief Enums to select burst operation */
typedef enum {
    QSPI_APB_ACCESS = 0, QSPI_SINGLE_BURST = 2, QSPI_CONTINUOUS_BURST = 3,
} qspi_burstmode;

/*! @brief Enums to select delay count for Rx data */
typedef enum {
    QSPI_0CLK = 0, QSPI_1CLK = 1, QSPI_2CLK = 2, QSPI_3CLK = 3,
} qspi_rxdelay;

/*! @brief Enums to select fine Delay Count for Rx Data */
typedef enum {
    QSPI_0PS = 0, QSPI_100PS = 1, QSPI_200PS = 2, QSPI_300PS = 3,
} qspi_rxfinedelay;

/*! @brief Enums to select enable/disable rx delay half clock */
typedef enum {
    QSPI_RX_DLY_HALFCLK_DIS = 0, QSPI_RX_DLY_HALFCLK_EN = 1,
} qspi_rx_delay_half_clk;

/*! @brief Enums to select enable/disable bypass the phase shift */
typedef enum {
    QSPI_PH_SHIFT_BYPASS_DIS = 0, QSPI_PH_SHIFT_BYPASS_EN = 1,
} qspi_phase_shift_bypass;

/*! @brief Enums to select baud rate */
typedef enum {
    QSPI_DIV_1 = 0,
    QSPI_DIV_2 = 1,
    QSPI_DIV_4 = 2,
    QSPI_DIV_8 = 3,
    QSPI_DIV_16 = 4,
    QSPI_DIV_32 = 5,
    QSPI_DIV_64 = 6,
    QSPI_DIV_128 = 7,
} qspi_clk_div;

/*! @brief Enums to know qspi state machine current state */
typedef enum {
    QSPI_IDLE_STATE = 0,
    QSPI_CMD_STATE = 1,
    QSPI_ADDR_STATE = 2,
    QSPI_MODE_STATE = 3,
    QSPI_DUMMY_STATE = 4,
    QSPI_DLP_STATE = 5,
    QSPI_RXD_STATE = 6,
    QSPI_TXD_STATE = 7,
    QSPI_CFG_ERR = 8,
} qspi_status;

/*! @brief Enums to select sck enable/disable */
typedef enum {
    QSPI_SCK_ENABLE = 0, QSPI_SCK_DISABLE = 1,
} qspi_burst_ctrl_sckflag;

/*! @brief Enums to get current transaction end/continue  */
typedef enum {
    QSPI_CONT_TRANS = 0, QSPI_END_TRANS = 1,
} qspi_burst_ctrl_txflag;

/*! @brief QSPI controller config structure */
struct qspi_config{
    //little endian

    qspi_addr_24_32_t addrlen;
    qspi_length txlen;
    qspi_length rxlen;
    qspi_cs_i cs_i;
    qspi_bit_align bit_align;
    unsigned long clock_polarity;
    qspi_opmode opmode;
    qspi_speedmode speedmode;
    qspi_write_prot write_prot;
    qspi_hold hold;
    qspi_wrmode wrmode;
    qspi_burstmode burstmode;
    qspi_rxdelay rxdelay;
    qspi_rxfinedelay rxfinedelay;
    qspi_rx_delay_half_clk rx_dly_half_clk;
    qspi_phase_shift_bypass rx_phshift_bypass;
    qspi_clk_div clk_div;
};

/*! @brief QSPI controller config structure */
struct qspi_command_param {
	unsigned int custom_command;
	unsigned int dummy_stg;
	unsigned int mode_stg;
	unsigned int opcode;
	unsigned int addr;
};


struct __metal_driver_vtable_sifive_nb2qspi0 {
    const struct metal_qspi_vtable qspi;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_nb2qspi0)

struct __metal_driver_sifive_nb2qspi0 {
    struct metal_qspi qspi;
    unsigned long baud_rate;
    metal_clock_callback pre_rate_change_callback;
    metal_clock_callback post_rate_change_callback;
};

#endif
