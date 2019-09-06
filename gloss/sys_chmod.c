#include <errno.h>
#ifndef __SEGGER_LIBC__
#include <sys/types.h>
#else
typedef int mode_t;
#endif

int _chmod(const char *path, mode_t mode) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return -1;
}
