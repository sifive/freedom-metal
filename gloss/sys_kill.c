#include <errno.h>

int _kill(int pid, int sig) {
    errno = ENOSYS;
    return -1;
}
int kill(int pid, int sig) __attribute__((alias("_kill")));
