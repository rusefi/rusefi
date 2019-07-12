/**
 * @file	trigger_input.cpp
 * @brief	Position sensor hardware layer (ICU and PAL drivers)
 *
 * todo: code reuse with digital_input_hw.cpp was never finished
 * todo: at the moment due to half-done code reuse we already depend on EFI_ICU_INPUTS but still have custom code
 * todo: VVT implementation is a nasty copy-paste :(
 *
 * see digital_input_hw.cp
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"

#ifndef HAL_TRIGGER_USE_PAL
#define HAL_TRIGGER_USE_PAL FALSE
#endif /* HAL_TRIGGER_USE_PAL */


#if EFI_SHAFT_POSITION_INPUT && (HAL_TRIGGER_USE_PAL == TRUE || HAL_USE_ICU == TRUE) && (HAL_USE_COMP == FALSE)

#include "trigger_input.h"
#include "digital_input_hw.h"
#include "digital_input_exti.h"
#include "pin_repository.h"
#include "trigger_structure.h"
#include "trigger_central.h"
#include "engine_configuration.h"

#define TRIGGER_SUPPORTED_CHANNELS 2

extern bool hasFirmwareErrorFlag;

EXTERN_ENGINE
;
static Logging *logger;

int vvtEventRiseCounter = 0;
int vvtEventFallCounter = 0;

#if EFI_PROD_CODE
/* PAL based implementation */
#if (HAL_TRIGGER_USE_PAL == TRUE) && (PAL_USE_CALLBACKS == TRUE)

/* static vars for PAL implementation */
static ioline_t primary_line;

static void shaft_callback(void *arg) {
	bool rise;
	bool isPrimary;
	ioline_t pal_line;
	trigger_event_e signal;

	pal_line = (ioline_t)arg;
	// todo: support for 3rd trigger input channel
	// todo: start using real event time from HW event, not just software timer?
	if (hasFirmwareErrorFlag)
		return;

	isPrimary = pal_line == primary_line;
	if (!isPrimary && !TRIGGER_SHAPE(needSecondTriggerInput)) {
		return;
	}

	rise = (palReadLine(pal_line) == PAL_HIGH);
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
	bool rise;
	ioline_t pal_line = (ioline_t)arg;

	rise = (palReadLine(pal_line) == PAL_HIGH);

	if (rise) {
		vvtEventRiseCounter++;
		hwHandleVvtCamSignal(TV_RISE);
	} else {
		vvtEventFallCounter++;
		hwHandleVvtCamSignal(TV_FALL);
	}
}

static int turnOnTriggerInputPin(const char *msg, brain_pin_e brainPin, bool is_shaft) {
	ioline_t pal_line;

	scheduleMsg(logger, "turnOnTriggerInputPin(PAL) %s %s", msg, hwPortname(brainPin));

	/* TODO:
	 * * do not set to both edges if we need only one
	 * * simplify callback in case of one edge */
	pal_line = PAL_LINE(getHwPort("trg", brainPin), getHwPin("trg", brainPin));
	return efiExtiEnablePin(msg, brainPin, PAL_EVENT_MODE_BOTH_EDGES, is_shaft ? shaft_callback : cam_callback, (void *)pal_line);
}

static void turnOffTriggerInputPin(brain_pin_e brainPin) {
	efiExtiDisablePin(brainPin);
}

static void setPrimaryChannel(brain_pin_e brainPin) {
	primary_line = PAL_LINE(getHwPort("trg", brainPin), getHwPin("trg", brainPin));
}

/* ICU based implementation */
#elif (HAL_USE_ICU)

/* static vars for ICU implementation */
static ICUDriver *primaryCrankDriver;

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

static int turnOnTriggerInputPin(const char *msg, brain_pin_e brainPin, bool is_shaft) {
	ICUConfig *icucfg;

	if (brainPin == GPIO_UNASSIGNED)
		return -1;

	if (is_shaft)
		icucfg = &shaft_icucfg;
	else
		icucfg = &cam_icucfg;

	// configure pin
	turnOnCapturePin(msg, brainPin);
	icucfg->channel = getInputCaptureChannel(brainPin);

	ICUDriver *driver = getInputCaptureDriver(msg, brainPin);
	scheduleMsg(logger, "turnOnTriggerInputPin %s", hwPortname(brainPin));
	// todo: reuse 'setWaveReaderMode' method here?
	if (driver != NULL) {
		// todo: once http://forum.chibios.org/phpbb/viewtopic.php?f=16&t=1757 is fixed
//		bool needWidthCallback = !CONFIG(useOnlyRisingEdgeForTrigger) || TRIGGER_SHAPE(useRiseEdge);
//		shaft_icucfg.width_cb = needWidthCallback ? shaft_icu_width_callback : NULL;

//		bool needPeriodCallback = !CONFIG(useOnlyRisingEdgeForTrigger) || !TRIGGER_SHAPE(useRiseEdge);
//		shaft_icucfg.period_cb = needPeriodCallback ? shaft_icu_period_callback : NULL;

		efiIcuStart(msg, driver, icucfg);
		if (driver->state == ICU_READY) {
			efiAssert(CUSTOM_ERR_ASSERT, driver != NULL, "ti: driver is NULL", -1);
			efiAssert(CUSTOM_ERR_ASSERT, driver->state == ICU_READY, "ti: driver not ready", -1);
            icuStartCapture(driver); // this would change state from READY to WAITING
            icuEnableNotifications(driver);
		} else {
			// we would be here for example if same pin is used for multiple input capture purposes
			firmwareError(CUSTOM_ERR_ICU_STATE, "ICU unexpected state [%s]", hwPortname(brainPin));
		}
	}
	return 0;
}

static void turnOffTriggerInputPin(brain_pin_e brainPin) {
	ICUDriver *driver = getInputCaptureDriver("trigger_off", brainPin);
	if (driver != NULL) {
        icuDisableNotifications(driver);
        icuStopCapture(driver);
		icuStop(driver);
		scheduleMsg(logger, "turnOffTriggerInputPin %s", hwPortname(brainPin));
		turnOffCapturePin(brainPin);
	}
}

static void setPrimaryChannel(brain_pin_e brainPin) {
	primaryCrankDriver = getInputCaptureDriver("primary", brainPin);
}

#endif /* HAL_USE_ICU */
#endif /* EFI_PROD_CODE */

/*==========================================================================*/
/* Exported functions.														*/
/*==========================================================================*/

void turnOnTriggerInputPins(Logging *sharedLogger) {
	logger = sharedLogger;

	applyNewTriggerInputPins();
}

void stopTriggerInputPins(void) {
#if EFI_PROD_CODE
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (CONFIGB(triggerInputPins)[i]
				!= activeConfiguration.bc.triggerInputPins[i]) {
			turnOffTriggerInputPin(activeConfiguration.bc.triggerInputPins[i]);
		}
	}
	if (engineConfiguration->camInputs[0] != activeConfiguration.camInputs[0]) {
		turnOffTriggerInputPin(activeConfiguration.camInputs[0]);
	}
#endif /* EFI_PROD_CODE */
}

void startTriggerInputPins(void) {
#if EFI_PROD_CODE
	for (int i = 0; i < TRIGGER_SUPPORTED_CHANNELS; i++) {
		if (CONFIGB(triggerInputPins)[i]
				!= activeConfiguration.bc.triggerInputPins[i]) {
			const char * msg = (i == 0 ? "trigger#1" : (i == 1 ? "trigger#2" : "trigger#3"));
			turnOnTriggerInputPin(msg, CONFIGB(triggerInputPins)[i], true);
		}
	}

	if (engineConfiguration->camInputs[0] != activeConfiguration.camInputs[0]) {
		turnOnTriggerInputPin("cam", engineConfiguration->camInputs[0], false);
	}

	setPrimaryChannel(CONFIGB(triggerInputPins)[0]);
#endif /* EFI_PROD_CODE */
}

void applyNewTriggerInputPins(void) {
	// first we will turn off all the changed pins
	stopTriggerInputPins();
	// then we will enable all the changed pins
	startTriggerInputPins();
}

#endif /* (EFI_SHAFT_POSITION_INPUT && (HAL_TRIGGER_USE_PAL == TRUE || HAL_USE_ICU == TRUE) && (HAL_USE_COMP == FALSE)) */
