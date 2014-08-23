/**
 * @file	trigger_input.c
 * @brief	Position sensor hardware layer
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"

#if EFI_SHAFT_POSITION_INPUT

#include "trigger_input.h"
#include "wave_analyzer_hw.h"
#include "pin_repository.h"
#include "trigger_structure.h"
#include "trigger_central.h"
#include "engine_configuration.h"
#include "wave_analyzer_hw.h"

static WaveReaderHw primaryCrankInput;
static WaveReaderHw secondaryCrankInput;

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;
extern board_configuration_s *boardConfiguration;

static inline ICUDriver *getPrimaryInputCaptureDriver(void) {
	return getInputCaptureDriver(boardConfiguration->triggerInputPins[0]);
}

static inline ICUDriver *getSecondaryInputCaptureDriver(void) {
	return getInputCaptureDriver(boardConfiguration->triggerInputPins[1]);
}

/**
 * that's hardware timer input capture IRQ entry point
 * 'width' events happens before the 'period' event
 */
static void shaft_icu_width_callback(ICUDriver *icup) {
	int isPrimary = icup == getPrimaryInputCaptureDriver();
	if (!isPrimary && !engineConfiguration2->triggerShape.needSecondTriggerInput) {
		return;
	}
	//	icucnt_t last_width = icuGetWidth(icup); so far we are fine with system time
	// todo: add support for 3rd channel
	trigger_event_e signal = isPrimary ? SHAFT_PRIMARY_UP : SHAFT_SECONDARY_UP;

	hwHandleShaftSignal(signal);
}

static void shaft_icu_period_callback(ICUDriver *icup) {
	int isPrimary = icup == getPrimaryInputCaptureDriver();
	if (!isPrimary && !engineConfiguration2->triggerShape.needSecondTriggerInput) {
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

void initShaftPositionInputCapture(void) {

	ICUDriver *driver;

	driver = getInputCaptureDriver(boardConfiguration->triggerInputPins[0]);

	// todo: extract method!
	// initialize primary Input Capture Unit pin
	initWaveAnalyzerDriver(&primaryCrankInput, driver, getHwPort(boardConfiguration->triggerInputPins[0]),
			getHwPin(boardConfiguration->triggerInputPins[0]));
	/**
	 * Start primary Input Capture Unit using given configuration
	 * @see shaft_icucfg for callback entry points
	 */
	shaft_icucfg.channel = ICU_CHANNEL_1;
	print("initShaftPositionInputCapture 1 %s\r\n", hwPortname(boardConfiguration->triggerInputPins[0]));
	if (driver != NULL) {
		icuStart(driver, &shaft_icucfg);
		icuEnable(driver);
	}

	driver = getInputCaptureDriver(boardConfiguration->triggerInputPins[1]);
	// initialize secondary Input Capture Unit pin
	initWaveAnalyzerDriver(&secondaryCrankInput, getSecondaryInputCaptureDriver(),
			getHwPort(boardConfiguration->triggerInputPins[1]), getHwPin(boardConfiguration->triggerInputPins[1]));
	shaft_icucfg.channel = ICU_CHANNEL_1;
	print("initShaftPositionInputCapture 2 %s\r\n", hwPortname(boardConfiguration->triggerInputPins[1]));
	if (driver != NULL) {
		icuStart(driver, &shaft_icucfg);
		icuEnable(driver);
	}

	print("crank input disabled\r\n");
}

#endif /* EFI_SHAFT_POSITION_INPUT */
