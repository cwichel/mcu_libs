/**
 * @code 	#include "hardware/uart.h" @endcode
 * @file	libs/hardware/uart.h
 * @brief	AVR interrupt UART library.
 */
//###############################################
#ifndef _HARDWARE_UART_H_
#define _HARDWARE_UART_H_
//###############################################
#include <hardware/uart_defs.h>
//###############################################
#if defined(AVR_UART0)
ISR(UART0_RX_VECT);						//!< Declare this to simplify the RX0 ISR declaration.
ISR(UART0_TX_VECT);						//!< Declare this to simplify the TX0 ISR declaration.
#endif
#if defined(AVR_UART1)
ISR(UART1_RX_VECT);						//!< Declare this to simplify the RX1 ISR declaration.
ISR(UART1_TX_VECT);						//!< Declare this to simplify the TX1 ISR declaration.
#endif

//###############################################
/**
 * @class	UART
 * @brief	UART bus general implementation.
 */
class UART : public Stream {
public:
	//======================================
	/** This allows to use the variants of write function. */
	using Stream::write;

	//======================================
	/**
	 * @brief	Get an instance for an specific device.
	 * @param	uart		UART device.
	 * @return	Device singleton.
	 */
	static UART* get_instance(uart_inst_t uart);

	/**
	 * @brief	Deletes the instance of an specific device. Note that this
	 * 			action will turn off the interface and delete all the
	 * 			configurations.
	 * @param	uart		UART device.
	 */
	static void delete_instance(uart_inst_t uart);

	//======================================
	/**
	 * @brief	Enable UART and set baudrate.
	 * @param	baudrate	Specify the baudrate.
	 */
	void enable(uint32_t baudrate);

	/**
	 * @brief	General implementation for enable UART.
	 * @param	baudrate	Specify the baudrate.
	 * @param	msgbit		Data packet size.
	 * @param	stbit		Stop bits.
	 * @param	pmode		Parity mode.
	 */
	void enable(uint32_t baudrate, uart_msgbit_t msgbit, uart_stbit_t stbit, uart_pmode_t pmode);

	/**
	 * @brief 	Finishes the transmission and disable the UART.
	 */
	void disable(void);

	//======================================
	/** @copydoc Stream::rx_available() */
	int16_t rx_available(void);

	/** @copydoc Stream::peek() */
	int16_t peek(void);

	/**
	 * @copydoc Stream::read()
	 * @note	After the byte is retrieved the buffer index will be incremented
	 * 			giving space for more bytes to be read.
	 */
	int16_t read(void);

	//======================================
	/** @copydoc Stream::tx_available(void) */
	uint8_t tx_available(void);

	/** @copydoc Stream::flush(void) */
	void flush(void);

	/** @copydoc Stream::flush(void) */
	size_t write(uint8_t byte);

	//======================================
	/**
	 * @brief	This callback is used to handle the reception of data packets.
	 */
	inline void _rx_callback(void);

	/**
	 * @brief	This callback is used to handle the transmission of data packets.
	 */
	inline void _tx_callback(void);

protected:
	//======================================
	#if defined(AVR_UART0)
	static UART*	_inst_0;							//!< UART0 device Singleton.
	friend void UART0_RX_VECT();						//!< Used to make available private UART0 methods on the RX0 ISR.
	friend void UART0_TX_VECT();						//!< Used to make available private UART0 methods on the TX0 ISR.
	#endif
	#if defined(AVR_UART1)
	static UART*	_inst_1;							//!< UART1 device Singleton.
	friend void UART1_RX_VECT();						//!< Used to make available private UART1 methods on the RX1 ISR.
	friend void UART1_TX_VECT();						//!< Used to make available private UART1 methods on the TX1 ISR.
	#endif

	//======================================
	volatile uart_ctrl_t*  	_ctrl;						//!< UART control registers.
	volatile uint16_t*		_baud;						//!< UART baudrate register.
	volatile uint8_t* 		_data;						//!< UART data register.
	volatile bool 			_used;						//!< Flag that indicates if the bus was used.
	volatile uint8_t 		_rx_buff[UART_BUFF_SIZE];	//!< RX buffer.
	volatile uint8_t 		_rx_buff_head;				//!< RX buffer head index.
	volatile uint8_t 		_rx_buff_tail;				//!< RX buffer tail index.
	volatile uint8_t 		_tx_buff[UART_BUFF_SIZE];	//!< TX buffer.
	volatile uint8_t 		_tx_buff_head;				//!< TX buffer head index.
	volatile uint8_t 		_tx_buff_tail;				//!< TX buffer tail index.

	//======================================
	/**
	 * @brief	UART constructor.
	 * @param	ctrl		UART control registers initial address.
	 * @param	baud		UART baudrate register address.
	 * @param	data		UART data register address.
	 */
	inline UART(volatile uint8_t* ctrl, volatile uint16_t* baud, volatile uint8_t* data);

};

//###############################################
#endif /* _HARDWARE_UART_H_ */
//###############################################
