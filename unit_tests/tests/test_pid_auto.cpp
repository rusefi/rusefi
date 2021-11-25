/*
 * @file test_pid_auto.cpp
 *
 * @date Sep 14, 2017
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "pid_auto_tune.h"

efitimems_t mockTimeMs = 0;

efitimems_t currentTimeMillis(void) {
	return mockTimeMs;
}

LoggingWithStorage logging("test");

static float zigZagOffset = 0;

#define CYCLE 20

// range of oscillation
static float oscRange;

/**
 * output linearly goes from 0 to 100 and back within each 'CYCLE' steps
 */
static float zigZagValue(int index) {
	int i = index % CYCLE;
	if ( i <= CYCLE / 2) {
		return i * (oscRange / 2 / CYCLE) + zigZagOffset;
	} else {
		return (CYCLE - i) * (oscRange / 2 / CYCLE) + zigZagOffset;
	}
}

static void testPidAutoZigZagStable() {
	printf("*************************************************** testPidAutoZigZagStable\r\n");

	oscRange = 100;
	mockTimeMs = 0;

	PID_AutoTune at;
	at.SetLookbackSec(5);
	at.SetControlType(PID_AutoTune::ZIEGLER_NICHOLS_PI);
	at.sampleTime = 0; // not used in math only used to filter values out
	ASSERT_EQ( 20,  at.nLookBack) << "nLookBack";


	at.outputStart = 50;

	at.input = zigZagValue(mockTimeMs);
	at.Runtime(&logging);

	mockTimeMs++;
	at.input = zigZagValue(mockTimeMs);
	at.Runtime(&logging);
//	assertEqualsLM("min@1", 0, at.absMin);
//	assertEqualsLM("max@1", 10, at.absMax);
	ASSERT_EQ( 0,  at.peakCount) << "peakCount";
	int startMockMs = mockTimeMs;

	for (; mockTimeMs <= 10 + startMockMs; mockTimeMs++) {
		at.input = zigZagValue(mockTimeMs);
		bool result = at.Runtime(&logging);
		ASSERT_FALSE(result) << "should be false#1";

	}
//	assertEqualsLM("min@11", 0, at.absMin);
//	assertEqualsLM("max@11", 100, at.absMax);
	ASSERT_EQ( 0,  at.peakCount) << "peakCount";

	for (; mockTimeMs <= 21; mockTimeMs++) {
		at.input = zigZagValue(mockTimeMs);
		bool result = at.Runtime(&logging);
		ASSERT_FALSE(result) << "should be false#2";
	}
	ASSERT_EQ( 0,  at.peakCount) << "peakCount@21";

	for (; mockTimeMs <= 41; mockTimeMs++) {
		at.input = zigZagValue(mockTimeMs);
		bool result = at.Runtime(&logging);
		ASSERT_FALSE(result) << "should be false#2_2";
	}
	ASSERT_EQ( 2,  at.peakCount) << "peakCount@41";
//	ASSERT_EQ( 1,  cisnan(at.Pu)) << "Pu@41";

	for (; mockTimeMs <= 60; mockTimeMs++) {
		at.input = zigZagValue(mockTimeMs);
		bool result = at.Runtime(&logging);
		ASSERT_FALSE(result) << "should be false#4";
	}
	ASSERT_EQ( 4,  at.peakCount) << "peakCount@60";
	//assertEqualsM("Pu@60", 0.02, at.Pu);

//	zigZagOffset = 10;

	for (; mockTimeMs <= 69; mockTimeMs++) {

		at.input = zigZagValue(mockTimeMs);
		bool result = at.Runtime(&logging);
		ASSERT_FALSE(result) << "should be false#4";
	}

	at.input = zigZagValue(mockTimeMs);
	bool result = at.Runtime(&logging);
	ASSERT_EQ( 1,  result) << "should be true";

	assertEqualsM("testPidAutoZigZagStable::output", 0.0, at.output);

	ASSERT_EQ( 5,  at.peakCount) << "peakCount@80";
	assertEqualsM("ki", 27.7798, at.GetKi());
	assertEqualsM("kd", 0.0, at.GetKd());

	// todo: test the same code with noisy zig-zag function
}

static void testPidAutoZigZagGrowingOsc() {
	printf("*************************************************** testPidAutoZigZagGrowingOsc\r\n");

	oscRange = 100;
	mockTimeMs = 0;

	PID_AutoTune at;
	at.SetLookbackSec(5);
	at.sampleTime = 0; // not used in math only used to filter values out

	int startMockMs;

	for (int i =0;i<11;i++) {
		startMockMs = mockTimeMs;
		printf("loop=%d %d\r\n", i, startMockMs);
		for (; mockTimeMs < CYCLE + startMockMs; mockTimeMs++) {
			at.input = zigZagValue(mockTimeMs);
			bool result = at.Runtime(&logging);
			ASSERT_FALSE(result) << "should be false#4";
		}
		oscRange *= 1.5;
	}

	startMockMs = mockTimeMs;
//	for (; mockTimeMs < CYCLE + startMockMs; mockTimeMs++) {
//		printf("loop2=%d\r\n", mockTimeMs);
//		at.input = zigZagValue(mockTimeMs);
//		bool result = at.Runtime(&logging);
//		ASSERT_FALSE(result) << "should be false#5";
//	}

	at.input = zigZagValue(mockTimeMs);
	bool result = at.Runtime(&logging);
	ASSERT_TRUE(result) << "should be true#2";
	assertEqualsM("FAiled", FAILED, at.state);

	assertEqualsM("output Growing", 0.0, at.output);

}

TEST(pidAutoTune, zeroLine) {
	mockTimeMs = 0;

	PID_AutoTune at;
	at.SetLookbackSec(5);
	at.sampleTime = 0; // not used in math only used to filter values out

	int startMockMs;

		for (int i =0;i<110;i++) {
			startMockMs = mockTimeMs;
			printf("loop=%d %d\r\n", i, startMockMs);
			for (; mockTimeMs < CYCLE + startMockMs; mockTimeMs++) {
				at.input = 0;
				bool result = at.Runtime(&logging);
				ASSERT_FALSE(result) << "should be false#4";
			}
		}
		// nothing happens in this test since we do not allow time play a role
}

TEST(pidAutoTune, delayLine) {

	static const int delayBufSize = 8;
	
	// we use a small FIFO buf to imitate some "response delay" of our virtual PID-controlled "device"
	cyclic_buffer<float, delayBufSize> delayBuf;
	delayBuf.clear();
	
	mockTimeMs = 0;

	PID_AutoTune at;
	at.SetLookbackSec(5);
	at.sampleTime = 0; // not used in math only used to filter values out
	
	int startMockMs;
	bool result = false;
	for (int i = 0; i < 110 && !result; i++) {
		startMockMs = mockTimeMs;
		//at.input = delayBuf.get(delayBuf.currentIndex - 1);
		int numElems = minI(delayBuf.getSize(), delayBuf.getCount());
		// our "device" is an averaging delay line
		at.input = (numElems == 0) ? 0 : (delayBuf.sum(numElems) / delayBuf.getSize());
		result = at.Runtime(&logging);
		// this is how our "device" is controlled by auto-tuner
		delayBuf.add(at.output);
		printf("[%d] %d in=%f out=%f\r\n", i, startMockMs, at.input, at.output);
		mockTimeMs++;
	}

	if (result)
		printf("*** Converged! Got result: P=%f I=%f D=%f\r\n", at.GetKp(), at.GetKi(), at.GetKd());
	ASSERT_TRUE(result) << "should be true#5";
}

TEST(misc, testPidAuto) {
	printf("*************************************************** testPidAuto\r\n");

	testPidAutoZigZagStable();

	testPidAutoZigZagGrowingOsc();
}
