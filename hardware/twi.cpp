/**
 * @file 	libs/hardware/twi.cpp
 * @brief 	TWI Interrupt library.
 */
//###############################################
#include <hardware/twi.h>
//###############################################
TWI* TWI::_inst = NULL;						//!< Device singleton initialization.

//###############################################
fun_ptr_vd_u8 TWI::_user_on_receive;
fun_ptr_vd_vd TWI::_user_on_request;

//###############################################
TWI* TWI::get_instance(void) {
	if (!_inst) _inst = new TWI();
	return _inst;
}

void TWI::delete_instance(void) {
	if (!_inst) return;
	_inst->disable();
	delete _inst;
	_inst = NULL;
}

//===============================================
void TWI::disable(void) {
	// Disable TWI
	ATOMIC({
		// Disable slave (clear address)
		TWAR = 0x00;
		// Disable bus, interrupts and ACK generation
		TWCR &= ~(_BV(TWEN) | _BV(TWIE) | _BV(TWEA));
	});
	// Deactivate bus pins
	pin_write(SDA, false);
	pin_write(SCL, false);
	// Clear slave RX buffer (if any)
	if (_srx_buff_size > 0) {
		free(_srx_buff);
		_srx_buff = NULL;
		_srx_buff_size = 0;
	}
}

//===============================================
void TWI::set_clock(uint16_t freq) {
	// Calculate the new TWI clock
	uint8_t twbr = freq_sel(F_CPU, freq);
	// Set the new value
	ATOMIC({
		TWBR = twbr;
	})
}

//###############################################
void TWI::enable(void) {
	// Initialize TWI state machine, bus pins and clock
	init();
	// Configure master
	ATOMIC({
		// Enable bus, interrupts and ACK generation
		TWCR = (_BV(TWEN) | _BV(TWIE) | _BV(TWEA));
	});
}

//===============================================
uint16_t TWI::write(uint8_t addr, uint8_t src, bool stop) {
	return write(addr, &src, 1, stop);
}

uint16_t TWI::write(uint8_t addr, uint8_t* src, size_t size, bool stop) {
	// Check if the buffer is available
	if ((size == 0) || (src == NULL))
		return 0x0005;
	// Result aux.
	uint16_t result;
	// Wait until TWI is ready
	while (_state != TWI_READY);
	// Set TWI state machine
	_state = TWI_MTX;
	_error = TWI_ERR_NONE;
	_send_stop = stop;
	// Initialize master buffer vars.
	_mx_buff = src;
	_mx_buff_idx = 0;
	_mx_buff_size = size;
	// Build slave address
	_slarw = (addr << 1) | TW_WRITE;
	// Start TWI state machine
	start();
	// Wait until ready
	while (_state == TWI_MTX);
	// Get bytes transmitted
	result = (_mx_buff_idx << 8);
	// Handle errors
	switch (_error) {
		case TWI_ERR_NONE:		result |= 0x00;		break;
		case TW_MT_SLA_NACK:	result |= 0x01;		break;
		case TW_MT_DATA_NACK:	result |= 0x02; 	break;
		case TW_MT_ARB_LOST:	result |= 0x03;		break;
		default:				result |= 0x04; 	break;
	}
	// Return result
	return result;
}

uint16_t TWI::write_reg(uint8_t addr, uint8_t reg, uint8_t src, bool stop) {
	return write_reg(addr, reg, &src, 1, stop);
}

uint16_t TWI::write_reg(uint8_t addr, uint8_t reg, uint8_t* src, size_t size, bool stop) {
	// Check if the buffer is available
	if ((size == 0) || (src == NULL))
		return 0x05;
	// Put the "reg" and "src" in one auxiliary array and send
	uint8_t* reg_src = (uint8_t*) malloc((size + 1));
	memcpy(reg_src, &reg, 1);
	memcpy((reg_src + 1), src, size);
	// Write the register
	uint8_t result = write(addr, reg_src, (size + 1), stop);
	// Free the auxiliary space
	free(reg_src);
	// Return the result
	return result;
}

//===============================================
uint16_t TWI::read(uint8_t addr, uint8_t* rec, bool stop) {
	return read(addr, rec, 1, stop);
}

uint16_t TWI::read(uint8_t addr, uint8_t* rec, size_t size, bool stop) {
	// Check if the buffer is available
	if ((size == 0) || (rec == NULL))
		return 0x0005;
	// Result aux.
	uint16_t result;
	// Wait until TWI is ready
	while (_state != TWI_READY);
	// Set TWI state machine
	_state = TWI_MRX;
	_error = TWI_ERR_NONE;
	_send_stop = stop;
	// Initialize master buffer vars.
	_mx_buff = rec;
	_mx_buff_idx = 0;
	_mx_buff_size = size - 1;		// See note *(2)
	// Build slave address
	_slarw = (addr << 1) | TW_READ;
	// Start TWI state machine
	start();
	// Wait until ready
	while (_state == TWI_MRX);
	// Get bytes transmitted
	result = (_mx_buff_idx << 8);
	// Handle errors
	switch (_error) {
		case TWI_ERR_NONE:		result |= 0x00;		break;
		case TW_MR_SLA_NACK:	result |= 0x01;		break;
		case TW_MR_ARB_LOST:	result |= 0x03;		break;
		default:				result |= 0x04; 	break;
	}
	// Return result
	return result;
}

uint16_t TWI::read_reg(uint8_t addr, uint8_t reg, uint8_t* rec, bool stop) {
	return read_reg(addr, reg, rec, 1, stop);
}

uint16_t TWI::read_reg(uint8_t addr, uint8_t reg, uint8_t* rec, size_t size, bool stop) {
	// Locate the register
	uint16_t result = write(addr, &reg, 1, false);
	// If an error is detected, return
	if ((result & 0x0F) != 0)
		return result;
	// Read bytes
	result = read(addr, rec, size, stop);
	// Return the result
	return result;
}

//===============================================
TWI::Device::Device(void): _dev_twi(NULL), _dev_addr(0x00) {
}

TWI::Device::Device(TWI* twi, uint8_t addr): _dev_twi(twi), _dev_addr(addr) {
}

uint16_t TWI::Device::write(uint8_t src, bool stop) {
	return _dev_twi->write(_dev_addr, src, stop);
}

uint16_t TWI::Device::write(uint8_t* src, size_t size, bool stop){
	return _dev_twi->write(_dev_addr, src, size, stop);
}

uint16_t TWI::Device::write_reg(uint8_t reg, uint8_t src, bool stop) {
	return _dev_twi->write_reg(_dev_addr, reg, src, stop);
}

uint16_t TWI::Device::write_reg(uint8_t reg, uint8_t* src, size_t size, bool stop) {
	return _dev_twi->write_reg(_dev_addr, reg, src, size, stop);
}

uint16_t TWI::Device::read(uint8_t* rec, bool stop) {
	return _dev_twi->read(_dev_addr, rec, stop);
}

uint16_t TWI::Device::read(uint8_t* rec, size_t size, bool stop) {
	return _dev_twi->read(_dev_addr, rec, size, stop);
}

uint16_t TWI::Device::read_reg(uint8_t reg, uint8_t* rec, bool stop) {
	return _dev_twi->read_reg(_dev_addr, reg, rec, stop);
}

uint16_t TWI::Device::read_reg(uint8_t reg, uint8_t* rec, size_t size, bool stop) {
	return _dev_twi->read_reg(_dev_addr, reg, rec, size, stop);
}

//###############################################
void TWI::enable(uint8_t addr, uint8_t rx_buff_size) {
	// Check the desired buffer length
	if (rx_buff_size == 0)
		return;
	// Set the slave RX buffer
	_srx_buff_size = rx_buff_size;
	_srx_buff = (uint8_t *)malloc((rx_buff_size * sizeof(uint8_t)));
	// Set callbacks
	_user_on_receive = fun_vd_u8_dummy;
	_user_on_request = fun_vd_vd_dummy;
	// Initialize state machine, pins and clock
	init();
	// Configure slave
	ATOMIC({
		// Set TWI address (Activate slave mode)
		TWAR = addr << 1;
		// Enable bus, interrupts and ACK generation
		TWCR = (_BV(TWEN) | _BV(TWIE) | _BV(TWEA));
	});
}

//===============================================
void TWI::on_receive(fun_ptr_vd_u8 fun) {
	_user_on_receive = (fun)? fun : fun_vd_u8_dummy;
}

uint8_t TWI::get_rx_buff(uint8_t idx) {
	return _srx_buff[idx];
}

//===============================================
void TWI::on_request(fun_ptr_vd_vd fun) {
	_user_on_request = (fun)? fun : fun_vd_vd_dummy;
}

void TWI::send_response(uint8_t* tx_buff, uint8_t tx_buff_size) {
	// Check if the buffer is available
	if ((tx_buff == NULL) || (tx_buff_size == 0))
		return;
	// Set slave TX buffer
	_stx_buff = tx_buff;
	_stx_buff_size = tx_buff_size;
}

//###############################################
TWI::TWI(void) {
	_mx_buff = NULL;
	_srx_buff = NULL;
	_stx_buff = NULL;
	_user_on_receive = fun_vd_u8_dummy;
	_user_on_request = fun_vd_vd_dummy;
}

//===============================================
uint8_t TWI::freq_sel(uint32_t xtal_cpu, uint32_t freq) {
	return (uint8_t)((xtal_cpu / (2UL * freq * 1000UL)) - 8UL);
}

//===============================================
void TWI::init(void) {
	// Set initial TWI SM state
	_state = TWI_READY;
	_send_stop = true;
	_rep_start = false;
	// Activate pullups for TWI
	pin_write(SDA, true);
	pin_write(SCL, true);
	// Configure basics
	ATOMIC({
		// Clean TWI control register
		TWCR = 0x00;
		// Initialize TWI prescaler as 1
		TWBR = 0x00;
		// Set default frequency
		TWBR = freq_sel(F_CPU, TWI_DEF_FREQ_KHZ);
	});
}

void TWI::start(void) {
	if (_rep_start) {
		// Clear rep_start flag.
		_rep_start = false;
		// Write slave address on data register
		do {
			TWDR = _slarw;
		} while (TWCR & _BV(TWWC));		// Handle collisions
		// Enable interruptions and send address (start already set, see *(1)).
		TWCR = (_BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA));
	} else {
		// Send start condition
		TWCR = (_BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTA));
	}
}

void TWI::stop(void) {
	// Transmit stop condition
	TWCR = (_BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTO));
	// Wait to stop condition to be executed
	loop_until_bit_is_clear(TWCR, TWSTO);
	// Update TWI SM state
	_state = TWI_READY;
}

void TWI::release_bus(void) {
	// Release bus
	TWCR = (_BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA));
	// Update TWI SM state
	_state = TWI_READY;
}

void TWI::reply(bool ack) {
	if (ack) {
		TWCR = (_BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA));
	} else {
		TWCR = (_BV(TWEN) | _BV(TWIE) | _BV(TWINT));
	}
}

//===============================================
void TWI::on_receive_callback(uint8_t last_rx_idx) {
	_user_on_receive(last_rx_idx);
}

void TWI::on_request_callback(void) {
	_user_on_request();
}

//===============================================
void TWI::state_machine(void) {
	/* TWI state machine */
	switch (TW_STATUS) {
		/****************************************
		 * MASTER:
		 * 1.- Start / Rep. Start:
		 * 		1.1.- Copy slave address to buffer.
		 * 		1.2.- Send address (expect ACK).
		 ***************************************/
		case TW_START:
		case TW_REP_START: {
			TWDR = _slarw;
			reply(true);
		} break;
		/****************************************
		 * MASTER TRANSMITTER:
		 * 1.- Slave address/data sent, ACK received:
		 * 		1.1.- If there's data to send (idx < len):
		 * 			1.1.1.- Copy byte to buffer.
		 * 			1.1.2.- Send byte (expect ACK).
		 * 		1.2.- Otherwise, go to "Master End Transaction Handler"
		 ***************************************/
		case TW_MT_SLA_ACK:
		case TW_MT_DATA_ACK: {
			if (_mx_buff_idx < _mx_buff_size) {
				TWDR = _mx_buff[_mx_buff_idx++];
				reply(true);
			} else {
				goto _tw_meth;
			}
		} break;
		/****************************************
		 * MASTER RECEIVER:
		 * 1.- Slave address sent, ACK received:
		 * 		1.1.- If data is expected (idx < len), send ACK (prepare bus for reception).
		 * 		1.2.- Otherwise, send NACK.
		 ***************************************/
		case TW_MR_SLA_ACK: {
			_tw_mrgdh:		// Master Receiver Get Data Handler
			if (_mx_buff_idx < _mx_buff_size) {
				reply(true);
			} else {
				reply(false);
			}
		} break;
		/* 2.- Slave data received, ACK sent:
		 * 		2.1.- Copy byte from buffer.
		 * 		2.2.- Go to "Master Receiver Get Data Handler" (MR 1.-)
		 ***************************************/
		case TW_MR_DATA_ACK: {
			_mx_buff[_mx_buff_idx++] = TWDR;
			goto _tw_mrgdh;
		} break;
		/* 3.- Slave data received, NACK sent:
		 * 		3.1.- Copy final byte from buffer.
		 * 		3.2.- Go to "Master End Transaction Handler"
		 ***************************************/
		case TW_MR_DATA_NACK: {
			_mx_buff[_mx_buff_idx] = TWDR;
			goto _tw_meth;
		} break;
		/****************************************
		 * If "Bus Error" or "Slave address sent, NACK received" (MT/MR)
		 * or "Data sent, NACK received" (MT):
		 * 		1.- Save error.
		 * 		2.- Stop transaction.
		 ***************************************/
		case TW_MT_SLA_NACK:
		case TW_MR_SLA_NACK:
		case TW_MT_DATA_NACK: {
			_error = TW_STATUS;
			stop();
		} break;
		/****************************************
		 * If "Arbitration lost" (MT/MR):
		 * 		1.- Save error.
		 * 		2.- Release bus.
		 ***************************************/
		case TW_MT_ARB_LOST: {
			_error = TW_STATUS;
			release_bus();
		} break;
		/****************************************
		 * SLAVE TRANSMITTER
		 * 1.- Adressed / Arbitration lost, ACK sent:
		 * 		1.1.- Enter slave TX mode (STX).
		 * 		1.2.- Reset TX data size and index.
		 * 		1.3.- Execute slave TX event: Must prepare data to send using
		 * 			twi_slave_write(uint8_t* data, size_t size). If data is
		 * 			not set, prepare to send 0x00.
		 * 		1.4.- Go to "Slave Transmitter Send Data Handler" (ST 2.-)
		 ***************************************/
		case TW_ST_SLA_ACK:
		case TW_ST_ARB_LOST_SLA_ACK: {
			_state = TWI::TWI_STX;
			_stx_buff_idx = 0;
			_stx_buff_size = 0;
			on_request_callback();
			if (_stx_buff_size == 0) {
				_stx_buff_size = 1;
				*_stx_buff = 0x00;
			}
			goto _tw_stsdh;
		} break;
		/* 2.- Data sent, ACK received:
		 *		2.1.- Put byte on buffer.
		 *		2.3.- If there's data to send (idx < len), send byte (expect ACK).
		 *		2.4.- Otherwise, send byte (expect NACK).
		 ***************************************/
		case TW_ST_DATA_ACK: {
			_tw_stsdh:		// Slave Transmitter Send Data Handler
			TWDR = _stx_buff[_stx_buff_idx++];
			if (_stx_buff_idx < _stx_buff_size) {
				reply(true);
			} else {
				reply(false);
			}
		} break;
		/* 3.- Data sent / Last data sent, NACK received:
		 * 		3.1.- ACK future responses.
		 * 		3.2.- Leave slave TX state.
		 ***************************************/
		case TW_ST_DATA_NACK:
		case TW_ST_LAST_DATA: {
			reply(true);
			_state = TWI::TWI_READY;
		} break;
		/****************************************
		 * SLAVE RECEIVER
		 * 1.- Adressed / Arbitration lost, ACK sent:
		 * 		1.1.- Enter slave RX mode (SRX).
		 * 		1.2.- Reset RX data index.
		 * 		1.3.- Prepare line for reception (send ACK).
		 ***************************************/
		case TW_SR_SLA_ACK:
		case TW_SR_GCALL_ACK:
		case TW_SR_ARB_LOST_SLA_ACK:
		case TW_SR_ARB_LOST_GCALL_ACK: {
			_state = TWI::TWI_SRX;
			_srx_buff_idx = 0;
			reply(true);
		} break;
		/* 2.- Data received, ACK sent:
		 * 		2.1.- If there's space on buffer:
		 * 			2.1.1.- Copy byte from buffer.
		 * 			2.1.2.- Send ACK (expect more data).
		 * 		2.2.- Otherwise, send NACK.
		 ***************************************/
		case TW_SR_DATA_ACK:
		case TW_SR_GCALL_DATA_ACK: {
			if (_srx_buff_idx < _srx_buff_size) {
				_srx_buff[_srx_buff_idx++] = TWDR;
				reply(true);
			} else {
				reply(false);
			}
		} break;
		/* 3.- Data received, NACK sent:
		 * 		3.1.- Send NACK.
		 ***************************************/
		case TW_SR_DATA_NACK:
		case TW_SR_GCALL_DATA_NACK: {
			reply(false);
		} break;
		/* 4.- Stop / Rep. start received:
		 * 		4.1.- Release bus.
		 * 		4.2.- If there's space, put a termination value on Rx buffer ('\0').
		 * 		4.3.- Execute slave RX event: Must handle input data using
		 * 			twi_slave_rx_event(uint8_t last_idx). Where last_idx points the
		 * 			last index used on the RX buffer.
		 * 		4.4.- Reset RX buffer index.
		 ***************************************/
		case TW_SR_STOP: {
			release_bus();
			if (_srx_buff_idx < _srx_buff_size) {
				_srx_buff[_srx_buff_idx] = '\0';
			}
			on_receive_callback(_srx_buff_idx);
			_srx_buff_idx = 0;
		} break;
		/****************************************
		 * If "Slave address sent, NACK received" (MT/MR)
		 * or "Data sent, NACK received" (MT):
		 * 		1.- Save error.
		 * 		2.- Stop transaction.
		 ***************************************/
		case TW_BUS_ERROR: {
			_error = TW_STATUS;
			stop();
		} break;
		/****************************************
		 * If "No state information" (any):
		 *		1.- Do nothing.
		 ***************************************/
		case TW_NO_INFO: {
		} break;
	}
	/* End of ISR */
	return;

	/********************************************
	 * Master End Transaction Handler
	 * 		1.1.- If send_stop flag is active:
	 * 			1.1.1.- Stop transaction.
	 * 		1.2.- Otherwise, prepare rep. start.
	 * 			1.2.1.- Put rep_start flag active.
	 * 			1.2.2.- Send start but disable interruptions*(1).
	 * 			1.2.3.- Set TWI state machine as ready.
	 ********************************************/
	_tw_meth: {
		if (_send_stop) {
			stop();
		} else {
			_rep_start = true;
			TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTA);
			_state = TWI::TWI_READY;
		}
	} return;
}

//###############################################
/**
 * @brief	TWI ISR.
 */
ISR(TWI_vect) {
	TWI::_inst->state_machine();
}

//###############################################
/************************************************
NOTES:
	*(1): In this case, we're gonna send the START
		and don't enable the interrupt. The idea is
		that the transaction will be resumed on the
		next action (read/write).
		If rep_start flag is active the start procedure
		will be slightly different (on the action
		function):
			1.- Put the target address on buffer.
			2.- Send the address and wait for ACK.
		Instead of:
			1.- Send the start condition.
			2.- Let the TWI state machine send the
				address.

	*(2): We receive the byte, advance the index and
		at least we check if is more space available in
		the buffer. So we have to know if the next byte
		is the last one (to send the NACK).
 ***********************************************/


