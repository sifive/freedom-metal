#include <errno.h>
#include <metal/tty.h>
#define STDOUT_FILENO 1
typedef int size_t;
typedef int ssize_t;

/* Write to a file.  */
ssize_t _write(int file, const void *ptr, size_t len) {
    if (file != STDOUT_FILENO) {
        errno = EINVAL;
        return -1;
    }

    const char *bptr = ptr;
    for (size_t i = 0; i < len; ++i)
        metal_tty_putc(bptr[i]);
    return 0;
}
