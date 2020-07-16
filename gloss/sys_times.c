#include <errno.h>
#include <metal/cpu.h>
#include <metal/timer.h>
#include <sys/time.h>
#include <sys/times.h>

/* v * num / den while avoiding overflow
 *
 * Modulus on unsigned values is defined as:
 *
 *	v % den = v - (v / den) * den
 *	v = v % den + (v / den) * den
 *
 * This lets us break the computation down as follows:
 *
 *	r = (v * num) / den
 *	  = ((v % den + (v / den) * den) * num) / den
 *	  = ((v % den) * num) / den + ((v / den) * den) * num / den
 *	  = ((v % den) * num) / den + ((v / den) * num
 *
 * As long as num * den fits in 64 bits, then this computation will fit
 * in 64 bits. CLOCKS_PER_SEC is defined as 1,000,000, so we just need
 * timebase to be less than about 4,000,000,000,000 (4 trillion).
 */

static inline unsigned long long
muldiv(unsigned long long v, unsigned long long num, unsigned long long den) {
    return (((v % den) * num) / den) + ((v / den) * num);
}

/* Timing information for current process. From
   newlib/libc/include/sys/times.h the tms struct fields are as follows:

   - clock_t tms_utime  : user clock ticks
   - clock_t tms_stime  : system clock ticks
   - clock_t tms_cutime : children's user clock ticks
   - clock_t tms_cstime : children's system clock ticks

   Since maven does not currently support processes we set both of the
   children's times to zero. Eventually we might want to separately
   account for user vs system time, but for now we just return the total
   number of cycles since starting the program.  */
clock_t _times(struct tms *buf) {
    return -1;
}

extern __typeof(_times) times __attribute__((__weak__, __alias__("_times")));
