/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <errno.h>
#include <string.h>

#include <metal/ringbuf.h>

int metal_ringbuf_create(struct metal_ringbuf *rb, void *buf, size_t capacity,
                         size_t item_size) {
    if (rb == NULL || buf == NULL) {
        return -EINVAL;
    }

    rb->_buf = buf;
    rb->_start = buf;
    rb->_end = buf;
    rb->_capacity = capacity;
    rb->_item_size = item_size;
    rb->_empty = true;

    return 0;
}

size_t metal_ringbuf_num_items(struct metal_ringbuf *rb) {
    if (rb == NULL) {
        return -EINVAL;
    }
    if (rb->_start > rb->_end) {
        /*
         *  size = 10
         *  empty = 0
         *  address  0 1 2 3 4 5 6 7 8 9
         *  buf     | |a|b|c|d| | | | | |
         *  start      ^
         *  end                ^
         *  len = (end - start)
         *      = (5 - 1)
         *      = 4
         */
        return (rb->_capacity - ((rb->_start - rb->_end) / rb->_item_size));
    } else if (rb->_start < rb->_end) {
        /*
         *  size = 10
         *  empty = 0
         *  address  0 1 2 3 4 5 6 7 8 9
         *  buf     |d| | | | | | |a|b|c|
         *  start                  ^
         *  end        ^
         *  len = size - (start - end)
         *      = 10 - (7 - 1)
         *      = 4
         */
        return ((rb->_end - rb->_start) / rb->_item_size);
    } else if (!rb->_empty) {
        /*
         *  size = 10
         *  empty = 0
         *  address  0 1 2 3 4 5 6 7 8 9
         *  buf     |d|e|f|g|h|i|j|a|b|c|
         *  start                  ^
         *  end                    ^
         *  len = size
         */
        return rb->_capacity;
    }
    /*
     *  size = 10
     *  empty = 1
     *  address  0 1 2 3 4 5 6 7 8 9
     *  buf     | | | | | | | | | | |
     *  start                  ^
     *  end                    ^
     *  len = 0
     */
    return 0;
}

int metal_ringbuf_put(struct metal_ringbuf *rb, const void *val) {
    if (rb == NULL || val == NULL) {
        return -EINVAL;
    }
    size_t len = metal_ringbuf_num_items(rb);
    if (len == rb->_capacity) {
        /* buffer is full */
        return -ENOMEM;
    }

    /* write to the buffer */
    memcpy(rb->_end, val, rb->_item_size);

    if (len == 0) {
        /* mark the buffer as no longer empty */
        rb->_empty = false;
    }

    /* increment the end pointer, wrapping if we've hit the end of the buffer */
    if (rb->_end == (rb->_buf + ((rb->_capacity - 1) * rb->_item_size))) {
        rb->_end = rb->_buf;
    } else {
        rb->_end += rb->_item_size;
    }

    return 0;
}

int metal_ringbuf_get(struct metal_ringbuf *rb, void *val) {
    if (rb == NULL || val == NULL) {
        return -EINVAL;
    }
    if (rb->_empty) {
        return -ENODATA;
    }

    /* read from the buffer */
    memcpy(val, rb->_start, rb->_item_size);

    /* increment the start pointer, wrapping if we've hit the end of the buffer
     */
    if (rb->_start == (rb->_buf + ((rb->_capacity - 1) * rb->_item_size))) {
        rb->_start = rb->_buf;
    } else {
        rb->_start += rb->_item_size;
    }

    if (rb->_start == rb->_end) {
        /* mark the buffer as now empty */
        rb->_empty = true;
    }

    return 0;
}

int metal_ringbuf_peek(struct metal_ringbuf *rb, void *val) {
    if (rb == NULL || val == NULL) {
        return -EINVAL;
    }
    if (rb->_empty) {
        return -ENODATA;
    }

    /* read from the buffer */
    memcpy(val, rb->_start, rb->_item_size);

    return 0;
}
