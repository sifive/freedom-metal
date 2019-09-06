#include <errno.h>
struct utimbuf;

int _utime(const char *path, const struct utimbuf *times) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return -1;
}
