/**
 * @code 	#include "hardware/gpio.h" @endcode
 * @file 	libs/hardware/gpio.h
 * @brief 	General Purpose Digital I/O library.
 */
//###############################################
#ifndef _HARDWARE_GPIO_H_
#define _HARDWARE_GPIO_H_
//###############################################
#include <hardware/atomic.h>
#include <hardware/board.h>

//###############################################
/**
 * @brief	Return the port control register address from a given board
 * 			port/pin value.
 * @param	brd_val			Board port/pin value.
 * @return	Port control register.
 */
#define GPIO_PORT(brd_val) 		((brd_val >> 8) & 0xFF)

/**
 * @brief	Return the port available pins mask from a given board port
 * 			value.
 * @param	port			Board port value.
 * @return	Available pins mask.
 */
#define GPIO_PORT_MASK(port)	(port & 0xFF)

/**
 * @brief	Return the pin bit position from a given board pin value.
 * @param	pin				Board pin value.
 * @return	Pin bit position.
 */
#define GPIO_PIN_BIT(pin)		(pin & 0xFF)

/**
 * @brief	Return the pin mask from a given board pin value.
 * @param	pin 			Board pin value.
 * @return	Pin bit mask.
 */
#define GPIO_PIN_MASK(pin) 		_BV(GPIO_PIN_BIT(pin))

//###############################################
/**
 * @struct 	gpio_reg_t
 * @brief	Pin control registers
 */
typedef struct {
	volatile uint8_t PIN;	//!< Pin input register (Read pin value)
	volatile uint8_t DDR;	//!< Pin direction register (I/O selection)
	volatile uint8_t PORT;	//!< PORT register (Set pin value)
} gpio_reg_t;

/**
 * @enum	gpio_mode_t
 * @brief	GPIO operation modes
 */
typedef enum {
	INPUT,				//!< Pin configured as input (DDR(0))
	OUTPUT,				//!< Pin configured as output (DDR(1))
	INPUT_PULLUP			//!< Pin configured as input with pullup (DDR(0), PORT(1))
} gpio_mode_t;

//###############################################
/**
 * @brief	Return the port control registers from a given board port value.
 * @param	port 			Board port value.
 * @return	Port control registers.
 */
inline gpio_reg_t* port_sfr(ports_t port) {
	return (gpio_reg_t*)GPIO_PORT(port);
}

/**
 * @brief	Return the port control registers from a given board pin value.
 * @param	pin				Board pin value.
 * @return	Port control registers.
 */
inline gpio_reg_t* pin_sfr(pins_t pin) {
	return (gpio_reg_t*)GPIO_PORT(pin);
}

//###############################################
/**
 * @brief	Initialize a pin with a certain mode (INPUT, INPUT_PULLUP,
 * 			OUTPUT).
 * @param	pin 			Board pin value.
 * @param	mode 			Pin operation mode.
 */
inline void pin_mode(pins_t pin, gpio_mode_t mode) {
	if (pin == NO_PIN)
		return;
	ATOMIC({
		if (mode == INPUT) {
			pin_sfr(pin)->DDR &= ~GPIO_PIN_MASK(pin);
		} else if (mode == INPUT_PULLUP) {
			pin_sfr(pin)->DDR &= ~GPIO_PIN_MASK(pin);
			pin_sfr(pin)->PORT |= GPIO_PIN_MASK(pin);
		} else {
			pin_sfr(pin)->DDR |= GPIO_PIN_MASK(pin);
		}
	});
}

/**
 * @brief	Write a digital value on the selected pin.
 * @param	pin				Board pin value.
 * @param	value 			State of the pin: on(true), off(false).
 */
inline void pin_write(pins_t pin, bool value) {
	if (pin == NO_PIN)
		return;
	ATOMIC({
		if (value) {
			pin_sfr(pin)->PORT |= GPIO_PIN_MASK(pin);
		} else {
			pin_sfr(pin)->PORT &= ~GPIO_PIN_MASK(pin);
		}
	});
}

/**
 * @brief	Toggle the pin value.
 * @param	pin				Board pin value.
 */
inline void pin_toggle(pins_t pin) {
	if (pin == NO_PIN)
		return;
	ATOMIC({
		pin_sfr(pin)->PIN |= GPIO_PIN_MASK(pin);
	});
}

/**
 * @brief	Return the pin value.
 * @param	pin				Board pin value.
 * @return 	Pin state: on(true), off(false).
 */
inline bool pin_read(pins_t pin) {
	if (pin == NO_PIN)
		return false;
	return ((pin_sfr(pin)->PIN & GPIO_PIN_MASK(pin)) != 0);
}

//###############################################
/**
 * @brief	Initialize a port with a certain mode (INPUT, INPUT_PULLUP,
 * 			OUTPUT).
 * @param	port			Board port value.
 * @param	mask			Pins to be configured.
 * @param	mode			Pins operation mode.
 */
inline void port_mode(ports_t port, uint8_t mask, gpio_mode_t mode) {
	if (port == NO_PORT)
		return;
	gpio_reg_t* sfr = port_sfr(port);
	ATOMIC({
		mask = GPIO_PORT_MASK(port) & mask;
		if (mode == INPUT) {
			sfr->DDR &= ~mask;
		} else if (mode == INPUT_PULLUP) {
			sfr->DDR &= ~mask;
			sfr->PORT |= mask;
		} else {
			sfr->DDR |= mask;
		}
	});
}

/**
 * @brief	Write a given value over a port (only on the pins selected as
 * 			outputs).
 * @param	port 			Board port value.
 * @param	value			Pin values: on(1), off(0).
 */
inline void port_write(ports_t port, uint8_t value) {
	if (port == NO_PORT)
		return;
	gpio_reg_t* sfr = port_sfr(port);
	ATOMIC({
		value &= GPIO_PORT_MASK(port);
		value = ((sfr->PORT & ~sfr->DDR) | (value & sfr->DDR));
		sfr->PORT = value;
	});
}

/**
 * @brief	Toggle the state of the pins selected as output on the given
 * 			port.
 * @param	port			Board port value.
 */
inline void port_toggle(ports_t port) {
	if (port == NO_PORT)
		return;
	gpio_reg_t* sfr = port_sfr(port);
	uint8_t 	mask = GPIO_PORT_MASK(port);
	ATOMIC({
		mask &= sfr->DDR;
		sfr->PIN |= mask;
	});
}

/**
 * @brief	Return the value of the pins selected as input on the given
 * 			port.
 * @param	port 			Board port value.
 * @return 	Port state: on(1), off(0).
 */
inline uint8_t port_read(ports_t port) {
	if (port == NO_PORT)
		return 0x00;
	gpio_reg_t* sfr = port_sfr(port);
	return ((sfr->PIN & GPIO_PORT_MASK(port)) & ~sfr->DDR);
}

//###############################################
#endif /* _HARDWARE_GPIO_H_ */
//###############################################
