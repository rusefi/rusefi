/**
 * @file SingleTimerExecutor.cpp
 *
 * This class combines the powers of a 1MHz hardware timer from microsecond_timer.c
 * and pending events queue event_queue.cpp
 *
 * As of version 2.6.x, ChibiOS tick-based kernel is not capable of scheduling events
 * with the level of precision we need, and realistically it should not.
 *
 * See https://rusefi.com/forum/viewtopic.php?f=5&t=373&sid=e285ebd9a5677a83235116007e8eb65d&start=360#p30895
 * for some performance data: with 'debug' firmware we spend about 5% of CPU in TIM5 handler which seem to be executed
 * about 1500 times a second
 *
 * http://sourceforge.net/p/rusefi/tickets/24/
 *
 * @date: Apr 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "SingleTimerExecutor.h"
#include "efitime.h"
#include "efilib2.h"

#if EFI_SIGNAL_EXECUTOR_ONE_TIMER || defined(__DOXYGEN__)
#include "microsecond_timer.h"
#include "tunerstudio_configuration.h"
#include "rfiutil.h"

#include "engine.h"
EXTERN_ENGINE;

extern schfunc_t globalTimerCallback;

//static int timerIsLate = 0;
//static efitime_t callbackTime = 0;
/**
 * these fields are global in order to facilitate debugging
 */
static efitime_t nextEventTimeNt = 0;

uint32_t hwSetTimerDuration;
uint32_t lastExecutionCount;

static void executorCallback(void *arg) {
	(void)arg;
	efiAssertVoid(CUSTOM_ERR_6624, getCurrentRemainingStack() > 256, "lowstck#2y");

//	callbackTime = getTimeNowNt();
//	if((callbackTime > nextEventTimeNt) && (callbackTime - nextEventTimeNt > US2NT(5000))) {
//		timerIsLate++;
//	}

	___engine.executor.onTimerCallback();
}

SingleTimerExecutor::SingleTimerExecutor() {
	reentrantFlag = false;
	doExecuteCounter = scheduleCounter = timerCallbackCounter = 0;
	/**
	 * todo: a good comment
	 */
	queue.setLateDelay(US2NT(100));
}

void SingleTimerExecutor::scheduleForLater(scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) {
	scheduleByTimestamp(scheduling, getTimeNowUs() + delayUs, callback, param);
}

/**
 * @brief Schedule an event at specific delay after now
 *
 * Invokes event callback after the specified amount of time.
 * callback would be executed either on ISR thread or current thread if we would need to execute right away
 *
 * @param [in, out] scheduling Data structure to keep this event in the collection.
 * @param [in] delayUs the number of microseconds before the output signal immediate output if delay is zero.
 * @param [in] dwell the number of ticks of output duration.
 */
void SingleTimerExecutor::scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, schfunc_t callback,
		void *param) {
	scheduleCounter++;
	bool alreadyLocked = true;
	if (!reentrantFlag) {
		// this would guard the queue and disable interrupts
		alreadyLocked = lockAnyContext();
	}
	bool needToResetTimer = queue.insertTask(scheduling, US2NT(timeUs), callback, param);
	if (!reentrantFlag) {
		doExecute();
		if (needToResetTimer) {
			scheduleTimerCallback();
		}
		if (!alreadyLocked)
			unlockAnyContext();
	}
}

void SingleTimerExecutor::onTimerCallback() {
	timerCallbackCounter++;
	bool alreadyLocked = lockAnyContext();
	doExecute();
	scheduleTimerCallback();
	if (!alreadyLocked)
		unlockAnyContext();
}

/*
 * this private method is executed under lock
 */
void SingleTimerExecutor::doExecute() {
	doExecuteCounter++;
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
		firmwareError(CUSTOM_ERR_LOCK_ISSUE, "Someone has stolen my lock");
		return;
	}
	reentrantFlag = false;
}

/**
 * This method is always invoked under a lock
 */
void SingleTimerExecutor::scheduleTimerCallback() {
	/**
	 * Let's grab fresh time value
	 */
	efitick_t nowNt = getTimeNowNt();
	nextEventTimeNt = queue.getNextEventTime(nowNt);
	efiAssertVoid(CUSTOM_ERR_6625, nextEventTimeNt > nowNt, "setTimer constraint");
	if (nextEventTimeNt == EMPTY_QUEUE)
		return; // no pending events in the queue
	int32_t hwAlarmTime = NT2US((int32_t)nextEventTimeNt - (int32_t)nowNt);
	uint32_t beforeHwSetTimer = GET_TIMESTAMP();
	setHardwareUsTimer(hwAlarmTime == 0 ? 1 : hwAlarmTime);
	hwSetTimerDuration = GET_TIMESTAMP() - beforeHwSetTimer;
}

void initSingleTimerExecutorHardware(void) {
	globalTimerCallback = executorCallback;
	initMicrosecondTimer();
}

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

void executorStatistics() {
	if (engineConfiguration->debugMode == DBG_EXECUTOR) {
#if (EFI_TUNER_STUDIO && EFI_SIGNAL_EXECUTOR_ONE_TIMER) || defined(__DOXYGEN__)
		tsOutputChannels.debugIntField1 = ___engine.executor.timerCallbackCounter;
		tsOutputChannels.debugIntField2 = ___engine.executor.doExecuteCounter;
		tsOutputChannels.debugIntField3 = ___engine.executor.scheduleCounter;
#endif /* EFI_TUNER_STUDIO */
	}
}

#endif /* EFI_SIGNAL_EXECUTOR_ONE_TIMER */

