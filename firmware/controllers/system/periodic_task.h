/*
 * @file periodic_task.h
 *
 * @date Jul 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

class PeriodicTimerController;

void runAndScheduleNext(PeriodicTimerController *controller);

/**
 * this is an intermediate implementation - we should probably move from using virtual_timer_t which works on interrupts
 * into sharing one thread with potentially lower priority instead
 */

class PeriodicTimerController {

public:
#if !EFI_UNIT_TEST
	virtual_timer_t timer;
#endif /* EFI_UNIT_TEST */

	virtual void PeriodicTask() = 0;

	/**
	 * This method is invoked after corresponding PeriodicTask() invocation
	 */
	virtual int getPeriodMs() = 0;

	/**
	 * This invokes PeriodicTask() immediately and starts the cycle of invocations and sleeps
	 */
	virtual void Start() {
		chVTObjectInit(&timer);
		runAndScheduleNext(this);
	}
};

