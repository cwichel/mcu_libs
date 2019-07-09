/**
 * @file	libs/hardware/usb_core.cpp
 * @brief	Based on Arduino USBCore
 */
//###############################################
#include <hardware/usb_core.h>
//###############################################
#if defined(USBCON)
//###############################################
#if !defined(USB_PRODUCT)
#define USB_PRODUCT     "USB IO Board"
#endif

#if defined(USB_MANUFACTURER)
#undef USB_MANUFACTURER
#endif

#if USB_VID == 0x2341
#define USB_MANUFACTURER "Arduino LLC"
#elif USB_VID == 0x1b4f
#define USB_MANUFACTURER "SparkFun"
#else
#define USB_MANUFACTURER "Unknown"
#endif

#define DEVICE_CLASS 0x02

#define EP_SINGLE_64 0x32	// EP0
#define EP_DOUBLE_64 0x36	// Other endpoints
#define EP_SINGLE_16 0x12

#define USB_RECV_TIMEOUT

//###############################################
const uint16_t 	STRING_LANGUAGE[] PROGMEM = {(3<<8) | (2+2), 0x0409};	// English
const uint8_t 	STRING_PRODUCT[] PROGMEM = USB_PRODUCT;
const uint8_t 	STRING_MANUFACTURER[] PROGMEM = USB_MANUFACTURER;
const DeviceDescriptor USB_DeviceDescriptorIAD PROGMEM = D_DEVICE(0xEF,0x02,0x01,64,USB_VID,USB_PID,0x100,IMANUFACTURER,IPRODUCT,ISERIAL,1);

//===============================================
volatile uint8_t _usbConfiguration = 0;
volatile uint8_t _usbCurrentStatus = 0; // meaning of bits see usb_20.pdf, Figure 9-4. Information Returned by a GetStatus() Request to a Device
volatile uint8_t _usbSuspendState = 0; // copy of UDINT to check SUSPI and WAKEUPI bits

//===============================================
static int16_t _cmark;
static int16_t _cend;

//===============================================
uint8_t _initEndpoints[USB_ENDPOINTS] = {
	0,                      // Control Endpoint
	EP_TYPE_INTERRUPT_IN,   // CDC_ENDPOINT_ACM
	EP_TYPE_BULK_OUT,       // CDC_ENDPOINT_OUT
	EP_TYPE_BULK_IN,        // CDC_ENDPOINT_IN
	// Following endpoints are automatically initialized to 0
};

//###############################################
static inline void wait_in(void);
static inline void wait_out(void);
static inline void clear_in(void);
static inline void clear_out(void);
static inline uint8_t wait_for_io(void);
static inline void recv(volatile uint8_t* data, uint8_t count);
static inline uint8_t recv8(void);
static inline void send8(uint8_t d);
static inline uint8_t received_setup_int(void);
static inline void clear_setup_int(void);
static inline uint8_t rw_allowed(void);
static inline void stall(void);
static inline uint8_t stalled(void);
static inline uint8_t fifo_byte_count(void);
static inline uint8_t fifo_free();
static inline void release_RX(void);
static inline void release_TX(void);
static inline uint8_t frame_number(void);

//===============================================
static inline void EP_set(uint8_t ep);
static void EP_init(void);
static void EP_init(uint8_t index, uint8_t type, uint8_t size);

//===============================================
static void init_control(int16_t end);
static bool send_control(uint8_t d);
static bool send_descriptor(USBSetup& setup);
static bool send_configuration(int16_t maxlen);
static uint8_t send_interfaces(void);

//===============================================
static inline uint8_t USB_get_configuration(void);
static inline void USB_clock_enable(void);
static inline void USB_clock_disable(void);
static bool USB_send_string_descriptor(const uint8_t*string_P, uint8_t string_len, uint8_t flags);

//===============================================
//	Handle CLASS_INTERFACE requests
static bool class_interface_request(USBSetup& setup);

//###############################################
class LockEP {
	uint8_t _sreg;
public:
	LockEP(uint8_t ep) : _sreg(SREG) {
		cli();
		EP_set(ep & 7);
	}

	~LockEP() {
		SREG = _sreg;
	}
};

//###############################################
//	Clipped by _cmark/_cend
int16_t USB_send_control(uint8_t flags, const void* d, int16_t len) {
	int16_t sent = len;
	const uint8_t* data = (const uint8_t*)d;
	bool pgm = flags & TRANSFER_PGM;
	while (len--) {
		uint8_t c = pgm ? pgm_read_byte(data++) : *data++;
		if (!send_control(c))
			return -1;
	}
	return sent;
}

//	Does not timeout or cross fifo boundaries
int16_t USB_recv_control(void* d, int16_t len) {
	int16_t length = len;
	while(length) {
		// Dont receive more than the USB Control EP has to offer
		// Use fixed 64 because control EP always have 64 bytes even on 16u2.
		int16_t recvLength = length;
		if(recvLength > 64) {
			recvLength = 64;
		}

		// Write data to fit to the end (not the beginning) of the array
		wait_out();
		recv((uint8_t*)d + len - length, recvLength);
		clear_out();
		length -= recvLength;
	}
	return len;
}

//===============================================
//	Number of bytes, assumes a rx endpoint
uint8_t USB_available(uint8_t ep) {
	LockEP lock(ep);
	return fifo_byte_count();
}

//	Space in send EP
uint8_t USB_send_space(uint8_t ep) {
	LockEP lock(ep);
	if (!rw_allowed())
		return 0;
	return USB_EP_SIZE - fifo_byte_count();
}

//	Blocking Send of data to an endpoint
int16_t USB_send(uint8_t ep, const void* d, int16_t len) {
	if (!_usbConfiguration)
		return -1;

	if (_usbSuspendState & (1<<SUSPI)) {
		//send a remote wakeup
		UDCON |= (1 << RMWKUP);
	}

	int16_t r = len;
	const uint8_t* data = (const uint8_t*)d;
	uint8_t timeout = 250;		// 250ms timeout on send? TODO
	bool sendZlp = false;

	while (len || sendZlp) {
		uint8_t n = USB_send_space(ep);
		if (n == 0) {
			if (!(--timeout))
				return -1;
			_delay_ms(1);
			continue;
		}

		if (n > len) {
			n = len;
		}

		{
			LockEP lock(ep);
			// Frame may have been released by the SOF interrupt handler
			if (!rw_allowed())
				continue;
			len -= n;

			if (ep & TRANSFER_ZERO) {
				while (n--)
					send8(0);
			} else if (ep & TRANSFER_PGM) {
				while (n--)
					send8(pgm_read_byte(data++));
			} else {
				while (n--)
					send8(*data++);
			}
			if (sendZlp) {
				release_TX();
				sendZlp = false;
			} else if (!rw_allowed()) { // ...release if buffer is full...
				release_TX();
				if (len == 0) sendZlp = true;
			} else if ((len == 0) && (ep & TRANSFER_RELEASE)) { // ...or if forced with TRANSFER_RELEASE
				// XXX: TRANSFER_RELEASE is never used can be removed?
				release_TX();
			}
		}
	}
	return r;
}

//	Non Blocking receive
//	Return number of bytes read
int16_t USB_recv(uint8_t ep, void* d, int16_t len) {
	if (!_usbConfiguration || len < 0)
		return -1;
	LockEP lock(ep);
	uint8_t n = fifo_byte_count();
	len = min(n, len);
	n = len;
	uint8_t* dst = (uint8_t*)d;
	while (n--)
		*dst++ = recv8();
	if (len && !fifo_byte_count())	// release empty buffer
		release_RX();
	return len;
}

//	Recv 1 byte if ready
int16_t USB_recv(uint8_t ep) {
	uint8_t c;
	if (USB_recv(ep,&c,1) != 1)
		return -1;
	return c;
}

void USB_flush(uint8_t ep) {
	EP_set(ep);
	if (fifo_byte_count())
		release_TX();
}

//###############################################
USBDevice::USBDevice(void) {
}

void USBDevice::attach() {
	_usbConfiguration = 0;
	_usbCurrentStatus = 0;
	_usbSuspendState = 0;
	USB_clock_enable();

	UDINT &= ~((1<<WAKEUPI) | (1<<SUSPI)); // clear already pending WAKEUP / SUSPEND requests
	UDIEN = (1<<EORSTE) | (1<<SOFE) | (1<<SUSPE);	// Enable interrupts for EOR (End of Reset), SOF (start of frame) and SUSPEND
}

void USBDevice::detach() {
}

//	Check for interrupts
//	TODO: VBUS detection
bool USBDevice::is_configured() {
	return _usbConfiguration;
}

void USBDevice::poll() {
}

bool USBDevice::wakeup_host() {
	// clear any previous wakeup request which might have been set but could be processed at that time
	// e.g. because the host was not suspended at that time
	UDCON &= ~(1 << RMWKUP);

	if(!(UDCON & (1 << RMWKUP))
	  && (_usbSuspendState & (1<<SUSPI))
	  && (_usbCurrentStatus & FEATURE_REMOTE_WAKEUP_ENABLED)) {
		// This short version will only work, when the device has not been suspended. Currently the
		// Arduino core doesn't handle SUSPEND at all, so this is ok.
		USB_clock_enable();
		UDCON |= (1 << RMWKUP); // send the wakeup request
		return true;
	}
	return false;
}

USBDevice USB_device;

//###############################################
//	General interrupt
ISR(USB_GEN_vect) {
	uint8_t udint = UDINT;
	UDINT &= ~((1<<EORSTI) | (1<<SOFI)); // clear the IRQ flags for the IRQs which are handled here, except WAKEUPI and SUSPI (see below)

	//	End of Reset
	if (udint & (1<<EORSTI)) {
		EP_init(0,EP_TYPE_CONTROL,EP_SINGLE_64);	// init ep0
		_usbConfiguration = 0;			// not configured yet
		UEIENX = 1 << RXSTPE;			// Enable interrupts for ep0
	}
	//	Start of Frame - happens every millisecond so we use it for TX and RX LED one-shot timing, too
	if (udint & (1<<SOFI)) {
		USB_flush(CDC_TX);				// Send a tx frame if found
	}
	// the WAKEUPI interrupt is triggered as soon as there are non-idle patterns on the data
	// lines. Thus, the WAKEUPI interrupt can occur even if the controller is not in the "suspend" mode.
	// Therefore the we enable it only when USB is suspended
	if (udint & (1<<WAKEUPI)) {
		UDIEN = (UDIEN & ~(1<<WAKEUPE)) | (1<<SUSPE); // Disable interrupts for WAKEUP and enable interrupts for SUSPEND
		//TODO
		// WAKEUPI shall be cleared by software (USB clock inputs must be enabled before).
		//USB_clock_enable();
		UDINT &= ~(1<<WAKEUPI);
		_usbSuspendState = (_usbSuspendState & ~(1<<SUSPI)) | (1<<WAKEUPI);
	} else if (udint & (1<<SUSPI)) { // only one of the WAKEUPI / SUSPI bits can be active at time
		UDIEN = (UDIEN & ~(1<<SUSPE)) | (1<<WAKEUPE); // Disable interrupts for SUSPEND and enable interrupts for WAKEUP
		//TODO
		//USB_clock_disable();
		UDINT &= ~((1<<WAKEUPI) | (1<<SUSPI)); // clear any already pending WAKEUP IRQs and the SUSPI request
		_usbSuspendState = (_usbSuspendState & ~(1<<WAKEUPI)) | (1<<SUSPI);
	}
}

//	VBUS or counting frames
//	Any frame counting?
uint8_t USBConnected() {
	uint8_t f = UDFNUML;
	_delay_ms(3);
	return f != UDFNUML;
}

//###############################################
//	Endpoint 0 interrupt
ISR(USB_COM_vect) {
	EP_set(0);
	if (!received_setup_int())
		return;

	USBSetup setup;
	recv((uint8_t*)&setup,8);
	clear_setup_int();

	uint8_t requestType = setup.bmRequestType;
	if (requestType & REQUEST_DEVICETOHOST)
		wait_in();
	else
		clear_in();

    bool ok = true;
	if (REQUEST_STANDARD == (requestType & REQUEST_TYPE)) {
		//	Standard Requests
		uint8_t r = setup.bRequest;
		uint16_t wValue = setup.wValueL | (setup.wValueH << 8);
		if (GET_STATUS == r) {
			if (requestType == (REQUEST_DEVICETOHOST | REQUEST_STANDARD | REQUEST_DEVICE)) {
				send8(_usbCurrentStatus);
				send8(0);
			} else {
				// TODO: handle the HALT state of an endpoint here
				// see "Figure 9-6. Information Returned by a GetStatus() Request to an Endpoint" in usb_20.pdf for more information
				send8(0);
				send8(0);
			}
		} else if (CLEAR_FEATURE == r) {
			if((requestType == (REQUEST_HOSTTODEVICE | REQUEST_STANDARD | REQUEST_DEVICE))
				&& (wValue == DEVICE_REMOTE_WAKEUP)) {
				_usbCurrentStatus &= ~FEATURE_REMOTE_WAKEUP_ENABLED;
			}
		} else if (SET_FEATURE == r) {
			if((requestType == (REQUEST_HOSTTODEVICE | REQUEST_STANDARD | REQUEST_DEVICE))
				&& (wValue == DEVICE_REMOTE_WAKEUP)) {
				_usbCurrentStatus |= FEATURE_REMOTE_WAKEUP_ENABLED;
			}
		} else if (SET_ADDRESS == r) {
			wait_in();
			UDADDR = setup.wValueL | (1<<ADDEN);
		} else if (GET_DESCRIPTOR == r) {
			ok = send_descriptor(setup);
		} else if (SET_DESCRIPTOR == r) {
			ok = false;
		} else if (GET_CONFIGURATION == r) {
			send8(1);
		} else if (SET_CONFIGURATION == r) {
			if (REQUEST_DEVICE == (requestType & REQUEST_RECIPIENT)) {
				EP_init();
				_usbConfiguration = setup.wValueL;
			} else {
				ok = false;
			}
		} else if (GET_INTERFACE == r) {
		} else if (SET_INTERFACE == r) {
		}
	} else {
		init_control(setup.wLength);		//	Max length of transfer
		ok = class_interface_request(setup);
	}
	if (ok) {
		clear_in();
	} else {
		stall();
	}
}

//###############################################
static inline void wait_in(void) {
	while (!(UEINTX & (1<<TXINI)));
}

static inline void wait_out(void) {
	while (!(UEINTX & (1<<RXOUTI)));
}

static inline void clear_in(void) {
	UEINTX = ~(1<<TXINI);
}

static inline void clear_out(void) {
	UEINTX = ~(1<<RXOUTI);
}

static inline uint8_t wait_for_io(void) {
	while (!(UEINTX & ((1<<TXINI)|(1<<RXOUTI))));
	return (UEINTX & (1<<RXOUTI)) == 0;
}

static inline void recv(volatile uint8_t* data, uint8_t count) {
	while (count--)
		*data++ = UEDATX;
}

static inline uint8_t recv8(void) {
	return UEDATX;
}

static inline void send8(uint8_t d) {
	UEDATX = d;
}

static inline uint8_t received_setup_int(void) {
	return UEINTX & (1<<RXSTPI);
}

static inline void clear_setup_int(void) {
	UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
}

static inline uint8_t rw_allowed(void) {
	return UEINTX & (1<<RWAL);
}

static inline void stall(void) {
	UECONX = (1<<STALLRQ) | (1<<EPEN);
}

static inline uint8_t stalled(void) {
	return UEINTX & (1<<STALLEDI);
}

static inline uint8_t fifo_byte_count(void) {
	return UEBCLX;
}

static inline uint8_t fifo_free() {
	return UEINTX & (1<<FIFOCON);
}

static inline void release_RX(void) {
	UEINTX = 0x6B;	// FIFOCON=0 NAKINI=1 RWAL=1 NAKOUTI=0 RXSTPI=1 RXOUTI=0 STALLEDI=1 TXINI=1
}

static inline void release_TX(void) {
	UEINTX = 0x3A;	// FIFOCON=0 NAKINI=0 RWAL=1 NAKOUTI=1 RXSTPI=1 RXOUTI=0 STALLEDI=1 TXINI=0
}

static inline uint8_t frame_number(void) {
	return UDFNUML;
}

//===============================================
static inline void EP_set(uint8_t ep) {
	UENUM = ep;
}

static void EP_init(void) {
	for (uint8_t i = 1; i < sizeof(_initEndpoints) && _initEndpoints[i] != 0; i++) {
		UENUM = i;
		UECONX = (1<<EPEN);
		UECFG0X = _initEndpoints[i];
		#if USB_EP_SIZE == 16
		UECFG1X = EP_SINGLE_16;
		#elif USB_EP_SIZE == 64
		UECFG1X = EP_DOUBLE_64;
		#else
		#error Unsupported value for USB_EP_SIZE
		#endif
	}
	UERST = 0x7E;	// And reset them
	UERST = 0;
}

static void EP_init(uint8_t index, uint8_t type, uint8_t size) {
	UENUM = index;
	UECONX = (1<<EPEN);
	UECFG0X = type;
	UECFG1X = size;
}

//===============================================
static void init_control(int16_t end) {
	EP_set(0);
	_cmark = 0;
	_cend = end;
}

static bool send_control(uint8_t d) {
	if (_cmark < _cend) {
		if (!wait_for_io())
			return false;
		send8(d);
		if (!((_cmark + 1) & 0x3F))
			clear_in();	// Fifo is full, release this packet
	}
	_cmark++;
	return true;
}

static bool send_descriptor(USBSetup& setup) {
	int16_t ret;
	uint8_t t = setup.wValueH;
	if (USB_CONFIGURATION_DESCRIPTOR_TYPE == t)
		return send_configuration(setup.wLength);

	init_control(setup.wLength);
	#ifdef PLUGGABLE_USB_ENABLED
		ret = PluggableUSB().get_descriptor(setup);
		if (ret != 0) {
			return (ret > 0 ? true : false);
		}
	#endif
	const uint8_t* desc_addr = 0;
	if (USB_DEVICE_DESCRIPTOR_TYPE == t) {
		desc_addr = (const uint8_t*)&USB_DeviceDescriptorIAD;
	} else if (USB_STRING_DESCRIPTOR_TYPE == t) {
		if (setup.wValueL == 0) {
			desc_addr = (const uint8_t*)&STRING_LANGUAGE;
		} else if (setup.wValueL == IPRODUCT) {
			return USB_send_string_descriptor(STRING_PRODUCT, strlen(USB_PRODUCT), TRANSFER_PGM);
		} else if (setup.wValueL == IMANUFACTURER) {
			return USB_send_string_descriptor(STRING_MANUFACTURER, strlen(USB_MANUFACTURER), TRANSFER_PGM);
		} else if (setup.wValueL == ISERIAL) {
			#ifdef PLUGGABLE_USB_ENABLED
			char name[ISERIAL_MAX_LEN];
			PluggableUSB().get_short_name(name);
			return USB_send_string_descriptor((uint8_t*)name, strlen(name), 0);
			#endif
		} else {
			return false;
		}
	}
	if (desc_addr == 0)
		return false;
	uint8_t desc_length = pgm_read_byte(desc_addr);
	USB_send_control(TRANSFER_PGM,desc_addr,desc_length);
	return true;
}

//	Construct a dynamic configuration descriptor
//	This really needs dynamic endpoint allocation etc
//	TODO
static bool send_configuration(int16_t maxlen) {
	//	Count and measure interfaces
	init_control(0);
	uint8_t interfaces = send_interfaces();
	ConfigDescriptor config = D_CONFIG(_cmark + sizeof(ConfigDescriptor),interfaces);

	//	Now send them
	init_control(maxlen);
	USB_send_control(0,&config,sizeof(ConfigDescriptor));
	send_interfaces();
	return true;
}

static uint8_t send_interfaces(void) {
	uint8_t interfaces = 0;
	CDC_get_interface(&interfaces);
	#ifdef PLUGGABLE_USB_ENABLED
	PluggableUSB().get_interface(&interfaces);
	#endif
	return interfaces;
}

//===============================================
static inline uint8_t USB_get_configuration(void) {
	return _usbConfiguration;
}

static inline void USB_clock_enable(void)  {
	#if defined(UHWCON)
	UHWCON |= (1<<UVREGE);				// power internal reg
	#endif
	USBCON = (1<<USBE) | (1<<FRZCLK);	// clock frozen, usb enabled
	#if defined(PINDIV)
		// ATmega32U4
		#if F_CPU == 16000000UL
		PLLCSR |= (1<<PINDIV);                   // Need 16 MHz xtal
		#elif F_CPU == 8000000UL
		PLLCSR &= ~(1<<PINDIV);                  // Need  8 MHz xtal
		#else
		#error "Clock rate of F_CPU not supported"
		#endif
	#elif defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__) || defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega8U2__)
		// for the u2 Series the datasheet is confusing. On page 40 its called PINDIV and on page 290 its called PLLP0
		#if F_CPU == 16000000UL
		// Need 16 MHz xtal
		PLLCSR |= (1 << PLLP0);
		#elif F_CPU == 8000000UL
		// Need 8 MHz xtal
		PLLCSR &= ~(1 << PLLP0);
		#endif
	#elif defined(PLLP2)
		// AT90USB646, AT90USB647, AT90USB1286, AT90USB1287
		#if F_CPU == 16000000UL
			#if defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__)
			// For Atmel AT90USB128x only. Do not use with Atmel AT90USB64x.
			PLLCSR = (PLLCSR & ~(1<<PLLP1)) | ((1<<PLLP2) | (1<<PLLP0)); // Need 16 MHz xtal
			#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__)
			// For AT90USB64x only. Do not use with AT90USB128x.
			PLLCSR = (PLLCSR & ~(1<<PLLP0)) | ((1<<PLLP2) | (1<<PLLP1)); // Need 16 MHz xtal
			#else
			#error "USB Chip not supported, please defined method of USB PLL initialization"
			#endif
		#elif F_CPU == 8000000UL
			// for Atmel AT90USB128x and AT90USB64x
			PLLCSR = (PLLCSR & ~(1<<PLLP2)) | ((1<<PLLP1) | (1<<PLLP0)); // Need 8 MHz xtal
		#else
			#error "Clock rate of F_CPU not supported"
		#endif
	#else
	#error "USB Chip not supported, please defined method of USB PLL initialization"
	#endif
	PLLCSR |= (1<<PLLE);
	while (!(PLLCSR & (1<<PLOCK)));		// wait for lock pll

	// Some tests on specific versions of macosx (10.7.3), reported some
	// strange behaviors when the board is reset using the serial
	// port touch at 1200 bps. This delay fixes this behavior.
	_delay_ms(1);
	#if defined(OTGPADE)
	USBCON = (USBCON & ~(1<<FRZCLK)) | (1<<OTGPADE);	// start USB clock, enable VBUS Pad
	#else
	USBCON &= ~(1 << FRZCLK);	// start USB clock
	#endif

	#if defined(RSTCPU)
		#if defined(LSM)
		UDCON &= ~((1<<RSTCPU) | (1<<LSM) | (1<<RMWKUP) | (1<<DETACH));	// enable attach resistor, set full speed mode
		#else // u2 Series
		UDCON &= ~((1 << RSTCPU) | (1 << RMWKUP) | (1 << DETACH));	// enable attach resistor, set full speed mode
		#endif
	#else
	// AT90USB64x and AT90USB128x don't have RSTCPU
	UDCON &= ~((1<<LSM) | (1<<RMWKUP) | (1<<DETACH));	// enable attach resistor, set full speed mode
	#endif
}


static inline void USB_clock_disable(void) {
	#if defined(OTGPADE)
	USBCON = (USBCON & ~(1<<OTGPADE)) | (1<<FRZCLK); // freeze clock and disable VBUS Pad
	#else // u2 Series
	USBCON = (1 << FRZCLK); // freeze clock
	#endif
	PLLCSR &= ~(1<<PLLE);  // stop PLL
}

// Send a USB descriptor string. The string is stored in PROGMEM as a
// plain ASCII string but is sent out as UTF-16 with the correct 2-byte
// prefix
static bool USB_send_string_descriptor(const uint8_t*string_P, uint8_t string_len, uint8_t flags) {
	send_control(2 + string_len * 2);
	send_control(3);
	bool pgm = flags & TRANSFER_PGM;
	for(uint8_t i = 0; i < string_len; i++) {
			bool r = send_control(pgm ? pgm_read_byte(&string_P[i]) : string_P[i]);
			r &= send_control(0); // high byte
			if(!r) {
					return false;
			}
	}
	return true;
}

//===============================================
//	Handle CLASS_INTERFACE requests
static bool class_interface_request(USBSetup& setup) {
	uint8_t i = setup.wIndex;
	if (CDC_ACM_INTERFACE == i)
		return CDC_setup(setup);

	#ifdef PLUGGABLE_USB_ENABLED
		return PluggableUSB().setup(setup);
	#endif
		return false;
}

//###############################################
#endif /* if defined(USBCON) */
//###############################################
