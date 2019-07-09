/**
 * @code	#include "hardware/int_ext.h" @endcode
 * @file 	libs/hardware/int_ext.h
 * @brief	Pin external interrupt library.
 */
//###############################################
#ifndef _HARDWARE_INT_EXT_H_
#define _HARDWARE_INT_EXT_H_
//###############################################
#include <hardware/int_defs.h>

//###############################################
/** External interrupt pins available */
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
	#define INT_EXT_PINS		5
#elif defined(__AVR_Atmega168__) || defined(__AVR_ATmega328P__)
	#define INT_EXT_PINS		2
#endif

//###############################################
/**
 * @brief	Enable an external interrupt pin and attach its ISR callback
 * 			function.
 * @param	pin				Board pin value.
 * @param	function		ISR callback.
 * @param	mode			Interrupt operation mode.
 * @return	Value that indicates if the operation was successful(true) or
 * 			not(false).
 */
extern bool int_ext_attach(pins_t pin, fun_ptr_vd_vd function, int_mode_t mode);

/**
 * @brief	Disable an external interrupt pin and detach its ISR callback
 * 			function.
 * @param	pin				Board pin value.
 */
extern void int_ext_detach(pins_t pin);

//###############################################
#endif /* _HARDWARE_INT_EXT_H_ */
//###############################################
