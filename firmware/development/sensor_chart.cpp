/**
 * @file	sensor_chart.cpp
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "sensor_chart.h"
#include "global.h"
#include "os_access.h"
#include "engine.h"
#include "rpm_calculator.h"

#if EFI_SENSOR_CHART
#include "status_loop.h"

#if EFI_TEXT_LOGGING
static char LOGGING_BUFFER[5000] CCM_OPTIONAL;
static Logging scLogging("analog chart", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));
#endif /* EFI_TEXT_LOGGING */

static int pendingData = false;
static int initialized = false;

EXTERN_ENGINE;

void scAddData(float angle, float value) {
#if EFI_TEXT_LOGGING
	if (!initialized) {
		return; // this is possible because of initialization sequence
	}

	if (engineConfiguration->sensorChartFrequency < 2) {
		/**
		 * analog chart frequency cannot be 1 because of the way
		 * data flush is implemented, see below
		 */
		//todofirmwareError()
		return;
	}

	if (engine->rpmCalculator.getRevolutionCounter() % engineConfiguration->sensorChartFrequency != 0) {
		/**
		 * We are here if we do NOT need to add an event to the analog chart
		 */
		if (pendingData) {
			/**
			 * We are here if that's the first time we do not need to add
			 * data after we have added some data - meaning it's time to flush
			 */
			// message terminator
			appendPrintf(&scLogging, DELIMETER);
			// output pending data
			if (getFullLog()) {
				scheduleLogging(&scLogging);
			}
			pendingData = false;
		}
		return;
	}
	if (!pendingData) {
		pendingData = true;
		resetLogging(&scLogging);
		// message header
		appendPrintf(&scLogging, "analog_chart%s", DELIMETER);
	}

	if (remainingSize(&scLogging) > 100) {
		appendPrintf(&scLogging, "%.2f|%.2f|", angle, value);
	}
#endif /* EFI_TEXT_LOGGING */
}

static void setSensorChartFrequency(int value) {
	engineConfiguration->sensorChartFrequency = value;
}

void initSensorChart(void) {
#if EFI_SIMULATOR
	printf("initSensorChart\n");
#endif
	addConsoleActionI("set_sensor_chart_freq", setSensorChartFrequency);

	initialized = true;
}

#endif /* EFI_SENSOR_CHART */
