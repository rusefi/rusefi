/**
 * @file rdi_perftest.c
 *
 * @date Nov 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "rfi_perftest.h"
#include "fuel_math.h"

//#include "rfirtc.h"
#include "eficonsole.h"
#include "time.h"
#include "engine_math.h"
#include "gpio_helper.h"
#include "efilib2.h"

//#define TEST_PORT GPIOB
//#define TEST_PIN 6

//static OutputPin testOutput;

Logging logger;

static void testSystemCalls(const int count) {
	time_t start, time;
	long result = 0;

	start = currentTimeMillis();
	for (int i = 0; i < count / 2; i++) {
//		setPinValue(&testOutput, 0);
//		setPinValue(&testOutput, 1);
	}

	time = currentTimeMillis() - start;
	// Finished 100000 iterations of 'setPinValue()' in 120ms
//	prin("Finished %d iterations of 'setPinValue()' in %dms\r\n", count, time);

	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		result += chTimeNow();
	time = currentTimeMillis() - start;
	if (result != 0) {
		// Finished 100000 iterations of 'chTimeNow()' in 33ms
		scheduleMsg(&logger, "Finished %d iterations of 'chTimeNow()' in %dms", count, time);
	}

	start = currentTimeMillis();
	for (int i = 0; i < count; i++) {
		chSysLock()
		;
		result += chTimeNow();
		chSysUnlock()
		;
	}
	time = currentTimeMillis() - start;
	if (result != 0) {
		// Finished 100000 iterations of 'chTimeNow()' with chSysLock in 144ms
		scheduleMsg(&logger, "Finished %d iterations of 'chTimeNow()' with chSysLock in %dms", count, time);
	}

	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		result += currentTimeMillis();
	time = currentTimeMillis() - start;
	if (result != 0)
		scheduleMsg(&logger, "Finished %d iterations of 'currentTimeMillis' in %dms", count, time);
}

static void testRusefiMethods(const int count) {
	time_t start, time;
	int tempi = 1;

	start = currentTimeMillis();

	for (int i = 0; i < count; i++)
		tempi += getBaseFuel(4020, 2.21111);
	time = currentTimeMillis() - start;
	if (tempi != 0)
		scheduleMsg(&logger, "Finished %d iterations of getBaseFuel in %dms", count, time);

	start = currentTimeMillis();

//	for (int i = 0; i < count; i++)
//		tempi += getDefaultFuel(4020, 2.21111);
//	time = currentTimeMillis() - start;
//	if (tempi == 0)
//		rint("Finished %d iterations of getDefaultFuel in %dms\r\n", count, time);
}

static void testMath(const int count) {
	time_t start, time;

	int64_t temp64 = 0;
	start = currentTimeMillis();
	for (int64_t i = 0; i < count; i++)
		temp64 += i;
	time = currentTimeMillis() - start;
	if (temp64 != 0)
		scheduleMsg(&logger, "Finished %d iterations of int64_t summation in %dms", count, time);

	temp64 = 1;
	start = currentTimeMillis();
	for (int64_t i = 0; i < count; i++)
		temp64 *= i;
	time = currentTimeMillis() - start;
	if (temp64 == 0)
		scheduleMsg(&logger, "Finished %d iterations of int64_t multiplication in %dms", count, time);


	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		;
	time = currentTimeMillis() - start;
	scheduleMsg(&logger, "Finished %d iterations of empty loop in %dms", count, time);

	int tempi = 1;
	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		tempi += tempi;
	time = currentTimeMillis() - start;
	if (tempi == 0)
		scheduleMsg(&logger, "Finished %d iterations of int summation in %dms", count, time);

	start = currentTimeMillis();
	tempi = 1;
	for (int i = 0; i < count; i++)
		tempi += (tempi + 100) / 130;
	time = currentTimeMillis() - start;
	if (tempi != 0)
		scheduleMsg(&logger, "Finished %d iterations of int division in %dms", count, time);

	start = currentTimeMillis();
	long templ = 1;
	for (int i = 0; i < count; i++)
		templ += templ;
	time = currentTimeMillis() - start;
	if (templ == 0)
		scheduleMsg(&logger, "Finished %d iterations of long summation in %dms", count, time);

	start = currentTimeMillis();
	templ = 1;
	for (int i = 0; i < count; i++)
		templ += (templ + 100) / 130;
	time = currentTimeMillis() - start;
	if (templ != 0) {
		// Finished 100000 iterations of long division in 45ms
		scheduleMsg(&logger, "Finished %d iterations of long division in %dms", count, time);
	}

	start = currentTimeMillis();
	float tempf = 1;
	for (int i = 0; i < count; i++)
		tempf += tempf;
	time = currentTimeMillis() - start;
	if (tempf != 0)
		scheduleMsg(&logger, "Finished %d iterations of float summation in %dms", count, time);

	start = currentTimeMillis();
	tempf = 1;
	for (int i = 0; i < count; i++)
		tempf += (tempf + 100) / 130.0;
	time = currentTimeMillis() - start;
	if (tempf != 0) {
		// Finished 100000 iterations of float division in 65ms
		scheduleMsg(&logger, "Finished %d iterations of float division in %dms", count, time);
	}

	start = currentTimeMillis();
	double tempd = 1;
	for (int i = 0; i < count; i++)
		tempd += tempd / 2;
	time = currentTimeMillis() - start;
	if (tempd != 0)
		scheduleMsg(&logger, "Finished %d iterations of double summation in %dms", count, time);

	start = currentTimeMillis();
	tempd = 1;
	for (int i = 0; i < count; i++)
		tempd += (tempd + 100) / 130.0;
	time = currentTimeMillis() - start;
	if (tempd != 0)
		scheduleMsg(&logger, "Finished %d iterations of double division in %dms", count, time);

}

static void runTests(const int count) {
	testRusefiMethods(count / 10);
	testSystemCalls(count);
	testMath(count);
}

extern Overflow64Counter halTime;

int rtcStartTime;

#include "chrtclib.h"

static void timeInfo(void) {
	scheduleMsg(&logger, "chTimeNow as seconds = %d", getTimeNowSeconds());
	scheduleMsg(&logger, "hal seconds = %d", halTime.get() / 168000000LL);

	int unix = rtcGetTimeUnixSec(&RTCD1) - rtcStartTime;
	scheduleMsg(&logger, "unix seconds = %d", unix);

}

void initTimePerfActions() {
	rtcStartTime = rtcGetTimeUnixSec(&RTCD1);


	initLogging(&logger, "perftest");
//	initOutputPin("test pad", &testOutput, TEST_PORT, TEST_PIN);
	addConsoleActionI("perftest", runTests);

	addConsoleAction("timeinfo", timeInfo);
}
