/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/otp.h>


extern inline int metal_otp_init(struct metal_otp *otp, void *ptr);
extern inline int metal_otp_read(struct metal_otp *otp, long int addr, const size_t len, uint8_t *rx_buff, int bits,uint32_t mask);
extern inline int metal_otp_write(struct metal_otp *otp, long int addr, const size_t len, uint8_t *tx_buff, int bits,uint32_t mask);
extern inline int metal_otp_read_block(struct metal_otp *otp, long int addr, const size_t len, uint8_t *rx_buff, int bits);
extern inline int metal_otp_write_block(struct metal_otp *otp, long int addr, const size_t len, uint8_t *tx_buff, int bits);


struct metal_otp *metal_otp_get_device(const int index)
{
  if (index > __METAL_DT_MAX_EMMC) {
    return NULL;
  }

  return (struct metal_otp *) __metal_otp_table[index];
}
