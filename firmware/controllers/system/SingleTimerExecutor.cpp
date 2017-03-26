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
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "SingleTimerExecutor.h"
#include "efitime.h"
#include "efilib2.h"

#if EFI_PROD_CODE
#include "microsecond_timer.h"
#endif

#if (EFI_SIGNAL_EXECUTOR_ONE_TIMER && EFI_PROD_CODE )|| defined(__DOXYGEN__)
#include "rfiutil.h"

static Executor instance;

extern schfunc_t globalTimerCallback;

//static int timerIsLate = 0;
//static efitime_t callbackTime = 0;
/**
 * these fields are global in order to facilitate debugging
 */
static efitime_t nextEventTimeNt = 0;

uint32_t beforeHwSetTimer;
uint32_t hwSetTimerTime;
uint32_t lastExecutionCount;

static void executorCallback(void *arg) {
	(void)arg;
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 256, "lowstck#2y");

//	callbackTime = getTimeNowNt();
//	if((callbackTime > nextEventTimeNt) && (callbackTime - nextEventTimeNt > US2NT(5000))) {
//		timerIsLate++;
//	}

	instance.onTimerCallback();
}

Executor::Executor() {
	reentrantFlag = false;
	/**
	 * todo: a good comment
	 */
	queue.setLateDelay(US2NT(100));
}

void Executor::scheduleByTime(const bool monitorReuse, scheduling_s *scheduling, efitimeus_t timeUs, schfunc_t callback,
		void *param) {
//	if (delayUs < 0) {
//		firmwareError(OBD_PCM_Processor_Fault, "Negative delayUs %s: %d", prefix, delayUs);
//		return;
//	}
//	if (delayUs == 0) {
//		callback(param);
//		return;
//	}
	if (!reentrantFlag) {
		// this would guard the queue and disable interrupts
		lockAnyContext();
	}
	bool needToResetTimer = queue.insertTask(scheduling, US2NT(timeUs), callback, param);
	if (!reentrantFlag) {
		doExecute();
		if (needToResetTimer) {
			scheduleTimerCallback();
		}
		unlockAnyContext();
	}
}

void Executor::onTimerCallback() {
	lockAnyContext();
	doExecute();
	scheduleTimerCallback();
	unlockAnyContext();
}

/*
 * this private method is executed under lock
 */
void Executor::doExecute() {
	/**
	 * Let's execute actions we should execute at this point.
	 * reentrantFlag takes care of the use case where the actions we are executing are scheduling
	 * further invocations
	 */
	reentrantFlag = true;
	int shouldExecute = 1;
	/**
	 * in real life it could be that while we executing listeners time passes and it's already time to execute
	 * next listeners.
	 * TODO: add a counter & figure out a limit of iterations?
	 */
	int totalExecuted = 0;
	while (shouldExecute > 0) {
		/**
		 * It's worth noting that that the actions might be adding new actions into the queue
		 */
		efitick_t nowNt = getTimeNowNt();
		shouldExecute = queue.executeAll(nowNt);
		totalExecuted += shouldExecute;
	}
	lastExecutionCount = totalExecuted;
	if (!isLocked()) {
		firmwareError(CUSTOM_ERR_6508, "Someone has stolen my lock");
		return;
	}
	reentrantFlag = false;
}

/**
 * This method is always invoked under a lock
 */
void Executor::scheduleTimerCallback() {
	/**
	 * Let's grab fresh time value
	 */
	efitick_t nowNt = getTimeNowNt();
	nextEventTimeNt = queue.getNextEventTime(nowNt);
	efiAssertVoid(nextEventTimeNt > nowNt, "setTimer constraint");
	if (nextEventTimeNt == EMPTY_QUEUE)
		return; // no pending events in the queue
	int32_t hwAlarmTime = NT2US((int32_t)nextEventTimeNt - (int32_t)nowNt);
	beforeHwSetTimer = GET_TIMESTAMP();
	setHardwareUsTimer(hwAlarmTime == 0 ? 1 : hwAlarmTime);
	hwSetTimerTime = GET_TIMESTAMP() - beforeHwSetTimer;
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
void scheduleTask(const bool monitorReuse, const char *prefix, scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) {
//	scheduling->name = prefix;
	instance.scheduleByTime(monitorReuse, scheduling, getTimeNowUs() + delayUs, callback, param);
}

void scheduleByTime(const bool monitorReuse, const char *prefix, scheduling_s *scheduling, efitimeus_t time, schfunc_t callback, void *param) {
//	scheduling->name = prefix;
	instance.scheduleByTime(monitorReuse, scheduling, time, callback, param);
}

void initSignalExecutorImpl(void) {
	globalTimerCallback = executorCallback;
#if EFI_PROD_CODE
	initMicrosecondTimer();
#endif /* EFI_PROD_CODE */
}

#endif

