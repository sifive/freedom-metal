#include <mee/shutdown.h>

void
_exit(int exit_status)
{
  mee_shutdown(exit_status);
  while (1);
}
