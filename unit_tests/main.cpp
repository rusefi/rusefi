/**
 * @file main.cpp
 * @file First step towards unit-testing rusEfi algorithms
 *
 * @author Andrey Belomutskiy (c) 2012-2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

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
#include "engine_configuration.h"

#include "test_idle_controller.h"

#include "test_signal_executor.h"

#include "test_util.h"

extern "C"
{
#include "map_resize.h"
#include "test_event_registry.h"
}

#include "engine_math.h"
#include "test_engine_math.h"
#include "test_trigger_decoder.h"

static engine_configuration_s ec;
engine_configuration_s *engineConfiguration = &ec;

int timeNow = 0;

uint64_t getTimeNowUs(void) {
	return timeNow;
}

uint64_t getTimeNowNt(void) {
	return getTimeNowUs() * US_TO_NT_MULTIPLIER;
}

void assertEqualsM2(const char *msg, float expected, float actual, float EPS) {
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

int getRevolutionCounter(void) {
	return 0;
}

int main(void) {
	testLogicExpressions();
	testOverflow64Counter();
	testInterpolate3d();
	testFindIndex();
	testInterpolate2d();
	testGpsParser();
	testMisc();
	testFuelMap();
	testEngineMath();
	testIgnitionPlanning();
	testEventRegistry();
	testSensors();
	testCyclicBuffer();
	testCrc();
	testMisc();

	testSignalExecutor();

	testHistogram();

	testTriggerDecoder();

	testMalfunctionCentral();

	testConsoleLogic();

	testAngleResolver();

	testPinHelper();
	testSetTableValue();

	testAccelEnrichment();

	testSpeedDensity();

	testFLStack();

	testIdleController();

	testMenuTree();
	testMafLookup();
	testMafFuelMath();

	testPidController();

	//	resizeMap();
	printf("Success 20150213\r\n");
	return EXIT_SUCCESS;
}

int warning(obd_code_e code, const char *fmt, ...) {
	printf("Warning: %s\r\n", fmt);
}

bool hasFirmwareErrorFlag = false;

void firmwareError(const char *fmt, ...) {
	  printf(fmt);
	  exit(-1);
}

void print(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

void fatal3(char *msg, char *file, int line) {
  printf(msg);
  exit(-1);
}

int warning(const char *fmt, ...) {
	 printf(fmt);
	 exit(-1);
}

bool isCranking(void) {
	return 0;
}

void initLogging(LoggingWithStorage *logging, const char *name) {
}

void scheduleMsg(Logging *logging, const char *fmt, ...) {
}
