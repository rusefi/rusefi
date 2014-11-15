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

#if EFI_PROD_CODE
#include "microsecond_timer.h"
#endif

#if (EFI_SIGNAL_EXECUTOR_ONE_TIMER && EFI_PROD_CODE )|| defined(__DOXYGEN__)
#include "rfiutil.h"

static Executor instance;

extern schfunc_t globalTimerCallback;

static int timerIsLate = 0;
/**
 * these fields are global in order to facilitate debugging
 */
static uint64_t nextEventTimeNt = 0;
static uint64_t hwAlarmTime = 0;
static uint64_t callbackTime = 0;

static void executorCallback(void *arg) {
	(void)arg;
	efiAssertVoid(getRemainingStack(chThdSelf()) > 256, "lowstck#2y");

//	callbackTime = getTimeNowNt();
//	if((callbackTime > nextEventTimeNt) && (callbackTime - nextEventTimeNt > US2NT(5000))) {
//		timerIsLate++;
//	}

	instance.onTimerCallback();
}

Executor::Executor() {
	reentrantLock = false;
	queue.setLateDelay(US2NT(100));
}

void Executor::lock(void) {
	lockAnyContext();
}

void Executor::unlock(void) {
	unlockAnyContext();
}

void Executor::schedule2(const char *prefix, scheduling_s *scheduling, uint64_t timeUs, schfunc_t callback,
		void *param) {
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
	queue.insertTask(scheduling, US2NT(timeUs), callback, param);
	if (!reentrantLock) {
		doExecute();
		unlock();
	}
}

void Executor::schedule(const char *prefix, scheduling_s *scheduling, uint64_t nowUs, int delayUs, schfunc_t callback,
		void *param) {
	schedule2(prefix, scheduling, nowUs + delayUs, callback, param);
}

void Executor::onTimerCallback() {
	lock();
	doExecute();
	unlock();
}

/*
 * this private method is executed under lock
 */
void Executor::doExecute() {
	/**
	 * Let's execute actions we should execute at this point.
	 * reentrantLock takes care of the use case where the actions we are executing are scheduling
	 * further invocations
	 */
	reentrantLock = TRUE;
	bool shouldExecute = true;
	/**
	 * in real life it could be that while we executing listeners time passes and it's already time to execute
	 * next listeners.
	 * TODO: add a counter & figure out a limit of iterations?
	 */
	while (shouldExecute) {
		/**
		 * It's worth noting that that the actions might be adding new actions into the queue
		 */
		uint64_t nowNt = getTimeNowNt();
		shouldExecute = queue.executeAll(nowNt);
	}
	if (!isLocked()) {
		firmwareError("Someone has stolen my lock");
		return;
	}
	uint64_t nowNt = getTimeNowNt();
	reentrantLock = false;
	/**
	 * 'executeAll' is potentially invoking heavy callbacks, let's grab fresh time value?
	 */
	/**
	 * Let's set up the timer for the next execution
	 */
	nextEventTimeNt = queue.getNextEventTime(nowNt);
	efiAssertVoid(nextEventTimeNt > nowNt, "setTimer constraint");
	if (nextEventTimeNt == EMPTY_QUEUE)
		return; // no pending events in the queue
	hwAlarmTime = NT2US(nextEventTimeNt - nowNt);
	setHardwareUsTimer(hwAlarmTime == 0 ? 1 : hwAlarmTime);
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

