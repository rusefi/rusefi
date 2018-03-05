/**
 * @file	test_signal_executor.cpp
 *
 * @date Nov 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include <time.h>
#include "main.h"

#include "signal_executor.h"
#include "test_signal_executor.h"
#include "io_pins.h"
#include "utlist.h"
#include "event_queue.h"
#include "unit_test_framework.h"

EventQueue schedulingQueue;

bool_t debugSignalExecutor = false;

void scheduleForLater(scheduling_s *scheduling, int delayUs,
		schfunc_t callback, void *param) {
	if (debugSignalExecutor) {
		printf("scheduleTask %d\r\n", delayUs);
	}
	scheduleByTimestamp(scheduling, getTimeNowUs() + delayUs, callback, param);
}

void scheduleByTimestamp(scheduling_s *scheduling,
		efitimeus_t time, schfunc_t callback, void *param) {
	if (debugSignalExecutor) {
		printf("scheduleByTime %d\r\n", time);
	}
	schedulingQueue.insertTask(scheduling, time, callback, param);
}

void initSignalExecutorImpl(void) {
}

static EventQueue eq;

static int callbackCounter = 0;

static void callback(void *a) {
	callbackCounter++;
}

static int complexTestNow;

typedef struct {
	scheduling_s s;
	int period;
} TestPwm;

static void complexCallback(TestPwm *testPwm) {
	callbackCounter++;

	eq.insertTask(&testPwm->s, complexTestNow + testPwm->period,
			(schfunc_t) complexCallback, testPwm);
}

static void testSignalExecutor2(void) {
	print("*************************************** testSignalExecutor2\r\n");
	eq.clear();
	TestPwm p1;
	TestPwm p2;
	p1.period = 2;
	p2.period = 3;

	complexTestNow = 0;
	callbackCounter = 0;
	eq.insertTask(&p1.s, 0, (schfunc_t) complexCallback, &p1);
	eq.insertTask(&p2.s, 0, (schfunc_t) complexCallback, &p2);
	eq.executeAll(complexTestNow);
	assertEqualsM("callbackCounter #1", 2, callbackCounter);
	assertEquals(2, eq.size());

	eq.executeAll(complexTestNow = 2);
	assertEqualsM("callbackCounter #2", 3, callbackCounter);
	assertEquals(2, eq.size());

	eq.executeAll(complexTestNow = 3);
	assertEqualsM("callbackCounter #3", 4, callbackCounter);
	assertEquals(2, eq.size());

}

static long prevValue = -1;

static void orderCallback(void *a) {
	long value = (long)a;

	printf("value=%d prevValue=%d\r\n", value, prevValue);
	assertTrueM("orderCallback", value > prevValue);

	prevValue = value;
}


static void testSignalExecutor3(void) {
	print("*************************************** testSignalExecutor3\r\n");
	eq.clear();

	scheduling_s s1;
	scheduling_s s2;
	scheduling_s s3;

	eq.insertTask(&s1, 10, orderCallback, (void*)1);
	eq.insertTask(&s2, 11, orderCallback, (void*)2);
	eq.insertTask(&s3, 12, orderCallback, (void*)3);

	eq.executeAll(100);
}

void testSignalExecutor(void) {
	testSignalExecutor3();
	print("*************************************** testSignalExecutor\r\n");

	eq.clear();
	assertEquals(EMPTY_QUEUE, eq.getNextEventTime(0));
	scheduling_s s1;
	scheduling_s s2;
	scheduling_s s3;
	scheduling_s s4;

	eq.insertTask(&s1, 10, callback, NULL);
	eq.insertTask(&s4, 10, callback, NULL);
	eq.insertTask(&s3, 12, callback, NULL);
	eq.insertTask(&s2, 11, callback, NULL);

	assertEquals(4, eq.size());
	assertEquals(10, eq.getHead()->momentX);
	assertEquals(10, eq.getHead()->next->momentX);
	assertEquals(11, eq.getHead()->next->next->momentX);
	assertEquals(12, eq.getHead()->next->next->next->momentX);

	callbackCounter = 0;
	eq.executeAll(10);
	assertEqualsM("callbackCounter/2", 2, callbackCounter);
	callbackCounter = 0;
	eq.executeAll(11);
	assertEqualsM("callbackCounter/1#1", 1, callbackCounter);
	eq.executeAll(100);
	assertEquals(0, eq.size());

	eq.insertTask(&s1, 12, callback, NULL);
	eq.insertTask(&s2, 11, callback, NULL);
	eq.insertTask(&s3, 10, callback, NULL);
	callbackCounter = 0;
	eq.executeAll(10);
	assertEqualsM("callbackCounter/1#2", 1, callbackCounter);
	callbackCounter = 0;
	eq.executeAll(11);
	assertEquals(1, callbackCounter);
	eq.executeAll(100);
	assertEquals(0, eq.size());

	callbackCounter = 0;
	eq.insertTask(&s1, 10, callback, NULL);
	assertEquals(10, eq.getNextEventTime(0));

	eq.executeAll(1);
	assertEqualsM("callbacks not expected", 0, callbackCounter);

	eq.executeAll(11);
	assertEquals(1, callbackCounter);

	assertEquals(EMPTY_QUEUE, eq.getNextEventTime(0));

	eq.insertTask(&s1, 10, callback, NULL);
	eq.insertTask(&s2, 13, callback, NULL);
	assertEquals(10, eq.getNextEventTime(0));

	eq.executeAll(1);
	assertEquals(10, eq.getNextEventTime(0));

	eq.executeAll(100);
	assertEquals(0, eq.size());
	callbackCounter = 0;
	// both events are scheduled for the same time
	eq.insertTask(&s1, 10, callback, NULL);
	eq.insertTask(&s2, 10, callback, NULL);

	eq.executeAll(11);

	assertEquals(2, callbackCounter);
	testSignalExecutor2();
}
