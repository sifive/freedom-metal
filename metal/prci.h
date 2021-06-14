/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__PRCI_H
#define METAL__PRCI_H

struct metal_prci;

struct metal_prci_vtable {
    unsigned long (*get_reg)(struct metal_prci *prci, unsigned long offset);
    unsigned long (*set_reg)(struct metal_prci *prci, unsigned long offset,
                             unsigned long value);
};

struct metal_prci {
    const struct metal_prci_vtable *vtable;
};

__inline__ unsigned long metal_prci_get_reg(struct metal_prci *prci,
                                            unsigned long offset) {
    return prci->vtable->get_reg(prci, offset);
}

__inline__ unsigned long metal_prci_set_reg(struct metal_prci *prci,
                                            unsigned long offset,
                                            unsigned long value) {
    return prci->vtable->set_reg(prci, offset, value);
}

struct metal_prci *metal_prci_get_device(void);

#endif /* METAL__PRCI_H */
