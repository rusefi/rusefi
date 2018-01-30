/**
 * @file	sensor_chart.cpp
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "sensor_chart.h"
#include "main.h"
#include "engine.h"
#include "rpm_calculator.h"

#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
#include "status_loop.h"

static char LOGGING_BUFFER[5000] CCM_OPTIONAL;
static Logging logging("analog chart", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

static int pendingData = false;
static int initialized = false;

extern engine_configuration_s *engineConfiguration;

void scAddData(float angle, float value) {
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

	if (getRevolutionCounter() % engineConfiguration->sensorChartFrequency != 0) {
		/**
		 * We are here if we do NOT need to add an event to the analog chart
		 */
		if (pendingData) {
			/**
			 * We are here if that's the first time we do not need to add
			 * data after we have added some data - meaning it's time to flush
			 */
			// message terminator
			appendPrintf(&logging, DELIMETER);
			// output pending data
			if (getFullLog()) {
				scheduleLogging(&logging);
			}
			pendingData = false;
		}
		return;
	}
	if (!pendingData) {
		pendingData = true;
		resetLogging(&logging);
		// message header
		appendPrintf(&logging, "analog_chart%s", DELIMETER);
	}

	if (remainingSize(&logging) > 100) {
		appendPrintf(&logging, "%.2f|%.2f|", angle, value);
	}
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
