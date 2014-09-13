/**
 * @file SingleTimerExecutor.cpp
 *
 * This class combines the powers of a 1MHz hardware timer from microsecond_timer.c
 * and pending events queue event_queue.cpp
 *
 * As of version 2.6.x, ChibiOS tick-based kernel is not capable of scheduling events
 * with the level of precision we need, and realistically it should not.
 *
 * http://sourceforge.net/p/rusefi/tickets/24/
 *
 * @date: Apr 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "SingleTimerExecutor.h"
#include "efitime.h"
#include "rfiutil.h"

#if EFI_PROD_CODE
#include "microsecond_timer.h"
#endif

#if EFI_SIGNAL_EXECUTOR_ONE_TIMER || defined(__DOXYGEN__)

static Executor instance;

extern schfunc_t globalTimerCallback;

static void executorCallback(void *arg) {
	instance.execute(getTimeNowUs());
}

Executor::Executor() {
	reentrantLock = false;
}

void Executor::lock(void) {
	lockAnyContext();
}

void Executor::unlock(void) {
	unlockAnyContext();
}

void Executor::schedule2(const char *prefix, scheduling_s *scheduling, uint64_t timeUs, schfunc_t callback, void *param) {
//	if (delayUs < 0) {
//		firmwareError("Negative delayUs %s: %d", prefix, delayUs);
//		return;
//	}
//	if (delayUs == 0) {
//		callback(param);
//		return;
//	}
	if (!reentrantLock) {
		// this would guard the queue and disable interrupts
		lock();
	}
	queue.insertTask(scheduling, timeUs, callback, param);
	if (!reentrantLock) {
		doExecute(getTimeNowUs());
		unlock();
	}
}


void Executor::schedule(const char *prefix, scheduling_s *scheduling, uint64_t nowUs, int delayUs, schfunc_t callback, void *param) {
	schedule2(prefix, scheduling, nowUs + delayUs, callback, param);
}

void Executor::execute(uint64_t nowUs) {
	lock();
	doExecute(nowUs);
	unlock();
}

/*
 * this private method is executed under lock
 */
void Executor::doExecute(uint64_t nowUs) {
	/**
	 * Let's execute actions we should execute at this point.
	 * reentrantLock takes care of the use case where the actions we are executing are scheduling
	 * further invocations
	 */
	reentrantLock = TRUE;
	queue.executeAll(nowUs);
	if (!isLocked()) {
		firmwareError("Someone has stolen my lock");
		return;
	}
	reentrantLock = false;
	/**
	 * Let's set up the timer for the next execution
	 */
	uint64_t nextEventTime = queue.getNextEventTime(nowUs);
	efiAssertVoid(nextEventTime > nowUs, "setTimer constraint");
	if (nextEventTime == EMPTY_QUEUE)
		return; // no pending events in the queue
	setHardwareUsTimer(nextEventTime - nowUs);
}

/**
 * @brief Schedule an event
 *
 * Invokes event callback after the specified amount of time.
 *
 * @param [in, out] scheduling Data structure to keep this event in the collection.
 * @param [in] delayUs the number of microseconds before the output signal immediate output if delay is zero.
 * @param [in] dwell the number of ticks of output duration.
 */
void scheduleTask(const char *prefix, scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) {
	instance.schedule(prefix, scheduling, getTimeNowUs(), delayUs, callback, param);
}

void scheduleTask2(const char *prefix, scheduling_s *scheduling, uint64_t time, schfunc_t callback, void *param) {
	instance.schedule2(prefix, scheduling, time, callback, param);
}


void initSignalExecutorImpl(void) {
	globalTimerCallback = executorCallback;
#if EFI_PROD_CODE
	initMicrosecondTimer();
#endif /* EFI_PROD_CODE */
}

#endif

