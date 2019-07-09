/**
 * @code 	#include "hardware/int_pc.h" @endcode
 * @file 	libs/hardware/int_pc.h
 * @brief	Pin change interrupt library.
 */
//###############################################
#ifndef _HARDWARE_INT_PC_H_
#define _HARDWARE_INT_PC_H_
//###############################################
#include <hardware/int_defs.h>

//###############################################
/** Pin-change Interrupt ports available */
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
	#define INT_PC_0		PINB
	#define	INT_PC_0_PORT 	GPIO_PORT(PTB)
#elif defined(__AVR_Atmega168__) || defined(__AVR_ATmega328P__)
	#define INT_PC_0		PINB
	#define INT_PC_0_PORT	GPIO_PORT(PTB)
	#define INT_PC_1		PINC
	#define INT_PC_1_PORT	GPIO_PORT(PTC)
	#define INT_PC_2		PIND
	#define INT_PC_2_PORT	GPIO_PORT(PTD)
#endif

//###############################################
/**
 * @brief	Enable a pin change interrupt and attach its ISR callback
 * 			function.
 * @param	pin				Board pin value.
 * @param	function		ISR callback.
 * @param	mode			Interrupt operation mode.
 * @return	Value that indicates if the operation was successful(true) or
 * 			not(false).
 */
extern bool int_pc_attach(pins_t pin, fun_ptr_vd_vd function, int_mode_t mode);

/**
 * @brief	Disable a pin change interrupt and detach its ISR callback
 * 			function.
 * @param	pin				Board pin value.
 */
extern void int_pc_detach(pins_t pin);

//###############################################
#endif /* _HARDWARE_INT_PC_H_ */
//###############################################
