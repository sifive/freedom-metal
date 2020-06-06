/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__QSPI_H
#define METAL__QSPI_H
#include <metal/interrupt.h>
#include<stdint.h>

struct metal_qspi;
typedef struct qspi_command_param qspi_command_config_t;
typedef struct qspi_config qspi_static_config_t;
/*! @brief The configuration for a SPI transfer */
struct metal_qspi_config {
    /*! @brief The protocol for the SPI transfer */
    enum {
        METAL_QSPI_SINGLE,
        METAL_QSPI_DUAL,
        METAL_QSPI_QUAD
    } protocol;

    /*! @brief The polarity of the SPI transfer, equivalent to CPOL */
    unsigned int polarity : 1;
    /*! @brief The phase of the SPI transfer, equivalent to CPHA */
    unsigned int phase : 1;
    /*! @brief The endianness of the SPI transfer */
    unsigned int little_endian : 1;
    /*! @brief The active state of the chip select line */
    unsigned int cs_active_high : 1;
    /*! @brief The chip select ID to activate for the SPI transfer */
    unsigned int csid;
    /*! @brief The spi command frame number (cycles = num * frame_len) */
    unsigned int cmd_num;
    /*! @brief The spi address frame number */
    unsigned int addr_num;
    /*! @brief The spi dummy frame number */
    unsigned int dummy_num;
    /*! @brief The Dual/Quad spi mode selection.*/

};

struct metal_qspi_vtable {
    void (*init)(struct metal_qspi *qspi, uint32_t baud_rate);
    int (*setconfig)(struct metal_qspi *qspi,qspi_static_config_t *cfg);
    void (*setcommand_params)(struct metal_qspi *qspi,qspi_command_config_t *cparams,qspi_static_config_t *cfg);
    int (*get_baud_rate)(struct metal_qspi *qspi);
    int (*set_baud_rate)(struct metal_qspi *qspi, uint32_t baud_rate);
    int (*read)(struct metal_qspi *qspi,uint32_t addr,size_t len,uint8_t *rx_buf);
    int (*write)(struct metal_qspi *qspi,uint32_t addr,size_t len,uint8_t *tx_buf);
    void (*execute_cmd)(struct metal_qspi *qspi);
};

/*! @brief A handle for a SPI device */
struct metal_qspi {
    const struct metal_qspi_vtable *vtable;
};

/*! @brief Get a handle for a SPI device
 * @param device_num The index of the desired SPI device
 * @return A handle to the SPI device, or NULL if the device does not exist*/
struct metal_qspi *metal_qspi_get_device(unsigned int device_num);

/*! @brief Initialize a SPI device with a certain baud rate
 * @param spi The handle for the SPI device to initialize
 * @param baud_rate The baud rate to set the SPI device to
 */
__inline__ void metal_qspi_init(struct metal_qspi *qspi, uint32_t baud_rate) { qspi->vtable->init(qspi, baud_rate); }


/*! @brief Get the current baud rate of the SPI device
 * @param spi The handle for the SPI device
 * @return The baud rate in Hz
 */
__inline__ int metal_qspi_get_baud_rate(struct metal_qspi *qspi) { return qspi->vtable->get_baud_rate(qspi); }

/*! @brief Set the current baud rate of the SPI device
 * @param spi The handle for the SPI device
 * @param baud_rate The desired baud rate of the SPI device
 * @return 0 if the baud rate is successfully changed
 */
__inline__ int metal_qspi_set_baud_rate(struct metal_qspi *qspi, uint32_t baud_rate) { return qspi->vtable->set_baud_rate(qspi, baud_rate); }

/* @brief Set configuration parameters */
__inline__ int metal_qspi_setconfig(struct metal_qspi *qspi,qspi_static_config_t *cfg) {return qspi->vtable->setconfig(qspi,cfg);}

/* @brief Set the command parameters */
__inline__ void metal_qspi_setcommand_params(struct metal_qspi *qspi,qspi_command_config_t *cparams,qspi_static_config_t *cfg){ qspi->vtable->setcommand_params(qspi,cparams,cfg);}

/* @brief read to block of len starting from addr */
__inline__ int metal_qspi_read(struct metal_qspi *qspi,uint32_t addr,size_t len,uint8_t *rx_buf){return qspi->vtable->read(qspi,addr,len,rx_buf);}

/* @brief write operation to block of len starting from addr */
__inline__ int metal_qspi_write(struct metal_qspi *qspi,uint32_t addr,size_t len,uint8_t *tx_buf){return qspi->vtable->write(qspi,addr,len,tx_buf);}

/* @brief Initiate any APB operation */
__inline__ void metal_qspi_execute_cmd(struct metal_qspi *qspi){ qspi->vtable->execute_cmd(qspi);}


#endif
