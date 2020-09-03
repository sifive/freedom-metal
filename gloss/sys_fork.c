#include <errno.h>

int _fork(void) {
    errno = ENOSYS;
    return -1;
}
