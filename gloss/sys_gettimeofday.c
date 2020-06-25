#include <errno.h>
#include <sys/time.h>

int _gettimeofday(struct timeval *tp, void *tzp) {
    return -1;
}

extern __typeof(_gettimeofday) gettimeofday
    __attribute__((__weak__, __alias__("_gettimeofday")));
