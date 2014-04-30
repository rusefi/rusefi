/**
 * @file	test_signal_executor.c
 *
 * @date Nov 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include <time.h>
#include "main.h"

#include "signal_executor.h"
#include "test_signal_executor.h"
#include "io_pins.h"
#include "utlist.h"
#include "event_queue.h"

extern OutputSignal *st_output_list;

static io_pin_e testLastToggledPin;
static int testToggleCounter;

int getRevolutionCounter(void) {
	return 0;
}

void setOutputPinValue(io_pin_e pin, int value) {
	// this is a test implementation of the method - we use it to see what's going on
	testLastToggledPin = pin;
	testToggleCounter++;
}

void scheduleTask(scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) {
}

void initSignalExecutorImpl(void) {
}

static EventQueue eq;

static int callbackCounter = 0;

static void callback(void *a) {
	callbackCounter++;
}

void testSignalExecutor() {
	print("*************************************** testSignalExecutor\r\n");

	assertEquals(EMPTY_QUEUE, eq.getNextEventTime(0));
	scheduling_s s1;
	scheduling_s s2;

	eq.insertTask(&s1, 0, 10, callback, NULL);
	assertEquals(10, eq.getNextEventTime(0));

	eq.executeAll(1);
	assertEqualsM("callbacks not expected", 0, callbackCounter);

	eq.executeAll(11);
	assertEquals(1, callbackCounter);

	assertEquals(EMPTY_QUEUE, eq.getNextEventTime(0));

	eq.insertTask(&s1, 0, 10, callback, NULL);
	eq.insertTask(&s2, 0, 13, callback, NULL);
	assertEquals(10, eq.getNextEventTime(0));

	eq.executeAll(1);
	assertEquals(10, eq.getNextEventTime(0));

	eq.clear();
	callbackCounter = 0;
	// both events are scheduled for the same time
	eq.insertTask(&s1, 0, 10, callback, NULL);
	eq.insertTask(&s2, 0, 10, callback, NULL);

	eq.executeAll(11);

	assertEquals(2, callbackCounter);
}
