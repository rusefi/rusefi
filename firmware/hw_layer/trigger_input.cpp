/**
 * @file	trigger_input.cpp
 * @brief	Position sensor hardware layer
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)

#include "trigger_input.h"
#include "wave_analyzer_hw.h"
#include "pin_repository.h"
#include "trigger_structure.h"
#include "trigger_central.h"
#include "engine_configuration.h"
#include "wave_analyzer_hw.h"

#define TRIGGER_SUPPORTED_CHANNELS 2

static ICUDriver *primaryCrankDriver;

EXTERN_ENGINE
;
static Logging *logger;

/**
 * that's hardware timer input capture IRQ entry point
 * 'width' events happens before the 'period' event
 */
static void shaft_icu_width_callback(ICUDriver *icup) {
// todo: support for 3rd trigger input channel
// todo: start using real event time from HW event, not just software timer?
	int isPrimary = icup == primaryCrankDriver;
	if (!isPrimary && !engine->triggerShape.needSecondTriggerInput) {
		return;
	}
	//	icucnt_t last_width = icuGetWidth(icup); so far we are fine with system time
	// todo: add support for 3rd channel
	trigger_event_e signal = isPrimary ? SHAFT_PRIMARY_UP : SHAFT_SECONDARY_UP;

	hwHandleShaftSignal(signal);
}

static void shaft_icu_period_callback(ICUDriver *icup) {
	int isPrimary = icup == primaryCrankDriver;
	if (!isPrimary && !engine->triggerShape.needSecondTriggerInput) {
		return;
	}

	// todo: add support for 3rd channel
	//	icucnt_t last_period = icuGetPeriod(icup); so far we are fine with system time
	trigger_event_e signal = isPrimary ? SHAFT_PRIMARY_DOWN : SHAFT_SECONDARY_DOWN;
	hwHandleShaftSignal(signal);
}

/**
 * the main purpose of this configuration structure is to specify the input interrupt callbacks
 */
static ICUConfig shaft_icucfg = { ICU_INPUT_ACTIVE_LOW, 100000, /* 100kHz ICU clock frequency.   */
shaft_icu_width_callback, shaft_icu_period_callback };

static ICUDriver *turnOnTriggerInputPin(brain_pin_e hwPin) {
	// configure pin
	turnOnCapturePin(hwPin);
	shaft_icucfg.channel = ICU_CHANNEL_1;

	ICUDriver *driver = getInputCaptureDriver(hwPin);
	scheduleMsg(logger, "turnOnTriggerInputPin %s", hwPortname(hwPin));
	// todo: reuse 'setWaveReaderMode' method here?
	if (driver != NULL) {
		efiIcuStart(driver, &shaft_icucfg);
		icuEnable(driver);
	}
	return driver;
}

static void turnOffTriggerInputPin(brain_pin_e hwPin) {
	ICUDriver *driver = getInputCaptureDriver(hwPin);
	if (driver != NULL) {
		icuDisable(driver);
		icuStop(driver);
		scheduleMsg(logger, "turnOffTriggerInputPin %s", hwPortname(hwPin));
		unmarkPin(hwPin);
	}
}

static void rememberPrimaryChannel(void) {
	primaryCrankDriver = getInputCaptureDriver(boardConfiguration->triggerInputPins[0]);
}

void turnOnTriggerInputPins(Logging *sharedLogger) {
	logger = sharedLogger;
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		turnOnTriggerInputPin(boardConfiguration->triggerInputPins[i]);
	}

	rememberPrimaryChannel();
}

void applyNewTriggerInputPins(engine_configuration_s *oldConfiguration) {
	// first we will turn off all the changed pins
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (boardConfiguration->triggerInputPins[i] != oldConfiguration->bc.triggerInputPins[i]) {
			turnOffTriggerInputPin(oldConfiguration->bc.triggerInputPins[i]);
		}
	}
	// then we will enable all the changed pins
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (boardConfiguration->triggerInputPins[i] != oldConfiguration->bc.triggerInputPins[i]) {
			turnOnTriggerInputPin(boardConfiguration->triggerInputPins[i]);
		}
	}

	turnOffTriggerInputPin(oldConfiguration->bc.triggerInputPins[1]);
	rememberPrimaryChannel();
}

#endif /* EFI_SHAFT_POSITION_INPUT */
