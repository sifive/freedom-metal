#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#if defined(_USE_SEMIHOST_)
#include <metal/semihosting.h>
#endif

off_t _lseek(int file, off_t offset, int whence) {
#if defined(_USE_SEMIHOST_)
    volatile semihostparam_t arg = {.param1 = (uintptr_t)file};

    if (whence == SEEK_CUR && offset == 0)
        return 0;

    if (whence == SEEK_END) {
        int flen = (int)semihost_call_host(SEMIHOST_SYS_FLEN, (uintptr_t)&arg);
        if (flen != -1) {
            whence = SEEK_SET;
            offset += flen;
        }
    }

    if (whence != SEEK_SET) {
        errno = EINVAL;
        return (off_t)-1;
    }

    arg.param2 = (uintptr_t)offset;

    int ret = (int)semihost_call_host(SEMIHOST_SYS_SEEK, (uintptr_t)&arg);
    if (ret == 0)
        return offset;
    errno = semihost_call_host(SEMIHOST_SYS_ERRNO, 0);
    return ret;
#else
    errno = ENOSYS;
    return -1;
#endif
}
