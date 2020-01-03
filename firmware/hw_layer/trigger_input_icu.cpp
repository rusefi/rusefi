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
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"

volatile int icuWidthCallbackCounter = 0;
volatile int icuWidthPeriodCounter = 0;

#if EFI_SHAFT_POSITION_INPUT && (HAL_USE_ICU == TRUE) && (HAL_USE_COMP == FALSE)

#include "trigger_input.h"
#include "digital_input_icu.h"

EXTERN_ENGINE;

extern bool hasFirmwareErrorFlag;

static Logging *logger;

static void vvtWidthCallback(void *arg) {
    (void)arg;
	hwHandleVvtCamSignal(TV_RISE);
}

static void vvtPeriodCallback(void *arg) {
    (void)arg;
	hwHandleVvtCamSignal(TV_FALL);
}

/**
 * that's hardware timer input capture IRQ entry point
 * 'width' events happens before the 'period' event
 */
static void shaftWidthCallback(bool isPrimary) {
	if (!engine->hwTriggerInputEnabled) {
		return;
	}
	icuWidthCallbackCounter++;
// todo: support for 3rd trigger input channel
// todo: start using real event time from HW event, not just software timer?
	if (hasFirmwareErrorFlag)
		return;
	if (!isPrimary && !TRIGGER_WAVEFORM(needSecondTriggerInput)) {
		return;
	}
	//	icucnt_t last_width = icuGetWidth(icup); so far we are fine with system time
	// todo: add support for 3rd channel
	trigger_event_e signal = isPrimary ? (engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_FALLING : SHAFT_PRIMARY_RISING) : (engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_FALLING : SHAFT_SECONDARY_RISING);

	hwHandleShaftSignal(signal);
}

static void shaftPeriodCallback(bool isPrimary) {
	if (!engine->hwTriggerInputEnabled) {
		return;
	}
	icuWidthPeriodCounter++;
	if (hasFirmwareErrorFlag)
		return;
	if (!isPrimary && !TRIGGER_WAVEFORM(needSecondTriggerInput)) {
		return;
	}

	// todo: add support for 3rd channel
	//	icucnt_t last_period = icuGetPeriod(icup); so far we are fine with system time
	trigger_event_e signal =
			isPrimary ? (engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING) : (engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING);
	hwHandleShaftSignal(signal);
}

void turnOnTriggerInputPin(const char *msg, int index, bool isTriggerShaft) {
	(void)msg;
	brain_pin_e brainPin = isTriggerShaft ? CONFIG(triggerInputPins)[index] : engineConfiguration->camInputs[index];
	if (brainPin == GPIO_UNASSIGNED) {
		return;
	}

	digital_input_s* input = startDigitalCapture("trigger", brainPin, true);
	if (input == NULL) {
		/* error already reported */
		return;
	}

	if (isTriggerShaft) {
		void * arg = (void*) (index == 0);
		input->setWidthCallback((VoidInt)(void*)shaftWidthCallback, arg);
		input->setPeriodCallback((VoidInt)(void*)shaftPeriodCallback, arg);
	} else {
		input->setWidthCallback((VoidInt)(void*)vvtWidthCallback, NULL);
		input->setPeriodCallback((VoidInt)(void*)vvtPeriodCallback, NULL);
	}
}

void turnOffTriggerInputPin(brain_pin_e brainPin) {

	stopDigitalCapture("trigger", brainPin);
}

void setPrimaryChannel(brain_pin_e brainPin) {
	(void)brainPin;
}

/*==========================================================================*/
/* Exported functions.														*/
/*==========================================================================*/

void turnOnTriggerInputPins(Logging *sharedLogger) {
	logger = sharedLogger;

	applyNewTriggerInputPins();
}
#endif /* (EFI_SHAFT_POSITION_INPUT && (HAL_USE_ICU == TRUE) && (HAL_USE_COMP == FALSE)) */
