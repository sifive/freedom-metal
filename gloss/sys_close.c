#include <errno.h>
#if defined(_USE_SEMIHOST_)
#include <metal/semihosting.h>
#endif

int _close(int file) {
#if defined(_USE_SEMIHOST_)
    volatile semihostparam_t arg = {.param1 = (uintptr_t)file};

    return semihost_call_host(SEMIHOST_SYS_CLOSE, (uintptr_t)&arg);
#else
    errno = ENOSYS;
    return -1;
#endif
}
