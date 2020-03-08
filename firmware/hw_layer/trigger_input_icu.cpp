/**
 * @file	trigger_input_icu.cpp
 * @brief	Position sensor hardware layer - ICU version
 *
 * todo: code reuse with digital_input_icu.cpp was never finished
 * todo: at the moment due to half-done code reuse we already depend on EFI_ICU_INPUTS but still have custom code
 * todo: VVT implementation is a nasty copy-paste :(
 *
 * see digital_input_icu.cpp
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"

int icuRisingCallbackCounter = 0;
int icuFallingCallbackCounter = 0;

#if EFI_SHAFT_POSITION_INPUT && (HAL_USE_ICU == TRUE)

#include "trigger_input.h"
#include "digital_input_icu.h"

EXTERN_ENGINE;

extern bool hasFirmwareErrorFlag;

static Logging *logger;

static void vvtRisingCallback(void *) {
	hwHandleVvtCamSignal(TV_RISE, getTimeNowNt());
}

static void vvtFallingCallback(void *) {
	hwHandleVvtCamSignal(TV_FALL, getTimeNowNt());
}

/**
 * that's hardware timer input capture IRQ entry point
 */
static void shaftRisingCallback(bool isPrimary) {
	efitick_t stamp = getTimeNowNt();

	if (!engine->hwTriggerInputEnabled) {
		return;
	}
	icuRisingCallbackCounter++;
// todo: support for 3rd trigger input channel
	if (hasFirmwareErrorFlag)
		return;
	if (!isPrimary && !TRIGGER_WAVEFORM(needSecondTriggerInput)) {
		return;
	}
	//	icucnt_t last_width = icuGetWidth(icup); so far we are fine with system time
	// todo: add support for 3rd channel
	trigger_event_e signal = isPrimary ? (engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_FALLING : SHAFT_PRIMARY_RISING) : (engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_FALLING : SHAFT_SECONDARY_RISING);

	hwHandleShaftSignal(signal, stamp);
}

static void shaftFallingCallback(bool isPrimary) {
	efitick_t stamp = getTimeNowNt();

	if (!engine->hwTriggerInputEnabled) {
		return;
	}

	icuFallingCallbackCounter++;

	if (hasFirmwareErrorFlag)
		return;
	if (!isPrimary && !TRIGGER_WAVEFORM(needSecondTriggerInput)) {
		return;
	}

	// todo: add support for 3rd channel
	trigger_event_e signal =
			isPrimary ? (engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING) : (engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING);
	hwHandleShaftSignal(signal, stamp);
}

/*==========================================================================*/
/* Exported functions.														*/
/*==========================================================================*/

int icuTriggerTurnOnInputPin(const char *msg, int index, bool isTriggerShaft) {
	(void)msg;
	brain_pin_e brainPin = isTriggerShaft ? CONFIG(triggerInputPins)[index] : engineConfiguration->camInputs[index];
	if (brainPin == GPIO_UNASSIGNED) {
		return -1;
	}

	digital_input_s* input = startDigitalCapture("trigger", brainPin);
	if (input == NULL) {
		/* error already reported */
		return -1;
	}

	if (isTriggerShaft) {
		void * arg = (void*) (index == 0);
		input->setWidthCallback((VoidInt)(void*)shaftRisingCallback, arg);
		input->setPeriodCallback((VoidInt)(void*)shaftFallingCallback, arg);
	} else {
		input->setWidthCallback((VoidInt)(void*)vvtRisingCallback, NULL);
		input->setPeriodCallback((VoidInt)(void*)vvtFallingCallback, NULL);
	}

	return 0;
}

void icuTriggerTurnOffInputPin(brain_pin_e brainPin) {

	stopDigitalCapture("trigger", brainPin);
}

void icuTriggerSetPrimaryChannel(brain_pin_e brainPin) {
	(void)brainPin;
}

void icuTriggerTurnOnInputPins(Logging *sharedLogger) {
	logger = sharedLogger;
}
#endif /* (EFI_SHAFT_POSITION_INPUT && (HAL_USE_ICU == TRUE)) */
