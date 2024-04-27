/**
 * @file global_mocks.cpp
 */

#include "pch.h"

#include "trigger_central.h"

bool verboseMode = false;

efitimems_t getTimeNowMs() {
	return US2MS(getTimeNowUs());
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
