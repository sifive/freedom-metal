/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/remapper.h>

extern int __metal_remapper_enable_remap(struct metal_remapper *remapper,
                                         int idx);
extern int __metal_remapper_disable_remap(struct metal_remapper *remapper,
                                          int idx);
extern int __metal_remapper_enable_remaps(struct metal_remapper *remapper,
                                          int idxs[], int num_idxs);
extern int __metal_remapper_disable_remaps(struct metal_remapper *remapper,
                                           int idxs[], int num_idxs);
extern uint32_t __metal_remapper_get_valid(struct metal_remapper *remapper,
                                           int idx);
extern int __metal_remapper_set_valid(struct metal_remapper *remapper, int idx,
                                      uint32_t val);
extern int __metal_remapper_flush(struct metal_remapper *remapper);
extern uint64_t
__metal_remapper_get_from_region_base(struct metal_remapper *remapper);
extern uint64_t
__metal_remapper_get_from_region_size(struct metal_remapper *remapper);
extern uint64_t
__metal_remapper_get_to_region_base(struct metal_remapper *remapper);
extern uint64_t
__metal_remapper_get_to_region_size(struct metal_remapper *remapper);
extern uint64_t __metal_remapper_get_max_from_entry_region_size(
    struct metal_remapper *remapper);
extern uint32_t __metal_remapper_get_version(struct metal_remapper *remapper);
extern int __metal_remapper_set_version(struct metal_remapper *remapper,
                                        uint32_t version);
extern uint32_t __metal_remapper_get_entries(struct metal_remapper *remapper);
extern int __metal_remapper_set_remap(struct metal_remapper *remapper,
                                      struct metal_remapper_entry *entry);
extern int __metal_remapper_set_remaps(struct metal_remapper *remapper,
                                       struct metal_remapper_entry *entries[],
                                       int num_entries);
extern uint64_t __metal_remapper_get_from(struct metal_remapper *remapper,
                                          int idx);
extern uint64_t __metal_remapper_get_to(struct metal_remapper *remapper,
                                        int idx);

struct metal_remapper *metal_remapper_get_device(void) {
#ifdef __METAL_DT_REMAPPER_HANDLE
    return __METAL_DT_REMAPPER_HANDLE;
#else
    return NULL;
#endif
}

int metal_remapper_enable_remap(struct metal_remapper *remapper, int idx) {

    return __metal_remapper_enable_remap(remapper, idx);
}

int metal_remapper_disable_remap(struct metal_remapper *remapper, int idx) {
    return __metal_remapper_disable_remap(remapper, idx);
}

int metal_remapper_enable_remaps(struct metal_remapper *remapper, int idxs[],
                                 int num_idxs) {
    return __metal_remapper_enable_remaps(remapper, idxs, num_idxs);
}

int metal_remapper_disable_remaps(struct metal_remapper *remapper, int idxs[],
                                  int num_idxs) {
    return __metal_remapper_disable_remaps(remapper, idxs, num_idxs);
}

uint32_t metal_remapper_get_valid(struct metal_remapper *remapper, int idx) {
    return __metal_remapper_get_valid(remapper, idx);
}

int metal_remapper_set_valid(struct metal_remapper *remapper, int idx,
                             uint32_t val) {
    return __metal_remapper_set_valid(remapper, idx, val);
}

int metal_remapper_flush(struct metal_remapper *remapper) {
    return __metal_remapper_flush(remapper);
}

uint64_t metal_remapper_get_from_region_base(struct metal_remapper *remapper) {
    return __metal_remapper_get_from_region_base(remapper);
}

uint64_t metal_remapper_get_from_region_size(struct metal_remapper *remapper) {
    return __metal_remapper_get_from_region_size(remapper);
}

uint64_t metal_remapper_get_to_region_base(struct metal_remapper *remapper) {
    return __metal_remapper_get_to_region_base(remapper);
}

uint64_t metal_remapper_get_to_region_size(struct metal_remapper *remapper) {
    return __metal_remapper_get_to_region_size(remapper);
}

uint64_t
metal_remapper_get_max_from_entry_region_size(struct metal_remapper *remapper) {
    return __metal_remapper_get_max_from_entry_region_size(remapper);
}

uint32_t metal_remapper_get_version(struct metal_remapper *remapper) {
    return __metal_remapper_get_version(remapper);
}

uint32_t metal_remapper_get_entries(struct metal_remapper *remapper) {
    return __metal_remapper_get_entries(remapper);
}

int metal_remapper_set_remap(struct metal_remapper *remapper,
                             struct metal_remapper_entry *entry) {
    return __metal_remapper_set_remap(remapper, entry);
}

int metal_remapper_set_remaps(struct metal_remapper *remapper,
                              struct metal_remapper_entry *entries[],
                              int num_entries) {
    return __metal_remapper_set_remaps(remapper, entries, num_entries);
}

uint64_t metal_remapper_get_from(struct metal_remapper *remapper, int idx) {
    return __metal_remapper_get_from(remapper, idx);
}

uint64_t metal_remapper_get_to(struct metal_remapper *remapper, int idx) {
    return __metal_remapper_get_to(remapper, idx);
}
