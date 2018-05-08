/**
 * @file	wave_analyzer.cpp
 * @brief	Initialization of Input Capture pins used for dev console sniffer
 *
 * This file is responsible for sniffing of external digital signals and registering
 * these digital events in WaveChart used by the Sniffer tab of Dev Console.
 *
 * this is rusEfi build-in logic analyzer
 *
 * @date Jan 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "wave_analyzer.h"
#include "eficonsole.h"
#include "data_buffer.h"
#include "pin_repository.h"
#include "engine_state.h"
#include "signal_executor.h"
#include "engine_configuration.h"
#include "trigger_central.h"
#include "rfiutil.h"
#include "engine_math.h"
#include "engine.h"
#include "rpm_calculator.h"
#include "engine_sniffer.h"

#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)

EXTERN_ENGINE;

#define CHART_RESET_DELAY 1
#define MAX_ICU_COUNT 5

#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
extern WaveChart waveChart;
#endif
extern bool hasFirmwareErrorFlag;

/**
 * Difference between current 1st trigger event and previous 1st trigger event.
 */
static volatile uint32_t engineCycleDurationUs;
static volatile efitime_t previousEngineCycleTimeUs = 0;

static int waveReaderCount = 0;
static WaveReader readers[MAX_ICU_COUNT];

static THD_WORKING_AREA(waThreadStack, UTILITY_THREAD_STACK_SIZE);
static Logging * logger;

static void ensureInitialized(WaveReader *reader) {
	efiAssertVoid(reader->hw->started, "wave analyzer NOT INITIALIZED");
}

#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)

static void waAnaWidthCallback(WaveReader *reader) {
	efitick_t nowUs = getTimeNowUs();
	reader->riseEventCounter++;
	reader->lastActivityTimeUs = nowUs;
	addEngineSniffferEvent(reader->name, WC_UP);

	uint32_t width = nowUs - reader->periodEventTimeUs;
	reader->last_wave_low_widthUs = width;

	reader->signalPeriodUs = nowUs - reader->widthEventTimeUs;
	reader->widthEventTimeUs = nowUs;
}

void WaveReader::onFallEvent() {
	efitick_t nowUs = getTimeNowUs();
	fallEventCounter++;
	lastActivityTimeUs = nowUs;
	addEngineSniffferEvent(name, WC_DOWN);

	efitick_t width = nowUs - widthEventTimeUs;
	last_wave_high_widthUs = width;

	int revolutionCounter = getRevolutionCounter();

	totalOnTimeAccumulatorUs += width;
	if (currentRevolutionCounter != revolutionCounter) {
		/**
		 * We are here in case of a new engine cycle
		 */
		currentRevolutionCounter = revolutionCounter;
		prevTotalOnTimeUs = totalOnTimeAccumulatorUs;
		totalOnTimeAccumulatorUs = 0;

		waveOffsetUs = nowUs - previousEngineCycleTimeUs;

	}

	periodEventTimeUs = nowUs;

//	uint32_t period = engineCycleDurationUs;  // local copy of volatile variable
}

static void waIcuPeriodCallback(WaveReader *reader) {
	reader->onFallEvent();
}

static void setWaveModeSilent(int index, int mode) {
	WaveReader *reader = &readers[index];

	startInputDriver(reader->hw, mode);
}

//static int getEventCounter(int index) {
//	WaveReader *reader = &readers[index];
//	ensureInitialized(reader);
//	return reader->eventCounter;
//}

static void initWave(const char *name, int index) {
	brain_pin_e brainPin = boardConfiguration->logicAnalyzerPins[index];

	bool mode = boardConfiguration->logicAnalyzerMode[index];

	waveReaderCount++;
	efiAssertVoid(index < MAX_ICU_COUNT, "too many ICUs");
	WaveReader *reader = &readers[index];
	reader->name = name;

	reader->hw = addWaveAnalyzerDriver("wave input", brainPin);


	reader->hw->widthListeners.registerCallback((VoidInt) waAnaWidthCallback, (void*) reader);

	reader->hw->periodListeners.registerCallback((VoidInt) waIcuPeriodCallback, (void*) reader);


	print("wave%d input on %s\r\n", index, hwPortname(brainPin));
	startInputDriver(reader->hw, mode);
}
#endif

WaveReader::WaveReader() {
	hw = NULL;
	last_wave_high_widthUs = 0;
	name = NULL;
	fallEventCounter = riseEventCounter = 0;
	currentRevolutionCounter = 0;
	prevTotalOnTimeUs = 0;
	totalOnTimeAccumulatorUs = 0;
	lastActivityTimeUs = 0;
	periodEventTimeUs = 0;
	widthEventTimeUs = 0;
	signalPeriodUs = 0;
	waveOffsetUs = 0;
	last_wave_low_widthUs = 0;
}

static void waTriggerEventListener(trigger_event_e ckpSignalType, uint32_t index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	(void)ckpSignalType;
	if (index != 0) {
		return;
	}
	efitick_t nowUs = getTimeNowUs();
	engineCycleDurationUs = nowUs - previousEngineCycleTimeUs;
	previousEngineCycleTimeUs = nowUs;
}

static THD_FUNCTION(waThread, arg) {
	(void)arg;
	chRegSetThreadName("Wave Analyzer");
#if EFI_ENGINE_SNIFFER
	while (true) {
		chThdSleepSeconds(CHART_RESET_DELAY);

		waveChart.publishIfFull();
	}
#endif /* EFI_ENGINE_SNIFFER */
}

static uint32_t getWaveLowWidth(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	return reader->last_wave_low_widthUs;
}

static float getSignalOnTime(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	if (getTimeNowUs() - reader->lastActivityTimeUs > 4 * US_PER_SECOND) {
		return 0.0f; // dwell time has expired
	}
	return reader->last_wave_high_widthUs / 1000.0f;
}

static efitime_t getWaveOffset(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	return reader->waveOffsetUs;
}

static float getSignalPeriodMs(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	return reader->signalPeriodUs / 1000.0f;
}

//static efitime_t getWidthEventTime(int index) {
//	WaveReader *reader = &readers[index];
//	ensureInitialized(reader);
//	return reader->widthEventTimeUs;
//}

static void reportWave(Logging *logging, int index) {
	if (readers[index].hw->started) {
//	int counter = getEventCounter(index);
//	debugInt2(logging, "ev", index, counter);

		float dwellMs = getSignalOnTime(index);
		float periodMs = getSignalPeriodMs(index);

		appendPrintf(logging, "duty%d%s", index, DELIMETER);
		appendFloat(logging, 100.0f * dwellMs / periodMs, 2);
		appendPrintf(logging, "%s", DELIMETER);

		/**
		 * that's the ON time of the LAST signal
		 */
		appendPrintf(logging, "dwell%d%s", index, DELIMETER);
		appendFloat(logging, dwellMs, 2);
		appendPrintf(logging, "%s", DELIMETER);

		/**
		 * that's the total ON time during the previous engine cycle
		 */
		appendPrintf(logging, "total_dwell%d%s", index, DELIMETER);
		appendFloat(logging, readers[index].prevTotalOnTimeUs / 1000.0f, 2);
		appendPrintf(logging, "%s", DELIMETER);

		appendPrintf(logging, "period%d%s", index, DELIMETER);
		appendFloat(logging, periodMs, 2);
		appendPrintf(logging, "%s", DELIMETER);

		uint32_t offsetUs = getWaveOffset(index);
		int rpm = getRpmE(engine);
		if (rpm != 0) {
			float oneDegreeUs = getOneDegreeTimeUs(rpm);

			appendPrintf(logging, "advance%d%s", index, DELIMETER);
			float angle = (offsetUs / oneDegreeUs) - tdcPosition();
			fixAngle(angle, "waveAn");
			appendFloat(logging, angle, 3);
			appendPrintf(logging, "%s", DELIMETER);
		}
	}
}

void printWave(Logging *logging) {
	reportWave(logging, 0);
	reportWave(logging, 1);
}

void showWaveInfo(void) {
	scheduleMsg(logger, "logic input #1: %d/%d", readers[0].fallEventCounter, readers[0].riseEventCounter);
}

void initWaveAnalyzer(Logging *sharedLogger) {
	logger = sharedLogger;
	if (hasFirmwareError()) {
		return;
	}
#if EFI_WAVE_ANALYZER || defined(__DOXYGEN__)
	initWave(WA_CHANNEL_1, 0);
	initWave(WA_CHANNEL_2, 1);

	addTriggerEventListener(waTriggerEventListener, "wave analyzer", engine);

	addConsoleAction("waveinfo", showWaveInfo);

	addConsoleActionII("set_logic_input_mode", setWaveModeSilent);

	chThdCreateStatic(waThreadStack, sizeof(waThreadStack), NORMALPRIO, (tfunc_t)waThread, NULL);

#else
	print("wave disabled\r\n");
#endif
}

#endif
