/**
 * @file 	libs/hardware/int_ext.cpp
 * @brief 	Pin external interrupt library.
 */
//###############################################
#include <hardware/int_ext.h>
//###############################################
/**
 * @struct 	int_ext
 * @brief	Structure that holds all the external interrupts info.
 */
typedef struct {
	uint8_t idx_fun;								//!< Interrupt callback number.
	uint8_t idx_int;								//!< Interrupt number.
	uint8_t idx_mode;								//!< Interrupt mode field position.
	volatile uint8_t *reg_int;						//!< Interrupt enable register.
	volatile uint8_t *reg_mode;						//!< Interrupt mode register.
} int_ext_pin_t;

//###############################################
/** External interrupt callbacks */
volatile fun_ptr_vd_vd int_ext_callbacks[INT_EXT_PINS];

//###############################################
/**
 * @brief	Private function. Sets the external interrupt info.
 * @param	reg_int			Pointer to external interrupt mask register.
 * @param	reg_mode   		Pointer to external interrupt control register.
 * @param	idx_int    		External interrupt enable index (on interrupt mask register).
 * @param	idx_mode		External interrupt mode index (on interrupt control register).
 * @param	idx_fun			ISR function handler index.
 * @return	External interrupt info int_ext_pin_t.
 */
int_ext_pin_t* get_int_ext(volatile uint8_t* reg_int, volatile uint8_t* reg_mode,
		uint8_t idx_int, uint8_t idx_mode, uint8_t idx_fun);

/**
 * @brief	Private function. Get the pin external interrupt info.
 * @param	pin				Board pin value.
 * @return	External interrupt info int_ext_pin_t.
 */
int_ext_pin_t* get_int_ext_from_pin(pins_t pin);

//###############################################
bool int_ext_attach(pins_t pin, fun_ptr_vd_vd function, int_mode_t mode) {
	// Check if interrupt is valid
	int_ext_pin_t* ext_pin = get_int_ext_from_pin(pin);
	if ((ext_pin == NULL) || (function == NULL))
		return false;
	// Attach pin interruption
	ATOMIC({
		*ext_pin->reg_mode = set_field_value(*ext_pin->reg_mode, ext_pin->idx_mode, 2, mode);
		*ext_pin->reg_int |= _BV(ext_pin->idx_int);
	});
	// Set callback
	int_ext_callbacks[ext_pin->idx_fun] = function;
	// Free the space
	free(ext_pin);
	// Return success
	return true;
}

void int_ext_detach(pins_t pin) {
	// Check if interrupt is valid
	int_ext_pin_t* ext_pin = get_int_ext_from_pin(pin);
	if (ext_pin == NULL)
		return;
	// Detach pin interruption
	ATOMIC({
		*ext_pin->reg_int &= ~_BV(ext_pin->idx_int);
	});
	// Clear callback
	int_ext_callbacks[ext_pin->idx_fun] = NULL;
	// Free the space
	free(ext_pin);
}

//###############################################
int_ext_pin_t* get_int_ext(volatile uint8_t* reg_int, volatile uint8_t* reg_mode,
		uint8_t idx_int, uint8_t idx_mode, uint8_t idx_fun) {
	int_ext_pin_t* ext_pin = (int_ext_pin_t*) malloc(sizeof(int_ext_pin_t));
	ext_pin->reg_int  = reg_int;
	ext_pin->reg_mode = reg_mode;
	ext_pin->idx_int  = idx_int;
	ext_pin->idx_mode = idx_mode;
	ext_pin->idx_fun  = idx_fun;
	return ext_pin;
}

int_ext_pin_t* get_int_ext_from_pin(pins_t pin) {
	switch(pin) {
		#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
		case PTD0: return get_int_ext(&EIMSK, &EICRA, INT0, ISC00, 0); break;
		case PTD1: return get_int_ext(&EIMSK, &EICRA, INT1, ISC10, 1); break;
		case PTD2: return get_int_ext(&EIMSK, &EICRA, INT2, ISC20, 2); break;
		case PTD3: return get_int_ext(&EIMSK, &EICRA, INT3, ISC30, 3); break;
		case PTE6: return get_int_ext(&EIMSK, &EICRB, INT6, ISC60, 4); break;
		#elif defined(__AVR_Atmega168__) || defined(__AVR_ATmega328P__)
		case PTD2: return get_int_ext(&EIMSK, &EICRA, INT0, ISC00, 0); break;
		case PTD3: return get_int_ext(&EIMSK, &EICRA, INT1, ISC10, 1); break;
		#endif
		default: return NULL; break;
	}
}

//###############################################
/**
 * @brief	Used to simplify the external interrupt ISR definition.
 * @param	vect			ISR vector.
 * @param	int_num			External interrupt number.
 */
#define INT_EXT_ISR(vect, int_num)		\
	ISR(vect) {							\
		int_ext_callbacks[int_num]();	\
	}

/** ISR definitions. */
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
	INT_EXT_ISR(INT0_vect, 0)
	INT_EXT_ISR(INT1_vect, 1)
	INT_EXT_ISR(INT2_vect, 2)
	INT_EXT_ISR(INT3_vect, 3)
	INT_EXT_ISR(INT6_vect, 4)
#elif defined(__AVR_Atmega168__) || defined(__AVR_ATmega328P__)
	INT_EXT_ISR(INT0_vect, 0)
	INT_EXT_ISR(INT1_vect, 1)
#endif
