/*
 * rdi_perftest.c
 *
 *  Created on: Nov 30, 2012
 *      Author: Andrey Belomutskiy, (c) 2012-2013
 */

#include "main.h"
#include "rfi_perftest.h"
#include "fuel_math.h"

//#include "rfirtc.h"
#include "eficonsole.h"
#include "time.h"
#include "engine_math.h"
#include "gpio_helper.h"

//#define TEST_PORT GPIOB
//#define TEST_PIN 6

static OutputPin testOutput;

static void testSystemCalls(int count) {
	time_t start, time;
	long result = 0;

	start = currentTimeMillis();
	for (int i = 0; i < count / 2; i++) {
//		setPinValue(&testOutput, 0);
//		setPinValue(&testOutput, 1);
	}

	time = currentTimeMillis() - start;
	// Finished 100000 iterations of 'setPinValue()' in 120ms
//	print("Finished %d iterations of 'setPinValue()' in %dms\r\n", count, time);

	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		result += chTimeNow();
	time = currentTimeMillis() - start;
	if (result != 0) {
		// Finished 100000 iterations of 'chTimeNow()' in 33ms
		print("Finished %d iterations of 'chTimeNow()' in %dms\r\n", count, time);
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
		print("Finished %d iterations of 'chTimeNow()' with chSysLock in %dms\r\n", count, time);
	}

	count /= 10;

	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		result += currentTimeMillis();
	time = currentTimeMillis() - start;
	if (result != 0)
		print("Finished %d iterations of 'currentTimeMillis' in %dms\r\n", count, time);

}


static void testRusefiMethods(int count) {
	time_t start, time;
	int tempi = 1;

	start = currentTimeMillis();

	for (int i = 0; i < count; i++)
		tempi += getBaseFuel(4020, 2.21111);
	time = currentTimeMillis() - start;
	if (tempi != 0)
		print("Finished %d iterations of getBaseFuel in %dms\r\n", count, time);

	start = currentTimeMillis();

//	for (int i = 0; i < count; i++)
//		tempi += getDefaultFuel(4020, 2.21111);
//	time = currentTimeMillis() - start;
//	if (tempi == 0)
//		print("Finished %d iterations of getDefaultFuel in %dms\r\n", count, time);
}

static void testMath(int count) {
	time_t start, time;
	int tempi = 1;

	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		;
	time = currentTimeMillis() - start;
	print("Finished %d iterations of empty loop in %dms\r\n", count, time);

	start = currentTimeMillis();
	for (int i = 0; i < count; i++)
		tempi += tempi;
	time = currentTimeMillis() - start;
	if (tempi == 0)
		print("Finished %d iterations of int summation in %dms\r\n", count, time);

	start = currentTimeMillis();
	tempi = 1;
	for (int i = 0; i < count; i++)
		tempi += (tempi + 100) / 130;
	time = currentTimeMillis() - start;
	if (tempi != 0)
		print("Finished %d iterations of int division in %dms\r\n", count, time);

	start = currentTimeMillis();
	long templ = 1;
	for (int i = 0; i < count; i++)
		templ += templ;
	time = currentTimeMillis() - start;
	if (templ == 0)
		print("Finished %d iterations of long summation in %dms\r\n", count, time);

	start = currentTimeMillis();
	templ = 1;
	for (int i = 0; i < count; i++)
		templ += (templ + 100) / 130;
	time = currentTimeMillis() - start;
	if (templ != 0) {
		// Finished 100000 iterations of long division in 45ms
		print("Finished %d iterations of long division in %dms\r\n", count, time);
	}

	start = currentTimeMillis();
	float tempf = 1;
	for (int i = 0; i < count; i++)
		tempf += tempf;
	time = currentTimeMillis() - start;
	if (tempf != 0)
		print("Finished %d iterations of float summation in %dms\r\n", count, time);

	start = currentTimeMillis();
	tempf = 1;
	for (int i = 0; i < count; i++)
		tempf += (tempf + 100) / 130.0;
	time = currentTimeMillis() - start;
	if (tempf != 0) {
		// Finished 100000 iterations of float division in 65ms
		print("Finished %d iterations of float division in %dms\r\n", count, time);
	}

	start = currentTimeMillis();
	double tempd = 1;
	for (int i = 0; i < count; i++)
		tempd += tempd / 2;
	time = currentTimeMillis() - start;
	if (tempd != 0)
		print("Finished %d iterations of double summation in %dms\r\n", count, time);

	start = currentTimeMillis();
	tempd = 1;
	for (int i = 0; i < count; i++)
		tempd += (tempd + 100) / 130.0;
	time = currentTimeMillis() - start;
	if (tempd != 0)
		print("Finished %d iterations of double division in %dms\r\n", count, time);

}

static void runTests(int count) {
	testRusefiMethods(count);
	testSystemCalls(count);
	testMath(count);
}

void initTimePerfActions() {
//	initOutputPin("test pad", &testOutput, TEST_PORT, TEST_PIN);
	addConsoleActionI("testmath", runTests);
}
