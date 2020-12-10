/**
 * @file main.cpp
 * @file Unit tests (and some integration tests to be fair) of rusEFI
 *
 * @author Andrey Belomutskiy, (c) 2012-2020
 */


#include <stdlib.h>

#include "global.h"

#include "engine_configuration.h"

#include "test_signal_executor.h"
#include "trigger_central.h"
#include "map_resize.h"
#include "engine_math.h"
#include "engine_test_helper.h"
#include "gtest/gtest.h"

int timeNowUs = 0;

efitimeus_t getTimeNowUs(void) {
	return timeNowUs;
}

efitick_t getTimeNowNt(void) {
	return getTimeNowUs() * US_TO_NT_MULTIPLIER;
}

LoggingWithStorage sharedLogger("main");

extern bool printTriggerDebug;
extern bool printTriggerTrace;
bool verboseMode = false;

trigger_type_e focusOnTrigger = TT_UNUSED;

GTEST_API_ int main(int argc, char **argv) {
	if (focusOnTrigger != TT_UNUSED) {
		printTriggerDebug = true;
		printTriggerTrace = true;
	}

	//	resizeMap();
	printf("Success 20201203\r\n");
	exportAllTriggers();
	if (focusOnTrigger != TT_UNUSED) {
		return -1;
	}

	testing::InitGoogleTest(&argc, argv);
	// uncomment if you only want to run selected tests
	//::testing::GTEST_FLAG(filter) = "*testFasterEngineSpinningUp*";
	int result = RUN_ALL_TESTS();
	// windows ERRORLEVEL in Jenkins batch file seems to want negative value to detect failure
	return result == 0 ? 0 : -1;
}

void print(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

void initLogging(LoggingWithStorage *logging, const char *name) {
}
