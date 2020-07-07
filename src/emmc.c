/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/emmc.h>

extern inline int metal_emmc_boot(struct metal_emmc *emmc,eMMC_Parition_t partition,bool boot_ack,uint8_t *rx_data,uint8_t size);
extern inline int metal_emmc_set_partition(struct metal_emmc *emmc,eMMC_Parition_t partition, eMMC_ParitionAccess_t access);
extern inline int metal_emmc_get_partition(struct metal_emmc *emmc,eMMC_Parition_t *partition, eMMC_ParitionAccess_t *access);
extern inline int metal_emmc_get_boot_partition_size(struct metal_emmc *emmc,uint32_t bootpartion,uint32_t *size);
extern inline int metal_emmc_set_boot_write_protect(struct metal_emmc *emmc,uint32_t bootpartion,uint32_t wp);

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
