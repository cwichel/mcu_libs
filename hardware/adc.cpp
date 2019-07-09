/**
 * @file 	libs/hardware/adc.cpp
 * @brief 	AVR simple ADC library.
 */
//###############################################
#include <hardware/adc.h>
//###############################################
/** ADMUX register handler */
volatile adc_mux_t*	  _admux = (adc_mux_t*) &ADMUX;

/** ADCSRA register handler */
volatile adc_ctrla_t* _ctrla = (adc_ctrla_t*) &ADCSRA;

/** ADCSRB register handler */
volatile adc_ctrlb_t* _ctrlb = (adc_ctrlb_t*) &ADCSRB;

//###############################################
/**
 * @brief	This function returns the closest prescaler to ensure the good
 * 			behavior of the ADC converter:
 * 			| Clock [MHz]	| Prescaler [#]	| ADC clock [KHz]	| Code 	|
 * 			| :---------:	| :-----------:	| :-------------:	| :--: 	|
 * 			| > 16			| 128			| > 125				| 7		|
 * 			| > 8			| 64			| 125 - 250			| 6		|
 * 			| > 4			| 32			| 125 - 250			| 5		|
 * 			| > 2 			| 16			| 125 - 250			| 4		|
 * 			| > 1			| 8				| 125 - 250			| 3		|
 * 			| > 0.5			| 4				| 125 - 250			| 2		|
 * 			| < 0.5			| 2				| < 250				| 1		|
 *
 * @return	ADC clock prescaler.
 */
uint8_t adc_prescaler(void);

/**
 * @brief	Return the ADC line for a given pin.
 * @param	pin 			Board pin value.
 * @return	ADC line code.
 */
uint8_t	adc_line_from_pin(pins_t pin);

/**
 * @brief	Return the MCU temperature sensor ADC line (if available).
 * @return	ADC line code.
 */
uint8_t	adc_temp_sensor_line(void);

/**
 * @brief	Get ADC line value.
 * @param	code			ADC line code.
 * @return	ADC value.
 */
uint16_t adc_value(uint8_t code);

//###############################################
void adc_enable(void) {
	_ctrla->field.aden = true;
}

void adc_disable(void) {
	_ctrla->field.aden = false;
}

void adc_configure(adc_ref_t ref, bool hi_speed) {
	// Set ADC reference (by default ADLAR is disabled and ADC0 is selected)
	_admux->raw = 0x00;
	_admux->field.ref = ref;
	// Enable ADC and set the prescaler (by default ADC has no trigger and
	// is not interrupt driven).
	_ctrla->raw = 0x00;
	_ctrla->field.adps = adc_prescaler();
	// Set if the ADC will work on high speed (only if the mcu supports it.
	// by default the ADC is in free running mode and the analog comparator
	// multiplexer is disabled).
	_ctrlb->raw = 0x00;
	_ctrlb->field.adhsm = hi_speed;
}

uint16_t adc_read(pins_t pin) {
	uint8_t code = adc_line_from_pin(pin);
	return adc_value(code);
}

uint16_t adc_temp(void) {
	uint8_t code = adc_temp_sensor_line();
	return adc_value(code);
}

//###############################################
uint8_t adc_prescaler(void) {
	#if	F_CPU >= 16000000UL
		return 0x07;			// 16MHz  / 128 = 125[KHz]
	#elif F_CPU >= 8000000UL
		return 0x06;			// 8MHz   / 64 	= 125[KHz]
	#elif F_CPU >= 4000000UL
		return 0x05;			// 4MHz   / 32 	= 125[KHz]
	#elif F_CPU >= 2000000UL
		return 0x04;			// 2MHz   / 16 	= 125[KHz]
	#elif F_CPU >= 1000000UL
		return 0x03;			// 1MHz   / 8 	= 125[KHz]
	#elif F_CPU >=  500000UL
		return 0x02;			// 500KHz / 4 	= 125[KHz]
	#else
		return 0x01;			// Other...
	#endif
}

uint8_t	adc_line_from_pin(pins_t pin) {
	switch (pin) {
		#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
		case PTF0:	return 0x00; break;		// ADC 0
		case PTF1:	return 0x01; break;		// ADC 1
		case PTF4:	return 0x04; break;		// ADC 4
		case PTF5:	return 0x05; break;		// ADC 5
		case PTF6:	return 0x06; break;		// ADC 6
		case PTF7:	return 0x07; break;		// ADC 7
		case PTD4:	return 0x20; break;		// ADC 8
		case PTD6:	return 0x21; break;		// ADC 9
		case PTD7:	return 0x22; break;		// ADC 10
		case PTB4:	return 0x23; break;		// ADC 11
		case PTB5:	return 0x24; break;		// ADC 12
		case PTB6:	return 0x25; break;		// ADC 13
		#elif defined(__AVR_Atmega168__) || defined(__AVR_ATmega328P__)
		case PTC0:	return 0x00; break;
		case PTC1:	return 0x01; break;
		case PTC2:	return 0x02; break;
		case PTC3:	return 0x03; break;
		case PTC4:	return 0x04; break;
		case PTC5:	return 0x05; break;
		#endif
		default: 	return 0xFF; break;
	}
}

uint8_t	adc_temp_sensor_line(void) {
	#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
		return 0x27;
	#elif defined(__AVR_Atmega168__) || defined(__AVR_ATmega328P__)
		return 0x08;
	#else
		return 0xFF;
	#endif
}

uint16_t adc_value(uint8_t code) {
	// Check if the code is available
	if (code == 0xFF)
		return 0x0000;
	// Get ADC value
	// Set line
	_ctrlb->field.mux5 	= ((code & 0x20) >> 5);
	_admux->field.mux 	= (code & 0x1F);
	// Start conversion
	_ctrla->field.adsc = true;
	// Wait until is ready
	while (bit_is_set(ADCSRA, ADSC));
	// Return the value
	return ADC;
}

