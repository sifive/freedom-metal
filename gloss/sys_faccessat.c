#include <errno.h>

int _faccessat(int dirfd, const char *file, int mode, int flags) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return -1;
}
