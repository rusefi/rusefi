/**
 * @file	engine_sniffer.cpp
 * @brief	Dev console wave sniffer logic
 *
 * Here we have our own build-in logic analyzer. The data we aggregate here is sent to the
 * java UI Dev Console so that it can be displayed nicely in the Sniffer tab.
 *
 * Both external events (see wave_analyzer.c) and internal (see signal executors) are supported
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "main.h"
#include "engine_sniffer.h"
#include "adc_inputs.h"

#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)

#include "engine_configuration.h"
#include "eficonsole.h"
#include "status_loop.h"

#define CHART_DELIMETER	'!'

#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
#include "rfiutil.h"
#include "histogram.h"
static histogram_s engineSnifferHisto;
#endif

EXTERN_ENGINE
;
extern uint32_t maxLockedDuration;

/**
 * This is the number of events in the digital chart which would be displayed
 * on the 'digital sniffer' pane
 */
#if EFI_PROD_CODE
#define WAVE_LOGGING_SIZE 5000
#else
#define WAVE_LOGGING_SIZE 35000
#endif

int waveChartUsedSize;

//#define DEBUG_WAVE 1

#if DEBUG_WAVE
static Logging debugLogging;
#endif /* DEBUG_WAVE */

static LoggingWithStorage logger("wave info");

/**
 * We want to skip some engine cycles to skip what was scheduled before parameters were changed
 */
static uint32_t skipUntilEngineCycle = 0;

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
extern WaveChart waveChart;
static void resetNow(void) {
	skipUntilEngineCycle = engine->rpmCalculator.getRevolutionCounter() + 3;
	waveChart.reset();
}
#endif

void WaveChart::reset() {
#if DEBUG_WAVE
	scheduleSimpleMsg(&debugLogging, "reset while at ", counter);
#endif /* DEBUG_WAVE */
	resetLogging(&logging);
	counter = 0;
	startTimeNt = 0;
	appendPrintf(&logging, "wave_chart%s", DELIMETER);
}

static char WAVE_LOGGING_BUFFER[WAVE_LOGGING_SIZE] CCM_OPTIONAL;

bool WaveChart::isStartedTooLongAgo() {
	/**
	 * Say at 300rpm we should get at least four events per revolution.
	 * That's 300/60*4=20 events per second
	 * engineChartSize/20 is the longest meaningful chart.
	 *
	 */
	efitime_t chartDurationNt = getTimeNowNt() - startTimeNt;
	return startTimeNt != 0 && NT2US(chartDurationNt) > engineConfiguration->engineChartSize * 1000000 / 20;
}

bool WaveChart::isFull() {
	return counter >= CONFIG(engineChartSize);
}

static void printStatus(void) {
	scheduleMsg(&logger, "engine chart: %s", boolToString(engineConfiguration->isEngineChartEnabled));
	scheduleMsg(&logger, "engine chart size=%d", engineConfiguration->engineChartSize);
}

static void setChartActive(int value) {
	engineConfiguration->isEngineChartEnabled = value;
	printStatus();
#if EFI_CLOCK_LOCKS || defined(__DOXYGEN__)
	maxLockedDuration = 0; // todo: why do we reset this here? why only this and not all metrics?
#endif /* EFI_CLOCK_LOCKS */
}

void setChartSize(int newSize) {
	if (newSize < 5) {
		return;
	}
	engineConfiguration->engineChartSize = newSize;
	printStatus();
}

void WaveChart::publishIfFull() {
	if (isFull() || isStartedTooLongAgo()) {
		publish();
		reset();
	}
}

WaveChart::WaveChart() {
	isInitialized = false;
	startTimeNt = 0;
	counter = 0;
}

void WaveChart::init() {
	initLoggingExt(&logging, "wave chart", WAVE_LOGGING_BUFFER, sizeof(WAVE_LOGGING_BUFFER));
	isInitialized = true;
	reset();
}

void WaveChart::publish() {
	appendPrintf(&logging, DELIMETER);
	waveChartUsedSize = loggingSize(&logging);
#if DEBUG_WAVE
	Logging *l = &chart->logging;
	scheduleSimpleMsg(&debugLogging, "IT'S TIME", strlen(l->buffer));
#endif
	bool isFullLog = getFullLog();
	if (ENGINE(isEngineChartEnabled) && isFullLog) {
		scheduleLogging(&logging);
	}
}

static char timeBuffer[10];

/**
 * @brief	Register an event for digital sniffer
 */
void WaveChart::addEvent3(const char *name, const char * msg) {
	if (!ENGINE(isEngineChartEnabled)) {
		return;
	}
	if (skipUntilEngineCycle != 0 && ENGINE(rpmCalculator.getRevolutionCounter()) < skipUntilEngineCycle)
		return;
	efiAssertVoid(name!=NULL, "WC: NULL name");

#if EFI_PROD_CODE
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 32, "lowstck#2c");
#endif

	efiAssertVoid(isInitialized, "chart not initialized");
#if DEBUG_WAVE
	scheduleSimpleMsg(&debugLogging, "current", chart->counter);
#endif
	if (isFull()) {
		return;
	}

#if EFI_HISTOGRAMS && EFI_PROD_CODE
	int beforeCallback = hal_lld_get_counter_value();
#endif

	efitick_t nowNt = getTimeNowNt();

	bool alreadyLocked = lockOutputBuffer(); // we have multiple threads writing to the same output buffer

	if (counter == 0) {
		startTimeNt = nowNt;
	}
	counter++;

	/**
	 * We want smaller times within a chart in order to reduce packet size.
	 */
	/**
	 * todo: migrate to binary fractions in order to eliminate
	 * this division? I do not like division
	 *
	 * at least that's 32 bit division now
	 */
	uint32_t diffNt = nowNt - startTimeNt;
	uint32_t time100 = NT2US(diffNt / 10);

	if (remainingSize(&logging) > 35) {
		/**
		 * printf is a heavy method, append is used here as a performance optimization
		 */
		appendFast(&logging, name);
		appendChar(&logging, CHART_DELIMETER);
		appendFast(&logging, msg);
		appendChar(&logging, CHART_DELIMETER);
//		time100 -= startTime100;

		itoa10(timeBuffer, time100);
		appendFast(&logging, timeBuffer);
		appendChar(&logging, CHART_DELIMETER);
		logging.linePointer[0] = 0;
	}
	if (!alreadyLocked) {
		unlockOutputBuffer();
	}

#if EFI_HISTOGRAMS && EFI_PROD_CODE
	int64_t diff = hal_lld_get_counter_value() - beforeCallback;
	if (diff > 0) {
		hsAdd(&engineSnifferHisto, diff);
	}
#endif /* EFI_HISTOGRAMS */

}

void showWaveChartHistogram(void) {
#if (EFI_HISTOGRAMS && EFI_PROD_CODE) || defined(__DOXYGEN__)
	printHistogram(&logger, &engineSnifferHisto);
#endif
}

void initWaveChart(WaveChart *chart) {
	/**
	 * constructor does not work because we need specific initialization order
	 */
	chart->init();

	printStatus();

#if DEBUG_WAVE || defined(__DOXYGEN__)
	initLoggingExt(&debugLogging, "wave chart debug", &debugLogging.DEFAULT_BUFFER, sizeof(debugLogging.DEFAULT_BUFFER));
#endif

#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
	initHistogram(&engineSnifferHisto, "wave chart");
#endif /* EFI_HISTOGRAMS */

	addConsoleActionI("chartsize", setChartSize);
	addConsoleActionI("chart", setChartActive);
#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
	addConsoleAction("reset_engine_chart", resetNow);
#endif
}

#endif /* EFI_ENGINE_SNIFFER */
