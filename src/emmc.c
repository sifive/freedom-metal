/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/emmc.h>

extern inline int metal_emmc_boot(struct metal_emmc *emmc);
extern inline int metal_emmc_init(struct metal_emmc *emmc, void *ptr);
extern inline int metal_emmc_read_block(struct metal_emmc *emmc, long int addr, const size_t len, char *rx_buff);
extern inline int metal_emmc_write_block(struct metal_emmc *emmc, long int addr, const size_t len, char *tx_buff);
extern inline int metal_emmc_erase_block(struct metal_emmc *emmc, long start_addr, long int end_addr);

struct metal_emmc *metal_emmc_get_device(const int index)
{
  if (index > __METAL_DT_MAX_EMMC) {
    return NULL;
  }

  return (struct metal_emmc *) __metal_emmc_table[index];
}
