/**
 * @file	wave_chart.c
 * @brief	Dev console wave sniffer logic
 *
 * Here we have our own build-in logic analyzer. The data we aggregate here is sent to the
 * java UI Dev Console so that it can be displayed nicely in the Sniffer tab.
 *
 * Both external events (see wave_analyzer.c) and internal (see signal executors) are supported
 *
 * @date Jun 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#include "wave_chart.h"
#include "main.h"

#if EFI_WAVE_CHART

#include "eficonsole.h"
#include "status_loop.h"

#define CHART_DELIMETER	"!"

/**
 * This is the number of events in the digital chart which would be displayed
 * on the 'digital sniffer' pane
 */
#if EFI_PROD_CODE
static volatile int chartSize = 100;
#define WAVE_LOGGING_SIZE 5000
#else
// need more events for automated test
static volatile int chartSize = 400;
#define WAVE_LOGGING_SIZE 35000
#endif

static int isChartActive = TRUE;
//static int isChartActive = FALSE;

//#define DEBUG_WAVE 1

#if DEBUG_WAVE
static Logging debugLogging;
#endif

static Logging logger;

void resetWaveChart(WaveChart *chart) {
#if DEBUG_WAVE
	scheduleSimpleMsg(&debugLogging, "reset while at ", chart->counter);
#endif
	resetLogging(&chart->logging);
	chart->counter = 0;
	appendPrintf(&chart->logging, "wave_chart%s", DELIMETER);
}

static char WAVE_LOGGING_BUFFER[WAVE_LOGGING_SIZE] CCM_OPTIONAL;

static void printStatus(void) {
	scheduleIntValue(&logger, "chart", isChartActive);
	scheduleIntValue(&logger, "chartsize", chartSize);
}

static void setChartActive(int value) {
	isChartActive = value;
	printStatus();
}

void setChartSize(int newSize) {
	if (newSize < 5)
		return;
	chartSize = newSize;
	printStatus();
}

void publishChartIfFull(WaveChart *chart) {
	if (isWaveChartFull(chart)) {
		publishChart(chart);
		resetWaveChart(chart);
	}
}

int isWaveChartFull(WaveChart *chart) {
	return chart->counter >= chartSize;
}

void publishChart(WaveChart *chart) {
	appendPrintf(&chart->logging, DELIMETER);
#if DEBUG_WAVE
	Logging *l = &chart->logging;
	scheduleSimpleMsg(&debugLogging, "IT'S TIME", strlen(l->buffer));
#endif
	if (isChartActive && getFullLog())
		scheduleLogging(&chart->logging);
}

/**
 * @brief	Register a change in sniffed signal
 */
void addWaveChartEvent3(WaveChart *chart, const char *name, const char * msg, const char * msg2) {
	efiAssertVoid(chart->isInitialized, "chart not initialized");
#if DEBUG_WAVE
	scheduleSimpleMsg(&debugLogging, "current", chart->counter);
#endif
	if (isWaveChartFull(chart))
		return;
	bool alreadyLocked = lockOutputBuffer(); // we have multiple threads writing to the same output buffer
	appendPrintf(&chart->logging, "%s%s%s%s", name, CHART_DELIMETER, msg, CHART_DELIMETER);
	int time100 = getTimeNowUs() / 10;
	appendPrintf(&chart->logging, "%d%s%s", time100, msg2, CHART_DELIMETER);
	chart->counter++;
	if (!alreadyLocked)
		unlockOutputBuffer();
}

void initWaveChart(WaveChart *chart) {
	initLogging(&logger, "wave info");

	if (!isChartActive)
		printMsg(&logger, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! chart disabled");

	printStatus();

	initLoggingExt(&chart->logging, "wave chart", WAVE_LOGGING_BUFFER, sizeof(WAVE_LOGGING_BUFFER));
	chart->isInitialized = TRUE;
#if DEBUG_WAVE
	initLoggingExt(&debugLogging, "wave chart debug", &debugLogging.DEFAULT_BUFFER, sizeof(debugLogging.DEFAULT_BUFFER));
#endif

	resetWaveChart(chart);
	addConsoleActionI("chartsize", setChartSize);
	addConsoleActionI("chart", setChartActive);
}

#endif /* EFI_WAVE_CHART */
