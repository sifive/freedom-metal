/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/drivers/riscv,rv32,pmp.h>

#define CONFIG_TO_INT(_config) (*((size_t *) &(_config)))
#define INT_TO_CONFIG(_int) (*((struct mee_pmp_config *) &(_int)))

void __mee_driver_riscv_rv32_pmp_init(struct mee_pmp *gpmp)
{
    struct __mee_driver_riscv_rv32_pmp *pmp = (void *)gpmp;
    
    struct mee_pmp_config init_config = {
        .L = MEE_PMP_UNLOCKED,
        .A = MEE_PMP_OFF,
        .X = 0,
        .W = 0,
        .R = 0,
    };

    for(unsigned int i = 0; i < pmp->num_regions; i++) {
        mee_pmp_set_region(gpmp, i, init_config, 0);
   }
}

int __mee_driver_riscv_rv32_pmp_set_region(struct mee_pmp *gpmp,
                                           unsigned int region,
                                           struct mee_pmp_config config,
                                           size_t address)
{
    struct __mee_driver_riscv_rv32_pmp *pmp = (void *)gpmp;
    struct mee_pmp_config old_config;
    size_t old_address;
    size_t cfgmask;
    size_t pmpcfg;
    int rc = 0;

    if(!pmp) {
        /* Device handle cannot be NULL */
        return 1;
    }

    if(region > pmp->num_regions) {
        /* Region outside of supported range */
        return 2;
    }

    rc = mee_pmp_get_region(gpmp, region, &old_config, &old_address);
    if(rc) {
        /* Error reading region */
        return rc;
    }

    if(old_config.L == MEE_PMP_LOCKED) {
        /* Cannot modify locked region */
        return 3;
    }

    /* Update the address first, because if the region is being locked we won't
     * be able to modify it after we set the config */
    if(old_address != address) {
        switch(region) {
        case 0:
            asm("csrw pmpaddr0, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 1:
            asm("csrw pmpaddr1, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 2:
            asm("csrw pmpaddr2, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 3:
            asm("csrw pmpaddr3, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 4:
            asm("csrw pmpaddr4, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 5:
            asm("csrw pmpaddr5, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 6:
            asm("csrw pmpaddr6, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 7:
            asm("csrw pmpaddr7, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 8:
            asm("csrw pmpaddr8, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 9:
            asm("csrw pmpaddr9, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 10:
            asm("csrw pmpaddr10, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 11:
            asm("csrw pmpaddr11, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 12:
            asm("csrw pmpaddr12, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 13:
            asm("csrw pmpaddr13, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 14:
            asm("csrw pmpaddr14, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        case 15:
            asm("csrw pmpaddr15, %[addr]"
                    :: [addr] "r" (address) :);
            break;
        }
    }

    if(CONFIG_TO_INT(old_config) != CONFIG_TO_INT(config)) {
        /* Mask to clear old pmpcfg */
        cfgmask = (0xFF << (8 * (region % 4)) );
        pmpcfg = (CONFIG_TO_INT(config) << (8 * (region % 4)) );
        
        switch(region / 4) {
        case 0:
            asm("csrc pmpcfg0, %[mask]"
                    :: [mask] "r" (cfgmask) :);

            asm("csrs pmpcfg0, %[cfg]"
                    :: [cfg] "r" (pmpcfg) :);
            break;
        case 1:
            asm("csrc pmpcfg1, %[mask]"
                    :: [mask] "r" (cfgmask) :);

            asm("csrs pmpcfg1, %[cfg]"
                    :: [cfg] "r" (pmpcfg) :);
            break;
        case 2:
            asm("csrc pmpcfg2, %[mask]"
                    :: [mask] "r" (cfgmask) :);

            asm("csrs pmpcfg2, %[cfg]"
                    :: [cfg] "r" (pmpcfg) :);
            break;
        case 3:
            asm("csrc pmpcfg3, %[mask]"
                    :: [mask] "r" (cfgmask) :);

            asm("csrs pmpcfg3, %[cfg]"
                    :: [cfg] "r" (pmpcfg) :);
            break;
        }
    }

    return 0;
}

int __mee_driver_riscv_rv32_pmp_get_region(struct mee_pmp *gpmp,
                                           unsigned int region,
                                           struct mee_pmp_config *config,
                                           size_t *address)
{
    struct __mee_driver_riscv_rv32_pmp *pmp = (void *)gpmp;
    size_t pmpcfg = 0;

    if(!pmp || !config || !address) {
        /* NULL pointers are invalid arguments */
        return 1;
    }

    if(region > pmp->num_regions) {
        /* Region outside of supported range */
        return 2;
    }

    switch(region / 4) {
    case 0:
        asm("csrr %[cfg], pmpcfg0"
                : [cfg] "=r" (pmpcfg) ::);
        break;
    case 1:
        asm("csrr %[cfg], pmpcfg1"
                : [cfg] "=r" (pmpcfg) ::);
        break;
    case 2:
        asm("csrr %[cfg], pmpcfg2"
                : [cfg] "=r" (pmpcfg) ::);
        break;
    case 3:
        asm("csrr %[cfg], pmpcfg3"
                : [cfg] "=r" (pmpcfg) ::);
        break;
    }

    pmpcfg = (0xFF & (pmpcfg >> (8 * (region % 4)) ) );
    *config = INT_TO_CONFIG(pmpcfg);

    switch(region) {
    case 0:
        asm("csrr %[addr], pmpaddr0"
                : [addr] "=r" (*address) ::);
        break;
    case 1:
        asm("csrr %[addr], pmpaddr1"
                : [addr] "=r" (*address) ::);
        break;
    case 2:
        asm("csrr %[addr], pmpaddr2"
                : [addr] "=r" (*address) ::);
        break;
    case 3:
        asm("csrr %[addr], pmpaddr3"
                : [addr] "=r" (*address) ::);
        break;
    case 4:
        asm("csrr %[addr], pmpaddr4"
                : [addr] "=r" (*address) ::);
        break;
    case 5:
        asm("csrr %[addr], pmpaddr5"
                : [addr] "=r" (*address) ::);
        break;
    case 6:
        asm("csrr %[addr], pmpaddr6"
                : [addr] "=r" (*address) ::);
        break;
    case 7:
        asm("csrr %[addr], pmpaddr7"
                : [addr] "=r" (*address) ::);
        break;
    case 8:
        asm("csrr %[addr], pmpaddr8"
                : [addr] "=r" (*address) ::);
        break;
    case 9:
        asm("csrr %[addr], pmpaddr9"
                : [addr] "=r" (*address) ::);
        break;
    case 10:
        asm("csrr %[addr], pmpaddr10"
                : [addr] "=r" (*address) ::);
        break;
    case 11:
        asm("csrr %[addr], pmpaddr11"
                : [addr] "=r" (*address) ::);
        break;
    case 12:
        asm("csrr %[addr], pmpaddr12"
                : [addr] "=r" (*address) ::);
        break;
    case 13:
        asm("csrr %[addr], pmpaddr13"
                : [addr] "=r" (*address) ::);
        break;
    case 14:
        asm("csrr %[addr], pmpaddr14"
                : [addr] "=r" (*address) ::);
        break;
    case 15:
        asm("csrr %[addr], pmpaddr15"
                : [addr] "=r" (*address) ::);
        break;
    }

    return 0;
}

