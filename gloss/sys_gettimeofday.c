#include <errno.h>
#include <sys/time.h>

int
_gettimeofday(struct timeval *tp, void *tzp)
{
  errno = -ENOSYS;
  return -1;
}
