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
#include "tooth_logger.h"

EXTERN_ENGINE;

static Logging *logger;

static void vvtRisingCallback(void *arg) {
	efitick_t now = getTimeNowNt();
	TRIGGER_BAIL_IF_DISABLED
#if HW_CHECK_MODE
	TRIGGER_BAIL_IF_SELF_STIM
#endif
	int index = (int)arg;

#if EFI_TOOTH_LOGGER
	if (!CONFIG(displayLogicLevelsInEngineSniffer)) {
		// real physical fronts go into engine sniffer
		LogTriggerTooth(SHAFT_SECONDARY_RISING, now);
	}
#endif /* EFI_TOOTH_LOGGER */
	hwHandleVvtCamSignal(engineConfiguration->invertCamVVTSignal ? TV_FALL : TV_RISE, now, index);
}

static void vvtFallingCallback(void * arg) {
	efitick_t now = getTimeNowNt();
	TRIGGER_BAIL_IF_DISABLED
#if HW_CHECK_MODE
	TRIGGER_BAIL_IF_SELF_STIM
#endif
	int index = (int)arg;
#if EFI_TOOTH_LOGGER
	if (!CONFIG(displayLogicLevelsInEngineSniffer)) {
		LogTriggerTooth(SHAFT_SECONDARY_FALLING, now);
	}
#endif /* EFI_TOOTH_LOGGER */
	hwHandleVvtCamSignal(engineConfiguration->invertCamVVTSignal ? TV_RISE : TV_FALL, now, index);
}

/**
 * that's hardware timer input capture IRQ entry point
 */
static void shaftRisingCallback(bool isPrimary) {
	efitick_t stamp = getTimeNowNt();

	TRIGGER_BAIL_IF_DISABLED
#if HW_CHECK_MODE
	TRIGGER_BAIL_IF_SELF_STIM
#endif
	icuRisingCallbackCounter++;
// todo: support for 3rd trigger input channel

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

	TRIGGER_BAIL_IF_DISABLED
#if HW_CHECK_MODE
	TRIGGER_BAIL_IF_SELF_STIM
#endif

	icuFallingCallbackCounter++;

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
	if (!isBrainPinValid(brainPin)) {
		return -1;
	}

	digital_input_s* input = startDigitalCapture(msg, brainPin);
	if (input == NULL) {
		/* error already reported */
		return -1;
	}

	if (isTriggerShaft) {
		void * arg = (void*) (index == 0);
		input->setWidthCallback((VoidInt)(void*)shaftRisingCallback, arg);
		input->setPeriodCallback((VoidInt)(void*)shaftFallingCallback, arg);
	} else {
		void * arg = (void *)index;
		input->setWidthCallback((VoidInt)(void*)vvtRisingCallback, arg);
		input->setPeriodCallback((VoidInt)(void*)vvtFallingCallback, arg);
	}

	return 0;
}

void icuTriggerTurnOffInputPin(brain_pin_e brainPin) {
	stopDigitalCapture("trigger", brainPin);
}

void icuTriggerTurnOnInputPins() {
}
#endif /* (EFI_SHAFT_POSITION_INPUT && (HAL_USE_ICU == TRUE)) */
