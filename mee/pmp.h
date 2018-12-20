/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__PMP_H
#define MEE__PMP_H

#include <stddef.h>

struct mee_pmp;

enum mee_pmp_address_mode {
    MEE_PMP_OFF   = 0,
    MEE_PMP_TOR   = 1,
    MEE_PMP_NA4   = 2,
    MEE_PMP_NAPOT = 3
};

struct mee_pmp_config {
    int R : 1;
    int W : 1;
    int X : 1;

    enum mee_pmp_address_mode A : 2;

    int _pad : 2;

    enum {
        MEE_PMP_UNLOCKED = 0,
        MEE_PMP_LOCKED   = 1
    } L : 1;
};

struct mee_pmp {
    const int num_regions;
};

/* Get the PMP device handle */
struct mee_pmp *mee_pmp_get_device(void);

/* Initializes the PMP. */
void mee_pmp_init(struct mee_pmp *pmp);

/* Configure a PMP region */
int mee_pmp_set_region(struct mee_pmp *pmp, unsigned int region, struct mee_pmp_config config, size_t address);

/* Get the configuration for a PMP region */
int mee_pmp_get_region(struct mee_pmp *pmp, unsigned int region, struct mee_pmp_config *config, size_t *address);

/* Get/Set individual features of a PMP region */
int mee_pmp_lock(struct mee_pmp *pmp, unsigned int region);

int mee_pmp_set_address(struct mee_pmp *pmp, unsigned int region, size_t address);
size_t mee_pmp_get_address(struct mee_pmp *pmp, unsigned int region);

int mee_pmp_set_address_mode(struct mee_pmp *pmp, unsigned int region, enum mee_pmp_address_mode mode);
enum mee_pmp_address_mode mee_pmp_get_address_mode(struct mee_pmp *pmp, unsigned int region);

int mee_pmp_set_executable(struct mee_pmp *pmp, unsigned int region, int X);
int mee_pmp_get_executable(struct mee_pmp *pmp, unsigned int region);

int mee_pmp_set_writeable(struct mee_pmp *pmp, unsigned int region, int W);
int mee_pmp_get_writeable(struct mee_pmp *pmp, unsigned int region);

int mee_pmp_set_readable(struct mee_pmp *pmp, unsigned int region, int R);
int mee_pmp_get_readable(struct mee_pmp *pmp, unsigned int region);

#endif
