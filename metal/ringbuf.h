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
    /* _read is the index into _buf to begin reading from the buffer */
    uint32_t _read;
    /* _count is the number of elements in the buffer */
    uint32_t _count;
    /* _capacity is the total number of bytes in the buffer */
    uint32_t _capacity;
    /* A Flexible Array Member for holding the contents of the ringbuffer,
     * requires C >= C99 and must be the last element in the struct */
    uint8_t _buf[];
};

/*!
 * @def METAL_RINGBUF_SIZEOF
 * @brief Get the required size of the memory to hold a ringbuf of N bytes
 *
 * When allocating memory for use in `metal_ringbuf_create()`, users should use
 * this macro to determine the amount of memory to allocate for a ring buffer
 * capacity of `capacity` bytes.
 */
#define METAL_RINGBUF_SIZEOF(capacity)                                         \
    (sizeof(struct metal_ringbuf) + (capacity))

/*!
 * @def METAL_RINGBUF_DECLARE
 * @brief Declare a ring buffer
 *
 * Ring buffers bust be declared with METAL_RINGBUF_DECLARE to
 * allocate storage for the buffer.
 */
#define METAL_RINGBUF_DECLARE(name, capacity)                                  \
    uint8_t                                                                    \
        __metal_ringbuf_##name[sizeof(struct metal_ringbuf) + (capacity)] = {  \
            0,                                                                 \
            0,                                                                 \
            0,                                                                 \
            0,                                                                 \
            0,                                                                 \
            0,                                                                 \
            0,                                                                 \
            0,                                                                 \
            (0xFF & (capacity)),                                               \
            (0xFF & ((capacity) >> 8)),                                        \
            (0xFF & ((capacity) >> 16)),                                       \
            (0xFF & ((capacity) >> 24))};                                      \
    struct __attribute__((__packed__)) metal_ringbuf *name =                   \
        (struct metal_ringbuf *)&__metal_ringbuf_##name;

/*!
 * @brief Creates a ring buffer, given the memory to hold the buffer
 *
 * This can be used in place of METAL_RINGBUF_DECLARE to create a ring buffer at
 * runtime
 *
 * @param rb A pointer to a struct metal_ringbuf to be initialized
 * @param capacity The number of bytes the buffer can hold
 * @return 0 if successfully created, -EINVAL if rb is NULL
 */
int metal_ringbuf_create(struct metal_ringbuf *rb, uint32_t capacity);

/*!
 * @brief Returns the number of bytes in the ring buffer
 *
 * WARNING: This action is not thread-safe! If your application requires
 * thread-safety, you should wrap this method in the appropriate synchronization
 * mechanism.
 *
 * @param rb The ring buffer
 * @return the number of bytes in the ring buffer, or -EINVAL if rb is NULL
 */
int metal_ringbuf_num_bytes(struct metal_ringbuf *rb);

/*!
 * @brief Append a byte to the ring buffer
 *
 * Adds a byte to the end of the ring buffer, if space exists
 *
 * WARNING: This action is not thread-safe! If your application requires
 * thread-safety, you should wrap this method in the appropriate synchronization
 * mechanism.
 *
 * @param rb The ring buffer
 * @param val The byte to append
 * @return 0 if the value is added, -ENOMEM if no space remains
 */
int metal_ringbuf_put(struct metal_ringbuf *rb, uint8_t val);

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
 * @param len The size of the value in bytes
 * @return 0 if the value is added, -ENOMEM if no space remains
 */
int metal_ringbuf_put_bytes(struct metal_ringbuf *rb, const void *val,
                            size_t len);

/*!
 * @brief Get a byte from the ring buffer
 *
 * Pops a byte off the front of the ring buffer, if the buffer is not empty
 *
 * WARNING: This action is not thread-safe! If your application requires
 * thread-safety, you should wrap this method in the appropriate synchronization
 * mechanism.
 *
 * @param rb The ring buffer
 * @param val Stores the popped value
 * @return 0 if a value is popped, -ENODATA if the buffer is empty
 */
int metal_ringbuf_get(struct metal_ringbuf *rb, uint8_t *val);

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
int metal_ringbuf_get_bytes(struct metal_ringbuf *rb, void *val, size_t len);

/*!
 * @brief Get a byte from the ring buffer without removing it from the buffer
 *
 * WARNING: This action is not thread-safe! If your application requires
 * thread-safety, you should wrap this method in the appropriate synchronization
 * mechanism.
 *
 * @param rb The ring buffer
 * @param val Stores the popped byte
 * @return 0 if a value is peeked, -ENODATA if the buffer is empty
 */
int metal_ringbuf_peek(struct metal_ringbuf *rb, uint8_t *val);

/*!
 * @brief Get a value from the ring buffer without removing it from the buffer
 *
 * WARNING: This action is not thread-safe! If your application requires
 * thread-safety, you should wrap this method in the appropriate synchronization
 * mechanism.
 *
 * @param rb The ring buffer
 * @param val Stores the popped value
 * @param len The size of the value in bytes
 * @return 0 if a value is peeked, -ENODATA if the buffer is empty
 */
int metal_ringbuf_peek_bytes(struct metal_ringbuf *rb, void *val, size_t len);

#endif /* METAL__RINGBUF_H */
