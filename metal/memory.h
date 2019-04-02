/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__MEMORY_H
#define METAL__MEMORY_H

#include <stdint.h>
#include <stddef.h>

/*!
 * @file memory.h
 *
 * @brief API for enumerating memories
 */

struct _metal_memory_attributes {
	int R : 1;
	int W : 1;
	int X : 1;
	int C : 1;
	int A : 1;
};

/*!
 * @brief A handle for a memory
 */
struct metal_memory {
	const uintptr_t _base_address;
	const size_t _size;
	const struct _metal_memory_attributes _attrs;
};

/*!
 * @brief Get the memory which services the given address
 *
 * Given a physical memory address, get a handle for the memory to which
 * that address is mapped.
 *
 * @param address The address to query
 * @return The memory handle, or NULL if the address is not mapped to a memory
 */
struct metal_memory *metal_get_memory_from_address(const uintptr_t address);

/*!
 * @brief Get the base address for a memory
 * @param memory The handle for the memory
 * @return The base address of the memory
 */
inline uintptr_t metal_memory_get_base_address(const struct metal_memory *memory) {
	return memory->_base_address;
}

/*!
 * @brief Get the size of a memory
 * @param memory The handle for the memory
 * @return The size of the memory
 */
inline size_t metal_memory_get_size(const struct metal_memory *memory) {
	return memory->_size;
}

/*!
 * @brief Query if a memory supports atomic operations
 * @param memory The handle for the memory
 * @return nonzero if the memory supports atomic operations
 */
inline int metal_memory_supports_atomics(const struct metal_memory *memory) {
	return memory->_attrs.A;
}

/*!
 * @brief Query if a memory is cacheable
 * @param memory The handle for the memory
 * @return nonzero if the memory is cachable
 */
inline int metal_memory_is_cachable(const struct metal_memory *memory) {
	return memory->_attrs.C;
}

#endif /* METAL__MEMORY_H */

