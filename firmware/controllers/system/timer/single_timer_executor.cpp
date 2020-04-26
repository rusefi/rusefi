/**
 * @file SingleTimerExecutor.cpp
 *
 * This class combines the powers of a 1MHz hardware timer from microsecond_timer.cpp
 * and pending events queue event_queue.cpp
 *
 * As of version 2.6.x, ChibiOS tick-based kernel is not capable of scheduling events
 * with the level of precision we need, and realistically it should not.
 *
 * Update: actually newer ChibiOS has tickless mode and what we have here is pretty much the same thing :)
 * open question if rusEfi should simply migrate to ChibiOS tickless scheduling (which would increase coupling with ChibiOS)
 *
 * See https://rusefi.com/forum/viewtopic.php?f=5&t=373&start=360#p30895
 * for some performance data: with 'debug' firmware we spend about 5% of CPU in TIM5 handler which seem to be executed
 * about 1500 times a second
 *
 * http://sourceforge.net/p/rusefi/tickets/24/
 *
 * @date: Apr 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "os_access.h"
#include "single_timer_executor.h"
#include "efitime.h"
#include "perf_trace.h"

#if EFI_SIGNAL_EXECUTOR_ONE_TIMER

#include "microsecond_timer.h"
#include "tunerstudio_configuration.h"
#include "os_util.h"

#include "engine.h"
EXTERN_ENGINE;

/**
 * these fields are global in order to facilitate debugging
 */
static efitime_t nextEventTimeNt = 0;

uint32_t hwSetTimerDuration;
uint32_t lastExecutionCount;

void globalTimerCallback() {
	efiAssertVoid(CUSTOM_ERR_6624, getCurrentRemainingStack() > EXPECTED_REMAINING_STACK, "lowstck#2y");

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

void SingleTimerExecutor::scheduleForLater(scheduling_s *scheduling, int delayUs, action_s action) {
	scheduleByTimestamp(scheduling, getTimeNowUs() + delayUs, action);
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
void SingleTimerExecutor::scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, action_s action) {
	scheduleByTimestampNt(scheduling, US2NT(timeUs), action);
}

void SingleTimerExecutor::scheduleByTimestampNt(scheduling_s* scheduling, efitime_t nt, action_s action) {
	ScopePerf perf(PE::SingleTimerExecutorScheduleByTimestamp);

#if EFI_ENABLE_ASSERTS
	int deltaTimeUs = NT2US(nt - getTimeNowNt());

	if (deltaTimeUs >= TOO_FAR_INTO_FUTURE_US) {
		// we are trying to set callback for too far into the future. This does not look right at all
		firmwareError(CUSTOM_ERR_TASK_TIMER_OVERFLOW, "scheduleByTimestampNt() too far: %d", deltaTimeUs);
		return;
	}
#endif

	scheduleCounter++;
	bool alreadyLocked = true;
	if (!reentrantFlag) {
		// this would guard the queue and disable interrupts
		alreadyLocked = lockAnyContext();
	}
	bool needToResetTimer = queue.insertTask(scheduling, nt, action);
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
	ScopePerf perf(PE::SingleTimerExecutorDoExecute);

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
	ScopePerf perf(PE::SingleTimerExecutorScheduleTimerCallback);

	/**
	 * Let's grab fresh time value
	 */
	efitick_t nowNt = getTimeNowNt();
	nextEventTimeNt = queue.getNextEventTime(nowNt);
	efiAssertVoid(CUSTOM_ERR_6625, nextEventTimeNt > nowNt, "setTimer constraint");
	if (nextEventTimeNt == EMPTY_QUEUE)
		return; // no pending events in the queue
	int32_t hwAlarmTime = NT2US((int32_t)nextEventTimeNt - (int32_t)nowNt);
	uint32_t beforeHwSetTimer = getTimeNowLowerNt();
	setHardwareUsTimer(hwAlarmTime == 0 ? 1 : hwAlarmTime);
	hwSetTimerDuration = getTimeNowLowerNt() - beforeHwSetTimer;
}

void initSingleTimerExecutorHardware(void) {
	initMicrosecondTimer();
}

void executorStatistics() {
	if (engineConfiguration->debugMode == DBG_EXECUTOR) {
#if EFI_TUNER_STUDIO && EFI_SIGNAL_EXECUTOR_ONE_TIMER
		tsOutputChannels.debugIntField1 = ___engine.executor.timerCallbackCounter;
		tsOutputChannels.debugIntField2 = ___engine.executor.doExecuteCounter;
		tsOutputChannels.debugIntField3 = ___engine.executor.scheduleCounter;
#endif /* EFI_TUNER_STUDIO */
	}
}

#endif /* EFI_SIGNAL_EXECUTOR_ONE_TIMER */

