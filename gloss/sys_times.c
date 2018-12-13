#include <sys/times.h>
#include <errno.h>

clock_t
_times(struct tms *buf)
{
  errno = ENOSYS;
  return -1;
}
