
#include "pch.h"
#include "usbconsole.h"
#include "hardware.h"

extern "C" {
	#include "boot.h"
	#include "shared_params.h"
}

class BlinkyThread : public chibios_rt::BaseStaticThread<256> {
protected:
	void main(void) override {
		Gpio yellow = getWarningLedPin();
		Gpio blue = getCommsLedPin();
		Gpio green = getRunningLedPin();

		efiSetPadMode("yellow", yellow, PAL_MODE_OUTPUT_PUSHPULL);
		efiSetPadMode("blue", blue, PAL_MODE_OUTPUT_PUSHPULL);
		efiSetPadMode("green", green, PAL_MODE_OUTPUT_PUSHPULL);

		auto yellowPort = getBrainPinPort(yellow);
		auto yellowPin = getBrainPinIndex(yellow);
		auto bluePort = getBrainPinPort(blue);
		auto bluePin = getBrainPinIndex(blue);
		auto greenPort = getBrainPinPort(green);
		auto greenPin = getBrainPinIndex(green);

		palSetPad(yellowPort, yellowPin);
		palSetPad(bluePort, bluePin);
		palSetPad(greenPort, greenPin);

		while (true) {
			palTogglePad(yellowPort, yellowPin);
			palTogglePad(bluePort, bluePin);
			palTogglePad(greenPort, greenPin);
			chThdSleepMilliseconds(250);
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

// very basic version, supports on chip pins only (really only used for USB, LEDs)
void efiSetPadMode(const char* msg, brain_pin_e brainPin, iomode_t mode) {
	ioportid_t port = getHwPort(msg, brainPin);
	ioportmask_t pin = getHwPin(msg, brainPin);
	/* paranoid */
	if (port == GPIO_NULL) {
		return;
	}

	palSetPadMode(port, pin, mode);
}
