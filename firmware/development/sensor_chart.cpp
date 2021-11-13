/**
 * @file	sensor_chart.cpp
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "os_access.h"
#include "sensor_chart.h"

#if EFI_SENSOR_CHART
#include "status_loop.h"

#if EFI_TEXT_LOGGING
static char LOGGING_BUFFER[SC_BUFFER_SIZE] CCM_OPTIONAL;
static Logging scLogging("analog chart", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));
#endif /* EFI_TEXT_LOGGING */

static int initialized = false;

enum class ScState {
	PreArm,
	Armed,
	Logging,
	Full
};

static ScState state = ScState::PreArm;
static uint32_t lastRevCount = 0;

void scAddData(float angle, float value) {
#if EFI_TEXT_LOGGING
	if (!initialized) {
		return; // this is possible because of initialization sequence
	}

	// Don't log if we need a flush
	if (state == ScState::Full) {
		return;
	}

	auto currentRev = getRevolutionCounter();

	if (state == ScState::PreArm) {
		// nothing to do - we just need to grab the rev counter once so we can detect a change
		state = ScState::Armed;
	} else if (state == ScState::Armed) {
		// If armed, wait for a NEW revolution to start
		if (lastRevCount != currentRev) {
			state = ScState::Logging;

			// Reset logging and append header
			scLogging.reset();
			scLogging.appendPrintf(PROTOCOL_ANALOG_CHART LOG_DELIMITER);
		}
	} else if (state == ScState::Logging) {
		// If running and the revolution idx changes, terminate logging and wait for flush
		if (lastRevCount != currentRev) {
			state = ScState::Full;
		}
	}

	lastRevCount = currentRev;

	if (state == ScState::Logging) {
		if (scLogging.remainingSize() > 100) {
			scLogging.appendPrintf( "%.2f|%.2f|", angle, value);
		} else {
			state = ScState::Full;
		}
	}
#endif /* EFI_TEXT_LOGGING */
}

void initSensorChart(void) {
#if EFI_SIMULATOR
	printf("initSensorChart\n");
#endif

	initialized = true;
}

void publishSensorChartIfFull() {
	if (state != ScState::Full) {
		return;
	}

	scLogging.appendPrintf(LOG_DELIMITER);
	scheduleLogging(&scLogging);

	state = ScState::Armed;
}

#endif /* EFI_SENSOR_CHART */
