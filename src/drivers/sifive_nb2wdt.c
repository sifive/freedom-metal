/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#define SIFIVE_NB2_WDT
#ifdef SIFIVE_NB2_WDT

#include <metal/machine/platform.h>
#include <metal/drivers/sifive_nb2wdt.h>
#include <metal/machine.h>
#include <limits.h>
#include <metal/io.h>
#include <math.h>



#define WDT_CR_EN   0
#define WDT_CR_RMOD 1
#define WDT_CR_RPL  2
#define WDT_INIT    4


#define DW_RUN_MODE_RESET 0
#define DW_RUN_MODE_INTR_AND_RESET 1
#define DW_RESET_PLENGTH  10



/*To enable the WDT*/
#define DW_ENABLE_WDT(base)\
	WDT_REGW(base,METAL_SIFIVE_NB2WDT_WDT_CR) &= ~(0x1) | (1)

#define DW_DISABLE_WDT(base)\
	WDT_REGW(base,METAL_SIFIVE_NB2WDT_WDT_CR) &= ~(0x1)

/*Configures WDT*/
#define DW_CONFIGURE_WDT(base,p_len,r_mod)\
	WDT_REGW(base,METAL_SIFIVE_NB2WDT_WDT_CR)=(WDT_REGW(base,METAL_SIFIVE_NB2WDT_WDT_CR) & ~(0x1E)) | (p_len << WDT_CR_RPL) | (r_mod << WDT_CR_RMOD)

/*Configure torr*/
#define DW_CONFIGURE_TORR(base,top)\
	WDT_REGW(base,METAL_SIFIVE_NB2WDT_WDT_TORR)= (WDT_REGW(base,METAL_SIFIVE_NB2WDT_WDT_TORR) & 0x0)| top

/*Get current count*/
#define DW_GET_WDT_COUNT(base)\
	WDT_REGW(base,METAL_SIFIVE_NB2WDT_WDT_CCVR)

/** Restart watchdog*/
#define DW_RESTART_WDT(base)\
	WDT_REGW(base,METAL_SIFIVE_NB2WDT_WDT_CRR)=0x76

#define DW_WDT_EOI()\
	WDT_REGW(base,METAL_SIFIVE_NB2WDT_WDT_EOI)



int __metal_driver_sifive_nb2wdt_feed(const struct metal_watchdog *const wdog)
{
	const uintptr_t base = (uintptr_t)__metal_driver_sifive_nb2wdt_control_base(wdog);

	DW_RESTART_WDT(base);

	/*int top=9;
	int rmod=DW_RUN_MODE_INTR_AND_RESET;

	DW_CONFIGURE_TORR(base,top);
	DW_CONFIGURE_WDT(base,DW_RESET_PLENGTH,rmod);

	return DW_GET_WDT_COUNT();
	*/
}

long int __metal_driver_sifive_nb2wdt_get_rate(const struct metal_watchdog *const wdog)
{
	const uintptr_t base = (uintptr_t)__metal_driver_sifive_nb2wdt_control_base(wdog);

	const struct metal_clock *const clock = __metal_driver_sifive_nb2wdt_clock(wdog);

	const long int clock_rate = metal_clock_get_rate_hz(clock);
	return clock_rate;
}



long int __metal_driver_sifive_nb2wdt_set_rate(const struct metal_watchdog *const wdog, const long int rate)
{
	const uintptr_t base = (uintptr_t)__metal_driver_sifive_nb2wdt_control_base(wdog);

	const struct metal_clock *const clock = __metal_driver_sifive_nb2wdt_clock(wdog);

	const long int clock_rate = metal_clock_get_rate_hz(clock);

	if(rate >= clock_rate) {
		/* We can't scale the rate above the driving clock. Clear the scale
		 * field and return the driving clock rate */
		return clock_rate;
	}
	/*We can not scale here return same what we get*/
	return clock_rate;
}

/*Explanation for converting TOP value in to sec.
 * let's say top =3 is returned,
 * no. of cycles = 2^(16 + TOP)
 * clock = 10 MHz
 * time period for one cycle = 0.1 micro-sec
 * hence, for top = 3,
 * timeout period returned = (no. of cycles) * 0.1 * 10^(-6) sec.
 */

long int __metal_driver_sifive_nb2wdt_get_timeout(const struct metal_watchdog *const wdog)
{
	const uintptr_t base = (uintptr_t)__metal_driver_sifive_nb2wdt_control_base(wdog);
	long int rd_dt = WDT_REGW(base, METAL_SIFIVE_NB2WDT_WDT_TORR);

	rd_dt = pow(2, (16 + (double)rd_dt)) * pow(10,-7);
	//get timeout from TORR register and conver it to seconds

	return rd_dt;
}
/**
WDT_USER_TOP_0	0	(2^0xffff)	*1e-7=0.0065536s
WDT_USER_TOP_1	1	(2^0x1ffff)	*1e-7=0.0131072s
WDT_USER_TOP_2	2	(2^0x3ffff)	*1e-7=0.0262144s
WDT_USER_TOP_3	3	(2^0x7ffff)	*1e-7=0.0524288s
WDT_USER_TOP_4	4	(2^0xfffff)	*1e-7=0.1048576s
WDT_USER_TOP_5	5	(2^0x1fffff)	*1e-7=0.2097152s
WDT_USER_TOP_6	6	(2^0x3fffff)	*1e-7=0.4194304s
WDT_USER_TOP_7	7	(2^0x7fffff)	*1e-7=0.8388608s
WDT_USER_TOP_8	8	(2^0xffffff)	*1e-7=1.6777216s
WDT_USER_TOP_9	9	(2^0x1ffffff)	*1e-7=3.3554432s
WDT_USER_TOP_10	10	(2^0x3ffffff)	*1e-7=6.7108864s
WDT_USER_TOP_11	11	(2^0x7ffffff)	*1e-7=13.4217728s
WDT_USER_TOP_12	12	(2^0xfffffff)	*1e-7=26.8435456s
WDT_USER_TOP_13	13	(2^0x1fffffff)	*1e-7=53.6870912s
WDT_USER_TOP_14	14	(2^0x3fffffff)	*1e-7=107.3741824s
WDT_USER_TOP_15	15	(2^0x7fffffff)	*1e-7=214.7483648s
*/

/*Explanation for setting timeout value
 * 	clock = 10MHz
 * 1 clock cycle = 0.1 micro sec
 * let's say,If user wants to set, timeout = 2 sec = 2 * 10^6 micro sec
 * total cycle count = (timeout)/(1 clock cycle) = 2 * 10^7 cycles
 * x = log of (total cycle count) to the base 2.
 * x = (16 + TOP)
 * TOP = x - 16
 * once we get TOP , we can floor or ceil it to nearest value
*/

long int __metal_driver_sifive_nb2wdt_set_timeout(const struct metal_watchdog *const wdog, const long int timeout)
{
	double x = 0x00;
	double temp_top = 0x00;
	int top;
	const uintptr_t base = (uintptr_t)__metal_driver_sifive_nb2wdt_control_base(wdog);

	long int temp_dt = timeout * pow(10,6);
	temp_dt = temp_dt * 10 ;
	x = log(temp_dt)/log(2);
	temp_top = x-16;
	if(fmod(temp_top,1.0) > 0.5)
		top = (int)temp_top + 1;
	else
		top = (int)temp_top;
	DW_DISABLE_WDT(base);

	//int top=9;//3.3sec

	int rmod=DW_RUN_MODE_INTR_AND_RESET;
	DW_CONFIGURE_TORR(base,top);
	DW_CONFIGURE_WDT(base,DW_RESET_PLENGTH,rmod);

	return timeout;
}

int __metal_driver_sifive_nb2wdt_set_result(const struct metal_watchdog *const wdog,
		const enum metal_watchdog_result result)
{
	const uintptr_t base = (uintptr_t)__metal_driver_sifive_nb2wdt_control_base(wdog);

	switch (result) {
		default:
			break;
		case METAL_WATCHDOG_NO_RESULT:
			break;
		case METAL_WATCHDOG_INTERRUPT:
			WDT_REGW(base, METAL_SIFIVE_NB2WDT_WDT_CR) &= ~(1 << 1) ;
			break;
		case METAL_WATCHDOG_FULL_RESET:
			WDT_REGW(base, METAL_SIFIVE_NB2WDT_WDT_CR) |= (1 << 1) ;
			break;
	}

	return 0;
}

int __metal_driver_sifive_nb2wdt_run(const struct metal_watchdog *const wdog,
		const enum metal_watchdog_run_option option)
{
	const uintptr_t base = (uintptr_t)__metal_driver_sifive_nb2wdt_control_base(wdog);

	switch (option) {
		default:
			break;
		case METAL_WATCHDOG_STOP:
			//disable the WDT_REGW
			DW_DISABLE_WDT(base);

			break;
		case METAL_WATCHDOG_RUN_ALWAYS:
			/* Feed the watchdog before starting to reset counter */
			__metal_driver_sifive_nb2wdt_feed(wdog);
			DW_ENABLE_WDT(base);			//enable the wdt
			break;
	}

	return 0;
}

struct metal_interrupt *__metal_driver_sifive_nb2wdt_get_interrupt(const struct metal_watchdog *const wdog)
{
	return __metal_driver_sifive_nb2wdt_interrupt_parent(wdog);
}

int __metal_driver_sifive_nb2wdt_get_interrupt_id(const struct metal_watchdog *const wdog)
{
	return __metal_driver_sifive_nb2wdt_interrupt_line(wdog);
}

int __metal_driver_sifive_nb2wdt_clear_interrupt(const struct metal_watchdog *const wdog)
{
	const uintptr_t base = (uintptr_t)__metal_driver_sifive_nb2wdt_control_base(wdog);

	uint32_t rd_dt;
	rd_dt = WDT_REGW(base, METAL_SIFIVE_NB2WDT_WDT_EOI);

	return 0;
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_nb2wdt) = {
	.watchdog.feed = __metal_driver_sifive_nb2wdt_feed,
	.watchdog.get_rate = __metal_driver_sifive_nb2wdt_get_rate,
	.watchdog.set_rate = __metal_driver_sifive_nb2wdt_set_rate,
	.watchdog.get_timeout = __metal_driver_sifive_nb2wdt_get_timeout,
	.watchdog.set_timeout = __metal_driver_sifive_nb2wdt_set_timeout,
	.watchdog.set_result = __metal_driver_sifive_nb2wdt_set_result,
	.watchdog.run = __metal_driver_sifive_nb2wdt_run,
	.watchdog.get_interrupt = __metal_driver_sifive_nb2wdt_get_interrupt,
	.watchdog.get_interrupt_id = __metal_driver_sifive_nb2wdt_get_interrupt_id,
	.watchdog.clear_interrupt = __metal_driver_sifive_nb2wdt_clear_interrupt,
};

#endif /*SIFIVE_NB2WDT */

typedef int no_empty_translation_units;

