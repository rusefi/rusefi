/*
 * @file periodic_timer_controller.h
 *
 * @date Jul 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#pragma once

#include "global.h"

class PeriodicTimerController {
private:
	virtual_timer_t timer;
	int periodMs;
	void runAndScheduleNext() {
		PeriodicTask();
		chVTSetAny(&timer, TIME_MS2I(periodMs), (vtfunc_t) &runAndScheduleNext, NULL);
	}
protected:
	virtual void PeriodicTask() = 0;


public:
    /**
     * sets milliseconds period
     */
    void setPeriod(int periodMs) {
    	this->periodMs = periodMs;
    }

    void start() {
    	runAndScheduleNext();
    }
};

