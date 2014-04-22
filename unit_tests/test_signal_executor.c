/**
 * @file	test_signal_executor.c
 *
 * @date Nov 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include <time.h>
#include "main.h"

#include "signal_executor.h"
#include "signal_executor_single_timer_algo.h"
#include "test_signal_executor.h"
#include "io_pins.h"
#include "utlist.h"

extern OutputSignal *st_output_list;

static io_pin_e testLastToggledPin;
static int testToggleCounter;

void setOutputPinValue(io_pin_e pin, int value) {
	// this is a test implementation of the method - we use it to see what's going on
	testLastToggledPin = pin;
	testToggleCounter++;
}

void scheduleTask(scheduling_s *scheduling, int delay, schfunc_t callback, void *param) {

}

void testSignalExecutor() {
	print("*************************************** testSignalExecutor\r\n");

//	OutputSignal s1;
//	OutputSignal s2;
//
//	registerSignal(&s1);
//	registerSignal(&s2);
//
//	OutputSignal *out;
//	int count;
//	LL_COUNT(st_output_list, out, count);
//	assertEquals(2, count);
//
//	s1.io_pin = 0;
//	initOutputSignalBase(&s1);
//	assertEqualsM("status", IDLE, s1.status);
//	scheduleOutputBase(&s1, 10, 100);
//
//	long now = 1;
//	print("now = 1\r\n");
//	testToggleCounter = 0;
//	assertEqualsM("duration", 10, GET_DURATION(&s1));
//	assertEquals(9, toggleSignalIfNeeded(&s1, now));
//	assertEquals(0, testToggleCounter);
//
//	now = 100;
//	print("now = 100\r\n");
//	testToggleCounter = 0;
//	assertEquals(100, toggleSignalIfNeeded(&s1, now));
//	assertEquals(1, testToggleCounter);
//
//	print("now = 300\r\n");
//	now = 300; // let's see what happens if the handler is late
//	testToggleCounter = 0;
//	assertEquals(10, toggleSignalIfNeeded(&s1, now));
//	assertEquals(1, testToggleCounter);
}
