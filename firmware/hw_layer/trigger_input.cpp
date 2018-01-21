/**
 * @file	trigger_input.cpp
 * @brief	Position sensor hardware layer
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)

#include "trigger_input.h"
#include "digital_input_hw.h"
#include "pin_repository.h"
#include "trigger_structure.h"
#include "trigger_central.h"
#include "engine_configuration.h"

#define TRIGGER_SUPPORTED_CHANNELS 2

static ICUDriver *primaryCrankDriver;

extern bool hasFirmwareErrorFlag;

EXTERN_ENGINE
;
static Logging *logger;

int vvtEventRiseCounter = 0;
int vvtEventFallCounter = 0;

static void cam_icu_width_callback(ICUDriver *icup) {
    (void)icup;
	vvtEventRiseCounter++;
	hwHandleVvtCamSignal(TV_RISE);
}

static void cam_icu_period_callback(ICUDriver *icup) {
    (void)icup;
	vvtEventFallCounter++;
	hwHandleVvtCamSignal(TV_FALL);
}

/**
 * that's hardware timer input capture IRQ entry point
 * 'width' events happens before the 'period' event
 */
static void shaft_icu_width_callback(ICUDriver *icup) {
// todo: support for 3rd trigger input channel
// todo: start using real event time from HW event, not just software timer?
	if (hasFirmwareErrorFlag)
		return;
	int isPrimary = icup == primaryCrankDriver;
	if (!isPrimary && !TRIGGER_SHAPE(needSecondTriggerInput)) {
		return;
	}
	//	icucnt_t last_width = icuGetWidth(icup); so far we are fine with system time
	// todo: add support for 3rd channel
	trigger_event_e signal = isPrimary ? (engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_FALLING : SHAFT_PRIMARY_RISING) : (engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_FALLING : SHAFT_SECONDARY_RISING);

	hwHandleShaftSignal(signal);
}

static void shaft_icu_period_callback(ICUDriver *icup) {
	if (hasFirmwareErrorFlag)
		return;
	int isPrimary = icup == primaryCrankDriver;
	if (!isPrimary && !TRIGGER_SHAPE(needSecondTriggerInput)) {
		return;
	}

	// todo: add support for 3rd channel
	//	icucnt_t last_period = icuGetPeriod(icup); so far we are fine with system time
	trigger_event_e signal =
			isPrimary ? (engineConfiguration->invertPrimaryTriggerSignal ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING) : (engineConfiguration->invertSecondaryTriggerSignal ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING);
	hwHandleShaftSignal(signal);
}

/**
 * the main purpose of this configuration structure is to specify the input interrupt callbacks
 */
static ICUConfig shaft_icucfg = { ICU_INPUT_ACTIVE_LOW,
                                  100000, /* 100kHz ICU clock frequency.   */
                                  shaft_icu_width_callback,
                                  shaft_icu_period_callback,
                                  NULL,
                                  ICU_CHANNEL_1,
                                  0};

/**
 * this is about VTTi and stuff kind of cam sensor
 */
static ICUConfig cam_icucfg = { ICU_INPUT_ACTIVE_LOW,
                                100000, /* 100kHz ICU clock frequency.   */
                                cam_icu_width_callback,
                                cam_icu_period_callback,
                                NULL,
                                ICU_CHANNEL_1,
                                0};


static ICUDriver *turnOnTriggerInputPin(const char *msg, brain_pin_e hwPin, ICUConfig *icucfg) {
	if (hwPin == GPIO_UNASSIGNED)
		return NULL;

	// configure pin
	turnOnCapturePin(msg, hwPin);
	icucfg->channel = getInputCaptureChannel(hwPin);

	ICUDriver *driver = getInputCaptureDriver(msg, hwPin);
	scheduleMsg(logger, "turnOnTriggerInputPin %s", hwPortname(hwPin));
	// todo: reuse 'setWaveReaderMode' method here?
	if (driver != NULL) {
		// todo: once http://forum.chibios.org/phpbb/viewtopic.php?f=16&t=1757 is fixed
//		bool needWidthCallback = !CONFIG(useOnlyRisingEdgeForTrigger) || TRIGGER_SHAPE(useRiseEdge);
//		shaft_icucfg.width_cb = needWidthCallback ? shaft_icu_width_callback : NULL;

//		bool needPeriodCallback = !CONFIG(useOnlyRisingEdgeForTrigger) || !TRIGGER_SHAPE(useRiseEdge);
//		shaft_icucfg.period_cb = needPeriodCallback ? shaft_icu_period_callback : NULL;

		efiIcuStart(driver, icucfg);
		if (driver->state == ICU_READY) {
			efiAssert(driver != NULL, "ti: driver is NULL", NULL);
			efiAssert(driver->state == ICU_READY, "ti: driver not ready", NULL);
            icuStartCapture(driver); // this would change state from READY to WAITING
            icuEnableNotifications(driver);
		} else {
			// we would be here for example if same pin is used for multiple input capture purposes
			firmwareError(CUSTOM_ERR_ICU_STATE, "ICU unexpected state [%s]", hwPortname(hwPin));
		}
	}
	return driver;
}

static void turnOffTriggerInputPin(brain_pin_e hwPin) {
	ICUDriver *driver = getInputCaptureDriver("trigger_off", hwPin);
	if (driver != NULL) {
        icuDisableNotifications(driver);
        icuStopCapture(driver);
		icuStop(driver);
		scheduleMsg(logger, "turnOffTriggerInputPin %s", hwPortname(hwPin));
		unmarkPin(hwPin);
	}
}

static void rememberPrimaryChannel(void) {
	primaryCrankDriver = getInputCaptureDriver("primary",
			boardConfiguration->triggerInputPins[0]);
}

void turnOnTriggerInputPins(Logging *sharedLogger) {
	logger = sharedLogger;

	applyNewTriggerInputPins();
}

void stopTriggerInputPins(void) {
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (boardConfiguration->triggerInputPins[i]
				!= activeConfiguration.bc.triggerInputPins[i]) {
			turnOffTriggerInputPin(activeConfiguration.bc.triggerInputPins[i]);
		}
	}
	if (engineConfiguration->camInput != activeConfiguration.camInput) {
		turnOffTriggerInputPin(activeConfiguration.camInput);
	}
}

void applyNewTriggerInputPins(void) {
// first we will turn off all the changed pins
	stopTriggerInputPins();

// then we will enable all the changed pins
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (boardConfiguration->triggerInputPins[i]
				!= activeConfiguration.bc.triggerInputPins[i]) {
			const char * msg = (i == 0 ? "trigger#1" : (i == 1 ? "trigger#2" : "trigger#3"));
			turnOnTriggerInputPin(msg, boardConfiguration->triggerInputPins[i], &shaft_icucfg);
		}
	}

	if (engineConfiguration->camInput != activeConfiguration.camInput) {
		turnOnTriggerInputPin("cam", engineConfiguration->camInput, &cam_icucfg);
	}

	rememberPrimaryChannel();
}

#endif /* EFI_SHAFT_POSITION_INPUT */
