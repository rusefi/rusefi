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

#include "main.h"
#include "wave_chart.h"

#if EFI_WAVE_CHART

#include "engine_configuration.h"
#include "eficonsole.h"
#include "status_loop.h"

#define CHART_DELIMETER	"!"

#if EFI_HISTOGRAMS
#include "rfiutil.h"
#include "histogram.h"
static histogram_s waveChartHisto;
#endif

extern engine_configuration_s *engineConfiguration;

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

static char WAVE_LOGGING_BUFFER[WAVE_LOGGING_SIZE] CCM_OPTIONAL
;

static int isWaveChartFull(WaveChart *chart) {
	return chart->counter >= engineConfiguration->digitalChartSize;
}

static void printStatus(void) {
	scheduleIntValue(&logger, "chart", isChartActive);
	scheduleIntValue(&logger, "chartsize", engineConfiguration->digitalChartSize);
}

static void setChartActive(int value) {
	isChartActive = value;
	printStatus();
}

void setChartSize(int newSize) {
	if (newSize < 5) {
		return;
	}
	engineConfiguration->digitalChartSize = newSize;
	printStatus();
}

void publishChartIfFull(WaveChart *chart) {
	if (isWaveChartFull(chart)) {
		publishChart(chart);
		resetWaveChart(chart);
	}
}

void publishChart(WaveChart *chart) {
	appendPrintf(&chart->logging, DELIMETER);
	waveChartUsedSize = loggingSize(&chart->logging);
#if DEBUG_WAVE
	Logging *l = &chart->logging;
	scheduleSimpleMsg(&debugLogging, "IT'S TIME", strlen(l->buffer));
#endif
	bool isFullLog = getFullLog();
	if (isChartActive && isFullLog) {
		scheduleLogging(&chart->logging);
	}
}

static char timeBuffer[10];

/**
 * @brief	Register an event for digital sniffer
 */
void addWaveChartEvent3(WaveChart *chart, const char *name, const char * msg, const char * msg2) {
	efiAssertVoid(chart->isInitialized, "chart not initialized");
#if DEBUG_WAVE
	scheduleSimpleMsg(&debugLogging, "current", chart->counter);
#endif
	if (isWaveChartFull(chart)) {
		return;
	}

#if EFI_HISTOGRAMS && EFI_PROD_CODE
	int beforeCallback = hal_lld_get_counter_value();
#endif

	int time100 = getTimeNowUs() / 10;

	bool alreadyLocked = lockOutputBuffer(); // we have multiple threads writing to the same output buffer

	if (chart->counter == 0) {
		chart->startTime = time100;
	}
	chart->counter++;
	if (remainingSize(&chart->logging) > 30) {
		/**
		 * printf is a heavy method, append is used here as a performance optimization
		 */
		appendFast(&chart->logging, name);
		appendFast(&chart->logging, CHART_DELIMETER);
		appendFast(&chart->logging, msg);
		appendFast(&chart->logging, CHART_DELIMETER);
		/**
		 * We want smaller times within a chart in order to reduce packet size.
		 */
		time100 -= chart->startTime;

		itoa10(timeBuffer, time100);
		appendFast(&chart->logging, timeBuffer);
		appendFast(&chart->logging, msg2);
		appendFast(&chart->logging, CHART_DELIMETER);
	}
	if (!alreadyLocked) {
		unlockOutputBuffer();
	}

#if EFI_HISTOGRAMS && EFI_PROD_CODE
	int64_t diff = hal_lld_get_counter_value() - beforeCallback;
	if (diff > 0) {
		hsAdd(&waveChartHisto, diff);
	}
#endif /* EFI_HISTOGRAMS */

}

void showWaveChartHistogram(void) {
#if EFI_PROD_CODE
	printHistogram(&logger, &waveChartHisto);
#endif
}

void initWaveChart(WaveChart *chart) {
	initLogging(&logger, "wave info");

	if (!isChartActive) {
		printMsg(&logger, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! chart disabled");
	}

	printStatus();

	initLoggingExt(&chart->logging, "wave chart", WAVE_LOGGING_BUFFER, sizeof(WAVE_LOGGING_BUFFER));
	chart->isInitialized = TRUE;
#if DEBUG_WAVE
	initLoggingExt(&debugLogging, "wave chart debug", &debugLogging.DEFAULT_BUFFER, sizeof(debugLogging.DEFAULT_BUFFER));
#endif

#if EFI_HISTOGRAMS
	initHistogram(&waveChartHisto, "wave chart");
#endif /* EFI_HISTOGRAMS */

	resetWaveChart(chart);
	addConsoleActionI("chartsize", setChartSize);
	addConsoleActionI("chart", setChartActive);
}

#endif /* EFI_WAVE_CHART */
