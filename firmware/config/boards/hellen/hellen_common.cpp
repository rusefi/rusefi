#include "pch.h"
#include "hellen_meta.h"

void hellenWbo() {
	engineConfiguration->enableAemXSeries = true;
}

void setHellen144LedPins() {
#ifdef EFI_COMMUNICATION_PIN
	engineConfiguration->communicationLedPin = EFI_COMMUNICATION_PIN;
#else
	engineConfiguration->communicationLedPin = H144_LED3;
#endif /* EFI_COMMUNICATION_PIN */
	engineConfiguration->runningLedPin = H144_LED2;
	engineConfiguration->warningLedPin = H144_LED4;
}

void setHellen176LedPins() {
#ifdef EFI_COMMUNICATION_PIN
	engineConfiguration->communicationLedPin = EFI_COMMUNICATION_PIN;
#else
	engineConfiguration->communicationLedPin = Gpio::H10;
#endif /* EFI_COMMUNICATION_PIN */
	engineConfiguration->runningLedPin = Gpio::H9;  // green
	engineConfiguration->warningLedPin = Gpio::H11; // yellow
}

// this should be called before setHellenXXXLedPins()
void detectHellenMcuType() {
	// we test the red LED1 pin because the red LED used has the smallest voltage drop,
	// and thus can be detected more accurately
	static const brain_pin_e led1Pins[2] = {
		// LED1 pin of the 176-pin mcu module (we check it first!)
		H176_LED1_RED,
		// LED1 pin of the 144-pin mcu module
		H144_LED1_RED,
	};
	int padState[2];
	// check each mcu module type sequentially
	for (int mcuType = 0; mcuType < 2; mcuType++) {
		brain_pin_e ledPin = led1Pins[mcuType];
		ioportid_t port = getBrainPinPort(ledPin);
		int hwIndex = getBrainPinIndex(ledPin);
		// set LED1 pin to output & clear the state (discharge parasitic capacitance)
		palSetPadMode(port, hwIndex, PAL_MODE_OUTPUT_PUSHPULL);
		palClearPad(port, hwIndex);
		// set LED1 pin to input
		palSetPadMode(port, hwIndex, PAL_MODE_INPUT); // todo: currently we don't use PAL_MODE_INPUT_PULLDOWN - needs more testing
		// wait for the pin state to settle down
		chThdSleepMilliseconds(1);
		// get the pin states
		padState[mcuType] = 1;
		for (int i = 0; i < 4; i++) {
			// we get "1" only if all readings are "1"
			padState[mcuType] &= palReadPad(port, hwIndex);
		}
		// back to output mode
		palSetPadMode(port, hwIndex, PAL_MODE_OUTPUT_PUSHPULL);
	}
	efiPrintf("Hellen board pin states = %d %d", padState[0], padState[1]);
	if (padState[0] && !padState[1]) {
		efiPrintf("* Hellen 176-pin mcu detected!");
	}
	else if (!padState[0] && padState[1]) {
		efiPrintf("* Hellen 144-pin mcu detected!");
	} else {
		efiPrintf("* Cannot detect Hellen mcu module!");
	}
}

void detectHellenBoardType() {
	detectHellenMcuType();
	engine->engineState.hellenBoardId = detectHellenBoardId();
}
