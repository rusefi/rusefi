/*
 * @file periodic_task.cpp
 *
 * @date: Jul 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "periodic_task.h"
#include "os_util.h"

void runAndScheduleNext(PeriodicTimerController *controller) {
#if !EFI_UNIT_TEST
	controller->PeriodicTask();
	chVTSetAny(&controller->timer, TIME_MS2I(controller->periodMs), (vtfunc_t) &runAndScheduleNext, controller);
#endif /* EFI_UNIT_TEST */
}


