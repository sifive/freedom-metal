#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#if defined(_USE_SEMIHOST_)
#include <metal/semihosting.h>
#endif

int _open(const char *name, int flags, int mode) {
#if defined(_USE_SEMIHOST_)
    int semiflags = 0;

    switch (flags & (O_RDONLY | O_WRONLY | O_RDWR)) {
    case O_RDONLY:
        semiflags = 0; /* 'r' */
        break;
    case O_WRONLY:
        if (flags & O_TRUNC)
            semiflags = 4; /* 'w' */
        else
            semiflags = 8; /* 'a' */
        break;
    default:
        if (flags & O_TRUNC)
            semiflags = 6; /* 'w+' */
        else
            semiflags = 10; /* 'a+' */
        break;
    }

    volatile semihostparam_t arg = {.param1 = (uintptr_t)name,
                                    .param2 = (uintptr_t)semiflags,
                                    .param3 = (uintptr_t)strlen(name)};

    int ret = (int)semihost_call_host(SEMIHOST_SYS_OPEN, (uintptr_t)&arg);
    if (ret == -1)
        errno = semihost_call_host(SEMIHOST_SYS_ERRNO, 0);
    return ret;
#else
    errno = ENOSYS;
    return -1;
#endif
}
