/**
 * @file	trigger_input.c
 * @brief	Position sensor hardware layer
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "trigger_input.h"

#include "wave_analyzer_hw.h"
#include "pin_repository.h"
#include "trigger_structure.h"
#include "trigger_central.h"
#include "engine_configuration.h"

static WaveReaderHw primaryCrankInput;
static WaveReaderHw secondaryCrankInput;

extern engine_configuration_s *engineConfiguration;

#if EFI_SHAFT_POSITION_INPUT

/**
 * that's hardware timer input capture IRQ entry point
 * 'width' events happens before the 'period' event
 */
static void shaft_icu_width_callback(ICUDriver *icup) {
	//	icucnt_t last_width = icuGetWidth(icup); so far we are fine with system time
	ShaftEvents signal = icup == &PRIMARY_SHAFT_POSITION_INPUT_DRIVER ? SHAFT_PRIMARY_UP : SHAFT_SECONDARY_UP;

	hwHandleShaftSignal(signal);
}

static void shaft_icu_period_callback(ICUDriver *icup) {
	//	icucnt_t last_period = icuGetPeriod(icup); so far we are fine with system time
	ShaftEvents signal = icup == &PRIMARY_SHAFT_POSITION_INPUT_DRIVER ? SHAFT_PRIMARY_DOWN : SHAFT_SECONDARY_DOWN;
	hwHandleShaftSignal(signal);
}

/**
 * the main purpose of this configuration structure is to specify the input interrupt callbacks
 */
static ICUConfig shaft_icucfg = { ICU_INPUT_ACTIVE_LOW, 100000, /* 100kHz ICU clock frequency.   */
shaft_icu_width_callback, shaft_icu_period_callback };
#endif

void initShaftPositionInputCapture(void) {

#if EFI_SHAFT_POSITION_INPUT

	// initialize primary Input Capture Unit pin
	initWaveAnalyzerDriver(&primaryCrankInput, &PRIMARY_SHAFT_POSITION_INPUT_DRIVER, PRIMARY_SHAFT_POSITION_INPUT_PORT,
	PRIMARY_SHAFT_POSITION_INPUT_PIN);
	/**
	 * Start primary Input Capture Unit using given configuration
	 * @see shaft_icucfg for callback entry points
	 */
	shaft_icucfg.channel = PRIMARY_SHAFT_POSITION_INPUT_CHANNEL;
	print("initShaftPositionInputCapture 1 %s:%d\r\n", portname(PRIMARY_SHAFT_POSITION_INPUT_PORT),
	PRIMARY_SHAFT_POSITION_INPUT_PIN);
	icuStart(&PRIMARY_SHAFT_POSITION_INPUT_DRIVER, &shaft_icucfg);
	icuEnable(&PRIMARY_SHAFT_POSITION_INPUT_DRIVER);

	// initialize secondary Input Capture Unit pin
	if (engineConfiguration->needSecondTriggerInput) {
		initWaveAnalyzerDriver(&secondaryCrankInput, &SECONDARY_SHAFT_POSITION_INPUT_DRIVER,
		SECONDARY_SHAFT_POSITION_INPUT_PORT,
		SECONDARY_SHAFT_POSITION_INPUT_PIN);
		shaft_icucfg.channel = SECONDARY_SHAFT_POSITION_INPUT_CHANNEL;
		print("initShaftPositionInputCapture 2 %s:%d\r\n", portname(SECONDARY_SHAFT_POSITION_INPUT_PORT),
		SECONDARY_SHAFT_POSITION_INPUT_PIN);
		icuStart(&SECONDARY_SHAFT_POSITION_INPUT_DRIVER, &shaft_icucfg);
		icuEnable(&SECONDARY_SHAFT_POSITION_INPUT_DRIVER);
	}

#else
	print("crank input disabled\r\n");
#endif
}
