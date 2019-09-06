#include <errno.h>

int _link(const char *old_name, const char *new_name) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return -1;
}
