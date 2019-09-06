#ifndef __SEGGER_LIBC__
#include <unistd.h>
#else
#define STDOUT_FILENO 1
#endif

int
_isatty(int file)
{
  return (file == STDOUT_FILENO);
}
