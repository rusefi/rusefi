/**
 * @file global_mocks.cpp
 */

#include "pch.h"

#include "trigger_central.h"

bool verboseMode = false;

int timeNowUs = 0;

efitimeus_t getTimeNowUs() {
	return timeNowUs;
}

efitimesec_t getTimeNowS() {
	return getTimeNowUs() / 1000 / 1000;
}

efitick_t getTimeNowNt() {
	return getTimeNowUs() * US_TO_NT_MULTIPLIER;
}

void initLogging(LoggingWithStorage *logging, const char *name) {
}

void setBoardConfigOverrides() {
}

void onCliCaseError(const char *token) {
}

void onCliDuplicateError(const char *token) {
}

void onCliOverflowError() {
}
