/*
 * @file periodic_task.cpp
 *
 * @date: Jul 8, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if !EFI_UNIT_TEST

#include "ch.hpp"

#include "thread_controller.h"

#include "periodic_task.h"
#include "os_util.h"
#include "perf_trace.h"

#define JOBS_QUEUE_SIZE 16

static jobs_queue_t jq;
static job_descriptor_t jobs[JOBS_QUEUE_SIZE];
static msg_t msg_queue[JOBS_QUEUE_SIZE];

class WQDispatcher : public ThreadController<3 * UTILITY_THREAD_STACK_SIZE> {
public:
	WQDispatcher(const char *name, tprio_t priority)
	: ThreadController(name, priority)
	{
	}

	void ThreadTask() override;
};

void WQDispatcher::ThreadTask() {
	msg_t msg;

	do {
		msg = chJobDispatch(&jq);
	} while (msg == MSG_OK);
}

static WQDispatcher dispatcher[2] = {
	WQDispatcher("dispatcher0", PRIO_WQDISPATCHER),
	WQDispatcher("dispatcher1", PRIO_WQDISPATCHER)
};

void runAndScheduleNext(PeriodicTimerController *controller) {
	{
		ScopePerf perf(PE::PeriodicTimerControllerPeriodicTask);
		controller->PeriodicTask();
	}

	chVTSetAny(&controller->timer, TIME_MS2I(controller->getPeriodMs()), (vtfunc_t) &runAndScheduleNextIsr, controller);
}

static void runAndScheduleNextWrapper(void *arg)
{
	runAndScheduleNext((PeriodicTimerController *)arg);
}

void runAndScheduleNextIsr(PeriodicTimerController *controller)
{
	job_descriptor_t *jdp;

	/* Note: The system lock is released before entering the callback and
	 * re-acquired immediately after. It is callback's responsibility
	 * to acquire the lock if needed. */
	osalSysLockFromISR();
	jdp = chJobGetI(&jq);
	if (jdp) {
		jdp->jobfunc = runAndScheduleNextWrapper;
		jdp->jobarg  = controller;
		chJobPostI(&jq, jdp);
	}
	/* else die? */
	osalSysUnlockFromISR();
}

int startJobScheduler(void)
{
	chJobObjectInit(&jq, JOBS_QUEUE_SIZE, jobs, msg_queue);

	dispatcher[0].start();
	dispatcher[1].start();

	return 0;
}

#endif /* EFI_UNIT_TEST */
