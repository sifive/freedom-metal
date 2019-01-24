/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__PMP_H
#define MEE__PMP_H

/*!
 * @file mee/pmp.h
 *
 * @brief API for Configuring Physical Memory Protection on RISC-V Cores
 *
 * The Physical Memory Protection (PMP) interface on RISC-V cores
 * is a form of memory protection unit which allows for a finite number
 * of physical memory regions to be configured with certain access
 * permissions. 
 *
 * Additional information about the use and configuration rules for PMPs
 * can be found by reading the RISC-V Privileged Architecture Specification
 * v1.10.
 */

#include <stddef.h>

struct mee_pmp;

/*!
 * @brief Set of available PMP addressing modes
 */
enum mee_pmp_address_mode {
    /*! @brief Disable the PMP region */
    MEE_PMP_OFF   = 0,
    /*! @brief Use Top-of-Range mode */
    MEE_PMP_TOR   = 1,
    /*! @brief Use naturally-aligned 4-byte region mode */
    MEE_PMP_NA4   = 2,
    /*! @brief Use naturally-aligned power-of-two mode */
    MEE_PMP_NAPOT = 3
};

/*!
 * @brief Configuration for a PMP region
 */
struct mee_pmp_config {
    /*! @brief Sets whether reads to the PMP region succeed */
    int R : 1;
    /*! @brief Sets whether writes to the PMP region succeed */
    int W : 1;
    /*! @brief Sets whether the PMP region is executable */
    int X : 1;

    /*! @brief Sets the addressing mode of the PMP region */
    enum mee_pmp_address_mode A : 2;

    int _pad : 2;

    /*! @brief Sets whether the PMP region is locked */
    enum mee_pmp_locked {
        MEE_PMP_UNLOCKED = 0,
        MEE_PMP_LOCKED   = 1
    } L : 1;
};

/*!
 * @brief A handle for the PMP device
 */
struct mee_pmp {
    /*!
     * @brief The number of regions in the PMP
     */
    const int num_regions;
};

/*!
 * @brief Get the PMP device handle
 */
struct mee_pmp *mee_pmp_get_device(void);

/*!
 * @brief Initialize the PMP
 * @param pmp The PMP device handle to be initialized
 *
 * The PMP initialization routine is optional and may be called as many times
 * as is desired. The effect of the initialization routine is to attempt to set
 * all regions to unlocked and disabled, as well as to clear the X, W, and R
 * bits.
 *
 * If any regions are fused to preset values by the implementation or locked,
 * the PMP region will silently remain uninitialized.
 */
void mee_pmp_init(struct mee_pmp *pmp);

/*!
 * @brief Configure a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to configure
 * @param config The desired configuration of the PMP region
 * @param address The desired address of the PMP region
 * @return 0 upon success
 */
int mee_pmp_set_region(struct mee_pmp *pmp, unsigned int region, struct mee_pmp_config config, size_t address);

/*! 
 * @brief Get the configuration for a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to read
 * @param config Variable to store the PMP region configuration
 * @param address Variable to store the PMP region address
 * @return 0 if the region is read successfully
 */
int mee_pmp_get_region(struct mee_pmp *pmp, unsigned int region, struct mee_pmp_config *config, size_t *address);

/*!
 * @brief Lock a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to lock
 * @return 0 if the region is successfully locked
 */
int mee_pmp_lock(struct mee_pmp *pmp, unsigned int region);

/*!
 * @brief Set the address for a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to set
 * @param address The desired address of the PMP region
 * @return 0 if the address is successfully set
 */
int mee_pmp_set_address(struct mee_pmp *pmp, unsigned int region, size_t address);

/*!
 * @brief Get the address of a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to read
 * @return The address of the PMP region, or 0 if the region could not be read
 */
size_t mee_pmp_get_address(struct mee_pmp *pmp, unsigned int region);

/*!
 * @brief Set the addressing mode of a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to set
 * @param mode The PMP addressing mode to set
 * @return 0 if the addressing mode is successfully set
 */
int mee_pmp_set_address_mode(struct mee_pmp *pmp, unsigned int region, enum mee_pmp_address_mode mode);

/*!
 * @brief Get the addressing mode of a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to read
 * @return The address mode of the PMP region
 */
enum mee_pmp_address_mode mee_pmp_get_address_mode(struct mee_pmp *pmp, unsigned int region);

/*!
 * @brief Set the executable bit for a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to set
 * @param X The desired value of the executable bit
 * @return 0 if the executable bit is successfully set
 */
int mee_pmp_set_executable(struct mee_pmp *pmp, unsigned int region, int X);

/*!
 * @brief Get the executable bit for a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to read
 * @return the value of the executable bit
 */
int mee_pmp_get_executable(struct mee_pmp *pmp, unsigned int region);

/*!
 * @brief Set the writable bit for a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to set
 * @param W The desired value of the writable bit
 * @return 0 if the writable bit is successfully set
 */
int mee_pmp_set_writeable(struct mee_pmp *pmp, unsigned int region, int W);

/*!
 * @brief Get the writable bit for a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to read
 * @return the value of the writable bit
 */
int mee_pmp_get_writeable(struct mee_pmp *pmp, unsigned int region);

/*!
 * @brief Set the readable bit for a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to set
 * @param R The desired value of the readable bit
 * @return 0 if the readable bit is successfully set
 */
int mee_pmp_set_readable(struct mee_pmp *pmp, unsigned int region, int R);

/*!
 * @brief Set the readable bit for a PMP region
 * @param pmp The PMP device handle
 * @param region The PMP region to read
 * @return the value of the readable bit
 */
int mee_pmp_get_readable(struct mee_pmp *pmp, unsigned int region);

#endif
