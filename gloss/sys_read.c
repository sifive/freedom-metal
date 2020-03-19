#include <errno.h>
#include <sys/types.h>
#if defined(_USE_SEMIHOST_)
#include <metal/semihosting.h>
#endif

ssize_t _read(int file, void *ptr, size_t len) {
#if defined(_USE_SEMIHOST_)
    volatile semihostparam_t arg = {.param1 = (uintptr_t)file,
                                    .param2 = (uintptr_t)ptr,
                                    .param3 = (uintptr_t)len};
    ssize_t ret =
        (ssize_t)semihost_call_host(SEMIHOST_SYS_READ, (uintptr_t)&arg);

    return (len - ret);
#else
    errno = ENOSYS;
    return -1;
#endif
}
