/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__REMAPPER_H
#define METAL__REMAPPER_H

#include <stdint.h>

struct metal_remapper;
struct metal_remapper_entry;

/*! @brief vtable for address remapper. */
struct metal_remapper_vtable {
    int (*enable_remap)(struct metal_remapper *remapper, int idx);
    int (*disable_remap)(struct metal_remapper *remapper, int idx);
    int (*enable_remaps)(struct metal_remapper *remapper, int idxs[],
                         int num_idxs);
    int (*disable_remaps)(struct metal_remapper *remapper, int idxs[],
                          int num_idxs);
    uint32_t (*get_valid)(struct metal_remapper *remapper, int idx);
    int (*set_valid)(struct metal_remapper *remapper, int idx, uint32_t val);
    int (*flush)(struct metal_remapper *remapper);
    uint64_t (*get_from_region_base)(struct metal_remapper *remapper);
    uint64_t (*get_from_region_size)(struct metal_remapper *remapper);
    uint64_t (*get_to_region_base)(struct metal_remapper *remapper);
    uint64_t (*get_to_region_size)(struct metal_remapper *remapper);
    uint64_t (*get_max_from_entry_region_size)(struct metal_remapper *remapper);
    uint32_t (*get_version)(struct metal_remapper *remapper);
    uint32_t (*get_entries)(struct metal_remapper *remapper);
    int (*set_remap)(struct metal_remapper *remapper,
                     struct metal_remapper_entry *entry);
    int (*set_remaps)(struct metal_remapper *remapper,
                      struct metal_remapper_entry *entries[], int num_entries);
    uint64_t (*get_from)(struct metal_remapper *remapper, int idx);
    uint64_t (*get_to)(struct metal_remapper *remapper, int idx);
};

/*! @brief A handle for a address remapper device. */
struct metal_remapper {
    const struct metal_remapper_vtable *vtable;
};

/*! @brief Remap entry descriptor. */
struct metal_remapper_entry {
    int idx;            /*!< Index of remap entry. */
    uint64_t from_addr; /*!< From[] entry address to be remapped. */
    uint64_t to_addr;   /*!< To[] entry address to be remapped. */
};

/*! @brief Enables an address remapper entry.
 * @param remapper Address remapper device handle.
 * @param idx Index of remap entry to enable.
 * @return 0 If no error.*/
inline int __metal_remapper_enable_remap(struct metal_remapper *remapper,
                                         int idx) {
    return remapper->vtable->enable_remap(remapper, idx);
}

/*! @brief Disables an address remapper entry.
 * @param remapper Address remapper device handle.
 * @param idx Index of remap entry to disable.
 * @return 0 If no error.
 */
inline int __metal_remapper_disable_remap(struct metal_remapper *remapper,
                                          int idx) {
    return remapper->vtable->disable_remap(remapper, idx);
}

/*! @brief Batch enables address remapper entries.
 * @param remapper Address remapper device handle.
 * @param idxs[] Array of indexes of remap entries to enable.
 * @param num_idxs Number of indexes passed to idxs[] array.
 * @return 0 If no error.*/
inline int __metal_remapper_enable_remaps(struct metal_remapper *remapper,
                                          int idxs[], int num_idxs) {
    return remapper->vtable->enable_remaps(remapper, idxs, num_idxs);
}

/*! @brief Batch disables an address remapper entries.
 * @param remapper Address remapper device handle.
 * @param idxs[] Array of indexes of remap entries to disable.
 * @param num_idxs Number of indexes passed to idxs[] array.
 * @return 0 If no error.
 */
inline int __metal_remapper_disable_remaps(struct metal_remapper *remapper,
                                           int idxs[], int num_idxs) {
    return remapper->vtable->disable_remaps(remapper, idxs, num_idxs);
}

/*! @brief Get remappervalid[] register content.
 * @param remapper Address remapper device handle.
 * @param idx Index of remappervalid[] register.
 * @return 0 If no error.
 */
inline uint32_t __metal_remapper_get_valid(struct metal_remapper *remapper,
                                           int idx) {
    return remapper->vtable->get_valid(remapper, idx);
}

/*! @brief Set remappervalid[] register with given value.
 * @param remapper Address remapper device handle.
 * @param idx Index of remappervalid[] register.
 * @param val Value to be set.
 * @return 0 If no error.
 */
inline int __metal_remapper_set_valid(struct metal_remapper *remapper, int idx,
                                      uint32_t val) {
    return remapper->vtable->set_valid(remapper, idx, val);
}

/*! @brief Disable all address remapper entries.
 * @param remapper Address remapper device handle.
 * @return 0 If no error.
 */
inline int __metal_remapper_flush(struct metal_remapper *remapper) {
    return remapper->vtable->flush(remapper);
}

/*! @brief Get hardware configured from region base address.
 * @param remapper Address remapper device handle.
 */
inline uint64_t
__metal_remapper_get_from_region_base(struct metal_remapper *remapper) {
    return remapper->vtable->get_from_region_base(remapper);
}

/*! @brief Get hardware configured from region size.
 * @param remapper Address remapper device handle.
 */
inline uint64_t
__metal_remapper_get_from_region_size(struct metal_remapper *remapper) {
    return remapper->vtable->get_from_region_size(remapper);
}

/*! @brief Get hardware configured to region base address.
 * @param remapper Address remapper device handle.
 */
inline uint64_t
__metal_remapper_get_to_region_base(struct metal_remapper *remapper) {
    return remapper->vtable->get_to_region_base(remapper);
}

/*! @brief Get hardware configured to region size.
 * @param remapper Address remapper device handle.
 */
inline uint64_t
__metal_remapper_get_to_region_size(struct metal_remapper *remapper) {
    return remapper->vtable->get_to_region_size(remapper);
}

/*! @brief Get hardware configured maximum from entry region size.
 * @param remapper Address remapper device handle.
 */
inline uint64_t __metal_remapper_get_max_from_entry_region_size(
    struct metal_remapper *remapper) {
    return remapper->vtable->get_max_from_entry_region_size(remapper);
}

/*! @brief Get hardware version of address remapper.
 * @param remapper Address remapper device handle.
 * @return current hardware version of address remapper.
 */
inline uint32_t __metal_remapper_get_version(struct metal_remapper *remapper) {
    return remapper->vtable->get_version(remapper);
}

/*! @brief Get number of entries in address remapper.
 * @param remapper Address remapper device handle.
 * @return number of entries in address remapper.
 */
inline uint32_t __metal_remapper_get_entries(struct metal_remapper *remapper) {
    return remapper->vtable->get_entries(remapper);
}

/*! @brief Set from/to remap entry.
 * @param remapper Address remapper device handle.
 * @param entry Pointer to the remap entry descriptor.
 * @return 0 If no error.
 */
inline int __metal_remapper_set_remap(struct metal_remapper *remapper,
                                      struct metal_remapper_entry *entry) {
    return remapper->vtable->set_remap(remapper, entry);
}

/*! @brief Batch set from/to remap entries.
 * @param remapper Address remapper device handle.
 * @param entries Array of pointers to the remap entry descriptors.
 * @return 0 If no error.
 */
inline int __metal_remapper_set_remaps(struct metal_remapper *remapper,
                                       struct metal_remapper_entry *entries[],
                                       int num_entries) {
    return remapper->vtable->set_remaps(remapper, entries, num_entries);
}

/*! @brief Get the from address of a remap entry.
 * @param remapper Address remapper device handle.
 * @param idx Index of remap entry.
 * @return From address of the remap entry.
 */
inline uint64_t __metal_remapper_get_from(struct metal_remapper *remapper,
                                          int idx) {
    return remapper->vtable->get_from(remapper, idx);
}

/*! @brief Get the to address of a remap entry.
 * @param remapper Address remapper device handle.
 * @param idx Index of remap entry.
 * @return To address of the remap entry.
 */
inline uint64_t __metal_remapper_get_to(struct metal_remapper *remapper,
                                        int idx) {
    return remapper->vtable->get_to(remapper, idx);
}

/*!
 * @brief The public METAL address remapper interfaces.
 */
struct metal_remapper *metal_remapper_get_device(void);
int metal_remapper_enable_remap(struct metal_remapper *remapper, int idx);
int metal_remapper_disable_remap(struct metal_remapper *remapper, int idx);
int metal_remapper_enable_remaps(struct metal_remapper *remapper, int idx[],
                                 int num_idxs);
int metal_remapper_disable_remaps(struct metal_remapper *remapper, int idx[],
                                  int num_idxs);
uint32_t metal_remapper_get_valid(struct metal_remapper *remapper, int idx);
int metal_remapper_set_valid(struct metal_remapper *remapper, int idx,
                             uint32_t val);
int metal_remapper_flush(struct metal_remapper *remapper);
uint64_t metal_remapper_get_from_region_base(struct metal_remapper *remapper);
uint64_t metal_remapper_get_from_region_size(struct metal_remapper *remapper);
uint64_t metal_remapper_get_to_region_base(struct metal_remapper *remapper);
uint64_t metal_remapper_get_to_region_size(struct metal_remapper *remapper);
uint64_t
metal_remapper_get_max_from_entry_region_size(struct metal_remapper *remapper);
uint32_t metal_remapper_get_version(struct metal_remapper *remapper);
uint32_t metal_remapper_get_entries(struct metal_remapper *remapper);
int metal_remapper_set_remap(struct metal_remapper *remapper,
                             struct metal_remapper_entry *entry);
int metal_remapper_set_remaps(struct metal_remapper *remapper,
                              struct metal_remapper_entry *entries[],
                              int num_entries);
uint64_t metal_remapper_get_from(struct metal_remapper *remapper, int idx);
uint64_t metal_remapper_get_to(struct metal_remapper *remapper, int idx);

#endif /*METAL__REMAPPER_H */
