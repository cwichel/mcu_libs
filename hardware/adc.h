/**
 * @code 	#include "hardware/adc.h" @endcode
 * @file	libs/hardware/adc.h
 * @brief	AVR simple ADC library.
 */
//###############################################
#ifndef _HARDWARE_ADC_H_
#define _HARDWARE_ADC_H_
//###############################################
#include <hardware/adc_defs.h>
//###############################################
/**
 * @brief	Enable the ADC converter.
 */
extern void adc_enable(void);

/**
 * @brief	Disable the ADC converter.
 */
extern void adc_disable(void);

/**
 * @brief	Set the configuration of the ADC.
 * @param	ref				Set the ADC converter reference.
 * @param	hi_speed		Enable, if available, the high-speed mode.
 */
extern void adc_configure(adc_ref_t ref, bool hi_speed);

/**
 * @brief	Get the analog value from a given ADC line.
 * @param	pin				Board pin value.
 * @return	ADC value.
 */
extern uint16_t adc_read(pins_t pin);

/**
 * @brief	If available, return the value of the temperature sensor of
 * 			the board.
 * @return	Temperature sensor value.
 */
extern uint16_t adc_temp(void);

//###############################################
#endif /* _HARDWARE_ADC_H_ */
//###############################################
