#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#if defined(_USE_SEMIHOST_)
#include <metal/semihosting.h>

#define ARGS_BUF_ARRAY_SIZE 80
#define ARGV_BUF_ARRAY_SIZE 10
#endif

void _getcmdline(void) {
    register uintptr_t r0 asm("a0");
    register uintptr_t r1 asm("a1");
    register uintptr_t r2 asm("a2");
#if defined(_USE_SEMIHOST_)
    // Array of chars to receive the command line from the host
    static char args_buf[ARGS_BUF_ARRAY_SIZE];

    // Array of pointers to store the final argv pointers (pointing
    // in the above array).
    static char *argv_buf[ARGV_BUF_ARRAY_SIZE];

    int argn = 0;
    int isInArgument = 0;
    volatile semihostparam_t arg = {.param1 = (uintptr_t)args_buf,
                                    .param2 = (uintptr_t)sizeof(args_buf) - 1};

    int ret =
        (int)semihost_call_host(SEMIHOST_SYS_GET_CMDLINE, (uintptr_t)&arg);
    if (ret == 0) {
        // In case the host send more than we can chew, limit the
        // string to our buffer.
        args_buf[ARGS_BUF_ARRAY_SIZE - 1] = '\0';

        // The command line is a null terminated string
        char *p = (char *)arg.param1;

        int delim = '\0';
        int ch;

        while ((ch = *p) != '\0') {
            if (isInArgument == 0) {
                if (!isblank(ch)) {
                    if (argn >=
                        (int)((sizeof(argv_buf) / sizeof(argv_buf[0])) - 1)) {
                        break;
                    }
                    if (ch == '"' || ch == '\'') {
                        // Remember the delimiter to search for the
                        // corresponding terminator
                        delim = ch;
                        ++p; // skip the delimiter
                        ch = *p;
                    }
                    // Remember the arg beginning address
                    argv_buf[argn++] = p;
                    isInArgument = 1;
                }
            } else if (delim != '\0') {
                if ((ch == delim)) {
                    delim = '\0';
                    *p = '\0';
                    isInArgument = 0;
                }
            } else if (isblank(ch)) {
                delim = '\0';
                *p = '\0';
                isInArgument = 0;
            }
            ++p;
        }
    }

    if (argn == 0) {
        // No args found in string, return a single empty name.
        args_buf[0] = '\0';
        argv_buf[0] = &args_buf[0];
        ++argn;
    }

    // Must end the array with a null pointer.
    argv_buf[argn] = NULL;

    r0 = (uintptr_t)argn;
    r1 = (uintptr_t)&argv_buf[0];
    r2 = (uintptr_t)NULL;
#else
    extern int argc;
    extern int argv;
    extern int envp;

    r0 = (uintptr_t)argc;
    r1 = (uintptr_t)argv;
    r2 = (uintptr_t)envp;
#endif
}