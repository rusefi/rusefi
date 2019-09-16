/**
 * @file main.cpp
 * @file First step towards unit-testing rusEfi algorithms
 *
 * @author Andrey Belomutskiy (c) 2012-2018
 */


#include <cstdlib>

#include "global.h"

#include "engine_configuration.h"

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

extern int revolutionCounterSinceBootForUnitTest;

int getRevolutionCounter(void) {
	return revolutionCounterSinceBootForUnitTest;
}

extern bool printTriggerDebug;
bool verboseMode = false;

GTEST_API_ int main(int argc, char **argv) {
//	printTriggerDebug = true;

	//	resizeMap();
	printf("Success 20190825\r\n");
	printAllTriggers();
//	printConvertedTable();
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

void scheduleMsg(Logging *logging, const char *format, ...) {
	if (verboseMode) {
		va_list ap;
		va_start(ap, format);
		vprintf(format, ap);
		va_end(ap);
		printf("\r\n");
	}
}
