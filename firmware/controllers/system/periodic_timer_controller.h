/*
 * @file periodic_timer_controller.h
 *
 * @date Jul 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#pragma once

#include "global.h"

class PeriodicTimerController;

void runAndScheduleNext(PeriodicTimerController *controller);

class PeriodicTimerController {

public:
	virtual_timer_t timer;
	int periodMs;


	virtual void PeriodicTask() = 0;

    /**
     * sets milliseconds period
     */
    void setPeriod(int periodMs) {
    	this->periodMs = periodMs;
    }

    void start() {
    	runAndScheduleNext(this);
    }
};

