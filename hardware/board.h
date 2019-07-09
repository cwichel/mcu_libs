/**
 * @code 	#include "hardware/board.h" @endcode
 * @file 	libs/hardware/board.h
 * @brief 	AVR boards definitions library.
 */
//###############################################
#ifndef _HARDWARE_BOARD_H_
#define _HARDWARE_BOARD_H_
//###############################################
extern "C" {
#include <avr/io.h>
}
#include <base/base.h>

//###############################################
/**
 * @brief 	Board port values are constructed from the port control register
 * 			address (PINx) and the available pins mask.
 * @param	port 			PINx register address.
 * @param	mask			Available pins mask.
 * @return	Board port value.
 */
#define BOARD_PORT(port, mask)	MAKE_U16(port, mask)

/**
 * @brief	Board pin values are constructed from the board port values and
 * 			the pin bit position.
 * @param	port			Board port value.
 * @param	pin				Pin bit.
 * @return	Board pin value.
 */
#define BOARD_PIN(port, pin) 	((0xFF00 & port) | pin)

//===============================================
#define CYCLES_PER_US()			(F_CPU / 1000000UL)

#define CYCLES_TO_US(x)			((x) / CYCLES_PER_US())

#define US_TO_CYCLES(x)			((x) * CYCLES_PER_US())

//###############################################
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
	/**
	 * @enum	ports_t
	 * @brief	Store information about the available ports for the board.
	 * 			The information is stored as a 16bit value, the MSB contains
	 * 			the PINx address and the LSB a mask with the available pins.
	 */
	typedef enum {
		NO_PORT,
		PTB = BOARD_PORT(0x23, 0xFF),	//!< PINB: 0x23 - Pins: 0 - 7.
		PTC = BOARD_PORT(0x26, 0xC0),	//!< PINC: 0x26 - Pins: 6, 7.
		PTD = BOARD_PORT(0x29, 0xFF),	//!< PIND: 0x29 - Pins: 0 - 7.
		PTE = BOARD_PORT(0x2C, 0x42),	//!< PINE: 0x2C - Pins: 6.
		PTF = BOARD_PORT(0x2F, 0xF3)	//!< PINF: 0x2F - Pins: 0, 1, 4 - 7.
	} ports_t;

	/**
	 * @enum	pins_t
	 * @brief	Store information about the board pins.
	 * 			The information is stored as a 16bit value, the MSB contains
	 * 			the PINx address and the LSB the pin position in the port.
	 */
	typedef enum {
		NO_PIN,
		PTB0 = BOARD_PIN(PTB, 0),		//!< PB0: PCINT0, !SS
		PTB1 = BOARD_PIN(PTB, 1),		//!< PB1: PCINT1, SCLK
		PTB2 = BOARD_PIN(PTB, 2),		//!< PB2: PCINT2, MOSI, PDI
		PTB3 = BOARD_PIN(PTB, 3),		//!< PB3: PCINT3, MISO, PDO
		PTB4 = BOARD_PIN(PTB, 4),		//!< PB4: PCINT4, ADC11
		PTB5 = BOARD_PIN(PTB, 5),		//!< PB5: PCINT5, ADC12, 				OC1A 					!OC4B
		PTB6 = BOARD_PIN(PTB, 6),		//!< PB6: PCINT6, ADC13,				OC1B  					OC4B
		PTB7 = BOARD_PIN(PTB, 7),		//!< PB7: PCINT7, !RTS,			OC0A	OC1C

		PTC6 = BOARD_PIN(PTC, 6),		//!< PC6: 												OC3A 	!OC4A
		PTC7 = BOARD_PIN(PTC, 7),		//!< PC7: CLK0, 										ICP3	OC4A

		PTD0 = BOARD_PIN(PTD, 0),		//!< PD0: INT0, SCL, 			OC0B
		PTD1 = BOARD_PIN(PTD, 1),		//!< PD1: INT1, SDA
		PTD2 = BOARD_PIN(PTD, 2),		//!< PD2: INT2, RXD1
		PTD3 = BOARD_PIN(PTD, 3),		//!< PD3: INT3, TXD1
		PTD4 = BOARD_PIN(PTD, 4),		//!< PD4: ADC8							ICP1
		PTD5 = BOARD_PIN(PTD, 5),		//!< PD5: CTS, XCK1
		PTD6 = BOARD_PIN(PTD, 6),		//!< PD6: ADC9,							T1	  					!OC4D
		PTD7 = BOARD_PIN(PTD, 7),		//!< PD7: ADC10, 				T0 								OC4D

		PTE6 = BOARD_PIN(PTE, 6),		//!< PE6: INT6, AIN0

		PTF0 = BOARD_PIN(PTF, 0),		//!< PF0: ADC0
		PTF1 = BOARD_PIN(PTF, 1),		//!< PF1: ADC1
		PTF4 = BOARD_PIN(PTF, 4),		//!< PF4: TCK, ADC4
		PTF5 = BOARD_PIN(PTF, 5),		//!< PF5: TMS, ADC5
		PTF6 = BOARD_PIN(PTF, 6),		//!< PF6: TDO, ADC6
		PTF7 = BOARD_PIN(PTF, 7),		//!< PF7: TDI, ADC7

		SS   = PTB0,					//!< SPI Slave Select
		SCK  = PTB1,					//!< SPI Clock
		MOSI = PTB2,					//!< SPI Master Output Slave Input
		MISO = PTB3,					//!< SPI Master Input Slave Output

		SCL  = PTD0, 					//!< TWI Clock
		SDA  = PTD1						//!< TWI Data
	} pins_t;

#elif defined(__AVR_Atmega168__) || defined(__AVR_ATmega328P__)
	/**
	 * @enum	ports_t
	 * @brief	Store information about the available ports for the board.
	 * 			The information is stored as a 16bit value, the MSB contains
	 * 			the PINx address and the LSB a mask with the available pins.
	 */
	typedef enum {
		NO_PORT,
		PTB = BOARD_PORT(0x23, 0x1F),	//!< PINB: 0x23 - Pins: 0 - 5.
		PTC = BOARD_PORT(0x26, 0x1F),	//!< PINC: 0x26 - Pins: 0 - 5.
		PTD = BOARD_PORT(0x29, 0xFF),	//!< PIND: 0x29 - Pins: 0 - 7.
	} ports_t;

	/**
	 * @enum	pins_t
	 * @brief	Store information about the board pins.
	 * 			The information is stored as a 16bit value, the MSB contains
	 * 			the PINx address and the LSB the pin position in the port.
	 */
	typedef enum {
		NO_PIN,
		PTB0 = BOARD_PIN(PTB, 0),		//!< PB0: PCINT0, CLK0, 				ICP1
		PTB1 = BOARD_PIN(PTB, 1),		//!< PB1: PCINT1, 						OC1A
		PTB2 = BOARD_PIN(PTB, 2),		//!< PB2: PCINT2, !SS, 					OC1B
		PTB3 = BOARD_PIN(PTB, 3),		//!< PB3: PCINT3, MOSI, 						OC2A
		PTB4 = BOARD_PIN(PTB, 4),		//!< PB4: PCINT4, MISO
		PTB5 = BOARD_PIN(PTB, 5),		//!< PB5: PCINT5, SCLK

		PTC0 = BOARD_PIN(PTC, 0),		//!< PC6: PCINT8, ADC0
		PTC1 = BOARD_PIN(PTC, 1),		//!< PC6: PCINT9, ADC1
		PTC2 = BOARD_PIN(PTC, 2),		//!< PC6: PCINT10, ADC2
		PTC3 = BOARD_PIN(PTC, 3),		//!< PC6: PCINT11, ADC3
		PTC4 = BOARD_PIN(PTC, 4),		//!< PC6: PCINT12, SDA, ADC4
		PTC5 = BOARD_PIN(PTC, 5),		//!< PC7: PCINT13, SCL. ADC5

		PTD0 = BOARD_PIN(PTD, 0),		//!< PD0: PCINT16, RXD
		PTD1 = BOARD_PIN(PTD, 1),		//!< PD1: PCINT17, TXD
		PTD2 = BOARD_PIN(PTD, 2),		//!< PD2: PCINT18, INT0
		PTD3 = BOARD_PIN(PTD, 3),		//!< PD3: PCINT19, INT1							OC2B
		PTD4 = BOARD_PIN(PTD, 4),		//!< PD4: PCINT20, XCK			T0
		PTD5 = BOARD_PIN(PTD, 5),		//!< PD5: PCINT21,  			OC0B	T1
		PTD6 = BOARD_PIN(PTD, 6),		//!< PD6: PCINT22, AIN0, 		OC0A
		PTD7 = BOARD_PIN(PTD, 7),		//!< PD7: PCINT23, AIN1

		SS   = PTB2,					//!< SPI Slave Select
		SCK  = PTB5,					//!< SPI Clock
		MOSI = PTB3,					//!< SPI Master Output Slave Input
		MISO = PTB4,					//!< SPI Master Input Slave Output

		SCL  = PTC5, 					//!< TWI Clock
		SDA  = PTC4						//!< TWI Data
	} pins_t;

#else
#error Board.h: AVR MCU not supported
#endif
//###############################################
#endif /* _HARDWARE_BOARD_H_ */
//###############################################
/* NOTES:
 * 	*(1): Atomic means that the expression will not be
 * 		interrupted while is executed. This is because
 * 		somthing atomic is "indivisible".
 */
