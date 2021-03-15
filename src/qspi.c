/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/qspi.h>

extern __inline__ int metal_qspi_init(struct metal_qspi *qspi, uint32_t baud_rate);
extern __inline__ int metal_qspi_setconfig(struct metal_qspi *qspi,qspi_static_config_t *cfg);
extern __inline__ int metal_qspi_setcommand_params(struct metal_qspi *qspi,qspi_command_config_t *cparams,qspi_static_config_t *cfg);
extern __inline__ int metal_qspi_get_baud_rate(struct metal_qspi *qspi);
extern __inline__ int metal_qspi_set_baud_rate(struct metal_qspi *qspi, uint32_t baud_rate);
extern __inline__ int metal_qspi_read(struct metal_qspi *qspi,uint32_t addr,size_t len,uint8_t *rx_buf);
extern __inline__ int metal_qspi_write(struct metal_qspi *qspi,uint32_t addr,size_t len,uint8_t *tx_buf);
extern __inline__ int metal_qspi_execute_cmd(struct metal_qspi *qspi);

struct metal_qspi *metal_qspi_get_device(unsigned int device_num)
{
#if __METAL_DT_MAX_QSPIS > 0
    if (device_num < __METAL_DT_MAX_QSPIS) {
        return (struct metal_qspi *) __metal_qspi_table[device_num];
    }
#endif

    return NULL;
}
