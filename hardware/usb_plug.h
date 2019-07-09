/**
 * @code	#include "hardware/usb_plug.h" @endcode
 * @file	libs/hardware/usb_plug.h
 * @brief	Based on Arduino PluggableUSB
 */
//###############################################
#ifndef _HARDWARE_USB_PLUG_
#define _HARDWARE_USB_PLUG_
//###############################################
#include <base/base.h>
#include <hardware/usb_desc.h>

//###############################################
#if defined(USBCON)
//###############################################
typedef struct {
	uint8_t		bmRequestType;
	uint8_t 	bRequest;
	uint8_t 	wValueL;
	uint8_t 	wValueH;
	uint16_t 	wIndex;
	uint16_t 	wLength;
} USBSetup;

//###############################################
class PluggableUSBModule {
public:
	//======================================
	PluggableUSBModule(uint8_t numEps, uint8_t numIfs, uint8_t *epType);

protected:
	//======================================
	friend class PluggableUSB_;
	//======================================
	const uint8_t 	numEndpoints;
	const uint8_t 	numInterfaces;
	const uint8_t*	endpointType;
	uint8_t 		pluggedInterface;
	uint8_t 		pluggedEndpoint;
	PluggableUSBModule *next = NULL;
	//======================================
	virtual bool 	setup(USBSetup& setup) = 0;
	virtual int 	get_interface(uint8_t* interfaceCount) = 0;
	virtual int 	get_descriptor(USBSetup& setup) = 0;
	virtual uint8_t get_short_name(char *name) { name[0] = 'A'+pluggedInterface; return 1; }

};

//###############################################
class PluggableUSB_ {
public:
	PluggableUSB_();
	bool 	setup(USBSetup& setup);
	bool	plug(PluggableUSBModule *node);
	int 	get_interface(uint8_t* interfaceCount);
	int 	get_descriptor(USBSetup& setup);
	void 	get_short_name(char *iSerialNum);

private:
	uint8_t 		lastIf;
	uint8_t 		lastEp;
	PluggableUSBModule* rootNode;
};

// Replacement for global singleton.
// This function prevents static-initialization-order-fiasco
// https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
PluggableUSB_& PluggableUSB();

//###############################################
#endif /* if defined(USBCON) */
//###############################################
#endif /* _HARDWARE_USB_PLUG_ */
//###############################################
