/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/machine.h>
#include <mee/pmp.h>

extern inline void mee_pmp_init(struct mee_pmp *pmp);
extern inline int mee_pmp_set_region(struct mee_pmp *pmp, unsigned int region, struct mee_pmp_config config, size_t address);
extern inline int mee_pmp_get_region(struct mee_pmp *pmp, unsigned int region, struct mee_pmp_config *config, size_t *address);

struct mee_pmp *mee_pmp_get_device(void)
{
#ifdef __MEE_DT_PMP_HANDLE
    return __MEE_DT_PMP_HANDLE;
#else
    return NULL;
#endif
}

int mee_pmp_lock(struct mee_pmp *pmp, unsigned int region)
{
    struct mee_pmp_config config;
    size_t address;
    int rc = 0;

    rc = mee_pmp_get_region(pmp, region, &config, &address);
    if(rc) {
        return rc;
    }

    if(config.L == MEE_PMP_LOCKED) {
        return 0;
    }

    config.L = MEE_PMP_LOCKED;

    rc = mee_pmp_set_region(pmp, region, config, address);

    return rc;
}


int mee_pmp_set_address(struct mee_pmp *pmp, unsigned int region, size_t address)
{
    struct mee_pmp_config config;
    size_t old_address;
    int rc = 0;

    rc = mee_pmp_get_region(pmp, region, &config, &old_address);
    if(rc) {
        return rc;
    }

    rc = mee_pmp_set_region(pmp, region, config, address);

    return rc;
}

size_t mee_pmp_get_address(struct mee_pmp *pmp, unsigned int region)
{
    struct mee_pmp_config config;
    size_t address = 0;

    mee_pmp_get_region(pmp, region, &config, &address);

    return address;
}


int mee_pmp_set_address_mode(struct mee_pmp *pmp, unsigned int region, enum mee_pmp_address_mode mode)
{
    struct mee_pmp_config config;
    size_t address;
    int rc = 0;

    rc = mee_pmp_get_region(pmp, region, &config, &address);
    if(rc) {
        return rc;
    }

    config.A = mode;

    rc = mee_pmp_set_region(pmp, region, config, address);

    return rc;
}

enum mee_pmp_address_mode mee_pmp_get_address_mode(struct mee_pmp *pmp, unsigned int region)
{
    struct mee_pmp_config config;
    size_t address = 0;

    mee_pmp_get_region(pmp, region, &config, &address);

    return config.A;
}


int mee_pmp_set_executable(struct mee_pmp *pmp, unsigned int region, int X)
{
    struct mee_pmp_config config;
    size_t address;
    int rc = 0;

    rc = mee_pmp_get_region(pmp, region, &config, &address);
    if(rc) {
        return rc;
    }

    config.X = X;

    rc = mee_pmp_set_region(pmp, region, config, address);

    return rc;
}

int mee_pmp_get_executable(struct mee_pmp *pmp, unsigned int region)
{
    struct mee_pmp_config config;
    size_t address = 0;

    mee_pmp_get_region(pmp, region, &config, &address);

    return config.X;
}


int mee_pmp_set_writeable(struct mee_pmp *pmp, unsigned int region, int W)
{
    struct mee_pmp_config config;
    size_t address;
    int rc = 0;

    rc = mee_pmp_get_region(pmp, region, &config, &address);
    if(rc) {
        return rc;
    }

    config.W = W;

    rc = mee_pmp_set_region(pmp, region, config, address);

    return rc;
}

int mee_pmp_get_writeable(struct mee_pmp *pmp, unsigned int region)
{
    struct mee_pmp_config config;
    size_t address = 0;

    mee_pmp_get_region(pmp, region, &config, &address);

    return config.W;
}


int mee_pmp_set_readable(struct mee_pmp *pmp, unsigned int region, int R)
{
    struct mee_pmp_config config;
    size_t address;
    int rc = 0;

    rc = mee_pmp_get_region(pmp, region, &config, &address);
    if(rc) {
        return rc;
    }

    config.R = R;

    rc = mee_pmp_set_region(pmp, region, config, address);

    return rc;
}

int mee_pmp_get_readable(struct mee_pmp *pmp, unsigned int region)
{
    struct mee_pmp_config config;
    size_t address = 0;

    mee_pmp_get_region(pmp, region, &config, &address);

    return config.R;
}

