#include "pch.h"
#include "hellen_meta.h"

void hellenWbo() {
	engineConfiguration->enableAemXSeries = true;
}

void setHellenDefaultVrThresholds() {
	for (int i = 0;i<VR_THRESHOLD_COUNT;i++) {
		setLinearCurve(engineConfiguration->vrThreshold[i].rpmBins, 600, 7000, 100);
		setLinearCurve(engineConfiguration->vrThreshold[i].values, 0.6, 1.2, 0.1);
	}
}

void setHellen144LedPins() {
#ifdef EFI_COMMUNICATION_PIN
	engineConfiguration->communicationLedPin = EFI_COMMUNICATION_PIN;
#else
	engineConfiguration->communicationLedPin = GPIOE_7;
#endif /* EFI_COMMUNICATION_PIN */
	engineConfiguration->runningLedPin = GPIOG_1;
	engineConfiguration->warningLedPin = GPIOE_8;
}

void setHellen176LedPins() {
#ifdef EFI_COMMUNICATION_PIN
	engineConfiguration->communicationLedPin = EFI_COMMUNICATION_PIN;
#else
	engineConfiguration->communicationLedPin = GPIOH_10;
#endif /* EFI_COMMUNICATION_PIN */
	engineConfiguration->runningLedPin = GPIOH_9;  // green
	engineConfiguration->warningLedPin = GPIOH_11; // yellow
}

// this should be called before setHellenXXXLedPins()
void detectHellenBoardType() {
	// we test the LED1 pin because the red LED used has the smallest voltage drop,
	// and thus can be detected more accurately
	static const brain_pin_e led1Pins[2] = {
		// LED1 pin of the 176-pin mcu module (we check it first!)
		GPIOH_8,
		// LED1 pin of the 144-pin mcu module
		GPIOG_0,
	};
	int padState[2];
	// check each mcu module type sequentially
	for (int mcuType = 0; mcuType < 2; mcuType++) {
		brain_pin_e ledPin = led1Pins[mcuType];
		// set LED1 pin to output & clear the state (discharge parasitic capacitance)
		palSetPadMode(getBrainPinPort(ledPin), getBrainPinIndex(ledPin), PAL_MODE_OUTPUT_PUSHPULL);
		palClearPad(getBrainPinPort(ledPin), getBrainPinIndex(ledPin));
		// set LED1 pin to input
		palSetPadMode(getBrainPinPort(ledPin), getBrainPinIndex(ledPin), PAL_MODE_INPUT); // todo: currently we don't use PAL_MODE_INPUT_PULLDOWN - needs more testing
		// wait for the pin state to settle down
		chThdSleepMilliseconds(1);
		// get the pin states
		padState[mcuType] = 1;
		for (int i = 0; i < 4; i++) {
			// we get "1" only if all readings are "1"
			padState[mcuType] &= palReadPad(getBrainPinPort(ledPin), getBrainPinIndex(ledPin));
		}
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
