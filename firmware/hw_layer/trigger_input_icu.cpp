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

static ICUDriver *primaryCrankDriver;

static void cam_icu_width_callback(ICUDriver *icup) {
    (void)icup;
	hwHandleVvtCamSignal(TV_RISE);
}

static void cam_icu_period_callback(ICUDriver *icup) {
    (void)icup;
	hwHandleVvtCamSignal(TV_FALL);
}

/**
 * that's hardware timer input capture IRQ entry point
 * 'width' events happens before the 'period' event
 */
static void shaft_icu_width_callback(ICUDriver *icup) {
	if (!engine->hwTriggerInputEnabled) {
		return;
	}
	icuWidthCallbackCounter++;
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
	if (!engine->hwTriggerInputEnabled) {
		return;
	}
	icuWidthPeriodCounter++;
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

int turnOnTriggerInputPin(const char *msg, int index, bool isVvtShaft) {

	brain_pin_e brainPin = isVvtShaft ? engineConfiguration->camInputs[index] : CONFIGB(triggerInputPins)[index];


	ICUConfig *icucfg;

	if (brainPin == GPIO_UNASSIGNED) {
		return -1;
	}

	//digital_input_s* input = startDigitalCapture("trigger", brainPin, true);
	if (isVvtShaft) {
		icucfg = &shaft_icucfg;
	} else {
		icucfg = &cam_icucfg;
	}

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

void turnOffTriggerInputPin(brain_pin_e brainPin) {

	ICUDriver *driver = getInputCaptureDriver("trigger_off", brainPin);
	if (driver != NULL) {
        icuDisableNotifications(driver);
        icuStopCapture(driver);
		icuStop(driver);
		scheduleMsg(logger, "turnOffTriggerInputPin %s", hwPortname(brainPin));
		turnOffCapturePin(brainPin);
	}
}

void setPrimaryChannel(brain_pin_e brainPin) {
	primaryCrankDriver = getInputCaptureDriver("primary", brainPin);
}

/*==========================================================================*/
/* Exported functions.														*/
/*==========================================================================*/

void turnOnTriggerInputPins(Logging *sharedLogger) {
	logger = sharedLogger;

	applyNewTriggerInputPins();
}
#endif /* (EFI_SHAFT_POSITION_INPUT && (HAL_USE_ICU == TRUE) && (HAL_USE_COMP == FALSE)) */
