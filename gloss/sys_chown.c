#include <errno.h>
#ifndef __SEGGER_LIBC__
#include <sys/types.h>
#else
typedef int uid_t;
typedef int gid_t;
#endif

int _chown(const char *path, uid_t owner, gid_t group) {
#ifdef __SEGGER_LIBC__
  errno = EINVAL;
#else
  errno = ENOSYS;
#endif
  return -1;
}
