#include <errno.h>
#ifndef __SEGGER_LIBC__
#include <sys/types.h>
#else
typedef int size_t;
typedef int ssize_t;
#endif

ssize_t _read(int file, void *ptr, size_t len) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return -1;
}
