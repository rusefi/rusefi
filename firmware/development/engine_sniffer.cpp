/**
 * @file	engine_sniffer.cpp
 * @brief	rusEfi console wave sniffer logic
 *
 * Here we have our own build-in logic analyzer. The data we aggregate here is sent to the
 * java UI rusEfi Console so that it can be displayed nicely in the Sniffer tab.
 *
 * Both external events (see logic_analyzer.cpp) and internal (see signal executors) are supported
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
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

#include "pch.h"
#include "os_access.h"
#include "engine_sniffer.h"

#if EFI_ENGINE_SNIFFER

#include "eficonsole.h"
#include "status_loop.h"

#define CHART_DELIMETER	'!'

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

static char WAVE_LOGGING_BUFFER[WAVE_LOGGING_SIZE] CCM_OPTIONAL;

int waveChartUsedSize;

//#define DEBUG_WAVE 1

/**
 * We want to skip some engine cycles to skip what was scheduled before parameters were changed
 */
static uint32_t skipUntilEngineCycle = 0;

#if ! EFI_UNIT_TEST
extern WaveChart waveChart;
static void resetNow(void) {
	skipUntilEngineCycle = getRevolutionCounter() + 3;
	waveChart.reset();
}
#endif // EFI_UNIT_TEST

WaveChart::WaveChart() : logging("wave chart", WAVE_LOGGING_BUFFER, sizeof(WAVE_LOGGING_BUFFER)) {
}

void WaveChart::init() {
	isInitialized = true;
	reset();
}

void WaveChart::reset() {
#if DEBUG_WAVE
	efiPrintf("reset while at ", counter);
#endif /* DEBUG_WAVE */
	logging.reset();
	counter = 0;
	startTimeNt = 0;
	collectingData = false;
	logging.appendPrintf( "%s%s", PROTOCOL_ENGINE_SNIFFER, LOG_DELIMITER);
}

void WaveChart::startDataCollection() {
	collectingData = true;
}

bool WaveChart::isStartedTooLongAgo() const {
	/**
	 * Say at 300rpm we should get at least four events per revolution.
	 * That's 300/60*4=20 events per second
	 * engineChartSize/20 is the longest meaningful chart.
	 *
	 */
	efitick_t chartDurationNt = getTimeNowNt() - startTimeNt;
	return startTimeNt != 0 && NT2US(chartDurationNt) > engineConfiguration->engineChartSize * 1000000 / 20;
}

bool WaveChart::isFull() const {
	return counter >= CONFIG(engineChartSize);
}

int WaveChart::getSize() {
	return counter;
}

#if ! EFI_UNIT_TEST
static void printStatus(void) {
	efiPrintf("engine chart: %s", boolToString(engineConfiguration->isEngineChartEnabled));
	efiPrintf("engine chart size=%d", engineConfiguration->engineChartSize);
}

static void setChartActive(int value) {
	engineConfiguration->isEngineChartEnabled = value;
	printStatus();
#if EFI_CLOCK_LOCKS
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
#endif // EFI_UNIT_TEST

void WaveChart::publishIfFull() {
	if (isFull() || isStartedTooLongAgo()) {
		publish();
		reset();
	}
}

void WaveChart::publish() {
	logging.appendPrintf( LOG_DELIMITER);
	waveChartUsedSize = logging.loggingSize();
#if DEBUG_WAVE
	Logging *l = &chart->logging;
	efiPrintf("IT'S TIME", strlen(l->buffer));
#endif
	if (ENGINE(isEngineChartEnabled)) {
		scheduleLogging(&logging);
	}
}

/**
 * @brief	Register an event for digital sniffer
 */
void WaveChart::addEvent3(const char *name, const char * msg) {
	ScopePerf perf(PE::EngineSniffer);
	efitick_t nowNt = getTimeNowNt();

	if (nowNt < pauseEngineSnifferUntilNt) {
		return;
	}
#if EFI_TEXT_LOGGING
	if (!ENGINE(isEngineChartEnabled)) {
		return;
	}
	if (skipUntilEngineCycle != 0 && getRevolutionCounter() < skipUntilEngineCycle)
		return;
#if EFI_SIMULATOR
	// todo: add UI control to enable this for firmware if desired
	// CONFIG(alignEngineSnifferAtTDC) &&
	if (!collectingData) {
		return;
	}
#endif
	efiAssertVoid(CUSTOM_ERR_6651, name!=NULL, "WC: NULL name");

#if EFI_PROD_CODE
	efiAssertVoid(CUSTOM_ERR_6652, getCurrentRemainingStack() > 32, "lowstck#2c");
#endif /* EFI_PROD_CODE */

	efiAssertVoid(CUSTOM_ERR_6653, isInitialized, "chart not initialized");
#if DEBUG_WAVE
	efiPrintf("current", chart->counter);
#endif /* DEBUG_WAVE */
	if (isFull()) {
		return;
	}

	// we have multiple threads writing to the same output buffer
	chibios_rt::CriticalSectionLocker csl;

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
	uint32_t time100 = NT2US(diffNt / ENGINE_SNIFFER_UNIT_US);

	if (logging.remainingSize() > 35) {
		/**
		 * printf is a heavy method, append is used here as a performance optimization
		 */
		logging.appendFast(name);
		logging.appendChar(CHART_LOG_DELIMITER);
		logging.appendFast(msg);
		logging.appendChar(CHART_LOG_DELIMITER);
//		time100 -= startTime100;

		itoa10(timeBuffer, time100);
		logging.appendFast(timeBuffer);
		logging.appendChar(CHART_LOG_DELIMITER);
		logging.terminate();
	}
#endif /* EFI_TEXT_LOGGING */
}

void initWaveChart(WaveChart *chart) {
	/**
	 * constructor does not work because we need specific initialization order
	 */
	chart->init();

#if EFI_HISTOGRAMS
	initHistogram(&engineSnifferHisto, "wave chart");
#endif /* EFI_HISTOGRAMS */

#if ! EFI_UNIT_TEST
	printStatus();
	addConsoleActionI("chartsize", setChartSize);
	addConsoleActionI("chart", setChartActive);
	addConsoleAction(CMD_RESET_ENGINE_SNIFFER, resetNow);
#endif // EFI_UNIT_TEST
}

#endif /* EFI_ENGINE_SNIFFER */
