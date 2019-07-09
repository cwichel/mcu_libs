/**
 * @code	#include "hardware/usb_desc.h" @endcode
 * @file	libs/hardware/usb_desc.h
 * @brief	Based on Arduino USBDesc
 */
//###############################################
#ifndef _HARDWARE_USB_DESC_H_
#define _HARDWARE_USB_DESC_H_
//###############################################
extern "C" {
#include <avr/io.h>
}

//###############################################
#define USB_VID				0xF055
#define USB_PID				0x0003

//###############################################
#define	PLUGGABLE_USB_ENABLED

#if defined(EPRST6)
#define USB_ENDPOINTS	7	// ATMegaXXU4
#else
#define USB_ENDPOINTS	5	// ATMegaXXU2
#endif

#define ISERIAL_MAX_LEN     20

#define CDC_INTERFACE_COUNT	2
#define CDC_ENPOINT_COUNT	3

#define CDC_ACM_INTERFACE	0	// CDC ACM
#define CDC_DATA_INTERFACE	1	// CDC Data
#define CDC_FIRST_ENDPOINT	1
#define CDC_ENDPOINT_ACM	(CDC_FIRST_ENDPOINT)							// CDC First
#define CDC_ENDPOINT_OUT	(CDC_FIRST_ENDPOINT+1)
#define CDC_ENDPOINT_IN		(CDC_FIRST_ENDPOINT+2)

#define INTERFACE_COUNT		(MSC_INTERFACE + MSC_INTERFACE_COUNT)

#define CDC_RX CDC_ENDPOINT_OUT
#define CDC_TX CDC_ENDPOINT_IN

#define IMANUFACTURER   	1
#define IPRODUCT        	2
#define ISERIAL         	3

//###############################################
// Bootloader related fields
// Old Caterina bootloader places the MAGIC key into unsafe RAM locations (it can be rewritten
// by the running sketch before to actual reboot).
// Newer bootloaders, recognizable by the LUFA "signature" at the end of the flash, can handle both
// the usafe and the safe location.
#ifndef MAGIC_KEY
#define MAGIC_KEY 0x7777
#endif

#ifndef MAGIC_KEY_POS
#define MAGIC_KEY_POS 0x0800
#endif

#ifndef NEW_LUFA_SIGNATURE
#define NEW_LUFA_SIGNATURE 0xDCFB
#endif

//###############################################
#endif /* _HARDWARE_USB_DESC_H_ */
//###############################################

