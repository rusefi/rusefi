/*
 * @file periodic_task.h
 *
 * @date Jul 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
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
	virtual int getPeriodMs() = 0;

	virtual void PeriodicTask() = 0;

    void start() {
    	runAndScheduleNext(this);
    }
};

