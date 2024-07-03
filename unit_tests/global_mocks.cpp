/**
 * @file global_mocks.cpp
 */

#include "pch.h"

#include "trigger_central.h"

bool verboseMode = false;

static efitick_t timeNowNt;

efitick_t getTimeNowNt() {
	return timeNowNt;
}

void setTimeNowUs(int us) {
	timeNowNt = US2NT(us).count();
}

void advanceTimeUs(int us) {
	timeNowNt += US2NT(us);
}

void initLogging(LoggingWithStorage *logging, const char *name) {
}

void setBoardConfigOverrides() {
}

