#include <metal/tty.h>
#include <errno.h>
#ifndef __SEGGER_LIBC__
#include <sys/types.h>
#include <unistd.h>
#else
#define STDOUT_FILENO 1
typedef int size_t;
typedef int ssize_t;
#endif

/* Write to a file.  */
ssize_t _write(int file, const void *ptr, size_t len) {
    if (file != STDOUT_FILENO) {
#ifdef __SEGGER_LIBC__
        errno = EINVAL;
#else
        errno = ENOSYS;
#endif
        return -1;
    }

  const char *bptr = ptr;
  for (size_t i = 0; i < len; ++i)
    metal_tty_putc(bptr[i]);
  return 0;
}
