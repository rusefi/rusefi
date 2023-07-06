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
void runAndScheduleNext(virtual_timer_t* t, PeriodicTimerController *controller);

static void runAndScheduleNext2(
		virtual_timer_t* t,
		void *param) {
	// C++ kludge :(
	PeriodicTimerController *controller = (PeriodicTimerController *) param;
	runAndScheduleNext(t, controller);
}

void runAndScheduleNext(virtual_timer_t*, PeriodicTimerController *controller) {
	{
		ScopePerf perf(PE::PeriodicTimerControllerPeriodicTask);
		controller->PeriodicTask();
	}
	vtfunc_t f = (vtfunc_t) &runAndScheduleNext2;

	chVTSetAny(&controller->timer, TIME_MS2I(controller->getPeriodMs()), f, controller);
}
#endif /* EFI_UNIT_TEST */
