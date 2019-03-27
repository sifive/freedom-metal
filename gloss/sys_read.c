#include <metal/tty.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

ssize_t
_read(int file, void *ptr, size_t len)
{
  if (file != STDIN_FILENO) {
    errno = ENOSYS;
    return -1;
  }

  char c = metal_tty_getc();

  /* If we got back -1, no default input device is available */
  if(c == -1) {
    errno = EBADF;
    return -1;
  }

  *((char *) ptr) = c;

  return 1;
}
