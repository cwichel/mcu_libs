/**
 * @code 	#include "hardware/atomic.h" @endcode
 * @file 	libs/hardware/atomic.h
 * @brief 	Atomic library.
 */
//###############################################
#ifndef _HARDWARE_ATOMIC_H_
#define _HARDWARE_ATOMIC_H_
//###############################################
extern "C" {
#include <avr/interrupt.h>
}

//###############################################
/**
 * Disable interrupts while execute code.
 * @param	code			Code to be executed atomically.
 */
#define ATOMIC(code) { 								\
	do {											\
		uint8_t sreg = SREG;						\
		__asm__ __volatile__("cli" ::: "memory");	\
		code										\
		SREG = sreg;								\
	} while(0);										\
}

//###############################################
#endif /* _HARDWARE_ATOMIC_H_ */
//###############################################
