/**
 * @file 	libs/hardware/int_pc.cpp
 * @brief 	Pin change interrupt library.
 */
//###############################################
#include <hardware/int_pc.h>
//###############################################
/**
 * @brief	Used to simplify the pin change interrupt ISR definition.
 * @param	vect			ISR vector.
 * @param	int_pc			Pin-change interrupt configuration.
 * @param	pcmsk			Pin-change interrupt enable register.
 * @param	input			Pin-change interrupt port input.
 */
#define INT_PC_ISR(vect, int_pc, pcmsk, input)										\
	ISR(vect) {																		\
		uint8_t new_state = input;													\
		uint8_t pin_trigs = (pcmsk & (int_pc.old_state ^ new_state) & ((int_pc.rising & new_state) | (int_pc.falling & ~new_state))); \
		int_pc.old_state = new_state;												\
		if ((pin_trigs & _BV(0)) != 0) int_pc.callbacks[0]();						\
		if ((pin_trigs & _BV(1)) != 0) int_pc.callbacks[1]();						\
		if ((pin_trigs & _BV(2)) != 0) int_pc.callbacks[2]();						\
		if ((pin_trigs & _BV(3)) != 0) int_pc.callbacks[3]();						\
		if ((pin_trigs & _BV(4)) != 0) int_pc.callbacks[4]();						\
		if ((pin_trigs & _BV(5)) != 0) int_pc.callbacks[5]();						\
		if ((pin_trigs & _BV(6)) != 0) int_pc.callbacks[6]();						\
		if ((pin_trigs & _BV(7)) != 0) int_pc.callbacks[7]();						\
	}

//###############################################
/**
 * @struct	int_pc_t
 * @brief	Structure that holds all the pin-change interrupt info for a
 * 			given port.
 */
typedef struct {
	uint8_t rising;							//!< Port interrupt on rising mask.
	uint8_t falling;						//!< Port interrupt on falling mask.
	volatile uint8_t old_state;				//!< Port old state.
	volatile fun_ptr_vd_vd callbacks[8];	//!< Port pin change interrupt callbacks.
} int_pc_t;

/**
 * @struct	int_pc_pin_t
 * @brief	Structure that holds all the pin-change interrupt info for a
 * 			given pin
 */
typedef struct {
	uint8_t idx_pin;						//!< Pin position in the port.
	uint8_t idx_int;						//!< Interrupt number.
	volatile uint8_t *pcicr;				//!< Interrupt enable register.
	volatile uint8_t *pcmsk;				//!< Interrupt mask register.
	int_pc_t *config;						//!< Pin change port configuration.
} int_pc_pin_t;

//###############################################
/** Prepare all pin-change interrupt ISRs */
#if defined(INT_PC_0)
static int_pc_t int_pc_0;
INT_PC_ISR(PCINT0_vect, int_pc_0, PCMSK0, INT_PC_0)
#endif
#if defined(INT_PC_1)
static int_pc_t int_pc_1;
INT_PC_ISR(PCINT1_vect, int_pc_1, PCMSK1, INT_PC_1)
#endif
#if defined(INT_PC_2)
static int_pc_t int_pc_2;
INT_PC_ISR(PCINT2_vect, int_pc_2, PCMSK2, INT_PC_2)
#endif

//###############################################
/**
 * @brief	Private function. Returns the pin-change interrupt info.
 * @param	pin				Board pin value.
 * @return	Pin change interrupt info int_pc_pin_t.
 */
int_pc_pin_t* get_int_pc_from_pin(pins_t pin);

//###############################################
bool int_pc_attach(pins_t pin, fun_ptr_vd_vd function, int_mode_t mode) {
	int_pc_pin_t *int_pc = get_int_pc_from_pin(pin);
	// Verify if pin-change interrupt exists...
	if (int_pc) {
		// Enable interrupt
		ATOMIC({
			// Set callback
			int_pc->config->callbacks[int_pc->idx_pin] = function;
			// Set mode
			int_pc->config->rising  = set_field_value(int_pc->config->rising, int_pc->idx_pin, 1, (((mode == RISING) || (mode == CHANGE))? 1 : 0));
			int_pc->config->falling = set_field_value(int_pc->config->falling, int_pc->idx_pin, 1, (((mode == FALLING) || (mode == CHANGE))? 1 : 0));
			// Activate
			*(int_pc->pcmsk) |= _BV(int_pc->idx_pin);
			*(int_pc->pcicr) |= _BV(int_pc->idx_int);
			// Update state for pin
			int_pc->config->old_state = set_field_value(int_pc->config->old_state, int_pc->idx_pin, 1, pin_read(pin));
		});
		// Make sure we free the space!
		free(int_pc);
		return true;
	}
	return false;
}

void int_pc_detach(pins_t pin) {
	int_pc_pin_t *int_pc = get_int_pc_from_pin(pin);
	// Verify if pin-change interrupt exists...
	if (int_pc) {
		// Disable interrupt
		ATOMIC({
			// Clear callback
			int_pc->config->callbacks[int_pc->idx_pin] = NULL;
			// Clean mode
			int_pc->config->rising  = set_field_value(int_pc->config->rising, int_pc->idx_pin, 1, 0);
			int_pc->config->falling = set_field_value(int_pc->config->falling, int_pc->idx_pin, 1, 0);
			// Deactivate
			*(int_pc->pcmsk) &= ~_BV(int_pc->idx_pin);
			// If any pin defined as interrupt, full disable.
			if (!*(int_pc->pcmsk)) {
				*(int_pc->pcicr) &= ~_BV(int_pc->idx_int);
			}
		});
		// Make sure we free the space!
		free(int_pc);
	}
}

//###############################################
int_pc_pin_t* get_int_pc_from_pin(pins_t pin) {
	int_pc_pin_t* pc_pin = (int_pc_pin_t*) malloc(sizeof(int_pc_pin_t));
	switch (GPIO_PORT(pin)) {
		#if defined(INT_PC_0)
		case INT_PC_0_PORT: {
			pc_pin->pcmsk = &PCMSK0;
			pc_pin->pcicr = &PCICR;
			pc_pin->idx_pin = GPIO_PIN_BIT(pin);
			pc_pin->idx_int = 0;
			pc_pin->config = &int_pc_0;
			return pc_pin;
		} break;
		#endif
		#if defined(INT_PC_1)
		case INT_PC_1_PORT: {
			pc_pin->pcmsk = &PCMSK1;
			pc_pin->pcicr = &PCICR;
			pc_pin->idx_pin = GPIO_PIN_BIT(pin);
			pc_pin->idx_int = 1;
			pc_pin->config = &int_pc_1;
			return pc_pin;
		} break;
		#endif
		#if defined(INT_PC_2)
		case INT_PC_2_PORT: {
			pc_pin->pcmsk = &PCMSK2;
			pc_pin->pcicr = &PCICR;
			pc_pin->idx_pin = GPIO_PIN_BIT(pin);
			pc_pin->idx_int = 2;
			pc_pin->config = &int_pc_2;
			return pc_pin;
		} break;
		#endif
	}
	free(pc_pin);
	return NULL;
}
