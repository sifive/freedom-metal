/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/prci.h>

extern __inline__ unsigned long metal_prci_get_reg(struct metal_prci *prci,
                                                   unsigned long offset);
extern __inline__ unsigned long metal_prci_set_reg(struct metal_prci *prci,
                                                   unsigned long offset,
                                                   unsigned long value);

struct metal_prci *metal_prci_get_device(void) {
#ifdef __METAL_DT_SIFIVE_PRCI0_HANDLE
    return (struct metal_prci *)__METAL_DT_SIFIVE_PRCI0_HANDLE;
#else
    return NULL;
#endif
}
