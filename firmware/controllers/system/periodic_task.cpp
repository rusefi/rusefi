/*
 * @file periodic_task.cpp
 *
 * @date: Jul 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "periodic_task.h"
#include "os_util.h"
#include "perf_trace.h"

#if !EFI_UNIT_TEST
void runAndScheduleNext(virtual_timer_t *vtp, PeriodicTimerController *controller) {
	{
		ScopePerf perf(PE::PeriodicTimerControllerPeriodicTask);
		controller->PeriodicTask();
	}

	chVTSetAny(vtp, TIME_MS2I(controller->getPeriodMs()), (vtfunc_t) &runAndScheduleNext, controller);
}
#endif /* EFI_UNIT_TEST */
