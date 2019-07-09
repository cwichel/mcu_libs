/**
 * @file	libs/hardware/usb_cdc.cpp
 * @brief	Based on Arduino CDC
 */
//###############################################
#include <hardware/usb_api.h>
//###############################################
#if defined(USBCON)
//###############################################
static volatile LineInfo _usbLineInfo = { 57600, 0x00, 0x00, 0x00, 0x00 };
static volatile int32_t breakValue = -1;
static uint8_t wdtcsr_save;
const CDCDescriptor _cdcInterface PROGMEM = {
	D_IAD(0,2,CDC_COMMUNICATION_INTERFACE_CLASS,CDC_ABSTRACT_CONTROL_MODEL,1),

	//	CDC communication interface
	D_INTERFACE(CDC_ACM_INTERFACE,1,CDC_COMMUNICATION_INTERFACE_CLASS,CDC_ABSTRACT_CONTROL_MODEL,0),
	D_CDCCS(CDC_HEADER,0x10,0x01),								// Header (1.10 bcd)
	D_CDCCS(CDC_CALL_MANAGEMENT,1,1),							// Device handles call management (not)
	D_CDCCS4(CDC_ABSTRACT_CONTROL_MANAGEMENT,6),				// SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported
	D_CDCCS(CDC_UNION,CDC_ACM_INTERFACE,CDC_DATA_INTERFACE),	// Communication interface is master, data interface is slave 0
	D_ENDPOINT(USB_ENDPOINT_IN (CDC_ENDPOINT_ACM),USB_ENDPOINT_TYPE_INTERRUPT,0x10,0x40),

	//	CDC data interface
	D_INTERFACE(CDC_DATA_INTERFACE,2,CDC_DATA_INTERFACE_CLASS,0,0),
	D_ENDPOINT(USB_ENDPOINT_OUT(CDC_ENDPOINT_OUT),USB_ENDPOINT_TYPE_BULK,USB_EP_SIZE,0),
	D_ENDPOINT(USB_ENDPOINT_IN (CDC_ENDPOINT_IN ),USB_ENDPOINT_TYPE_BULK,USB_EP_SIZE,0)
};

//###############################################
bool is_lufa_boot() {
	return pgm_read_word(FLASHEND - 1) == NEW_LUFA_SIGNATURE;
}

bool CDC_setup(USBSetup& setup) {
	uint8_t r = setup.bRequest;
	uint8_t requestType = setup.bmRequestType;

	if (REQUEST_DEVICETOHOST_CLASS_INTERFACE == requestType) {
		if (CDC_GET_LINE_CODING == r) {
			USB_send_control(0,(void*)&_usbLineInfo, 7);
			return true;
		}
	}

	if (REQUEST_HOSTTODEVICE_CLASS_INTERFACE == requestType) {
		if (CDC_SEND_BREAK == r) {
			breakValue = ((uint16_t)setup.wValueH << 8) | setup.wValueL;
		}
		if (CDC_SET_LINE_CODING == r) {
			USB_recv_control((void*)&_usbLineInfo, 7);
		}
		if (CDC_SET_CONTROL_LINE_STATE == r) {
			_usbLineInfo.lineState = setup.wValueL;

			// auto-reset into the bootloader is triggered when the port, already
			// open at 1200 bps, is closed.  this is the signal to start the watchdog
			// with a relatively long period so it can finish housekeeping tasks
			// like servicing endpoints before the sketch ends
			uint16_t magic_key_pos = MAGIC_KEY_POS;

			// If we don't use the new RAMEND directly, check manually if we have a newer bootloader.
			// This is used to keep compatible with the old leonardo bootloaders.
			// You are still able to set the magic key position manually to RAMEND-1 to save a few bytes for this check.
			#if MAGIC_KEY_POS != (RAMEND-1)
			// For future boards save the key in the inproblematic RAMEND
			// Which is reserved for the main() return value (which will never return)
			if (is_lufa_boot()) {
				// horray, we got a new bootloader!
				magic_key_pos = (RAMEND-1);
			}
			#endif

			// We check DTR state to determine if host port is open (bit 0 of lineState).
			if (1200 == _usbLineInfo.dwDTERate && (_usbLineInfo.lineState & 0x01) == 0) {
				#if MAGIC_KEY_POS != (RAMEND-1)
				// Backup ram value if its not a newer bootloader and it hasn't already been saved.
				// This should avoid memory corruption at least a bit, not fully
				if (magic_key_pos != (RAMEND-1) && *(uint16_t *)magic_key_pos != MAGIC_KEY) {
					*(uint16_t *)(RAMEND-1) = *(uint16_t *)magic_key_pos;
				}
				#endif
				// Store boot key
				*(uint16_t *)magic_key_pos = MAGIC_KEY;
				// Save the watchdog state in case the reset is aborted.
				wdtcsr_save = WDTCSR;
				wdt_enable(WDTO_120MS);
			} else if (*(uint16_t *)magic_key_pos == MAGIC_KEY) {
				// Most OSs do some intermediate steps when configuring ports and DTR can
				// twiggle more than once before stabilizing.
				// To avoid spurious resets we set the watchdog to 120ms and eventually
				// cancel if DTR goes back high.
				// Cancellation is only done if an auto-reset was started, which is
				// indicated by the magic key having been set.
				wdt_reset();
				// Restore the watchdog state in case the sketch was using it.
				WDTCSR |= (1<<WDCE) | (1<<WDE);
				WDTCSR = wdtcsr_save;

				#if MAGIC_KEY_POS != (RAMEND-1)
				// Restore backed up (old bootloader) magic key data
				if (magic_key_pos != (RAMEND-1)) {
					*(uint16_t *)magic_key_pos = *(uint16_t *)(RAMEND-1);
				} else
				#endif
				{
				// Clean up RAMEND key
					*(uint16_t *)magic_key_pos = 0x0000;
				}
			}
		}
		return true;
	}
	return false;
}

int16_t CDC_get_interface(uint8_t* interfaceNum) {
	interfaceNum[0] += 2;	// uses 2
	return USB_send_control(TRANSFER_PGM,&_cdcInterface,sizeof(_cdcInterface));
}

//###############################################
USBSerial* USBSerial::_inst = NULL;

//###############################################
USBSerial* USBSerial::get_instance(void) {
	if(!_inst) _inst = new USBSerial();
	return _inst;
}

void USBSerial::delete_instance(void) {
	if (!_inst) return;
	_inst->disable();
	delete _inst;
	_inst = NULL;
}

//###############################################
void USBSerial::enable(uint32_t baudrate) {
	_peek_buffer = -1;
}

void USBSerial::enable(uint32_t baudrate, uint8_t config) {
	_peek_buffer = -1;
}

void USBSerial::disable(void) {
}

//===============================================
int16_t USBSerial::rx_available(void) {
	if (_peek_buffer >= 0) {
		return 1 + USB_available(CDC_RX);
	}
	return USB_available(CDC_RX);
}

int16_t USBSerial::peek(void) {
	if (_peek_buffer < 0)
		_peek_buffer = USB_recv(CDC_RX);
	return _peek_buffer;
}

int16_t USBSerial::read(void) {
	if (_peek_buffer >= 0) {
		int16_t c = _peek_buffer;
		_peek_buffer = -1;
		return c;
	}
	return USB_recv(CDC_RX);
}

//===============================================
uint8_t USBSerial::tx_available(void) {
	return USB_send_space(CDC_TX);
}

void USBSerial::flush(void) {
	USB_flush(CDC_TX);
}

size_t USBSerial::write(uint8_t c) {
	return write(&c, 1);
}

size_t USBSerial::write(uint8_t* buff, size_t size) {
	/* only try to send bytes if the high-level CDC connection itself
	 is open (not just the pipe) - the OS should set lineState when the port
	 is opened and clear lineState when the port is closed.
	 bytes sent before the user opens the connection or after
	 the connection is closed are lost - just like with a UART. */

	// TODO - ZE - check behavior on different OSes and test what happens if an
	// open connection isn't broken cleanly (cable is yanked out, host dies
	// or locks up, or host virtual serial port hangs)
	if (_usbLineInfo.lineState > 0)	{
		int16_t r = USB_send(CDC_TX, buff, size);
		if (r > 0)
			return r;
	}
	return 0;

}

//===============================================
int32_t USBSerial::read_break(void) {
	int32_t ret;
	// Disable IRQs while reading and clearing breakValue to make
	// sure we don't overwrite a value just set by the ISR.
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		ret = breakValue;
		breakValue = -1;
	}
	return ret;
}

uint32_t USBSerial::get_baudrate(void) {
	// Disable interrupts while reading a multi-byte value
	uint32_t baudrate;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		baudrate =  _usbLineInfo.dwDTERate;
	}
	return baudrate;
}

uint8_t USBSerial::get_stop_bits() {
	return _usbLineInfo.bCharFormat;
}

uint8_t USBSerial::get_parity_type() {
	return _usbLineInfo.bParityType;
}

uint8_t USBSerial::get_num_bits() {
	return _usbLineInfo.bDataBits;
}

bool USBSerial::get_dtr() {
	return _usbLineInfo.lineState & 0x1;
}

bool USBSerial::get_rts() {
	return _usbLineInfo.lineState & 0x2;
}

bool USBSerial::is_connected(void) {
	return (USBSTA & _BV(VBUS));
}

//===============================================
USBSerial::operator bool() {
	bool result = false;
	if (_usbLineInfo.lineState > 0)
		result = true;
	_delay_ms(10);
	return result;
}

//###############################################
USBSerial::USBSerial(void) {
	_peek_buffer = -1;
}

//###############################################
#endif /* if defined(USBCON) */
//###############################################
