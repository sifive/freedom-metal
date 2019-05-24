/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_SPI0

#include <metal/drivers/sifive,spi0.h>
#include <metal/io.h>
#include <metal/machine.h>

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

#define METAL_SPI_REG(offset)   (((unsigned long)control_base + offset))
#define METAL_SPI_REGB(offset)  (__METAL_ACCESS_ONCE((__metal_io_u8  *)METAL_SPI_REG(offset)))
#define METAL_SPI_REGW(offset)  (__METAL_ACCESS_ONCE((__metal_io_u32 *)METAL_SPI_REG(offset)))

static int configure_spi(struct __metal_driver_sifive_spi0 *spi, struct metal_spi_config *config)
{
    long control_base = __metal_driver_sifive_spi0_control_base((struct metal_spi *)spi);
    /* Set protocol */
    METAL_SPI_REGW(METAL_SIFIVE_SPI0_FMT) &= ~(METAL_SPI_PROTO_MASK);
    switch(config->protocol) {
    case METAL_SPI_SINGLE:
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_FMT) |= METAL_SPI_PROTO_SINGLE;
        break;
    case METAL_SPI_DUAL:
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_FMT) |= METAL_SPI_PROTO_DUAL;
        break;
    case METAL_SPI_QUAD:
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_FMT) |= METAL_SPI_PROTO_QUAD;
        break;
    default:
        /* Unsupported value */
        return -1;
    }

    /* Set Polarity */
    if(config->polarity) {
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_SCKMODE) |= (1 << METAL_SPI_SCKMODE_PHA_SHIFT);
    } else {
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_SCKMODE) &= ~(1 << METAL_SPI_SCKMODE_PHA_SHIFT);
    }

    /* Set Phase */
    if(config->phase) {
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_SCKMODE) |= (1 << METAL_SPI_SCKMODE_POL_SHIFT);
    } else {
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_SCKMODE) &= ~(1 << METAL_SPI_SCKMODE_POL_SHIFT);
    }

    /* Set Endianness */
    if(config->little_endian) {
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_FMT) |= (1 << METAL_SPI_ENDIAN_LSB);
    } else {
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_FMT) &= ~(1 << METAL_SPI_ENDIAN_LSB);
    }

    /* Always populate receive FIFO */
    METAL_SPI_REGW(METAL_SIFIVE_SPI0_FMT) &= ~(1 << METAL_SPI_DISABLE_RX);

    /* Set CS Active */
    if(config->cs_active_high) {
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_CSDEF) = 0;
    } else {
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_CSDEF) = 1;
    }

    /* Set frame length */
    if((METAL_SPI_REGW(METAL_SIFIVE_SPI0_FMT) & METAL_SPI_FRAME_LEN_MASK) != (8 << METAL_SPI_FRAME_LEN_SHIFT)) {
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_FMT) &= ~(METAL_SPI_FRAME_LEN_MASK);
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_FMT) |= (8 << METAL_SPI_FRAME_LEN_SHIFT);
    }

    /* Set CS line */
    METAL_SPI_REGW(METAL_SIFIVE_SPI0_CSID) = config->csid;

    return 0;
}

int __metal_driver_sifive_spi0_transfer(struct metal_spi *gspi,
                                      struct metal_spi_config *config,
                                      size_t len,
                                      char *tx_buf,
                                      char *rx_buf)
{
    struct __metal_driver_sifive_spi0 *spi = (void *)gspi;
    long control_base = __metal_driver_sifive_spi0_control_base(gspi);
    int rc = 0;
    int rxdata = 0;

    rc = configure_spi(spi, config);
    if(rc != 0) {
        return rc;
    }

    /* Hold the chip select line for all len transferred */
    METAL_SPI_REGW(METAL_SIFIVE_SPI0_CSMODE) = METAL_SPI_CSMODE_HOLD;

    for(int i = 0; i < len; i++) {
        /* Wait for TXFIFO to not be full */
        while(METAL_SPI_REGW(METAL_SIFIVE_SPI0_TXDATA) & METAL_SPI_TXDATA_FULL) ;

        /* Transfer byte */
        if(tx_buf) {
            METAL_SPI_REGW(METAL_SIFIVE_SPI0_TXDATA) = tx_buf[i];
        } else {
            METAL_SPI_REGW(METAL_SIFIVE_SPI0_TXDATA) = 0;
        }

        if(i == (len - 1)) {
            /* On the last byte, set CSMODE to auto so that the chip select
             * transitions back to high */
            METAL_SPI_REGW(METAL_SIFIVE_SPI0_CSMODE) = METAL_SPI_CSMODE_AUTO;
        }

        /* Wait for RXFIFO to not be empty */
        while((rxdata = METAL_SPI_REGW(METAL_SIFIVE_SPI0_RXDATA)) & METAL_SPI_RXDATA_EMPTY) ;

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
    long control_base = __metal_driver_sifive_spi0_control_base(gspi);
    struct metal_clock *clock = __metal_driver_sifive_spi0_clock(gspi);
    struct __metal_driver_sifive_spi0 *spi = (void *)gspi;

    spi->baud_rate = baud_rate;

    if (clock != NULL) {
        long clock_rate = clock->vtable->get_rate_hz(clock);

        /* Calculate divider */
        long div = (clock_rate / (2 * baud_rate)) - 1;

        if(div > METAL_SPI_SCKDIV_MASK) {
            /* The requested baud rate is lower than we can support at
             * the current clock rate */
            return -1;
        }

        /* Set divider */
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_SCKDIV) &= ~METAL_SPI_SCKDIV_MASK;
        METAL_SPI_REGW(METAL_SIFIVE_SPI0_SCKDIV) |= (div & METAL_SPI_SCKDIV_MASK);
    }
    return 0;
}

static void pre_rate_change_callback(void *priv)
{
    long control_base = __metal_driver_sifive_spi0_control_base((struct metal_spi *)priv);

    /* Detect when the TXDATA is empty by setting the transmit watermark count
     * to zero and waiting until an interrupt is pending */

    METAL_SPI_REGW(METAL_SIFIVE_SPI0_TXMARK) &= ~(METAL_SPI_TXMARK_MASK);

    while((METAL_SPI_REGW(METAL_SIFIVE_SPI0_IP) & METAL_SPI_TXWM) == 0) ;
}

static void post_rate_change_callback(void *priv)
{
    struct __metal_driver_sifive_spi0 *spi = priv;
    metal_spi_set_baud_rate(&spi->spi, spi->baud_rate);
}

void __metal_driver_sifive_spi0_init(struct metal_spi *gspi, int baud_rate)
{
    struct __metal_driver_sifive_spi0 *spi = (void *)(gspi);
    struct metal_clock *clock = __metal_driver_sifive_spi0_clock(gspi);
    struct __metal_driver_sifive_gpio0 *pinmux = __metal_driver_sifive_spi0_pinmux(gspi);

    if(clock != NULL) {
        metal_clock_register_pre_rate_change_callback(clock, &pre_rate_change_callback, spi);
        metal_clock_register_post_rate_change_callback(clock, &post_rate_change_callback, spi);
    }

    metal_spi_set_baud_rate(&(spi->spi), baud_rate);

    if (pinmux != NULL) {
        long pinmux_output_selector = __metal_driver_sifive_spi0_pinmux_output_selector(gspi);
        long pinmux_source_selector = __metal_driver_sifive_spi0_pinmux_source_selector(gspi);
        pinmux->gpio.vtable->enable_io(
            (struct metal_gpio *) pinmux,
            pinmux_output_selector,
            pinmux_source_selector
        );
    }
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_spi0) = {
    .spi.init          = __metal_driver_sifive_spi0_init,
    .spi.transfer      = __metal_driver_sifive_spi0_transfer,
    .spi.get_baud_rate = __metal_driver_sifive_spi0_get_baud_rate,
    .spi.set_baud_rate = __metal_driver_sifive_spi0_set_baud_rate,
};
#endif /* METAL_SIFIVE_SPI0 */
