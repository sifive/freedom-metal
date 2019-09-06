#include <errno.h>
#ifndef __SEGGER_LIBC__
#include <sys/stat.h>
#endif

int _fstatat(int dirfd, const char *file, struct stat *st, int flags) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return -1;
}
