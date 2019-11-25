/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>
#include <string.h>

#include <metal/ringbuf.h>

int metal_ringbuf_create(struct metal_ringbuf *rb, uint32_t capacity) {
    if (rb == NULL) {
        return -EINVAL;
    }

    rb->_read = 0;
    rb->_count = 0;
    rb->_capacity = capacity;

    return 0;
}

int metal_ringbuf_num_bytes(struct metal_ringbuf *rb) {
    if (rb == NULL) {
        return -EINVAL;
    }
    return rb->_count;
}

int metal_ringbuf_put(struct metal_ringbuf *rb, uint8_t val) {
    if (rb == NULL) {
        return -EINVAL;
    }
    if (rb->_count == rb->_capacity) {
        /* buffer is full */
        return -ENOMEM;
    }

    rb->_buf[(rb->_read + rb->_count) % rb->_capacity] = val;
    rb->_count += 1;

    return 0;
}

int metal_ringbuf_put_bytes(struct metal_ringbuf *rb, const void *val,
                            size_t len) {
    if (rb == NULL) {
        return -EINVAL;
    }
    if ((rb->_count + len) > rb->_capacity) {
        /* buffer doesn't have enough space */
        return -ENOMEM;
    }

    const uint8_t *arr = (const uint8_t *)val;

    for (size_t i = 0; i < len; i++) {
        metal_ringbuf_put(rb, arr[i]);
    }

    return 0;
}

int metal_ringbuf_get(struct metal_ringbuf *rb, uint8_t *val) {
    if (rb == NULL || val == NULL) {
        return -EINVAL;
    }
    if (rb->_count == 0) {
        return -ENODATA;
    }

    *val = rb->_buf[rb->_read];
    rb->_read = (rb->_read + 1) % rb->_capacity;
    rb->_count -= 1;

    return 0;
}

int metal_ringbuf_get_bytes(struct metal_ringbuf *rb, void *val, size_t len) {
    if (rb == NULL || val == NULL) {
        return -EINVAL;
    }
    if (rb->_count < len) {
        return -ENODATA;
    }

    for (size_t i = 0; i < len; i++) {
        metal_ringbuf_get(rb, (uint8_t *)(val + i));
    }

    return 0;
}

int metal_ringbuf_peek(struct metal_ringbuf *rb, uint8_t *val) {
    if (rb == NULL || val == NULL) {
        return -EINVAL;
    }
    if (rb->_count == 0) {
        return -ENODATA;
    }

    *val = rb->_buf[rb->_read];

    return 0;
}

int metal_ringbuf_peek_bytes(struct metal_ringbuf *rb, void *val, size_t len) {
    if (rb == NULL || val == NULL) {
        return -EINVAL;
    }
    if (rb->_count < len) {
        return -ENODATA;
    }

    for (size_t i = 0; i < len; i++) {
        *((uint8_t *)val + i) = rb->_buf[(rb->_read + i) & rb->_capacity];
    }

    return 0;
}
