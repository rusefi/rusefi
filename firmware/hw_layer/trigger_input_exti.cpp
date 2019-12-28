/**
 * @file	trigger_input_exti.cpp
 * @brief	Position sensor hardware layer - PAL version
 *
 * todo: VVT implementation is a nasty copy-paste :(
 *
 * see digital_input_icu.cp
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"

#if EFI_SHAFT_POSITION_INPUT && (HAL_TRIGGER_USE_PAL == TRUE) && (HAL_USE_COMP == FALSE)

#include "trigger_input.h"
#include "digital_input_exti.h"

extern bool hasFirmwareErrorFlag;

static Logging *logger;

EXTERN_ENGINE;
static ioline_t primary_line;

static void shaft_callback(void *arg) {
	ioline_t pal_line = (ioline_t)arg;
	// todo: support for 3rd trigger input channel
	// todo: start using real event time from HW event, not just software timer?
	if (hasFirmwareErrorFlag)
		return;

	bool isPrimary = pal_line == primary_line;
	if (!isPrimary && !TRIGGER_WAVEFORM(needSecondTriggerInput)) {
		return;
	}

	bool rise = (palReadLine(pal_line) == PAL_HIGH);
	trigger_event_e signal;
	// todo: add support for 3rd channel
	if (rise) {
		signal = isPrimary ?
					(engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_FALLING : SHAFT_PRIMARY_RISING) :
					(engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_FALLING : SHAFT_SECONDARY_RISING);
	} else {
		signal = isPrimary ?
					(engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING) :
					(engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING);
	}

	hwHandleShaftSignal(signal);
}

static void cam_callback(void *arg) {
	ioline_t pal_line = (ioline_t)arg;

	bool rise = (palReadLine(pal_line) == PAL_HIGH);

	if (rise) {
		hwHandleVvtCamSignal(TV_RISE);
	} else {
		hwHandleVvtCamSignal(TV_FALL);
	}
}

void turnOnTriggerInputPin(const char *msg, int index, bool isTriggerShaft) {
	brain_pin_e brainPin = isTriggerShaft ? CONFIG(triggerInputPins)[index] : engineConfiguration->camInputs[index];

	scheduleMsg(logger, "turnOnTriggerInputPin(PAL) %s %s", msg, hwPortname(brainPin));

	/* TODO:
	 * * do not set to both edges if we need only one
	 * * simplify callback in case of one edge */
	ioline_t pal_line = PAL_LINE(getHwPort("trg", brainPin), getHwPin("trg", brainPin));
	efiExtiEnablePin(msg, brainPin, PAL_EVENT_MODE_BOTH_EDGES, isTriggerShaft ? shaft_callback : cam_callback, (void *)pal_line);
}

void turnOffTriggerInputPin(brain_pin_e brainPin) {
	efiExtiDisablePin(brainPin);
}

void setPrimaryChannel(brain_pin_e brainPin) {
	primary_line = PAL_LINE(getHwPort("trg", brainPin), getHwPin("trg", brainPin));
}

void turnOnTriggerInputPins(Logging *sharedLogger) {
	logger = sharedLogger;

	applyNewTriggerInputPins();
}

#endif /* (EFI_SHAFT_POSITION_INPUT && (HAL_TRIGGER_USE_PAL == TRUE) && (HAL_USE_COMP == FALSE)) */
