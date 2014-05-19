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
extern board_configuration_s *boardConfiguration;


static inline ICUDriver *getPrimaryInputCaptureDriver(void) {
	return getInputCaptureDriver(boardConfiguration->primaryTriggerInputPin);
}

static inline ICUDriver *getSecondaryInputCaptureDriver(void) {
	return getInputCaptureDriver(boardConfiguration->secondaryTriggerInputPin);
}

/**
 * that's hardware timer input capture IRQ entry point
 * 'width' events happens before the 'period' event
 */
static void shaft_icu_width_callback(ICUDriver *icup) {
	int isPrimary = icup == getPrimaryInputCaptureDriver();
	if (!isPrimary && !engineConfiguration->needSecondTriggerInput)
		return;
	//	icucnt_t last_width = icuGetWidth(icup); so far we are fine with system time
	ShaftEvents signal = isPrimary ? SHAFT_PRIMARY_UP : SHAFT_SECONDARY_UP;

	hwHandleShaftSignal(signal);
}

static void shaft_icu_period_callback(ICUDriver *icup) {
	int isPrimary = icup == getPrimaryInputCaptureDriver();
	if (!isPrimary && !engineConfiguration->needSecondTriggerInput)
		return;

	//	icucnt_t last_period = icuGetPeriod(icup); so far we are fine with system time
	ShaftEvents	signal = isPrimary ? SHAFT_PRIMARY_DOWN : SHAFT_SECONDARY_DOWN;
	hwHandleShaftSignal(signal);
}

/**
 * the main purpose of this configuration structure is to specify the input interrupt callbacks
 */
static ICUConfig shaft_icucfg = { ICU_INPUT_ACTIVE_LOW, 100000, /* 100kHz ICU clock frequency.   */
shaft_icu_width_callback, shaft_icu_period_callback };

void initShaftPositionInputCapture(void) {


	// initialize primary Input Capture Unit pin
	initWaveAnalyzerDriver(&primaryCrankInput, getPrimaryInputCaptureDriver(),
			getHwPort(boardConfiguration->primaryTriggerInputPin),
			getHwPin(boardConfiguration->primaryTriggerInputPin));
	/**
	 * Start primary Input Capture Unit using given configuration
	 * @see shaft_icucfg for callback entry points
	 */
	shaft_icucfg.channel = ICU_CHANNEL_1;
	print("initShaftPositionInputCapture 1 %s:%d\r\n", portname(getHwPort(boardConfiguration->primaryTriggerInputPin)),
			getHwPin(boardConfiguration->primaryTriggerInputPin));
	icuStart(getPrimaryInputCaptureDriver(), &shaft_icucfg);
	icuEnable(getPrimaryInputCaptureDriver());

	// initialize secondary Input Capture Unit pin
	initWaveAnalyzerDriver(&secondaryCrankInput, getSecondaryInputCaptureDriver(),
			getHwPort(boardConfiguration->secondaryTriggerInputPin),
			getHwPin(boardConfiguration->secondaryTriggerInputPin));
	shaft_icucfg.channel = ICU_CHANNEL_1;
	print("initShaftPositionInputCapture 2 %s:%d\r\n", portname(getHwPort(boardConfiguration->secondaryTriggerInputPin)),
			getHwPin(boardConfiguration->secondaryTriggerInputPin));
	icuStart(getSecondaryInputCaptureDriver(), &shaft_icucfg);
	icuEnable(getSecondaryInputCaptureDriver());

	print("crank input disabled\r\n");
}

#endif /* EFI_SHAFT_POSITION_INPUT */
