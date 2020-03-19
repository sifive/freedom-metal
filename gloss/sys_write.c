#include <errno.h>
#include <metal/tty.h>
#include <sys/types.h>
#include <unistd.h>
#if defined(_USE_SEMIHOST_)
#include <metal/semihosting.h>
#endif

/* Write to a file.  */
ssize_t _write(int file, const void *ptr, size_t len) {
#if defined(_USE_SEMIHOST_)
    volatile semihostparam_t arg = {.param1 = (uintptr_t)file,
                                    .param2 = (uintptr_t)ptr,
                                    .param3 = (uintptr_t)len};
    ssize_t ret =
        (ssize_t)semihost_call_host(SEMIHOST_SYS_WRITE, (uintptr_t)&arg);

    return (len - ret);
#else
    if (file != STDOUT_FILENO) {
        errno = ENOSYS;
        return -1;
    }

    const char *bptr = ptr;
    for (size_t i = 0; i < len; ++i)
        metal_tty_putc(bptr[i]);
    return 0;
#endif
}
