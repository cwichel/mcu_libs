/**
 * @file 	libs/hardware/timing.cpp
 * @brief 	AVR general program timing functions.
 */
//###############################################
#include <hardware/timing.h>
//###############################################
#if F_CPU >= 16000000UL
#define TIMER0_CS				6			//!< Timer prescaler shift code. (1 << 6) = 64
#elif F_CPU >= 2000000UL
#define TIMER0_CS				3			//!< Timer prescaler shift code. (1 << 3) = 8
#else
#define TIMER0_CS				1			//!< Timer prescaler shift code. (1 << 1) = 1
#endif

//===============================================
#define TIMER0_US_PER_OVF		(CYCLES_TO_US(TIMER8_RESOLUTION << TIMER0_CS))	//!< Number of microseconds per timer0 OVF.
#define TIMER0_MS_FULL_PER_OVF	(TIMER0_US_PER_OVF / 1000)						//!< Number of milliseconds per timer0 OVF.
#define TIMER0_MS_FRAC_PER_OVF	((TIMER0_US_PER_OVF % 1000) >> 3)				//!< Fractional number of milliseconds per timer0 OVF.
#define MS_FRAC_MAX				(1000 >> 3)										//!< Fractional number limit.

//###############################################
TIMER8*	timer0;								//!< Pointer to TIMER0.

//===============================================
volatile uint32_t timer0_ovf_count = 0;		//!< Number of overflows made by the TIMER0
volatile uint32_t timer0_ms_full = 0;		//!< Number of [ms] counted by the timer.
volatile uint8_t timer0_ms_frac = 0;		//!< Holds the fractions of [ms] to compensate the timer.

static bool is_timing = false;				//!< Used to initialize the timer only once.

//###############################################
/**
 * @brief	This function updates the counters used by the general
 * 			timer.
 */
void timing_ISR(void);

//###############################################
void timing_enable(void) {
	if (!is_timing) {
		timer8_cs_t clk_sel;

		// Get the prescaler from the shift code
		switch (TIMER0_CS) {
			case 1:		clk_sel = T8_CS_1;		break;
			case 3:		clk_sel = T8_CS_8;		break;
			case 6:		clk_sel = T8_CS_64;		break;
			case 8:		clk_sel = T8_CS_256;	break;
			case 10:	clk_sel = T8_CS_1024;	break;
		}

		// Activate interruptions
		sei();

		// Enable the TIMER0
		timer0 = timer0->get_instance(TIMER_0);
		timer0->init(clk_sel, T8_OM_FPWM);
		timer0->int_attach(timing_ISR);
		timer0->start();

		// Avoid this to be executed again
		is_timing = true;
	}
}

void delay_millis(uint32_t ms) {
	uint32_t start = get_millis();
	while ((get_millis() - start) < ms);
}

void delay_micros(uint16_t us) {
	/* Assume a function overhead of:
	 * 10 - 12 cycles.
	 */
	#if F_CPU >= 24000000L
	/* Clock:		24[MHz]
	 * Cycle:		42[ns]
	 * ----------------------
	 * 				 cycles
	 * Overhead:	10 - 12		// fixed "us"  - variable "us"
	 * Check:		 3 -  4		// continue - return
	 * mult x6:			  7		// loop use 1/6 [us] on each execution
	 * sub  -6:			  2		// compensation
	 * ----------------------
	 * Worst case scenario (variable "us")
	 * 		1.- us == 0:
	 * 			Cycles used:	16
	 * 			Time used: 		< 680[ns]
	 * 		2.- us > 0 :
	 * 			Cycles used:	24 + 4*((6 * us) - 6)
	 * 			Time used:		>= 1050[ns]
	 */
	if (us == 0) return;
	us *= 6;
	us -= 6;

	#elif F_CPU >= 20000000L
	/* Clock:		20[MHz]
	 * Cycle:		50[ns]
	 * ----------------------
	 * 				 cycles
	 * Overhead:	10 - 12		// fixed "us"  - variable "us"
	 * delay:			  4		// Used to get 1[us] delay with the check.
	 * Check:		 3 -  4		// continue - return
	 * mult x5:			  7		// loop use 1/5 [us] on each execution
	 * sub  -7:			  2		// compensation
	 * ----------------------
	 * Worst case scenario (variable "us")
	 * 		1.- us <= 1:
	 * 			Cycles used:	20
	 * 			Time used: 		1[us]
	 * 		2.- us > 1 :
	 * 			Cycles used:	28 + 4*((5 * us) - 7)
	 * 			Time used:		>= 1.4[us]
	 */
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop" "\n\t"
		"nop" "\n\t"
		"nop"
	);
	if (us <= 1) return;
	us = (us << 2) + us;
	us -= 7;

	#elif F_CPU >= 16000000L
	/* Clock:		16[MHz]
	 * Cycle:		63[ns]
	 * ----------------------
	 * 				 cycles
	 * Overhead:	10 - 12		// fixed "us"  - variable "us"
	 * Check:		 3 -  4		// continue - return
	 * mult x4:			  4		// loop use 1/4 [us] on each execution
	 * sub  -5:			  2		// compensation
	 * ----------------------
	 * Worst case scenario (variable "us")
	 * 		1.- us <= 1:
	 * 			Cycles used:	16
	 * 			Time used: 		1008[ns]
	 * 		2.- us > 1 :
	 * 			Cycles used:	21 + 4*((4 * us) - 5)
	 * 			Time used:		>= 1323[ns]
	 */
	if (us <= 1) return;
	us <<= 2;
	us -= 5;

	#elif F_CPU >= 12000000L
	/* Clock:		12[MHz]
	 * Cycle:		84[ns]
	 * ----------------------
	 * 				 cycles
	 * Overhead:	10 - 12		// fixed "us"  - variable "us"
	 * Check:		 3 -  4		// continue - return
	 * mult x3:			  5		// loop use 1/3 [us] on each execution
	 * sub  -5:			  2		// compensation
	 * ----------------------
	 * Worst case scenario (variable "us")
	 * 		1.- us <= 1:
	 * 			Cycles used:	16
	 * 			Time used: 		1344[ns]
	 * 		2.- us > 1 :
	 * 			Cycles used:	22 + 4*((3 * us) - 5)
	 * 			Time used:		>= 1848[ns]
	 */
	if (us <= 1) return;
	us = (us << 1) + us;
	us -= 5;

	#elif F_CPU >= 8000000L
	/* Clock:		8[MHz]
	 * Cycle:		125[ns]
	 * ----------------------
	 * 				 cycles
	 * Overhead:	10 - 12		// fixed "us"  - variable "us"
	 * Check:		 3 -  4		// continue - return
	 * mult x2:			  2		// loop use 1/2 [us] on each execution
	 * sub  -4:			  2		// compensation
	 * ----------------------
	 * Worst case scenario (variable "us")
	 * 		1.- us <= 2:
	 * 			Cycles used:	16
	 * 			Time used: 		2[us]
	 * 		2.- us > 2 :
	 * 			Cycles used:	19 + 4*((2 * us) - 4)
	 * 			Time used:		>= 2375[ns]
	 */
	if (us <= 2) return;
	us <<= 1;
	us -= 4;

	#else
	/* Clock:		1[MHz]
	 * Cycle:		1[us]
	 * ----------------------
	 * 				 cycles
	 * Overhead:	10 - 12		// fixed "us"  - variable "us"
	 * Check 16us:	 3 -  4		// continue - return
	 * Check 25us:	 3 -  4		// continue - return
	 * sub -22:			  2		// loop use 4 [us] on each execution
	 * div   4:			  4		// compensation
	 * ----------------------
	 * Worst case scenario (variable "us")
	 * 		1.- us <= 16:
	 * 			Cycles used:	16
	 * 			Time used: 		16[us]
	 * 		1.- us <= 25:
	 * 			Cycles used:	19
	 * 			Time used:		19[us]
	 * 		2.- us > 25 :
	 * 			Cycles used:	24 + 4*((us - 22) / 4)
	 * 			Time used:		>= 24[us]
	 */
	if (us <= 16) return;
	if (us <= 25) return;
	us -= 24;
	us >>= 2;

	#endif
	/* Busy wait function from _delay_loop_2(). It use 4 cycles per
	 * execution. The number of executions is given by "us".
	 */
	__asm__ __volatile__ (
		"1: sbiw %0,1" "\n\t"
		"brne 1b"
		: "=w" (us)
		: "0" (us)
	);
}

uint32_t get_millis(void) {
	uint32_t time_ms;
	ATOMIC({
		time_ms = timer0_ms_full;
	});
	return time_ms;
}

uint32_t get_micros(void) {
	uint32_t	t0_ovf;
	uint8_t 	t0_cnt;
	ATOMIC({
		t0_ovf = timer0_ovf_count;
		t0_cnt = TCNT0;
		if ((TIFR0 & _BV(TOV0)) && (t0_cnt < TIMER8_RESOLUTION))
			t0_ovf++;
	});
	return CYCLES_TO_US((((t0_ovf << 8) + t0_cnt) << TIMER0_CS));
}

//###############################################
void timing_ISR(void) {
	timer0_ms_full += TIMER0_MS_FULL_PER_OVF;
	timer0_ms_frac += TIMER0_MS_FRAC_PER_OVF;

	if (timer0_ms_frac >= MS_FRAC_MAX) {
		timer0_ms_frac -= MS_FRAC_MAX;
		timer0_ms_full++;
	}

	timer0_ovf_count++;
}
