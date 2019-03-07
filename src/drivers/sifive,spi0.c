/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/drivers/sifive,spi0.h>
#include <metal/io.h>

/* Register byte offsets */
#define METAL_SPI_REG_SCKDIV  0x000 
#define METAL_SPI_REG_SCKMODE 0x004 
#define METAL_SPI_REG_CSID    0x010
#define METAL_SPI_REG_CSDEF   0x014
#define METAL_SPI_REG_CSMODE  0x018
#define METAL_SPI_REG_DELAY0  0x028
#define METAL_SPI_REG_DELAY1  0x02C
#define METAL_SPI_REG_FMT     0x040
#define METAL_SPI_REG_TXDATA  0x048
#define METAL_SPI_REG_RXDATA  0x04C
#define METAL_SPI_REG_TXMARK  0x050
#define METAL_SPI_REG_RXMARK  0x054
#define METAL_SPI_REG_FCTRL   0x060
#define METAL_SPI_REG_FFMT    0x064
#define METAL_SPI_REG_IE      0x070
#define METAL_SPI_REG_IP      0x074

/* Register fields */
#define METAL_SPI_SCKDIV_MASK         0xFFF

#define METAL_SPI_SCKMODE_PHA_SHIFT   0
#define METAL_SPI_SCKMODE_POL_SHIFT   1

#define METAL_SPI_CSMODE_MASK         0x3
#define METAL_SPI_CSMODE_AUTO         0
#define METAL_SPI_CSMODE_HOLD         2
#define METAL_SPI_CSMODE_OFF          3

#define METAL_SPI_PROTO_MASK          0x3
#define METAL_SPI_PROTO_SINGLE        0
#define METAL_SPI_PROTO_DUAL          1
#define METAL_SPI_PROTO_QUAD          2

#define METAL_SPI_ENDIAN_LSB          (1 << 2)

#define METAL_SPI_DISABLE_RX          (1 << 3)

#define METAL_SPI_FRAME_LEN_SHIFT     16
#define METAL_SPI_FRAME_LEN_MASK      (0x1F << METAL_SPI_FRAME_LEN_SHIFT)

#define METAL_SPI_TXDATA_FULL         (1 << 31)
#define METAL_SPI_RXDATA_EMPTY        (1 << 31)
#define METAL_SPI_TXMARK_MASK         0x3
#define METAL_SPI_TXWM                (1 << 0)

#define METAL_SPI_REG(offset)   (((unsigned long)(((struct __metal_driver_sifive_spi0 *)(spi))->control_base) + offset))
#define METAL_SPI_REGB(offset)  (__METAL_ACCESS_ONCE((__metal_io_u8  *)METAL_SPI_REG(offset)))
#define METAL_SPI_REGW(offset)  (__METAL_ACCESS_ONCE((__metal_io_u32 *)METAL_SPI_REG(offset)))

static int configure_spi(struct __metal_driver_sifive_spi0 *spi, struct metal_spi_config *config)
{
    /* Set protocol */
    METAL_SPI_REGW(METAL_SPI_REG_FMT) &= ~(METAL_SPI_PROTO_MASK);
    switch(config->protocol) {
    case METAL_SPI_SINGLE:
        METAL_SPI_REGW(METAL_SPI_REG_FMT) |= METAL_SPI_PROTO_SINGLE;
        break;
    case METAL_SPI_DUAL:
        METAL_SPI_REGW(METAL_SPI_REG_FMT) |= METAL_SPI_PROTO_DUAL;
        break;
    case METAL_SPI_QUAD:
        METAL_SPI_REGW(METAL_SPI_REG_FMT) |= METAL_SPI_PROTO_QUAD;
        break;
    default:
        /* Unsupported value */
        return -1;
    }

    /* Set Polarity */
    if(config->polarity) {
        METAL_SPI_REGW(METAL_SPI_REG_SCKMODE) |= (1 << METAL_SPI_SCKMODE_PHA_SHIFT);
    } else {
        METAL_SPI_REGW(METAL_SPI_REG_SCKMODE) &= ~(1 << METAL_SPI_SCKMODE_PHA_SHIFT);
    }

    /* Set Phase */
    if(config->phase) {
        METAL_SPI_REGW(METAL_SPI_REG_SCKMODE) |= (1 << METAL_SPI_SCKMODE_POL_SHIFT);
    } else {
        METAL_SPI_REGW(METAL_SPI_REG_SCKMODE) &= ~(1 << METAL_SPI_SCKMODE_POL_SHIFT);
    }

    /* Set Endianness */
    if(config->little_endian) {
        METAL_SPI_REGW(METAL_SPI_REG_FMT) |= (1 << METAL_SPI_ENDIAN_LSB);
    } else {
        METAL_SPI_REGW(METAL_SPI_REG_FMT) &= ~(1 << METAL_SPI_ENDIAN_LSB);
    }

    /* Always populate receive FIFO */
    METAL_SPI_REGW(METAL_SPI_REG_FMT) &= ~(1 << METAL_SPI_DISABLE_RX);

    /* Set CS Active */
    if(config->cs_active_high) {
        METAL_SPI_REGW(METAL_SPI_REG_CSDEF) = 0;
    } else {
        METAL_SPI_REGW(METAL_SPI_REG_CSDEF) = 1;
    }

    /* Set frame length */
    if((METAL_SPI_REGW(METAL_SPI_REG_FMT) & METAL_SPI_FRAME_LEN_MASK) != (8 << METAL_SPI_FRAME_LEN_SHIFT)) {
        METAL_SPI_REGW(METAL_SPI_REG_FMT) &= ~(METAL_SPI_FRAME_LEN_MASK);
        METAL_SPI_REGW(METAL_SPI_REG_FMT) |= (8 << METAL_SPI_FRAME_LEN_SHIFT);
    }

    /* Set CS line */
    METAL_SPI_REGW(METAL_SPI_REG_CSID) = config->csid;

    return 0;
}

int __metal_driver_sifive_spi0_transfer(struct metal_spi *gspi,
                                      struct metal_spi_config *config,
                                      size_t len,
                                      char *tx_buf,
                                      char *rx_buf)
{
    struct __metal_driver_sifive_spi0 *spi = (void *)gspi;
    int rc = 0;
    int rxdata = 0;

    rc = configure_spi(spi, config);
    if(rc != 0) {
        return rc;
    }

    /* Hold the chip select line for all len transferred */
    METAL_SPI_REGW(METAL_SPI_REG_CSMODE) = METAL_SPI_CSMODE_HOLD;

    for(int i = 0; i < len; i++) {
        /* Wait for TXFIFO to not be full */
        while(METAL_SPI_REGW(METAL_SPI_REG_TXDATA) & METAL_SPI_TXDATA_FULL) ;

        /* Transfer byte */
        if(tx_buf) {
            METAL_SPI_REGW(METAL_SPI_REG_TXDATA) = tx_buf[i];
        } else {
            METAL_SPI_REGW(METAL_SPI_REG_TXDATA) = 0;
        }

        if(i == (len - 1)) {
            /* On the last byte, set CSMODE to auto so that the chip select
             * transitions back to high */
            METAL_SPI_REGW(METAL_SPI_REG_CSMODE) = METAL_SPI_CSMODE_AUTO;
        }

        /* Wait for RXFIFO to not be empty */
        while((rxdata = METAL_SPI_REGW(METAL_SPI_REG_RXDATA)) & METAL_SPI_RXDATA_EMPTY) ;

        if(rx_buf) {
            rx_buf[i] = (char) (rxdata & 0xFF);
        }
    }

    return 0;
}

int __metal_driver_sifive_spi0_get_baud_rate(struct metal_spi *gspi)
{
    struct __metal_driver_sifive_spi0 *spi = (void *)gspi;
    return spi->baud_rate;
}

int __metal_driver_sifive_spi0_set_baud_rate(struct metal_spi *gspi, int baud_rate)
{
    struct __metal_driver_sifive_spi0 *spi = (void *)gspi;

    spi->baud_rate = baud_rate;

    if (spi->clock != NULL) {
        long clock_rate = spi->clock->vtable->get_rate_hz(spi->clock);

        /* Calculate divider */
        long div = (clock_rate / (2 * baud_rate)) - 1;

        if(div > METAL_SPI_SCKDIV_MASK) {
            /* The requested baud rate is lower than we can support at
             * the current clock rate */
            return -1;
        }

        /* Set divider */
        METAL_SPI_REGW(METAL_SPI_REG_SCKDIV) &= ~METAL_SPI_SCKDIV_MASK;
        METAL_SPI_REGW(METAL_SPI_REG_SCKDIV) |= (div & METAL_SPI_SCKDIV_MASK);
    }
    return 0;
}

static void pre_rate_change_callback(void *priv)
{
    struct __metal_driver_sifive_spi0 *spi = priv;

    /* Detect when the TXDATA is empty by setting the transmit watermark count
     * to zero and waiting until an interrupt is pending */

    METAL_SPI_REGW(METAL_SPI_REG_TXMARK) &= ~(METAL_SPI_TXMARK_MASK);

    while((METAL_SPI_REGW(METAL_SPI_REG_IP) & METAL_SPI_TXWM) == 0) ;
}

static void post_rate_change_callback(void *priv)
{
    struct __metal_driver_sifive_spi0 *spi = priv;
    metal_spi_set_baud_rate(&spi->spi, spi->baud_rate);
}

void __metal_driver_sifive_spi0_init(struct metal_spi *gspi, int baud_rate)
{
    struct __metal_driver_sifive_spi0 *spi = (void *)(gspi);

    if(spi->clock != NULL) {
        metal_clock_register_pre_rate_change_callback(spi->clock, &pre_rate_change_callback, spi);
        metal_clock_register_post_rate_change_callback(spi->clock, &post_rate_change_callback, spi);
    }

    metal_spi_set_baud_rate(&(spi->spi), baud_rate);

    if (spi->pinmux != NULL) {
        spi->pinmux->vtable->gpio.enable_io(
            (struct metal_gpio *) spi->pinmux,
            spi->pinmux_output_selector,
            spi->pinmux_source_selector
        );
    }
}
