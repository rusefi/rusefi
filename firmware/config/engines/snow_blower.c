/**
 * @file    snow_blower.c
 * @brief	Default configuration of a single-cylinder engine
 *
 * @date Sep 9, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"

#if EFI_ENGINE_SNOW_BLOWER

extern OutputSignal injectorOut1;

static Logging logger;

float getVRef(void) {
	return 12;
}

float getFuelMs() {
	return 1;
}

#define STROKE_TIME_CONSTANT2 (1000 * 60 * RPM_MULT * TICKS_IN_MS)

static int convertAngleToSysticks(int rpm, int advance) {
	return (int) (advance * STROKE_TIME_CONSTANT2 / 360 / rpm);
}

static void onShaftSignal(int ckpSignalType) {

	if (ckpSignalType != CKP_PRIMARY_DOWN)
		return;

	int offset = convertAngleToSysticks(getCurrentRpm(), 10);

	scheduleOutput(&injectorOut1, offset, TICKS_IN_MS);

}

void initMainEventListener() {
	initLogging(&logger, "main event handler", logger.DEFAULT_BUFFER, sizeof(logger.DEFAULT_BUFFER));

	registerCkpListener(&onShaftSignal, "main loop");

}

#endif
