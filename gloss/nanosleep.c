#include <errno.h>
#ifndef __SEGGER_LIBC__
#include <sys/time.h>
#endif

int nanosleep(const struct timespec *rqtp, struct timespec *rmtp) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return -1;
}
