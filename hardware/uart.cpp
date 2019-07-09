/**
 * @file 	libs/hardware/uart.cpp
 * @brief 	AVR interrupt UART library.
 */
//###############################################
#include <hardware/uart.h>
//###############################################
#if defined(AVR_UART0)|| defined(AVR_UART1)
//###############################################
/**
 * @brief	Used to simplify the UARTx ISR definition.
 * @param	vect			ISR vector.
 * @param	uart			UARTx object.
 * @param	fun				UART RX/TX callback.
 */
#define UART_ISR(vect, inst, fun)	\
	ISR(vect) {						\
		inst->fun();				\
	}

//###############################################
#if defined(AVR_UART0)
UART* UART::_inst_0 = NULL;								//!< UART0 device singleton initialization.
UART_ISR(UART0_RX_VECT, UART::_inst_0, _rx_callback)	//!< UART0 on-receive action.
UART_ISR(UART0_TX_VECT, UART::_inst_0, _tx_callback)	//!< UART0 on-transmit action.
#endif
#if defined(AVR_UART1)
UART* UART::_inst_1 = NULL;								//!< UART1 device singleton initialization.
UART_ISR(UART1_RX_VECT, UART::_inst_1, _rx_callback)	//!< UART1 on-receive action.
UART_ISR(UART1_TX_VECT, UART::_inst_1, _tx_callback)	//!< UART1 on-transmit action.
#endif

//###############################################
UART* UART::get_instance(uart_inst_t uart) {
	switch (uart) {
		#if defined(AVR_UART0)
		case UART_0: {
			if (!_inst_0) _inst_0 = new UART(&UART0_CTRL, &UART0_UBRR, &UART0_DATA);
			return _inst_0;
		} break;
		#endif
		#if defined(AVR_UART1)
		case UART_1: {
			if (!_inst_1) _inst_1 = new UART(&UART1_CTRL, &UART1_UBRR, &UART1_DATA);
			return _inst_1;
		} break;
		#endif
		default: return NULL; break;
	}
}

void UART::delete_instance(uart_inst_t uart) {
	switch (uart) {
		#if defined(AVR_UART0)
		case UART_0: {
			if (!_inst_0) return;
			_inst_0->disable();
			delete _inst_0;
			_inst_0 = NULL;
		} break;
		#endif
		#if defined(AVR_UART1)
		case UART_1: {
			if (!_inst_1) return;
			_inst_1->disable();
			delete _inst_1;
			_inst_1 = NULL;
		} break;
		#endif
	}
}

//###############################################
void UART::enable(uint32_t baudrate) {
	enable(baudrate, UART_MSG_8BIT, UART_ST_1BIT, UART_PAR_DISABLED);
}

void UART::enable(uint32_t baudrate, uart_msgbit_t msgbit, uart_stbit_t stbit, uart_pmode_t pmode) {
	uint32_t baud;
	// Prepare and set the baudrate
	baud = (F_CPU / 4 / baudrate - 1) / 2;
	if((baud > 4095) || ((F_CPU == 16000000) && (baudrate == 57600))) {
		// If the value is invalid or a special case is given, use the 1x mode
		baud = (F_CPU / 8 / baudrate - 1) / 2;
		_ctrl->stat.field.u2x = false;
	} else {
		// Otherwise, 2X mode.
		_ctrl->stat.field.u2x = true;
	}
	// Set everything
	ATOMIC({
		// Write the baudrate
		*_baud = baud;
		// Set the data bits, parity and stop bits
		_ctrl->ctrl2.raw = 0x00;
		_ctrl->ctrl2.field.upm = pmode;
		_ctrl->ctrl2.field.usbs = stbit;
		_ctrl->ctrl2.field.ucsz = msgbit;
		// Enable TX/RX and RX interruptions
		_ctrl->ctrl1.field.rxen = true;
		_ctrl->ctrl1.field.txen = true;
		_ctrl->ctrl1.field.rxcie = true;
	});
}

void UART::disable() {
	// Finish the TX action
	flush();
	// Disable the UART
	ATOMIC({
		_ctrl->ctrl1.field.rxen = false;
		_ctrl->ctrl1.field.txen = false;
		_ctrl->ctrl1.field.rxcie = false;
		_ctrl->ctrl1.field.txcie = false;
		_rx_buff_head = _rx_buff_tail;
	});
}

//===============================================
int16_t UART::rx_available(void) {
	// The byte reading from the buffer is made from the tail to the head.
	// In this context the tail point the start point and the head is used
	// to point the last space that is written.
	uint8_t out;
	ATOMIC({
		out = ((UART_BUFF_SIZE + _rx_buff_head - _rx_buff_tail) & UART_BUFF_MASK);
	});
	return out;
}

int16_t UART::peek(void) {
	// Check if there are bytes available
	ATOMIC({
		if (_rx_buff_head == _rx_buff_tail)
			return -1;
	});
	// Get the next byte
	uint8_t next_tail = (_rx_buff_tail + 1) & UART_BUFF_MASK;
	return _rx_buff[next_tail];
}

int16_t UART::read(void) {
	// Check if there are bytes available
	ATOMIC({
		if (_rx_buff_head == _rx_buff_tail)
			return -1;
	});
	// Read the byte and move the tail
	_rx_buff_tail = (_rx_buff_tail + 1) & UART_BUFF_MASK;
	return _rx_buff[_rx_buff_tail];
}

//===============================================
uint8_t UART::tx_available(void) {
	uint8_t tail;
	uint8_t head;
	// Get the head and tail values
	ATOMIC({
		head = _tx_buff_head;
		tail = _tx_buff_tail;
	});
	// Return the available bytes
	if (head >= tail)	return (UART_BUFF_SIZE - head + tail);
	else 				return (tail - head - 1);
}

void UART::flush(void) {
	// If the UART was never used to send a byte, return.
	if (!_used)
		return;
	// Otherwise, wait until the writing is completed.
	// While a transmission is completed or the tx data interrupt is active
	while (_ctrl->stat.field.txc || _ctrl->ctrl1.field.udrie) {
		// Check if the interruptions are on
		if (bit_is_clear(SREG, SREG_I) && _ctrl->ctrl1.field.udrie)
			// If the buffer is available, write
			if (_ctrl->stat.field.udre) _tx_callback();
	}
}

size_t UART::write(uint8_t byte) {
	uint8_t idx;
	// The TX was used
	_used = true;
	// If the buffer is empty and available, write the byte directly
	// and clear TXC to avoid malfunction on the UART
	if ((_tx_buff_head == _tx_buff_tail) && _ctrl->stat.field.udre) {
		ATOMIC({
			*_data = byte;
			_ctrl->stat.field.txc = true;
		});
		return 1;
	}
	// If the buffer has data we have to append the new byte after the
	// last header index.
	idx = (_tx_buff_head + 1) & UART_BUFF_MASK;
	// If the TX buffer is full, wait for space
	while (idx == _tx_buff_tail) {
		// In the case that the interrupts are disabled, we have to
		// poll the buffer ready flag and call the handler.
		if (bit_is_clear(SREG, SREG_I)) {
			if (_ctrl->stat.field.udre) _tx_callback();
		}
	}
	// Append the new value to buffer
	_tx_buff[idx] = byte;
	// Update the head index and enable the interrupts
	ATOMIC({
		_tx_buff_head = idx;
		_ctrl->ctrl1.field.udrie = true;
	});
	return 1;
}

//===============================================
UART::UART(volatile uint8_t* ctrl, volatile uint16_t* baud, volatile uint8_t* data): Stream(),
	_ctrl((uart_ctrl_t*) ctrl), _baud((uint16_t*) baud), _data(data), _used(false) {
	// Initialize the buffer indexes
	ATOMIC({
		_rx_buff_head = 0;
		_rx_buff_tail = 0;
		_tx_buff_head = 0;
		_tx_buff_tail = 0;
	});
}

//===============================================
inline void UART::_rx_callback(void) {
	// Check status
	if (!_ctrl->stat.field.upe) {
		// No parity error, get data and update index
		uint8_t new_head = (_rx_buff_head + 1) & UART_BUFF_MASK;
		uint8_t data = *_data;
		// Only store if the space is available!
		if (new_head != _rx_buff_tail) {
			_rx_buff[new_head] = data;
			_rx_buff_head = new_head;
		}
	} else {
		// Parity error, discard byte
		*_data;
	}
}

inline void UART::_tx_callback(void) {
	// Check head/tail
	if (_tx_buff_head != _tx_buff_tail) {
		// Data available to send! Send more!
		_tx_buff_tail = (_tx_buff_tail + 1) & UART_BUFF_MASK;
		*_data = _tx_buff[_tx_buff_tail];
	} else {
		// No more data available. Stop interruptions!
		_ctrl->ctrl1.field.udrie = false;
	}
}

//###############################################
#endif
//###############################################
