
#include "pch.h"
#include "usbconsole.h"
#include "hardware.h"

extern "C" {
	#include "boot.h"
	#include "shared_params.h"
}

int main(void) {
	halInit();
	chSysInit();

	baseMCUInit();

	// Init openblt shared params
	SharedParamsInit();

	// Init openblt itself
	BootInit();

	while (true) {
		BootTask();
	}
}

// very basic version, supports on chip pins only (really only used for USB)
void efiSetPadMode(const char* msg, brain_pin_e brainPin, iomode_t mode) {
	ioportid_t port = getHwPort(msg, brainPin);
	ioportmask_t pin = getHwPin(msg, brainPin);
	/* paranoid */
	if (port == GPIO_NULL) {
		return;
	}

	palSetPadMode(port, pin, mode);
}
