/**
 * @file rfi_perftest.cpp
 *
 * Method execution time micro test.
 *
 * @date Nov 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "rfi_perftest.h"
#include "fuel_math.h"

#include "eficonsole.h"
#include "time.h"

#include "console_io.h"

#if EFI_PERF_METRICS
#include "test.h"

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
		efiPrintf("Finished %d iterations of 'chTimeNow()' in %dms", count, time);
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
		efiPrintf("Finished %d iterations of 'chTimeNow()' with chSysLock in %dms", count, time);
	}

	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		result += currentTimeMillis();
	time = currentTimeMillis() - start;
	if (result != 0)
		efiPrintf("Finished %d iterations of 'currentTimeMillis' in %dms", count, time);
}

static Engine testEngine;

static void testRusefiMethods(const int count) {
	time_t start, time;
	int tempi = 1;

	start = currentTimeMillis();

	time = currentTimeMillis() - start;
	if (tempi != 0)
		efiPrintf("Finished %d iterations of getBaseFuel in %dms", count, time);

//	start = currentTimeMillis();
//	for (int i = 0; i < count; i++)
//		tempi += getInjectionDuration(1200, NULL); // todo
//	time = currentTimeMillis() - start;
//	if (tempi != 0)
//		efiPrintf("Finished %d iterations of getFuelMs in %dms", count, time);

	start = currentTimeMillis();
	for (int i = 0; i < count; i++) {
		testEngine.updateSlowSensors();
		tempi += testEngine.engineState.clt;
	}
	time = currentTimeMillis() - start;
	if (tempi != 0)
		efiPrintf("Finished %d iterations of updateSlowSensors in %dms", count, time);
}

static void testMath(const int count) {
	time_t start, time;

	int64_t temp64 = 0;
	start = currentTimeMillis();
	for (int64_t i = 0; i < count; i++) {
		temp64 += i;
	}
	time = currentTimeMillis() - start;
	if (temp64 != 0) {
		efiPrintf("Finished %d iterations of int64_t summation in %dms", count, time);
	}

	temp64 = 1;
	start = currentTimeMillis();
	for (int64_t i = 0; i < count; i++) {
		temp64 *= i;
	}
	time = currentTimeMillis() - start;
	if (temp64 == 0) {
		efiPrintf("Finished %d iterations of int64_t multiplication in %dms", count, time);
	}

	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		;
	time = currentTimeMillis() - start;
	efiPrintf("Finished %d iterations of empty loop in %dms", count, time);

	uint32_t tempi = 1;
	start = currentTimeMillis();
	for (int i = 0; i < count; i++) {
		tempi += tempi;
	}
	time = currentTimeMillis() - start;
	if (tempi == 0) {
		// 11ms is 1848000 ticks
		// 18.48 ticks per iteration
		// Finished 100000 iterations of uint32_t summation in 11ms
		efiPrintf("Finished %d iterations of uint32_t summation in %dms", count, time);
	}

	start = currentTimeMillis();
	tempi = 1;
	for (int i = 0; i < count; i++) {
		tempi += (tempi + 100) / 130;
	}
	time = currentTimeMillis() - start;
	if (tempi != 0) {
		// Finished 100000 iterations of uint32_t division in 16ms
		efiPrintf("Finished %d iterations of uint32_t division in %dms", count, time);
	}

	start = currentTimeMillis();
	temp64 = 1;
	for (int i = 0; i < count; i++) {
		temp64 += temp64;
	}
	time = currentTimeMillis() - start;
	if (temp64 == 0) {
		//  Finished 100000 iterations of int64_t summation in 21ms
		efiPrintf("Finished %d iterations of int64_t summation in %dms", count, time);
	}

	start = currentTimeMillis();
	temp64 = 1;
	for (int i = 0; i < count; i++) {
		temp64 += (temp64 + 100) / 130;
	}
	time = currentTimeMillis() - start;
	if (temp64 != 0) {
		// Finished 100000 iterations of int64_t division in 181ms
		efiPrintf("Finished %d iterations of int64_t division in %dms", count, time);
	}

	start = currentTimeMillis();
	float tempf = 1;
	for (int i = 0; i < count; i++) {
		tempf += tempf;
	}
	time = currentTimeMillis() - start;
	if (tempf != 0) {
		efiPrintf("Finished %d iterations of float summation in %dms", count, time);
	}

	start = currentTimeMillis();
	tempf = 1;
	for (int i = 0; i < count; i++) {
		tempf += tempf * 130.0f;
	}
	time = currentTimeMillis() - start;
	if (tempf != 0) {
		//  ms =  ticks
		//  ticks per iteration
		// Finished 100000 iterations of float division in ms
		efiPrintf("Finished %d iterations of float multiplication in %dms", count, time);
	}

	start = currentTimeMillis();
	tempf = 1;
	for (int i = 0; i < count; i++) {
		tempf += (tempf + 100) / 130.0;
	}
	time = currentTimeMillis() - start;
	if (tempf != 0) {
		// 65 ms = 10920000 ticks
		// 109.2 ticks per iteration
		// Finished 100000 iterations of float division in 65ms
		efiPrintf("Finished %d iterations of float division in %dms", count, time);
	}

	start = currentTimeMillis();
	tempf = 1;
	for (int i = 0; i < count; i++) {
		tempf += logf(tempf);
	}
	time = currentTimeMillis() - start;
	if (tempf != 0) {
		// Finished 100000 iterations of float log in 191ms
		efiPrintf("Finished %d iterations of float log in %dms", count, time);
	}

	start = currentTimeMillis();
	double tempd = 1;
	for (int i = 0; i < count; i++)
		tempd += tempd / 2;
	time = currentTimeMillis() - start;
	if (tempd != 0) {
		// Finished 100000 iterations of double summation in 80ms
		efiPrintf("Finished %d iterations of double summation in %dms", count, time);
	}

	start = currentTimeMillis();
	tempd = 1;
	for (int i = 0; i < count; i++)
		tempd += (tempd + 100) / 130.0;
	time = currentTimeMillis() - start;
	if (tempd != 0) {
		// Finished 100000 iterations of double division in 497ms
		efiPrintf("Finished %d iterations of double division in %dms", count, time);
	}

	start = currentTimeMillis();
	tempd = 1;
	for (int i = 0; i < count; i++) {
		tempd += log(tempd);
	}
	time = currentTimeMillis() - start;
	if (tempd != 0) {
		// Finished 100000 iterations of double log in 242ms
		efiPrintf("Finished %d iterations of double log in %dms", count, time);
	}
}

static void runTests(const int count) {
	efiPrintf("Running tests: %d", count);
	testRusefiMethods(count / 10);
	testSystemCalls(count);
	testMath(count);
}

extern Overflow64Counter halTime;

#if EFI_RTC
static int rtcStartTime;
#endif

#include "chrtclib.h"

static void timeInfo() {
	efiPrintf("chTimeNow as seconds = %d", getTimeNowSeconds());
	efiPrintf("hal seconds = %d", halTime.get() / (long)CORE_CLOCK);

#if EFI_RTC
	int unix = rtcGetTimeUnixSec(&RTCD1) - rtcStartTime;
	efiPrintf("unix seconds = %d", unix);
#endif
}

static void runChibioTest() {
	print("EFI_SHAFT_POSITION_INPUT=%d\r\n", EFI_SHAFT_POSITION_INPUT);
	print("EFI_EMULATE_POSITION_SENSORS=%d\r\n", EFI_EMULATE_POSITION_SENSORS);
	print("EFI_ANALOG_SENSORS=%d\r\n", EFI_ANALOG_SENSORS);
	print("EFI_INTERNAL_ADC=%d\r\n", EFI_INTERNAL_ADC);
	print("EFI_HD44780_LCD=%d\r\n", EFI_HD44780_LCD);
	print("EFI_MAP_AVERAGING=%d\r\n", EFI_MAP_AVERAGING);
	print("EFI_LOGIC_ANALYZER=%d\r\n", EFI_LOGIC_ANALYZER);
	print("EFI_ENGINE_SNIFFER=%d\r\n", EFI_ENGINE_SNIFFER);
	print("EFI_SENSOR_CHART=%d\r\n", EFI_SENSOR_CHART);
	print("EFI_SHAFT_POSITION_INPUT=%d\r\n", EFI_SHAFT_POSITION_INPUT);
	print("EFI_ENGINE_CONTROL=%d\r\n", EFI_ENGINE_CONTROL);
	print("CH_DBG_SYSTEM_STATE_CHECK=%d\r\n", CH_DBG_SYSTEM_STATE_CHECK);
	print("CH_DBG_ENABLE_CHECKS=%d\r\n", CH_DBG_ENABLE_CHECKS);
	print("CH_DBG_ENABLE_ASSERTS=%d\r\n", CH_DBG_ENABLE_ASSERTS);
	print("CH_DBG_ENABLE_STACK_CHECK=%d\r\n", CH_DBG_ENABLE_STACK_CHECK);
	print("CH_DBG_THREADS_PROFILING=%d\r\n", CH_DBG_THREADS_PROFILING);
	TestThread(getConsoleChannel());
}

void initTimePerfActions() {
#if EFI_RTC
	rtcStartTime = rtcGetTimeUnixSec(&RTCD1);
#endif

	addConsoleActionI("perftest", runTests);

	addConsoleAction("timeinfo", timeInfo);
	addConsoleAction("chtest", runChibioTest);
}

#endif /* EFI_PERF_METRICS */
