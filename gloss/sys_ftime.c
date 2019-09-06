#include <errno.h>
#ifndef __SEGGER_LIBC__
#include <sys/timeb.h>
#endif

int _ftime(struct timeb *tp) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return -1;
}
