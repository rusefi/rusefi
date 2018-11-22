/*
 * test_pid_auto.cpp
 *
 *  Created on: Sep 14, 2017
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "test_pid_auto.h"
#include "pid_auto_tune.h"
#include "unit_test_framework.h"

efitimems_t mockTimeMs = 0;

efitimems_t currentTimeMillis(void) {
	return mockTimeMs;
}

Logging logging;

static float zigZagOffset = 0;

#define CYCLE 20

/**
 * output linearly goes from 0 to 100 and back within each 'CYCLE' steps
 */
static float zigZagValue(int index) {
	int i = index % CYCLE;
	if ( i <= CYCLE / 2) {
		return i * (100.0 / 2 / CYCLE) + zigZagOffset;
	} else {
		return (CYCLE - i) * (100.0 / 2 / CYCLE) + zigZagOffset;
	}
}

void testPidAutoZigZag() {
	printf("*************************************************** testPidAutoZigZag\r\n");

	mockTimeMs = 0;

	PID_AutoTune at;
	at.SetLookbackSec(5);
	at.sampleTime = 0; // not used in math only used to filter values out
	assertEqualsM("nLookBack", 20, at.nLookBack);


	at.outputStart = 50;

	at.input = zigZagValue(mockTimeMs);
	at.Runtime(&logging);

	mockTimeMs++;
	at.input = zigZagValue(mockTimeMs);
	at.Runtime(&logging);
//	assertEqualsLM("min@1", 0, at.absMin);
//	assertEqualsLM("max@1", 10, at.absMax);
	assertEqualsM("peakCount", 0, at.peakCount);
	int startMockMs = mockTimeMs;

	for (; mockTimeMs <= 10 + startMockMs; mockTimeMs++) {
		at.input = zigZagValue(mockTimeMs);
		at.Runtime(&logging);

	}
//	assertEqualsLM("min@11", 0, at.absMin);
//	assertEqualsLM("max@11", 100, at.absMax);
	assertEqualsM("peakCount", 0, at.peakCount);

	for (; mockTimeMs <= 21; mockTimeMs++) {
		at.input = zigZagValue(mockTimeMs);
		at.Runtime(&logging);
	}
	assertEqualsM("peakCount@21", 0, at.peakCount);

	for (; mockTimeMs <= 41; mockTimeMs++) {
		at.input = zigZagValue(mockTimeMs);
		at.Runtime(&logging);
	}
	assertEqualsM("peakCount@41", 2, at.peakCount);
//	assertEqualsM("Pu@41", 1, cisnan(at.Pu));

	for (; mockTimeMs <= 60; mockTimeMs++) {
		at.input = zigZagValue(mockTimeMs);
		at.Runtime(&logging);
	}
	assertEqualsM("peakCount@60", 4, at.peakCount);
	//assertEqualsM("Pu@60", 0.02, at.Pu);

//	zigZagOffset = 10;

	for (; mockTimeMs <= 80; mockTimeMs++) {
		at.input = zigZagValue(mockTimeMs);
		at.Runtime(&logging);
	}
	assertEqualsM("peakCount@80", 6, at.peakCount);
	assertEqualsM("ki", 27.7798, at.GetKi());
	assertEqualsM("kd", 0.0, at.GetKd());

	// todo: test the same code with noisy zig-zag function
}
