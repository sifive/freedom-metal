#include "SEGGER_SEMIHOST.h"
#include "__libc.h"
#include "stdio.h"
#include "time.h"
#include <metal/shutdown.h>
#include <metal/time.h>
#include <metal/timer.h>
#include <metal/tty.h>
#include <stdlib.h>

#define UNUSED(x) (void)(x)

/* metal heap start address is provided in target linker script */
extern char metal_segment_heap_target_start;
extern char __heap_size;

char __user_locale_name_buffer[16] __attribute__((weak));
unsigned long __SEGGER_HeapSize = (unsigned long)&__heap_size;

static inline int metal_writeC(char *pChar) {
    int r;

    r = metal_tty_putc((int)*pChar);
    return r;
}

static inline clock_t metal_clock(void) {
    int rv;
    clock_t res;
    static struct timeval t0;
    /* we use this init var due to some RTL issue that t0 is always 0 */
    static int timeval_init = 0;
    struct timeval t;
    unsigned long long timebase;

    if (timeval_init == 0) {
        metal_gettimeofday(&t0, 0);
        timeval_init = 1;
    }

    metal_gettimeofday(&t, 0);

    rv = metal_timer_get_timebase_frequency(0, &timebase);
    if (rv != 0) {
        return -1;
    }

    long long utime =
        (t.tv_sec - t0.tv_sec) * 1000000 + (t.tv_usec - t0.tv_usec);
    res = (clock_t)utime * timebase / 1000000;
    return res;
}

/*********************************************************************
 *
 *       SEGGER_SEMIHOST_X_Request()
 *
 *  Function description
 *    Notify the debugger host about the semihosting request
 *
 *  Parameters
 *    Op:         Operation.
 *    pPara:      Pointer to parameter block.
 *
 *  Return value
 *    Operation-specific return value, set by the debugger in R0.
 */
int SEGGER_SEMIHOST_X_Request(int Op, SEGGER_SEMIHOST_PARA *pPara) {
    int r0;

    switch (Op) {
    case SYS_WRITEC:
        r0 = metal_writeC((char *)pPara);
        break;
    case SYS_READC:
        r0 = 0;
        break;
    case SYS_CLOCK:
        r0 = metal_clock();
        break;
    case SYS_EXIT:
        r0 = pPara[0].I;
        metal_shutdown(r0);
        break;
    default:
        r0 = -1;
    }
    return r0;
}

int __SEGGER_RTL_X_get_time_of_day(struct timeval *tp) {
    metal_gettimeofday(tp, 0);
    return 0;
}

int __SEGGER_RTL_X_set_time_of_day(const struct timeval *tp) { return 0; }

void __SEGGER_RTL_X_assert(const char *stmt, const char *file, int line) {
    UNUSED(file);
    UNUSED(line);
    puts("Failed: ");
    puts(stmt);
    puts("\n");
    exit(1);
}

int __SEGGER_RTL_stdin_getc(void) {
    int r;

    r = SEGGER_SEMIHOST_ReadC();
    return r < 0 ? EOF : r;
}

int __SEGGER_RTL_stdout_putc(int c) {
    int r;

    r = SEGGER_SEMIHOST_WriteC((char)c);
    return r < 0 ? EOF : r;
}

clock_t clock(void) { return SEGGER_SEMIHOST_Clock(); }

void _execute_at_exit_fns(void);

void exit(int status) {
    _execute_at_exit_fns(); // defined in libc_segger.a
    SEGGER_SEMIHOST_Exit(status);
    while (1)
        ;
}

/*************************** End of file ****************************/
