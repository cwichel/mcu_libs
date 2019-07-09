/**
 * @code 	#include "hardware/int_defs.h" @endcode
 * @file 	libs/hardware/int_defs.h
 * @brief	AVR pin interrupt libraries definitions.
 */
//###############################################
#ifndef _HARDWARE_INT_DEFS_H_
#define _HARDWARE_INT_DEFS_H_
//###############################################
#include <hardware/gpio.h>

//###############################################
/**
 * @enum 	int_mode_t
 * @brief	Interrupt trigger modes.
 */
typedef enum {
	LOW 	= 0x00,		//!< The interrupt is activated every time the pin is LOW.
	CHANGE  = 0x01,		//!< The interrupt is activated when the pin change it state.
	FALLING = 0x02,		//!< The interrupt is activated when the pin value falls.
	RISING  = 0x03		//!< The interrupt is activated when the pin value rises.
} int_mode_t;

//###############################################
#endif /* _HARDWARE_INT_DEFS_H_ */
//###############################################
