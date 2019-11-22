/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__RINGBUF_H
#define METAL__RINGBUF_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*!
 * @brief The handle for a ring buffer
 */
struct metal_ringbuf {
    /* _buf points at the base of the ring buffer memory */
    void *_buf;
    /* _start points at the ring buffer location containing the first value to
     * be read by metal_ringbuf_get */
    void *_start;
    /* _end points at the first empty location after the buffer contents */
    void *_end;
    /* _capacity is the number of items the ring buffer can hold */
    size_t _capacity;
    /* _item_size is the size of each element in the ring buffer memory */
    size_t _item_size;
    /* _empty is 0 when the buffer is empty, 1 otherwise. This is used to
     * differentiate
     * the full condition from the empty condition, for which both cases have
     * _start == _end */
    bool _empty;
};

/*!
 * @def METAL_RINGBUF_DECLARE
 * @brief Declare a ring buffer
 *
 * Ring buffers bust be declared with METAL_RINGBUF_DECLARE to
 * allocate storage for the buffer.
 */
#define METAL_RINGBUF_DECLARE(name, capacity, item_size)                       \
    uint8_t __metal_ringbuf_##name##_buf[capacity * item_size];                \
    struct metal_ringbuf name = {._buf = (void *)__metal_ringbuf_##name##_buf, \
                                 ._start =                                     \
                                     (void *)__metal_ringbuf_##name##_buf,     \
                                 ._end = (void *)__metal_ringbuf_##name##_buf, \
                                 ._capacity = capacity,                        \
                                 ._item_size = item_size,                      \
                                 ._empty = true};

/*!
 * @brief Creates a ring buffer, given the memory to hold the buffer
 *
 * This can be used in place of METAL_RINGBUF_DECLARE to create a ring buffer at
 * runtime
 *
 * @param rb A pointer to a struct metal_ringbuf to be initialized
 * @param buf The memory for the buffer
 * @param capacity The number of items the buffer can hold
 * @param item_size the size of each item in bytes
 * @return 0 if successfully created, -EINVAL if rb or buf are NULL
 */
int metal_ringbuf_create(struct metal_ringbuf *rb, void *buf, size_t capacity,
                         size_t item_size);

/*!
 * @brief Returns the number of items in the ring buffer
 *
 * WARNING: This action is not thread-safe! If your application requires
 * thread-safety, you should wrap this method in the appropriate synchronization
 * mechanism.
 *
 * @param rb The ring buffer
 * @return the number of items in the ring buffer
 */
size_t metal_ringbuf_num_items(struct metal_ringbuf *rb);

/*!
 * @brief Append a value to the ring buffer
 *
 * Adds a value to the end of the ring buffer, if space exists
 *
 * WARNING: This action is not thread-safe! If your application requires
 * thread-safety, you should wrap this method in the appropriate synchronization
 * mechanism.
 *
 * @param rb The ring buffer
 * @param val The value to append
 * @return 0 if the value is added, -ENOMEM if no space remains
 */
int metal_ringbuf_put(struct metal_ringbuf *rb, const void *val);

/*!
 * @brief Get a value from the ring buffer
 *
 * Pops a value off the front of the ring buffer, if the buffer is not empty
 *
 * WARNING: This action is not thread-safe! If your application requires
 * thread-safety, you should wrap this method in the appropriate synchronization
 * mechanism.
 *
 * @param rb The ring buffer
 * @param val Stores the popped value
 * @return 0 if a value is popped, -ENODATA if the buffer is empty
 */
int metal_ringbuf_get(struct metal_ringbuf *rb, void *val);

/*!
 * @brief Get a value from the ring buffer without removing it from the buffer
 *
 * WARNING: This action is not thread-safe! If your application requires
 * thread-safety, you should wrap this method in the appropriate synchronization
 * mechanism.
 *
 * @param rb The ring buffer
 * @param val Stores the popped value
 * @return 0 if a value is peeked, -ENODATA if the buffer is empty
 */
int metal_ringbuf_peek(struct metal_ringbuf *rb, void *val);

#endif /* METAL__RINGBUF_H */
