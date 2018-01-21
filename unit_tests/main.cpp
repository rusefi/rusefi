/**
 * @file main.cpp
 * @file First step towards unit-testing rusEfi algorithms
 *
 * @author Andrey Belomutskiy (c) 2012-2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "main.h"
#include "error_handling.h"
#include "test_accel_enrichment.h"
#include "test_interpolation_3d.h"
#include "test_find_index.h"
#include "test_sensors.h"
#include "test_speed_density.h"

#include "test_fuel_map.h"
#include "fuel_math.h"
#include "test_logic_expression.h"
#include "test_pid_auto.h"
#include "engine_configuration.h"

#include "test_idle_controller.h"
#include "afm2mapConverter.h"
#include "test_signal_executor.h"
#include "trigger_central.h"
#include "test_util.h"
#include "map_resize.h"
#include "engine_math.h"
#include "test_engine_math.h"
#include "test_trigger_decoder.h"

typedef int32_t         msg_t;

#include "chstreams.h"
#include "memstreams.h"


static engine_configuration_s ec;
engine_configuration_s *engineConfiguration = &ec;

int timeNow = 0;

efitimeus_t getTimeNowUs(void) {
	return timeNow;
}

efitick_t getTimeNowNt(void) {
	return getTimeNowUs() * US_TO_NT_MULTIPLIER;
}

void assertEqualsM5(const char *prefix, const char *message, float expected, float actual, float EPS) {
	char msg[100];
	strcpy(msg, prefix);
	strcat(msg, message);
	if (cisnan(actual) && !cisnan(expected)) {
		printf("Assert failed: %s %.4f while expected %.4f\r\n", msg, actual, expected);
		exit(-1);
	}

	float delta = absF(actual - expected);
	if (delta > EPS) {
		printf("delta: %.7f\r\n", delta);
		printf("Unexpected: %s %.4f while expected %.4f\r\n", msg, actual, expected);
		exit(-1);
	}
	printf("Validated %s: %f\r\n", msg, expected);
}

void assertEqualsM2(const char *msg, float expected, float actual, float eps) {
	assertEqualsM5("", msg, expected, actual, eps);
}

void assertEqualsM4(const char *prefix, const char *msg, float expected, float actual) {
	assertEqualsM5(prefix, msg, expected, actual, 0.00001);
}

void assertEqualsLM(const char *msg, long expected, long actual) {
	if (expected != actual) {
		printf("Assert failed: %s %d while expected %d\r\n", msg, actual, expected);
		exit(-1);
	}
}

void assertEqualsM(const char *msg, float expected, float actual) {
	assertEqualsM2(msg, expected, actual, 0.0001);
}

void assertEquals(float expected, float actual) {
	assertEqualsM("", expected, actual);
}

void assertTrueM(const char *msg, float actual) {
	assertEqualsM(msg, TRUE, actual);
}

void assertTrue(float actual) {
	assertTrueM("", actual);
}

void assertFalseM(const char *msg, float actual) {
	assertEqualsM(msg, FALSE, actual);
}

void assertFalse(float actual) {
	assertFalseM("", actual);
}

void chDbgAssert(int c, char *msg, void *arg) {
	if (!c) {
		printf("assert failed: %s\r\n", msg);
		exit(-1);
	}
}

extern int revolutionCounterSinceBootForUnitTest;

int getRevolutionCounter(void) {
	return revolutionCounterSinceBootForUnitTest;
}
extern bool printTriggerDebug;

int main(void) {
//	printTriggerDebug = true;

	testDifferentInjectionModes();
	testPidAutoZigZag();
	testMissedSpark299();
	testSparkReverseOrderBug319();
	testFuelSchedulerBug299smallAndLarge();
	testFuelSchedulerBug299smallAndMedium();
	testLogicExpressions(); // fsio
	testOverflow64Counter();
	testInterpolate3d();
	testFindIndex();
	testInterpolate2d();
	testGpsParser();
	testMisc();
	testFuelMap();
	testEngineMath();
	testIgnitionPlanning();
	testSensors();
	testCyclicBuffer();
	testCrc();

	testSignalExecutor();

	testHistogram();


	testMalfunctionCentral();

	testConsoleLogic();

	testAngleResolver();

	testPinHelper();
	testSetTableValue();

	testAccelEnrichment();

	testSpeedDensity();

	testFLStack();

	testMenuTree();
	testMafLookup();
	testIgnitionMapGenerator();
	testMafFuelMath();

	testPidController();
	testTriggerDecoder();

	//	resizeMap();
	printf("Success 20180120\r\n");
	printAllTriggers();
//	printConvertedTable();
	return EXIT_SUCCESS;
}

void print(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

bool isCranking(void) {
	return 0;
}

void initLogging(LoggingWithStorage *logging, const char *name) {
}

void scheduleMsg(Logging *logging, const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	printf("\r\n");
}
