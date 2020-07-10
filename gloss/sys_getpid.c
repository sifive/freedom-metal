#include <errno.h>

int _getpid(void) { return 1; }
int getpid(void) __attribute__((alias("_getpid")));
