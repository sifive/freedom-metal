#include <metal/tty.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

/* Write to a file.  */
ssize_t
_write(int file, const void *ptr, size_t len)
{
  if (file != STDOUT_FILENO) {
    errno = EBADF;
    return -1;
  }

  const char *bptr = ptr;
  for (size_t i = 0; i < len; ++i) {
    metal_tty_putc(bptr[i]);
  }

  /* Should return the number of bytes actually written,
   * but methods tty.h and uart0.h do not indicate whether
   * bytes are actually written, return the whole write length
   * instead */
  return len;
}
