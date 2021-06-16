/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_REMAPPER2

#include <metal/drivers/sifive_remapper2.h>
#include <metal/io.h>
#include <metal/machine.h>
#include <stdint.h>
#include <stdio.h>

#define METAL_SIFIVE_REMAPPER2_VALID_REGS_NUM 7

#define METAL_SIFIVE_REMAPPER_ENABLE_KEY 0x51f15e

#define METAL_SIFIVE_REMAPPER2_FROM_REG_OFFSET(idx)                            \
    (METAL_SIFIVE_REMAPPER2_FROM_BASE + idx * 16)
#define METAL_SIFIVE_REMAPPER2_TO_REG_OFFSET(idx)                              \
    (METAL_SIFIVE_REMAPPER2_FROM_BASE + idx * 16 + 8)

/* Macros to access registers */
#define REMAPPER_REG(offset) (((unsigned long)base + offset))
#define REMAPPER_REGW(offset)                                                  \
    (__METAL_ACCESS_ONCE((__metal_io_u32 *)REMAPPER_REG(offset)))

static void
__metal_driver_sifive_remapper_set_key_state(struct metal_remapper *remapper) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_KEY) =
        METAL_SIFIVE_REMAPPER_ENABLE_KEY;
}

int __metal_driver_sifive_remapper_enable_remap(struct metal_remapper *remapper,
                                                int idx) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    int reg_idx = idx / 32;
    int bit_offset = idx % 32;

    __metal_driver_sifive_remapper_set_key_state(remapper);
    REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_VALID_BASE + reg_idx * 4) |=
        (1 << bit_offset);
    __METAL_IO_FENCE(o, rw);

    return 0;
}

int __metal_driver_sifive_remapper_disable_remap(
    struct metal_remapper *remapper, int idx) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    int reg_idx = idx / 32;
    int bit_offset = idx % 32;

    __metal_driver_sifive_remapper_set_key_state(remapper);
    REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_VALID_BASE + reg_idx * 4) &=
        ~(1 << bit_offset);
    __METAL_IO_FENCE(o, rw);

    return 0;
}

int __metal_driver_sifive_remapper_enable_remaps(
    struct metal_remapper *remapper, int idxs[], int num_idxs) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    uint32_t enables[METAL_SIFIVE_REMAPPER2_VALID_REGS_NUM] = {0};

    for (int i = 0; i < num_idxs; i++) {
        int reg_idx = idxs[i] / 32;
        int bit_offset = idxs[i] % 32;
        enables[reg_idx] |= (1 << bit_offset);
    }

    for (int i = 0; i < METAL_SIFIVE_REMAPPER2_VALID_REGS_NUM; i++) {
        uint32_t remappervalid =
            REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_VALID_BASE + i * 4);
        __metal_driver_sifive_remapper_set_key_state(remapper);
        REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_VALID_BASE + i * 4) =
            remappervalid | enables[i];
    }

    return 0;
}

int __metal_driver_sifive_remapper_disable_remaps(
    struct metal_remapper *remapper, int idxs[], int num_idxs) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    uint32_t disables[METAL_SIFIVE_REMAPPER2_VALID_REGS_NUM] = {0};

    for (int i = 0; i < num_idxs; i++) {
        int reg_idx = idxs[i] / 32;
        int bit_offset = idxs[i] % 32;
        disables[reg_idx] |= (1 << bit_offset);
    }

    for (int i = 0; i < METAL_SIFIVE_REMAPPER2_VALID_REGS_NUM; i++) {
        uint32_t remappervalid =
            REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_VALID_BASE + i * 4);
        __metal_driver_sifive_remapper_set_key_state(remapper);
        REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_VALID_BASE + i * 4) =
            remappervalid & ~disables[i];
    }

    return 0;
}

uint32_t
__metal_driver_sifive_remapper_get_valid(struct metal_remapper *remapper,
                                         int idx) {
    if (idx < 0 || idx >= METAL_SIFIVE_REMAPPER2_VALID_REGS_NUM) {
        return 0;
    }

    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    return REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_VALID_BASE + idx * 4);
}

int __metal_driver_sifive_remapper_set_valid(struct metal_remapper *remapper,
                                             int idx, uint32_t val) {
    if (idx < 0 || idx >= METAL_SIFIVE_REMAPPER2_VALID_REGS_NUM) {
        return 1;
    }

    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    __metal_driver_sifive_remapper_set_key_state(remapper);
    REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_VALID_BASE + idx * 4) = val;

    return 0;
}

int __metal_driver_sifive_remapper_flush(struct metal_remapper *remapper) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    __metal_driver_sifive_remapper_set_key_state(remapper);
    REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_FLUSH) = 1;
    __METAL_IO_FENCE(o, rw);

    return 0;
}

uint64_t __metal_driver_sifive_remapper_get_from_region_base(
    struct metal_remapper *remapper) {
    return __metal_driver_sifive_remapper2_from_region_base(remapper);
}

uint64_t __metal_driver_sifive_remapper_get_from_region_size(
    struct metal_remapper *remapper) {
    return __metal_driver_sifive_remapper2_from_region_size(remapper);
}

uint64_t __metal_driver_sifive_remapper_get_to_region_base(
    struct metal_remapper *remapper) {
    return __metal_driver_sifive_remapper2_to_region_base(remapper);
}

uint64_t __metal_driver_sifive_remapper_get_to_region_size(
    struct metal_remapper *remapper) {
    return __metal_driver_sifive_remapper2_to_region_size(remapper);
}

uint64_t __metal_driver_sifive_remapper_get_max_from_entry_region_size(
    struct metal_remapper *remapper) {
    return (1ULL << __metal_driver_sifive_remapper2_max_from_entry_addr_width(
                remapper));
}

uint32_t
__metal_driver_sifive_remapper_get_version(struct metal_remapper *remapper) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    return REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_VERSION);
}

uint32_t
__metal_driver_sifive_remapper_get_entries(struct metal_remapper *remapper) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    return REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_ENTRIES);
}

int __metal_driver_sifive_remapper_set_remap(
    struct metal_remapper *remapper, struct metal_remapper_entry *entry) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    __metal_driver_sifive_remapper_set_key_state(remapper);
    REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_FROM_REG_OFFSET(entry->idx)) =
        entry->from_addr;
    __metal_driver_sifive_remapper_set_key_state(remapper);
    REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_TO_REG_OFFSET(entry->idx)) =
        entry->to_addr;
    __METAL_IO_FENCE(o, rw);

    return 0;
}

int __metal_driver_sifive_remapper_set_remaps(
    struct metal_remapper *remapper, struct metal_remapper_entry *entries[],
    int num_entries) {
    for (int i = 0; i < num_entries; i++) {
        __metal_driver_sifive_remapper_set_remap(remapper, entries[i]);
    }

    return 0;
}

uint64_t
__metal_driver_sifive_remapper_get_from(struct metal_remapper *remapper,
                                        int idx) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    return REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_FROM_REG_OFFSET(idx));
}

uint64_t __metal_driver_sifive_remapper_get_to(struct metal_remapper *remapper,
                                               int idx) {
    unsigned long base = __metal_driver_sifive_remapper2_base(remapper);
    return REMAPPER_REGW(METAL_SIFIVE_REMAPPER2_TO_REG_OFFSET(idx));
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_remapper2) = {
    .remapper.enable_remap = __metal_driver_sifive_remapper_enable_remap,
    .remapper.disable_remap = __metal_driver_sifive_remapper_disable_remap,
    .remapper.enable_remaps = __metal_driver_sifive_remapper_enable_remaps,
    .remapper.disable_remaps = __metal_driver_sifive_remapper_disable_remaps,
    .remapper.get_valid = __metal_driver_sifive_remapper_get_valid,
    .remapper.set_valid = __metal_driver_sifive_remapper_set_valid,
    .remapper.flush = __metal_driver_sifive_remapper_flush,
    .remapper.get_from_region_base =
        __metal_driver_sifive_remapper_get_from_region_base,
    .remapper.get_from_region_size =
        __metal_driver_sifive_remapper_get_from_region_size,
    .remapper.get_to_region_base =
        __metal_driver_sifive_remapper_get_to_region_base,
    .remapper.get_to_region_size =
        __metal_driver_sifive_remapper_get_to_region_size,
    .remapper.get_max_from_entry_region_size =
        __metal_driver_sifive_remapper_get_max_from_entry_region_size,
    .remapper.get_version = __metal_driver_sifive_remapper_get_version,
    .remapper.get_entries = __metal_driver_sifive_remapper_get_entries,
    .remapper.set_remap = __metal_driver_sifive_remapper_set_remap,
    .remapper.set_remaps = __metal_driver_sifive_remapper_set_remaps,
    .remapper.get_from = __metal_driver_sifive_remapper_get_from,
    .remapper.get_to = __metal_driver_sifive_remapper_get_to,
};

#endif /* METAL_SIFIVE_REMAPPER2 */

typedef int no_empty_translation_units;
