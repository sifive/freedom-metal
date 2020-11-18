/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_SPI0

#include <metal/clock.h>
#include <metal/cpu.h>
#include <metal/drivers/sifive_spi0_regs.h>
#include <metal/io.h>
#include <metal/private/metal_private_sifive_spi0.h>
#include <metal/time.h>

static struct { uint64_t baud_rate; } spi_state[__METAL_DT_NUM_SPIS];

#define get_index(spi) ((spi).__spi_index)

#define SPI_FMT_PROTO_SINGLE 0
#define SPI_FMT_PROTO_DUAL 1
#define SPI_FMT_PROTO_QUAD 2

#define SPI_CSMODE_AUTO 0
#define SPI_CSMODE_HOLD 2
#define SPI_CSMODE_OFF 3

#define SPI_ENDIAN_LSB 1

#define SPI_BITS_PER_FRAME 8

#define SPI_CONTROL_IO 0
#define SPI_CONTROL_MAPPED 1

#define SPI_RXDATA_TIMEOUT 1

static int configure_spi(struct metal_spi spi,
                         struct metal_spi_config *config) {
    SPI_Type *SPI_regs = (SPI_Type *)dt_spi_data[get_index(spi)].base_addr;

    /* Set protocol */
    SPI_regs->FMT &= ~(SPI_FMT_PROTO_Msk);
    switch (config->protocol) {
    case METAL_SPI_SINGLE:
        SPI_regs->FMT |= SPI_FMT_PROTO_SINGLE;
        break;
    case METAL_SPI_DUAL:
        if (config->multi_wire == MULTI_WIRE_ALL)
            SPI_regs->FMT |= SPI_FMT_PROTO_DUAL;
        else
            SPI_regs->FMT |= SPI_FMT_PROTO_SINGLE;
        break;
    case METAL_SPI_QUAD:
        if (config->multi_wire == MULTI_WIRE_ALL)
            SPI_regs->FMT |= SPI_FMT_PROTO_QUAD;
        else
            SPI_regs->FMT |= SPI_FMT_PROTO_SINGLE;
        break;
    default:
        /* Unsupported value */
        return -1;
    }

    /* Set Polarity */
    if (config->polarity) {
        SPI_regs->SCKMODE |= SPI_SCKMODE_SCKMODE_POL_Msk;
    } else {
        SPI_regs->SCKMODE &= ~(SPI_SCKMODE_SCKMODE_POL_Msk);
    }

    /* Set Phase */
    if (config->phase) {
        SPI_regs->SCKMODE |= SPI_SCKMODE_SCKMODE_PHA_Msk;
    } else {
        SPI_regs->SCKMODE &= ~(SPI_SCKMODE_SCKMODE_PHA_Msk);
    }

    /* Set Endianness */
    if (config->little_endian) {
        SPI_regs->FMT |= SPI_FMT_ENDIAN_Msk;
    } else {
        SPI_regs->FMT &= ~(SPI_FMT_ENDIAN_Msk);
    }

    /* Always populate receive FIFO */
    SPI_regs->FMT &= ~(SPI_FMT_IODIR_Msk);

    /* Set CS Active */
    if (config->cs_active_high) {
        SPI_regs->CSDEF = 0;
    } else {
        SPI_regs->CSDEF = 1;
    }

    /* Set frame length */
    if ((SPI_regs->FMT & SPI_FMT_LEN_Msk) !=
        (SPI_BITS_PER_FRAME << SPI_FMT_LEN_Pos)) {
        SPI_regs->FMT &= ~(SPI_FMT_LEN_Msk);
        SPI_regs->FMT |= (SPI_BITS_PER_FRAME << SPI_FMT_LEN_Pos);
    }

    /* Set CS line */
    SPI_regs->CSID = config->csid;

    /* Toggle off memory-mapped SPI flash mode, toggle on programmable IO mode
     * It seems that with this line uncommented, the debugger cannot have access
     * to the chip at all because it assumes the chip is in memory-mapped mode.
     * I have to compile the code with this line commented and launch gdb,
     * reset cores, reset $pc, set *((int *) 0x20004060) = 0, (set the flash
     * interface control register to programmable I/O mode) and then continue
     * Alternative, comment out the "flash" line in openocd.cfg */
    SPI_regs->FCTRL = SPI_CONTROL_IO;

    return 0;
}

static void spi_mode_switch(struct metal_spi spi,
                            struct metal_spi_config *config,
                            unsigned int trans_stage) {
    SPI_Type *SPI_regs = (SPI_Type *)dt_spi_data[get_index(spi)].base_addr;

    if (config->multi_wire == trans_stage) {
        SPI_regs->FMT &= ~(SPI_FMT_PROTO_Msk);
        switch (config->protocol) {
        case METAL_SPI_DUAL:
            SPI_regs->FMT |= SPI_FMT_PROTO_DUAL;
            break;
        case METAL_SPI_QUAD:
            SPI_regs->FMT |= SPI_FMT_PROTO_QUAD;
            break;
        default:
            /* Unsupported value */
            return;
        }
    }
}

int sifive_spi0_transfer(struct metal_spi spi, struct metal_spi_config *config,
                         size_t len, char *tx_buf, char *rx_buf) {
    SPI_Type *SPI_regs = (SPI_Type *)dt_spi_data[get_index(spi)].base_addr;
    int rc = 0;
    size_t i = 0;

    rc = configure_spi(spi, config);
    if (rc != 0) {
        return rc;
    }

    /* Hold the chip select line for all len transferred */
    SPI_regs->CSMODE &= ~(SPI_CSMODE_CSMODE_Msk);
    SPI_regs->CSMODE |= SPI_CSMODE_HOLD;

    uint32_t rxdata;

    /* Declare time_t variables to break out of infinite while loop */
    time_t endwait;

    for (i = 0; i < config->cmd_num; i++) {

        while (SPI_regs->TXDATA & SPI_TXDATA_FULL_Msk) {
            /* wait */
        }

        if (tx_buf) {
            SPI_regs->TXDATA = (tx_buf[i] & SPI_TXDATA_DATA_Msk);
        } else {
            SPI_regs->TXDATA = 0;
        }

        endwait = metal_time() + SPI_RXDATA_TIMEOUT;

        while ((rxdata = SPI_regs->RXDATA) & SPI_RXDATA_EMPTY_Msk) {
            if (metal_time() > endwait) {
                SPI_regs->CSMODE &= ~(SPI_CSMODE_CSMODE_Msk);

                return 1;
            }
        }

        if (rx_buf) {
            rx_buf[i] = (char)(rxdata & SPI_RXDATA_DATA_Msk);
        }
    }

    /* switch to Dual/Quad mode */
    spi_mode_switch(spi, config, MULTI_WIRE_ADDR_DATA);

    /* Send Addr data */
    for (; i < (config->cmd_num + config->addr_num); i++) {

        while (SPI_regs->TXDATA & SPI_TXDATA_FULL_Msk) {
            /* wait */
        }

        if (tx_buf) {
            SPI_regs->TXDATA = (tx_buf[i] & SPI_TXDATA_DATA_Msk);
        } else {
            SPI_regs->TXDATA = 0;
        }

        endwait = metal_time() + SPI_RXDATA_TIMEOUT;

        while ((rxdata = SPI_regs->RXDATA) & SPI_RXDATA_EMPTY_Msk) {
            if (metal_time() > endwait) {
                SPI_regs->CSMODE &= ~(SPI_CSMODE_CSMODE_Msk);

                return 1;
            }
        }

        if (rx_buf) {
            rx_buf[i] = (char)(rxdata & SPI_RXDATA_DATA_Msk);
        }
    }

    /* Send Dummy data */
    for (; i < (config->cmd_num + config->addr_num + config->dummy_num); i++) {

        while (SPI_regs->TXDATA & SPI_TXDATA_FULL_Msk) {
            /* wait */
        }

        if (tx_buf) {
            SPI_regs->TXDATA = (tx_buf[i] & SPI_TXDATA_DATA_Msk);
        } else {
            SPI_regs->TXDATA = 0;
        }

        endwait = metal_time() + SPI_RXDATA_TIMEOUT;

        while ((rxdata = SPI_regs->RXDATA) & SPI_RXDATA_EMPTY_Msk) {
            if (metal_time() > endwait) {
                SPI_regs->CSMODE &= ~(SPI_CSMODE_CSMODE_Msk);

                return 1;
            }
        }
        if (rx_buf) {
            rx_buf[i] = (char)(rxdata & SPI_RXDATA_DATA_Msk);
        }
    }

    /* switch to Dual/Quad mode */
    spi_mode_switch(spi, config, MULTI_WIRE_DATA_ONLY);

    for (; i < len; i++) {
        /* Master send bytes to the slave */

        /* Wait for TXFIFO to not be full */
        while (SPI_regs->TXDATA & SPI_TXDATA_FULL_Msk) {
            /* wait */
        }

        /* Transfer byte by modifying the least significant byte in the TXDATA
         * register */
        if (tx_buf) {
            SPI_regs->TXDATA = (tx_buf[i] & SPI_TXDATA_DATA_Msk);
        } else {
            /* Transfer a 0 byte if the sending buffer is NULL */
            SPI_regs->TXDATA = 0;
        }

        /* Master receives bytes from the RX FIFO */

        /* Wait for RXFIFO to not be empty, but break the nested loops if
         * timeout this timeout method  needs refining, preferably taking into
         * account the device specs */
        endwait = metal_time() + SPI_RXDATA_TIMEOUT;

        while ((rxdata = SPI_regs->RXDATA) & SPI_RXDATA_EMPTY_Msk) {
            if (metal_time() > endwait) {
                /* If timeout, deassert the CS */
                SPI_regs->CSMODE &= ~(SPI_CSMODE_CSMODE_Msk);

                /* If timeout, return error code 1 immediately */
                return 1;
            }
        }

        /* Only store the dequeued byte if the receive_buffer is not NULL */
        if (rx_buf) {
            rx_buf[i] = (char)(rxdata & SPI_RXDATA_DATA_Msk);
        }
    }

    /* On the last byte, set CSMODE to auto so that the chip select transitions
     * back to high The reason that CS pin is not deasserted after transmitting
     * out the byte buffer is timing. The code on the host side likely executes
     * faster than the ability of FIFO to send out bytes. After the host
     * iterates through the array, fifo is likely not cleared yet. If host
     * deasserts the CS pin immediately, the following bytes in the output FIFO
     * will not be sent consecutively.
     * There needs to be a better way to handle this. */
    SPI_regs->CSMODE &= ~(SPI_CSMODE_CSMODE_Msk);

    return 0;
}

int sifive_spi0_get_baud_rate(struct metal_spi spi) {
    return spi_state[get_index(spi)].baud_rate;
}

int sifive_spi0_set_baud_rate(struct metal_spi spi, int baud_rate) {
    SPI_Type *SPI_regs = (SPI_Type *)dt_spi_data[get_index(spi)].base_addr;
    struct metal_clock clock = dt_spi_data[get_index(spi)].clock;

    spi_state[get_index(spi)].baud_rate = baud_rate;

    long clock_rate = metal_clock_get_rate_hz(clock);

    /* Calculate divider */
    long div = (clock_rate / (2 * baud_rate)) - 1;

    if (div > SPI_SCKDIV_SCKDIV_Msk) {
        /* The requested baud rate is lower than we can support at
         * the current clock rate */
        return -1;
    }

    /* Set divider */
    SPI_regs->SCKDIV &= ~SPI_SCKDIV_SCKDIV_Pos;
    /*
     * no needed to make mask div with SPI_SCKDIV_SCKDIV_Msk because
     * div <= SPI_SCKDIV_SCKDIV_Msk (we just test it above)
     */
    SPI_regs->SCKDIV |= div;

    return 0;
}

void _sifive_spi0_pre_rate_change_callback(uint32_t id) {
    if (!spi_state[id].baud_rate)
        return;

    struct metal_spi spi = (struct metal_spi){id};
    SPI_Type *SPI_regs = (SPI_Type *)dt_spi_data[get_index(spi)].base_addr;

    /* Detect when the TXDATA is empty by setting the transmit watermark count
     * to one and waiting until an interrupt is pending (indicating an empty
     * TXFIFO) */
    SPI_regs->TXMARK &= ~(SPI_TXMARK_TXMARK_Msk);
    SPI_regs->TXMARK |= ((1 << SPI_TXMARK_TXMARK_Pos) & SPI_TXMARK_TXMARK_Msk);

    while ((SPI_regs->IP & SPI_IP_TXWM_IP_Msk) == 0) {
        /* wait */
    }
}

void _sifive_spi0_post_rate_change_callback(uint32_t id) {
    if (!spi_state[id].baud_rate)
        return;

    struct metal_spi spi = (struct metal_spi){id};
    uint32_t baud_rate = spi_state[get_index(spi)].baud_rate;
    sifive_spi0_set_baud_rate(spi, baud_rate);
}

void sifive_spi0_init(struct metal_spi spi, int baud_rate) {
    uint32_t index = get_index(spi);

    sifive_spi0_set_baud_rate(spi, baud_rate);

    int has_pinmux = dt_spi_data[index].has_pinmux;
    if (has_pinmux) {
        struct metal_gpio pinmux = dt_spi_data[index].pinmux;
        uint32_t output_sel = dt_spi_data[index].pinmux_output_selector;
        uint32_t source_sel = dt_spi_data[index].pinmux_source_selector;
        metal_gpio_enable_pinmux(pinmux, output_sel, source_sel);
    }
}

#endif /* METAL_SIFIVE_SPI0 */

typedef int no_empty_translation_units;
