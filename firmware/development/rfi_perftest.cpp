/**
 * @file rfi_perftest.cpp
 *
 * @date Nov 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "rfi_perftest.h"
#include "fuel_math.h"

#include "eficonsole.h"
#include "time.h"
#include "engine_math.h"
#include "efiGpio.h"
#include "efilib2.h"
#include "console_io.h"
#include "engine.h"

#if EFI_PERF_METRICS || defined(__DOXYGEN__)
#include "test.h"

static Logging* logger;

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
		scheduleMsg(logger, "Finished %d iterations of 'chTimeNow()' in %dms", count, time);
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
		scheduleMsg(logger, "Finished %d iterations of 'chTimeNow()' with chSysLock in %dms", count, time);
	}

	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		result += currentTimeMillis();
	time = currentTimeMillis() - start;
	if (result != 0)
		scheduleMsg(logger, "Finished %d iterations of 'currentTimeMillis' in %dms", count, time);
}

static Engine testEngine;

static void testRusefiMethods(const int count) {
	time_t start, time;
	int tempi = 1;

	start = currentTimeMillis();

	for (int i = 0; i < count; i++)
		tempi += getBaseTableFuel(testEngine.engineConfiguration, 4020, 2.21111);
	time = currentTimeMillis() - start;
	if (tempi != 0)
		scheduleMsg(logger, "Finished %d iterations of getBaseFuel in %dms", count, time);

//	start = currentTimeMillis();
//	for (int i = 0; i < count; i++)
//		tempi += getInjectionDuration(1200, NULL); // todo
//	time = currentTimeMillis() - start;
//	if (tempi != 0)
//		scheduleMsg(logger, "Finished %d iterations of getFuelMs in %dms", count, time);

	start = currentTimeMillis();
	for (int i = 0; i < count; i++) {
		testEngine.updateSlowSensors();
		tempi += testEngine.engineState.clt;
	}
	time = currentTimeMillis() - start;
	if (tempi != 0)
		scheduleMsg(logger, "Finished %d iterations of updateSlowSensors in %dms", count, time);
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
		scheduleMsg(logger, "Finished %d iterations of int64_t summation in %dms", count, time);
	}

	temp64 = 1;
	start = currentTimeMillis();
	for (int64_t i = 0; i < count; i++) {
		temp64 *= i;
	}
	time = currentTimeMillis() - start;
	if (temp64 == 0) {
		scheduleMsg(logger, "Finished %d iterations of int64_t multiplication in %dms", count, time);
	}

	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		;
	time = currentTimeMillis() - start;
	scheduleMsg(logger, "Finished %d iterations of empty loop in %dms", count, time);

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
		scheduleMsg(logger, "Finished %d iterations of uint32_t summation in %dms", count, time);
	}

	start = currentTimeMillis();
	tempi = 1;
	for (int i = 0; i < count; i++) {
		tempi += (tempi + 100) / 130;
	}
	time = currentTimeMillis() - start;
	if (tempi != 0) {
		// Finished 100000 iterations of uint32_t division in 16ms
		scheduleMsg(logger, "Finished %d iterations of uint32_t division in %dms", count, time);
	}

	start = currentTimeMillis();
	temp64 = 1;
	for (int i = 0; i < count; i++) {
		temp64 += temp64;
	}
	time = currentTimeMillis() - start;
	if (temp64 == 0) {
		//  Finished 100000 iterations of int64_t summation in 21ms
		scheduleMsg(logger, "Finished %d iterations of int64_t summation in %dms", count, time);
	}

	start = currentTimeMillis();
	temp64 = 1;
	for (int i = 0; i < count; i++) {
		temp64 += (temp64 + 100) / 130;
	}
	time = currentTimeMillis() - start;
	if (temp64 != 0) {
		// Finished 100000 iterations of int64_t division in 181ms
		scheduleMsg(logger, "Finished %d iterations of int64_t division in %dms", count, time);
	}

	start = currentTimeMillis();
	float tempf = 1;
	for (int i = 0; i < count; i++) {
		tempf += tempf;
	}
	time = currentTimeMillis() - start;
	if (tempf != 0) {
		scheduleMsg(logger, "Finished %d iterations of float summation in %dms", count, time);
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
		scheduleMsg(logger, "Finished %d iterations of float multiplication in %dms", count, time);
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
		scheduleMsg(logger, "Finished %d iterations of float division in %dms", count, time);
	}

	start = currentTimeMillis();
	tempf = 1;
	for (int i = 0; i < count; i++) {
		tempf += logf(tempf);
	}
	time = currentTimeMillis() - start;
	if (tempf != 0) {
		// Finished 100000 iterations of float log in 191ms
		scheduleMsg(logger, "Finished %d iterations of float log in %dms", count, time);
	}

	start = currentTimeMillis();
	double tempd = 1;
	for (int i = 0; i < count; i++)
		tempd += tempd / 2;
	time = currentTimeMillis() - start;
	if (tempd != 0) {
		// Finished 100000 iterations of double summation in 80ms
		scheduleMsg(logger, "Finished %d iterations of double summation in %dms", count, time);
	}

	start = currentTimeMillis();
	tempd = 1;
	for (int i = 0; i < count; i++)
		tempd += (tempd + 100) / 130.0;
	time = currentTimeMillis() - start;
	if (tempd != 0) {
		// Finished 100000 iterations of double division in 497ms
		scheduleMsg(logger, "Finished %d iterations of double division in %dms", count, time);
	}

	start = currentTimeMillis();
	tempd = 1;
	for (int i = 0; i < count; i++) {
		tempd += log(tempd);
	}
	time = currentTimeMillis() - start;
	if (tempd != 0) {
		// Finished 100000 iterations of double log in 242ms
		scheduleMsg(logger, "Finished %d iterations of double log in %dms", count, time);
	}
}

static void runTests(const int count) {
	scheduleMsg(logger, "Running tests: %d", count);
	testRusefiMethods(count / 10);
	testSystemCalls(count);
	testMath(count);
}

extern Overflow64Counter halTime;

#if EFI_RTC || defined(__DOXYGEN__)
static int rtcStartTime;
#endif

#include "chrtclib.h"

static void timeInfo(void) {
	scheduleMsg(logger, "chTimeNow as seconds = %d", getTimeNowSeconds());
	scheduleMsg(logger, "hal seconds = %d", halTime.get() / 168000000LL);

#if EFI_RTC || defined(__DOXYGEN__)
	int unix = rtcGetTimeUnixSec(&RTCD1) - rtcStartTime;
	scheduleMsg(logger, "unix seconds = %d", unix);
#endif
}

static void runChibioTest(void) {
	print("EFI_SHAFT_POSITION_INPUT=%d\r\n", EFI_SHAFT_POSITION_INPUT);
	print("EFI_EMULATE_POSITION_SENSORS=%d\r\n", EFI_EMULATE_POSITION_SENSORS);
	print("EFI_ANALOG_SENSORS=%d\r\n", EFI_ANALOG_SENSORS);
	print("EFI_INTERNAL_ADC=%d\r\n", EFI_INTERNAL_ADC);
	print("EFI_HD44780_LCD=%d\r\n", EFI_HD44780_LCD);
	print("EFI_MAP_AVERAGING=%d\r\n", EFI_MAP_AVERAGING);
	print("EFI_WAVE_ANALYZER=%d\r\n", EFI_WAVE_ANALYZER);
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

void initTimePerfActions(Logging *sharedLogger) {
	logger = sharedLogger;
#if EFI_RTC || defined(__DOXYGEN__)
	rtcStartTime = rtcGetTimeUnixSec(&RTCD1);
#endif

	addConsoleActionI("perftest", runTests);

	addConsoleAction("timeinfo", timeInfo);
	addConsoleAction("chtest", runChibioTest);
}

#endif /* EFI_PERF_METRICS */
