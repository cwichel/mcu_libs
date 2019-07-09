/**
 * @code 	#include "hardware/twi_defs.h" @endcode
 * @file	libs/hardware/twi_defs.h
 * @brief	AVR interrupt I2C library definitions.
 */
//###############################################
#ifndef _HARDWARE_TWI_DEFS_H_
#define _HARDWARE_TWI_DEFS_H_
//###############################################
extern "C" {
#include <util/twi.h>
}
#include <hardware/gpio.h>

//###############################################
#define TWI_DEF_FREQ_KHZ	100UL	//!< TWI default transmission frequency in KHz.

#define TWI_ERR_NONE		0xFF	//!< TWI operation no errors.

//###############################################
#endif /* _HARDWARE_TWI_DEFS_H_ */
//###############################################
