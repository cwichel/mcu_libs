/**
 * @file 	libs/hardware/spi.cpp
 * @brief 	SPI library.
 */
//###############################################
#include <hardware/spi.h>
//###############################################
SPI* SPI::_inst = NULL;						//!< Device singleton initialization.

//###############################################
pins_t 	SPI::_ss_pin;
bool	SPI::_slave_active;
fun_ptr_u8_u8 SPI::_user_on_receive;
fun_ptr_vd_vd SPI::_user_on_request;

//###############################################
SPI* SPI::get_instance(void) {
	if (!_inst) _inst = new SPI();
	return _inst;
}

void SPI::delete_instance(void) {
	if (!_inst) return;
	_inst->disable();
	delete _inst;
	_inst = NULL;
}

//===============================================
void SPI::disable(void) {
	// Disable SPI (and the interrupts)
	ATOMIC({
		SPCR &= ~(_BV(SPIE) | _BV(SPE));
	});
}

//###############################################
void SPI::enable(void) {
	// Set SPI mode
	_mode = SPI_MASTER;
	// Set SS as master
	pin_mode(SS, OUTPUT);
	pin_write(SS, true);
	// Set SCK, MOSI and MISO
	pin_mode(SCK,  OUTPUT);
	pin_mode(MOSI, OUTPUT);
	pin_mode(MISO, INPUT);
}

void SPI::start(spi_conf_t* conf) {
	// Only for master mode
	if (_mode != SPI_MASTER)
		return;
	// Setup with given config.
	ATOMIC({
		SPCR = conf->ctrl.raw;
		SPSR = conf->stat.raw;
	});
}

void SPI::get_config(spi_conf_t* conf) {
	get_config(conf, SPI_MAX_FREQ/2, SPI_MODE_0, MSB_FIRST);
}

void SPI::get_config(spi_conf_t* conf, uint32_t freq, spi_tx_mode_t tx_mode, spi_ds_mode_t bit_order) {
	// Get prescaler
	uint8_t prescaler = (freq > SPI_MAX_FREQ) ? get_prescaler(SPI_MAX_FREQ) : get_prescaler(freq);
	// Set control register
	conf->ctrl.raw 			= 0x00;
	conf->ctrl.field.spe 	= true;
	conf->ctrl.field.mstr	= true;
	conf->ctrl.field.dord	= bit_order;
	conf->ctrl.field.cconf	= tx_mode;
	conf->ctrl.field.spr	= (prescaler & 0x03);
	// Set last prescaler bit on status register
	conf->stat.raw 			= 0x00;
	conf->stat.field.spi2x	= ((prescaler >> 2) & 0x01);
}

//===============================================
uint8_t SPI::transfer(uint8_t data) {
	// Only for master mode
	if (_mode != SPI_MASTER)
		return 0x00;
	// Prepare data
	SPDR = data;
	// Wait until the data is sent
	__asm__ __volatile__("nop" ::: "memory");
	loop_until_bit_is_set(SPSR, SPIF);
	// Return data received
	return SPDR;
}

void SPI::transfer(uint8_t* src, uint8_t* rec, size_t size, uint8_t off, uint16_t dly) {
	// Only for master mode
	if (_mode != SPI_MASTER)
		return;
	// Check buffers
	if ((size == 0) || (src == NULL) || (rec == NULL))
		return;
	// Prepare
	uint8_t state = (off > 0)? 0 : 1;
	// Transfer
	for (uint16_t idx = 0; idx < (size + off); idx++) {
		switch (state) {
			case 0: {
				transfer(*src++);
				if (idx >= (uint8_t)(off - 1)) state = 1;
			} break;
			case 1: {
				*rec++ = transfer(*src++);
				if (idx >= (uint8_t)(size - 1)) state = 2;
			} break;
			case 2: {
				*rec++ = transfer(0x00);
			} break;
		}
		if (dly > 0) delay_micros(dly);
	}
}

void SPI::write(uint8_t src, uint16_t dly) {
	write(&src, 1, dly);
}

void SPI::write(uint8_t *src, size_t size, uint16_t dly) {
	// Only for master mode
	if (_mode != SPI_MASTER)
		return;
	// Check buffer
	if ((size == 0) || (src == NULL))
		return;
	// Send
	while (size--) {
		transfer(*src++);
		if (dly > 0) delay_micros(dly);
	}
}

void SPI::write_reg(uint8_t reg, uint8_t src, uint16_t dly) {
	write_reg(reg, &src, 1, dly);
}

void SPI::write_reg(uint8_t reg, uint8_t* src, size_t size, uint16_t dly) {
	// Only for master mode
	if (_mode != SPI_MASTER)
		return;
	// Check buffer
	if ((size == 0) || (src == NULL))
		return;
	// Put the "reg" and "src" in one auxiliary array and send
	uint8_t* reg_src = (uint8_t*) malloc((size + 1));
	memcpy(reg_src, &reg, 1);
	memcpy((reg_src + 1), src, size);
	// Write the register
	write(reg_src, size, dly);
	// Free the auxiliary space
	free(reg_src);
}

//===============================================
void SPI::read(uint8_t *rec, size_t size, uint16_t dly) {
	// Only for master mode
	if (_mode != SPI_MASTER)
		return;
	// Check buffer
	if ((size == 0) || (rec == NULL))
		return;
	// Receive
	while (size--) {
		*rec++ = transfer(0x00);
		if (dly > 0) delay_micros(dly);
	}
}

void SPI::read_reg(uint8_t reg, uint8_t* rec, uint16_t dly) {
	read_reg(reg, rec, 1, dly);
}

void SPI::read_reg(uint8_t reg, uint8_t* rec, size_t size, uint16_t dly) {
	// Only for master mode
	if (_mode != SPI_MASTER)
		return;
	// Check buffer
	if ((size == 0) || (rec == NULL))
		return;
	// Write the register
	write(&reg, 1, dly);
	// Read the data
	read(rec, size, dly);
}

//======================================
SPI::Device::Device(void):
		_dev_spi(NULL), _dev_ss(NO_PIN), _dev_on(false) {
}

SPI::Device::Device(SPI* spi, pins_t ss_pin, bool dev_on):
		_dev_spi(spi), _dev_ss(ss_pin), _dev_on(dev_on) {
	_dev_spi->get_config(&_dev_conf);
	pin_mode(ss_pin, OUTPUT);
	pin_write(ss_pin, !_dev_on);
}

void SPI::Device::configure(uint32_t freq, spi_tx_mode_t tx_mode, spi_ds_mode_t bit_order) {
	_dev_spi->get_config(&_dev_conf, freq, tx_mode, bit_order);
}

void SPI::Device::begin_transaction(void) {
	_dev_spi->start(&_dev_conf);
	pin_write(_dev_ss, _dev_on);
}

void SPI::Device::end_transaction(void) {
	pin_write(_dev_ss, !_dev_on);
}

uint8_t SPI::Device::transfer(uint8_t data) {
	return _dev_spi->transfer(data);
}

void SPI::Device::transfer(uint8_t* src, uint8_t* rec, size_t size, uint8_t off, uint16_t dly) {
	_dev_spi->transfer(src, rec, size, off, dly);
}

void SPI::Device::write(uint8_t src, uint16_t dly) {
	_dev_spi->write(src, dly);
}

void SPI::Device::write(uint8_t* src, size_t size, uint16_t dly) {
	_dev_spi->write(src, size, dly);
}

void SPI::Device::write_reg(uint8_t reg, uint8_t src, uint16_t dly) {
	_dev_spi->write_reg(reg, src, dly);
}

void SPI::Device::write_reg(uint8_t reg, uint8_t* src, size_t size, uint16_t dly) {
	_dev_spi->write_reg(reg, src, size, dly);
}

void SPI::Device::read(uint8_t* rec, size_t size, uint16_t dly) {
	_dev_spi->read(rec, size, dly);
}

void SPI::Device::read_reg(uint8_t reg, uint8_t* rec, uint16_t dly) {
	_dev_spi->read_reg(reg, rec, dly);
}

void SPI::Device::read_reg(uint8_t reg, uint8_t* rec, size_t size, uint16_t dly) {
	_dev_spi->read_reg(reg, rec, size, dly);
}

//###############################################
bool SPI::enable(pins_t ss_int, fun_ptr_vd_vd fun) {
	bool status;
	// Set user-defined callbacks.
	_user_on_request = (fun)? fun : fun_vd_vd_dummy;
	// Set pin interrupt: try first with external, if not, with pin-change
	pin_mode(ss_int, INPUT_PULLUP);
	status = int_ext_attach(ss_int, on_request_callback, CHANGE);
	status = (!status)? int_pc_attach(ss_int, on_request_callback, CHANGE) : status;
	if (status) {
		// Save pin
		_ss_pin = ss_int;
		// Set SPI mode
		_mode = SPI_SLAVE;
		// Set SS as slave
		pin_mode(SS,   INPUT_PULLUP);
		// Set SCK, MOSI and MISO
		pin_mode(SCK,  INPUT);
		pin_mode(MOSI, INPUT);
		pin_mode(MISO, OUTPUT);
		// Enable interrupts
		ATOMIC({
			SPCR |= (_BV(SPE) | _BV(SPIE));
		});
	}
	return status;
}

void SPI::on_receive(fun_ptr_u8_u8 fun) {
	_user_on_receive = (fun)? fun : fun_u8_u8_dummy;
}

//###############################################
SPI::SPI(void) {
	// Start the SPI configuration
	_ss_pin = NO_PIN;
	_slave_active = false;
	_user_on_receive = fun_u8_u8_dummy;
	_user_on_request = fun_vd_vd_dummy;

	// Start the timing functions
	timing_enable();
}

//===============================================
uint8_t SPI::get_prescaler(uint32_t freq) {
	uint32_t scale = F_CPU / freq;
	if (scale <= 2)	 return 0x04;
	if (scale <= 4)  return 0x00;
	if (scale <= 8)  return 0x05;
	if (scale <= 16) return 0x01;
	if (scale <= 32) return 0x06;
	if (scale <= 64) return 0x07;
	return 0x03;
}

//===============================================
uint8_t SPI::on_receive_callback(uint8_t m_data) {
	if (!_slave_active)
		return 0x00;
	return _user_on_receive(m_data);
}

void SPI::on_request_callback(void) {
	if (pin_read(_ss_pin)) {
		_slave_active = false;
	} else {
		_slave_active = true;
		_user_on_request();
	}
}

//###############################################
/**
 * @brief	SPI ISR.
 */
ISR(SPI_STC_vect) {
	SPDR = SPI::_inst->on_receive_callback(SPDR);
}

