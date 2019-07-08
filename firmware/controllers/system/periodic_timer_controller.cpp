/*
 * periodic_timer_controller.cpp
 *
 *  Created on: Jul 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "periodic_timer_controller.h"
#include "os_util.h"

void runAndScheduleNext(PeriodicTimerController *controller) {
	controller->PeriodicTask();
	chVTSetAny(&controller->timer, TIME_MS2I(controller->periodMs), (vtfunc_t) &runAndScheduleNext, controller);
}


