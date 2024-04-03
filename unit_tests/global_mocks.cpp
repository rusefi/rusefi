/**
 * @file global_mocks.cpp
 */

#include "pch.h"

#include "trigger_central.h"

bool verboseMode = false;

efitimems_t mockTimeMs = 0;

efitimems_t getTimeNowMs() {
	return mockTimeMs;
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
