/**
 * @file	libs/hardware/usb_plug.cpp
 * @brief	Based on Arduino PluggableUSB
 */
//###############################################
#include <hardware/usb_plug.h>

//###############################################
#if defined(USBCON) && defined (PLUGGABLE_USB_ENABLED)
//###############################################
extern uint8_t _initEndpoints[];
//###############################################
PluggableUSBModule::PluggableUSBModule(uint8_t numEps, uint8_t numIfs, uint8_t *epType):
	numEndpoints(numEps), numInterfaces(numIfs), endpointType(epType) {

}

//###############################################
bool PluggableUSB_::setup(USBSetup& setup) {
	PluggableUSBModule* node;
	for (node = rootNode; node; node = node->next) {
		if (node->setup(setup)) {
			return true;
		}
	}
	return false;
}

bool PluggableUSB_::plug(PluggableUSBModule *node) {
	if ((lastEp + node->numEndpoints) > USB_ENDPOINTS) {
		return false;
	}
	if (!rootNode) {
		rootNode = node;
	} else {
		PluggableUSBModule *current = rootNode;
		while (current->next) {
			current = current->next;
		}
		current->next = node;
	}

	node->pluggedInterface = lastIf;
	node->pluggedEndpoint = lastEp;
	lastIf += node->numInterfaces;
	for (uint8_t i = 0; i < node->numEndpoints; i++) {
		_initEndpoints[lastEp] = node->endpointType[i];
		lastEp++;
	}
	return true;
	// restart USB layer???
}

int PluggableUSB_::get_interface(uint8_t* interfaceCount) {
	int sent = 0;
	PluggableUSBModule* node;
	for (node = rootNode; node; node = node->next) {
		int res = node->get_interface(interfaceCount);
		if (res < 0)
			return -1;
		sent += res;
	}
	return sent;
}

int PluggableUSB_::get_descriptor(USBSetup& setup) {
	PluggableUSBModule* node;
	for (node = rootNode; node; node = node->next) {
		int ret = node->get_descriptor(setup);
		// ret!=0 -> request has been processed
		if (ret)
			return ret;
	}
	return 0;
}

void PluggableUSB_::get_short_name(char *iSerialNum) {
	PluggableUSBModule* node;
	for (node = rootNode; node; node = node->next) {
		iSerialNum += node->get_short_name(iSerialNum);
	}
	*iSerialNum = 0;
}

PluggableUSB_& PluggableUSB() {
	static PluggableUSB_ obj;
	return obj;
}

PluggableUSB_::PluggableUSB_() :
	lastIf(CDC_ACM_INTERFACE + CDC_INTERFACE_COUNT),
	lastEp(CDC_FIRST_ENDPOINT + CDC_ENPOINT_COUNT),
	rootNode(NULL) {

}


//###############################################
#endif  /* if defined(USBCON) && defined (PLUGGABLE_USB_ENABLED) */
//###############################################
