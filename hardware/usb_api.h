/**
 * @code	#include "hardware/usb_api.h" @endcode
 * @file	libs/hardware/usb_api.h
 * @brief	Based on Arduino USBAPI
 */
//###############################################
#ifndef _HARDWARE_USB_API_H_
#define _HARDWARE_USB_API_H_
//###############################################
extern "C" {
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <util/atomic.h>
}
#include <base/base.h>

//###############################################
#if defined(USBCON)
//###############################################
#include <hardware/usb_plug.h>
#include <hardware/usb_core.h>
#include <utils/stream.h>
//###############################################
// USB
#define EP_TYPE_CONTROL							(0x00)
#define EP_TYPE_BULK_IN							(_BV(EPTYPE1) | _BV(EPDIR))
#define EP_TYPE_BULK_OUT						_BV(EPTYPE1)
#define EP_TYPE_ISOCHRONOUS_IN					(_BV(EPTYPE0) | _BV(EPDIR))
#define EP_TYPE_ISOCHRONOUS_OUT					_BV(EPTYPE0)
#define EP_TYPE_INTERRUPT_IN					(_BV(EPTYPE1) | _BV(EPTYPE0) | _BV(EPDIR))
#define EP_TYPE_INTERRUPT_OUT					(_BV(EPTYPE1) | _BV(EPTYPE0))

//===============================================
// Serial
#define HAVE_USB_CDC_SERIAL

#if !defined(SERIAL_BUFFER_SIZE)
#if ((RAMEND - RAMSTART) < 1023)
#define SERIAL_BUFFER_SIZE	16
#else
#define SERIAL_BUFFER_SIZE	64
#endif
#endif
#if (SERIAL_BUFFER_SIZE > 256)
#error Please reduce the USB serial Buffer size
#endif

//###############################################
// CORE (usb_core.cpp)
int16_t USB_send_control(uint8_t flags, const void* d, int16_t len);
int16_t USB_recv_control(void* d, int16_t len);
int16_t USB_recv_control_long(void* d, int16_t len);

uint8_t	USB_available(uint8_t ep);
uint8_t USB_send_space(uint8_t ep);
int16_t USB_send(uint8_t ep, const void* data, int16_t len);	// blocking
int16_t USB_recv(uint8_t ep, void* data, int16_t len);			// non-blocking
int16_t USB_recv(uint8_t ep);									// non-blocking
void USB_flush(uint8_t ep);

//===============================================
class USBDevice {
public:
	USBDevice(void);
	bool is_configured(void);
	void attach(void);
	void detach(void);		// Serial port goes down too...
	void poll(void);
	bool wakeup_host(void);	// Return false when wakeup cannot be processed
};

extern USBDevice USB_device;

//###############################################
// CDC (usb_cdc.cpp)
bool	CDC_setup(USBSetup& setup);

int16_t	CDC_get_interface(uint8_t* interfaceNum);

int16_t	CDC_get_descriptor(int16_t i);

//###############################################
// Serial (usb_cdc.cpp)
class USBSerial : public Stream {
public:
	//======================================
	/** This allows to use the variants of write function. */
	using Stream::write;

	//======================================
	static USBSerial* get_instance(void);

	static void delete_instance(void);

	//======================================
	void enable(uint32_t baudrate);

	void enable(uint32_t baudrate, uint8_t config);

	void disable(void);

	//======================================
	int16_t rx_available(void);

	int16_t peek(void);

	int16_t read(void);

	//======================================
	uint8_t tx_available(void);

	void flush(void);

	size_t write(uint8_t byte);

	size_t write(uint8_t* buff, size_t size);

	//======================================
	// This method allows processing "SEND_BREAK" requests sent by
	// the USB host. Those requests indicate that the host wants to
	// send a BREAK signal and are accompanied by a single uint16_t
	// value, specifying the duration of the break. The value 0
	// means to end any current break, while the value 0xffff means
	// to start an indefinite break.
	// readBreak() will return the value of the most recent break
	// request, but will return it at most once, returning -1 when
	// readBreak() is called again (until another break request is
	// received, which is again returned once).
	// This also mean that if two break requests are received
	// without readBreak() being called in between, the value of the
	// first request is lost.
	// Note that the value returned is a long, so it can return
	// 0-0xffff as well as -1.
	int32_t read_break(void);

	// These return the settings specified by the USB host for the
	// serial port. These aren't really used, but are offered here
	// in case a sketch wants to act on these settings.
	uint32_t get_baudrate(void);

	uint8_t get_stop_bits(void);

	uint8_t get_parity_type(void);

	uint8_t get_num_bits(void);

	bool get_dtr(void);

	bool get_rts(void);

	bool is_connected(void);

	//======================================
	// This operator is a convenient way for a sketch to check whether the
	// port has actually been configured and opened by the host (as opposed
	// to just being connected to the host).  It can be used, for example, in
	// setup() before printing to ensure that an application on the host is
	// actually ready to receive and display the data.
	// We add a short delay before returning to fix a bug observed by Federico
	// where the port is configured (lineState != 0) but not quite opened.
	operator bool();

protected:
	//======================================
	static USBSerial*	_inst;

	//======================================
	volatile uint8_t 	_rx_buffer_head;
	volatile uint8_t 	_rx_buffer_tail;
	volatile uint8_t 	_rx_buffer[SERIAL_BUFFER_SIZE];
	int16_t				_peek_buffer;

	//======================================
	inline USBSerial(void);

};

//###############################################
//extern USBSerial serial;

//###############################################
#endif /* defined(USBCON) */
//###############################################
#endif /* _HARDWARE_USB_API_H_ */
//###############################################
