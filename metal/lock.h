/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__LOCK_H
#define METAL__LOCK_H

/*!
 * @file lock.h
 * @brief An API for creating and using a software lock/mutex
 */

/*!
 * @def METAL_LOCK_DECLARE
 * @brief Declare a lock
 *
 * Locks must be declared with METAL_LOCK_DECLARE to ensure that the lock
 * is linked into a memory region which supports atomic memory operations.
 */
#define METAL_LOCK_DECLARE(name) \
		__attribute__((section(".data.locks"))) \
		struct metal_lock name

/*!
 * @brief A handle for a lock
 */
struct metal_lock {
	int _state;
};

/*!
 * @brief Initialize a lock
 * @param lock The handle for a lock
 * @return 0 if the lock is successfully initialized. A non-zero code indicates failure.
 *
 * If the lock cannot be initialized, attempts to take or give the lock
 * will result in a Store/AMO access fault.
 */
inline int metal_lock_init(struct metal_lock *lock);

/*!
 * @brief Take a lock
 * @param lock The handle for a lock
 * @return 0 if the lock is successfully taken
 *
 * If the lock initialization failed, attempts to take a lock will result in
 * a Store/AMO access fault.
 */
inline int metal_lock_take(struct metal_lock *lock);

/*!
 * @brief Give back a held lock
 * @param lock The handle for a lock
 * @return 0 if the lock is successfully given
 *
 * If the lock initialization failed, attempts to give a lock will result in
 * a Store/AMO access fault.
 */
inline int metal_lock_give(struct metal_lock *lock);

#endif /* METAL__LOCK_H */
