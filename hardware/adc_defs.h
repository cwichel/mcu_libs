/**
 * @code 	#include "hardware/adc_defs.h" @endcode
 * @file	libs/hardware/adc_defs.h
 * @brief	AVR simple ADC library definitions.
 */
//###############################################
#ifndef _HARDWARE_ADC_DEFS_H_
#define _HARDWARE_ADC_DEFS_H_
//###############################################
#include <hardware/gpio.h>

//###############################################
/**
 * @struct	adc_admux_t
 * @brief	ADC multiplexer register fields.
 */
typedef struct {
	uint8_t mux		: 5;	//!< MUX selector bits.
	uint8_t adlar	: 1;    //!< Left aligned result.
	uint8_t ref		: 2;    //!< Reference selection.
} adc_admux_t;

/**
 * @struct	adc_csra_t
 * @brief	ADC Control A register fields.
 */
typedef struct {
	uint8_t	adps	: 3;	//!< Prescaler bits.
	uint8_t adie	: 1;    //!< Interrupt enable.
	uint8_t adif	: 1;    //!< Interrupt flag.
	uint8_t adate	: 1;    //!< Auto-trigger enable.
	uint8_t adsc	: 1;    //!< Start conversion.
	uint8_t aden	: 1;    //!< Enable.
} adc_csra_t;

/**
 * @struct	adc_csrb_t
 * @brief	ADC Control B register fields.
 */
typedef struct {
	uint8_t adts	: 4;	//!< Auto-trigger source.
	uint8_t	unused	: 1;    //!< Unused bit.
	uint8_t	mux5	: 1;    //!< Mux selector bit.
	uint8_t	acme	: 1;    //!< Analog comparator multiplexer enable.
	uint8_t adhsm	: 1;    //!< High-speed mode.
} adc_csrb_t;

//###############################################
/**
 * @enum	adc_ref_t
 * @brief	ADC reference definition.
 */
typedef enum {
	ADC_REF_AREF	 = 0x00,	//!< AREF selected, internal VREF turned off.
	ADC_REF_AVCC	 = 0x01,	//!< AVCC with external capacitor at AREF pin.
	ADC_REF_INTERNAL = 0x03		//!< Internal reference with external capacitor at AREF pin.
} adc_ref_t;

//###############################################
/**
 * @union	adc_regs_t
 * @brief	General register handler.
 */
typedef union {
	uint8_t		raw;		//!< Raw data.
	adc_admux_t	mux;        //!< ADC multiplexer register.
	adc_csra_t	ctrla;      //!< ADC Control A register.
	adc_csrb_t	ctrlb;      //!< ADC Control B register.
} adc_regs_t;

/**
 * @union	adc_mux_t
 * @brief	Multiplexer register handler.
 */
typedef union {
	uint8_t		raw;		//!< Raw data.
	adc_admux_t	field;      //!< ADC multiplexer register.
} adc_mux_t;

/**
 * @union	adc_ctrla_t
 * @brief	Control A register handler.
 */
typedef union {
	uint8_t		raw;		//!< Raw data.
	adc_csra_t	field;      //!< ADC Control A register.
} adc_ctrla_t;

/**
 * @union	adc_ctrlb_t
 * @brief	Control B register handler.
 */
typedef union {
	uint8_t		raw;		//!< Raw data.
	adc_csrb_t	field;      //!< ADC Control B register.
} adc_ctrlb_t;

//###############################################
#endif /* _HARDWARE_ADC_DEFS_H_ */
//###############################################
