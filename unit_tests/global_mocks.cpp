/**
 * @file global_mocks.cpp
 */

#include "pch.h"

#include "trigger_central.h"
#include "map_resize.h"

bool verboseMode = false;

static int timeNowUs = 0;

efitick_t getTimeNowNt() {
	return (efitimeus_t)timeNowUs * US_TO_NT_MULTIPLIER;
}

void setTimeNowUs(int us) {
	timeNowUs = us;
}

void advanceTimeUs(int us) {
	timeNowUs += us;
}

void initLogging(LoggingWithStorage *logging, const char *name) {
}

void setBoardConfigOverrides() {
}

