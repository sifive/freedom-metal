/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */


#include <metal/machine.h>
#include <metal/flash.h>

extern __inline__ int metal_flash_init(struct metal_flash *flash,void *ptr);
extern __inline__ int metal_flash_read(struct metal_flash *flash, unsigned int addr, const size_t size, char *rx_buff);
extern __inline__ int metal_flash_write(struct metal_flash *flash, unsigned int addr, const size_t size, char *tx_buff);
extern __inline__ int metal_flash_erase(struct metal_flash *flash, unsigned int addr, const size_t size);
extern __inline__ int metal_flash_write_protect(struct metal_flash *flash, char* write_protect);
extern __inline__ int metal_flash_get_device_id(struct metal_flash *flash, unsigned int *pdevice_id);

struct metal_flash *metal_flash_get_device(const int device_num)
{
#if __METAL_DT_MAX_FLASH > 0
    if (device_num < __METAL_DT_MAX_FLASH) {
        return (struct metal_flash *) __metal_flash_table[device_num];
    }
#endif

    return NULL;
}



