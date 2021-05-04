#include <errno.h>
#if defined(_USE_SEMIHOST_)
#include <metal/semihosting.h>
#include <string.h>
#endif

int _unlink(const char *name) {
#if defined(_USE_SEMIHOST_)
    semihostparam_t arg = {.param1 = (uintptr_t)name,
                           .param2 = (uintptr_t)strlen(name)};
    int ret = semihost_call_host(SEMIHOST_SYS_REMOVE, (uintptr_t)&arg);
    if (ret == -1)
        errno = semihost_call_host(SEMIHOST_SYS_ERRNO, 0);
    return ret;
#else
    errno = ENOSYS;
    return -1;
#endif
}
