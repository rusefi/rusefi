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

#include "os_access.h"
#include "single_timer_executor.h"
#include "efitime.h"
#include "perf_trace.h"

#if EFI_SIGNAL_EXECUTOR_ONE_TIMER

#include "microsecond_timer.h"
#include "os_util.h"

uint32_t hwSetTimerDuration;

void globalTimerCallback() {
	efiAssertVoid(CUSTOM_ERR_6624, getCurrentRemainingStack() > EXPECTED_REMAINING_STACK, "lowstck#2y");

	___engine.executor.onTimerCallback();
}

SingleTimerExecutor::SingleTimerExecutor()
	// 8us is roughly the cost of the interrupt + overhead of a single timer event
	: queue(US2NT(8))
{
}

void SingleTimerExecutor::scheduleForLater(scheduling_s *scheduling, int delayUs, action_s action) {
	scheduleByTimestamp("scheduleForLater", scheduling, getTimeNowUs() + delayUs, action);
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
void SingleTimerExecutor::scheduleByTimestamp(const char *msg, scheduling_s *scheduling, efitimeus_t timeUs, action_s action) {
	scheduleByTimestampNt(msg, scheduling, US2NT(timeUs), action);
}

void SingleTimerExecutor::scheduleByTimestampNt(const char *msg, scheduling_s* scheduling, efitime_t nt, action_s action) {
	ScopePerf perf(PE::SingleTimerExecutorScheduleByTimestamp);

#if EFI_ENABLE_ASSERTS
	int32_t deltaTimeNt = (int32_t)nt - getTimeNowLowerNt();

	if (deltaTimeNt >= TOO_FAR_INTO_FUTURE_NT) {
		// we are trying to set callback for too far into the future. This does not look right at all
		firmwareError(CUSTOM_ERR_TASK_TIMER_OVERFLOW, "scheduleByTimestampNt() too far: %d %s", deltaTimeNt, msg);
		return;
	}
#endif

	scheduleCounter++;

	// Lock for queue insertion - we may already be locked, but that's ok
	chibios_rt::CriticalSectionLocker csl;

	bool needToResetTimer = queue.insertTask(scheduling, nt, action);
	if (!reentrantFlag) {
		executeAllPendingActions();
		if (needToResetTimer) {
			scheduleTimerCallback();
		}
	}
}

void SingleTimerExecutor::onTimerCallback() {
	timerCallbackCounter++;

	chibios_rt::CriticalSectionLocker csl;

	executeAllPendingActions();
	scheduleTimerCallback();
}

/*
 * this private method is executed under lock
 */
void SingleTimerExecutor::executeAllPendingActions() {
	ScopePerf perf(PE::SingleTimerExecutorDoExecute);

	executeAllPendingActionsInvocationCounter++;
	/**
	 * Let's execute actions we should execute at this point.
	 * reentrantFlag takes care of the use case where the actions we are executing are scheduling
	 * further invocations
	 */
	reentrantFlag = true;

	/**
	 * in real life it could be that while we executing listeners time passes and it's already time to execute
	 * next listeners.
	 * TODO: add a counter & figure out a limit of iterations?
	 */

	// starts at -1 because do..while will run a minimum of once
	executeCounter = -1;

	bool didExecute;
	do {
		efitick_t nowNt = getTimeNowNt();
		didExecute = queue.executeOne(nowNt);

		// if we're stuck in a loop executing lots of events, panic!
		if (executeCounter++ == 500) {
			firmwareError(CUSTOM_ERR_LOCK_ISSUE, "Maximum scheduling run length exceeded - CPU load too high");
		}

	} while (didExecute);

	maxExecuteCounter = maxI(maxExecuteCounter, executeCounter);

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
	expected<efitick_t> nextEventTimeNt = queue.getNextEventTime(nowNt);

	if (!nextEventTimeNt) {
		return; // no pending events in the queue
	}

	efiAssertVoid(CUSTOM_ERR_6625, nextEventTimeNt.Value > nowNt, "setTimer constraint");

	setHardwareSchedulerTimer(nowNt, nextEventTimeNt.Value);
}

void initSingleTimerExecutorHardware(void) {
	initMicrosecondTimer();
}

void executorStatistics() {
	if (engineConfiguration->debugMode == DBG_EXECUTOR) {
#if EFI_TUNER_STUDIO
		tsOutputChannels.debugIntField1 = ___engine.executor.timerCallbackCounter;
		tsOutputChannels.debugIntField2 = ___engine.executor.executeAllPendingActionsInvocationCounter;
		tsOutputChannels.debugIntField3 = ___engine.executor.scheduleCounter;
		tsOutputChannels.debugIntField4 = ___engine.executor.executeCounter;
		tsOutputChannels.debugIntField5 = ___engine.executor.maxExecuteCounter;
#endif /* EFI_TUNER_STUDIO */
	}
}

#endif /* EFI_SIGNAL_EXECUTOR_ONE_TIMER */

