#include <time.h>
#ifndef __SEGGER_LIBC__
#include <unistd.h>
#else
#define _SC_CLK_TCK 2
#endif
/* Get configurable system variables.  */

long
_sysconf(int name)
{
  switch (name)
    {
    case _SC_CLK_TCK:
      return CLOCKS_PER_SEC;
    }
    return -1;
}
