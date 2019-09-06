#include <errno.h>
#include <stddef.h>

char * _getcwd(char *buf, size_t size) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return NULL;
}
