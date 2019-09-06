#include <errno.h>
#ifndef __SEGGER_LIBC__
#include <sys/types.h>
#else
typedef int off_t;
#endif

off_t _lseek(int file, off_t ptr, int dir) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return -1;
}
