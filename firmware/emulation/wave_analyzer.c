/**
 * @file	wave_analyzer.c
 * @brief	Initialization of Input Capture pins used for dev console sniffer
 *
 * This file is responsible for sniffing of external digital signals and registering
 * these digital events in WaveChart used by the Sniffer tab of Dev Console.
 *
 *
 * @date Jan 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "wave_analyzer.h"
#include "eficonsole.h"
#include "wave_math.h"
#include "data_buffer.h"
#include "pin_repository.h"
#include "engine_state.h"
#include "signal_executor.h"
#include "engine_configuration.h"
#include "trigger_central.h"
#include "rfiutil.h"

#define CHART_RESET_DELAY 1

extern engine_configuration_s *engineConfiguration;

static volatile uint32_t ckpPeriod; // different between current crank signal and previous crank signal
static volatile int previousCrankSignalStart = 0;

#define MAX_ICU_COUNT 5

static int waveReaderCount = 0;
static WaveReader readers[MAX_ICU_COUNT];
extern WaveChart waveChart;

static Logging logger;

static void ensureInitialized(WaveReader *reader) {
	if (!reader->hw.started)
		fatal("wave analyzer NOT INITIALIZED");
}

#ifdef EFI_WAVE_ANALYZER

static void waAnaWidthCallback(WaveReader *reader) {
	systime_t now = chTimeNow();
	reader->eventCounter++;
	reader->lastActivityTime = now;
	addWaveChartEvent(reader->name, "up", "");

	int width = overflowDiff(now, reader->periodEventTime);
	reader->last_wave_low_width = width;

	reader->signalPeriod = overflowDiff(now, reader->widthEventTime);
	reader->widthEventTime = now;
}

static void waIcuPeriodCallback(WaveReader *reader) {
	systime_t now = chTimeNow();
	reader->eventCounter++;
	reader->lastActivityTime = now;
	addWaveChartEvent(reader->name, "down", "");

	int width = overflowDiff(now, reader->widthEventTime);
	reader->last_wave_high_width = width;

	reader->periodEventTime = now;

	//scheduleSimpleMsg(&irqLogging, "co", reader->chart.counter);

//	dbAdd(&wavePeriodTime, now);

	int period = ckpPeriod;  // local copy of volatile variable

	int offset = overflowDiff(now, previousCrankSignalStart);

	if (offset > period / 2) {
		/**
		 * here we calculate the offset in relation to future cranking signal
		 */
		offset -= period;
	}
	reader->waveOffsetSystimer = offset;

	// we want only the low phase length, so we subsctract high width from period
//	processSignal(1, &dataPinReader, last_period - last_adc_response_width);
}

static void setWaveModeSilent(int index, int mode) {
	WaveReader *reader = &readers[index];

	setWaveReaderMode(&reader->hw, mode);
}

void setWaveMode(int index, int mode) {
	setWaveModeSilent(index, mode);
	print("wavemode%d:%d\r\n", index, mode);
}

int getWaveMode(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	return reader->hw.activeMode;
}

int getEventCounter(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	return reader->eventCounter;
}

static void initWave(char *name, int index, ICUDriver *driver, ioportid_t port, int pin, int mode) {
	waveReaderCount++;
	chDbgCheck(index < MAX_ICU_COUNT, "too many ICUs");
	WaveReader *reader = &readers[index];
	WaveReaderHw *hw = &reader->hw;

	reader->name = name;

	registerCallback(&hw->widthListeners, (IntListener) waAnaWidthCallback, reader);

	registerCallback(&hw->periodListeners, (IntListener) waIcuPeriodCallback, reader);

	initWaveAnalyzerDriver(hw, driver, port, pin);

	print("wave%d input on %s%d\r\n", index, portname(reader->hw.port), reader->hw.pin);
	setWaveReaderMode(hw, mode);
}
#endif

//int getCrankStart() {
//	return previousCrankSignalStart;
//}

//static int getCrankPeriod(void) {
//	return ckpPeriod;
//}

static void onWaveShaftSignal(ShaftEvents ckpSignalType, int index) {
	if (index != 0)
		return;
	systime_t now = chTimeNow();
	ckpPeriod = overflowDiff(now, previousCrankSignalStart);
	previousCrankSignalStart = now;
}

static WORKING_AREA(waThreadStack, UTILITY_THREAD_STACK_SIZE);

//static Logging logger;

static msg_t waThread(void *arg) {
	chRegSetThreadName("Wave Analyzer");

	while (TRUE) {
		chThdSleepSeconds(CHART_RESET_DELAY);

		publishChartIfFull(&waveChart);
	}
#if defined __GNUC__
	return -1;
#endif
}

int getWaveLowWidth(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	return reader->last_wave_low_width;
}

int getWaveHighWidth(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	if (overflowDiff(chTimeNow(), reader->lastActivityTime) > 4 * ckpPeriod)
		return 0; // dwell time has expired
	return reader->last_wave_high_width;
}

int getWaveOffset(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	return reader->waveOffsetSystimer;
}

int getSignalPeriod(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	return reader->signalPeriod;
}

int getWidthEventTime(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	return reader->widthEventTime;
}

int getPeriodEventTime(int index) {
	WaveReader *reader = &readers[index];
	ensureInitialized(reader);
	return reader->periodEventTime;
}

int waveBufferReported = 0;

static void reportWave(Logging *logging, int index) {
//	int counter = getEventCounter(index);
//	debugInt2(logging, "ev", index, counter);

	int dwell = getWaveHighWidth(index);
	int period = getSignalPeriod(index);

	appendPrintf(logging, "duty%d%s", index, DELIMETER);
	appendFloat(logging, 100.0 * dwell / period, 2);
	appendPrintf(logging, "%s", DELIMETER);

	appendPrintf(logging, "dwell%d%s", index, DELIMETER);
	appendFloat(logging, ((float) dwell) / TICKS_IN_MS, 2);
	appendPrintf(logging, "%s", DELIMETER);

	appendPrintf(logging, "period%d%s", index, DELIMETER);
	appendFloat(logging, ((float) period) / TICKS_IN_MS, 2);
	appendPrintf(logging, "%s", DELIMETER);

//	int crank = getCrankPeriod();

//	int offset = getWaveOffset(index);
//	debugFloat2(logging, "advance", index, 90.0 * offset / crank, 3);
//	debugInt2(logging, "offset", index, offset);
}

void printWave(Logging *logging) {
	reportWave(logging, 0);
	reportWave(logging, 1);
}

void initWaveAnalyzer(void) {
#ifdef EFI_WAVE_ANALYZER
	initLogging(&logger, "wave");

	initWave("input1 A8", 0, &LOGIC_ANALYZER_1_DRIVER, LOGIC_ANALYZER_1_PORT, LOGIC_ANALYZER_1_PIN, 1);
	initWave("input2 E5", 1, &LOGIC_ANALYZER_2_DRIVER, LOGIC_ANALYZER_2_PORT, LOGIC_ANALYZER_2_PIN, 1);
	//	initWave("input0 C6", 2, &WAVE_TIMER, WAVE_INPUT_PORT, WAVE_INPUT_PIN, 0);

	registerShaftPositionListener(&onWaveShaftSignal, "wave analyzer");

	addConsoleActionII("wm", setWaveModeSilent);

	chThdCreateStatic(waThreadStack, sizeof(waThreadStack), NORMALPRIO, waThread, NULL);

#else
	print("wave disabled\r\n");
#endif
}
