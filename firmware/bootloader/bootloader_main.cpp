
#include "pch.h"
#include "usbconsole.h"
#include "hardware.h"

extern "C" {
	#include "boot.h"
	#include "flash.h"
	#include "shared_params.h"
}

class BlinkyThread : public chibios_rt::BaseStaticThread<256> {
protected:
	void main(void) override {
		Gpio yellow = getWarningLedPin();

		efiSetPadMode("yellow", yellow, PAL_MODE_OUTPUT_PUSHPULL);

		auto yellowPort = getBrainPinPort(yellow);
		auto yellowPin = getBrainPinIndex(yellow);

		palSetPad(yellowPort, yellowPin);

		while (true) {
			palTogglePad(yellowPort, yellowPin);
			// blink 3 times faster if Dual Bank is not enabled
			chThdSleepMilliseconds(isFlashDualBank() ? 250 : 80);
		}
	}
};

static BlinkyThread blinky;

int main(void) {
	halInit();
	chSysInit();

	baseMCUInit();

	// start the blinky thread
	blinky.start(NORMALPRIO + 10);

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
