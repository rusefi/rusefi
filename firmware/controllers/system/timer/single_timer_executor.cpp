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

#include "pch.h"


#include "single_timer_executor.h"
#include "efitime.h"

#if EFI_SIGNAL_EXECUTOR_ONE_TIMER

#include "microsecond_timer.h"
#include "os_util.h"

uint32_t hwSetTimerDuration;

void globalTimerCallback() {
	efiAssertVoid(ObdCode::CUSTOM_ERR_6624, hasLotsOfRemainingStack(), "lowstck#2y");

	___engine.scheduler.onTimerCallback();
}

SingleTimerExecutor::SingleTimerExecutor()
	// 8us is roughly the cost of the interrupt + overhead of a single timer event
	: queue(US2NT(8))
{
}

void SingleTimerExecutor::schedule(const char *msg, scheduling_s* scheduling, efitick_t nt, action_s const& action) {
	ScopePerf perf(PE::SingleTimerExecutorScheduleByTimestamp);

#if EFI_ENABLE_ASSERTS
	efidur_t deltaTimeNt = nt - getTimeNowNt();

	if (deltaTimeNt >= TOO_FAR_INTO_FUTURE_NT) {
		// we are trying to set callback for too far into the future. This does not look right at all
		int32_t intDeltaTimeNt = (int32_t)deltaTimeNt;
		firmwareError(ObdCode::RUNTIME_CRITICAL_TASK_TIMER_OVERFLOW, "schedule() too far: %ld %s", intDeltaTimeNt, msg);
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

void SingleTimerExecutor::cancel(scheduling_s* scheduling) {
	// Lock for queue removal - we may already be locked, but that's ok
	chibios_rt::CriticalSectionLocker csl;

	queue.remove(scheduling);
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

	scheduling_s* current = nullptr;
	do {
		efitick_t nowNt = getTimeNowNt();
		current = queue.pickOne(nowNt);

		if (current) {
#if EFI_UNIT_TEST
			//	efitick_t spinDuration = current->getMomentNt() - getTimeNowNt();
			//	if (spinDuration > 0) {
			//		throw std::runtime_error("Time Spin in unit test");
			//	}
#endif

			// near future - spin wait for the event to happen and avoid the
			// overhead of rescheduling the timer.
			// yes, that's a busy wait but that's what we need here
			while (current->getMomentNt() > getTimeNowNt()) {
#if EFI_UNIT_TEST
				// todo: remove this hack see https://github.com/rusefi/rusefi/issues/6457
extern bool unitTestBusyWaitHack;
				if (unitTestBusyWaitHack) {
					break;
				}
#endif
				UNIT_TEST_BUSY_WAIT_CALLBACK();
			}

			// now it is time to execute
			queue.executeAndFree(current);
		}
	} while ((current) && (++executeCounter < 500));

	maxExecuteCounter = maxI(maxExecuteCounter, executeCounter);

	reentrantFlag = false;

	// if we're stuck in a loop executing lots of events, panic!
	if (executeCounter >= 500) {
		firmwareError(ObdCode::CUSTOM_ERR_LOCK_ISSUE, "Maximum scheduling run length exceeded - CPU load too high");
	}

	if (!isLocked()) {
		firmwareError(ObdCode::CUSTOM_ERR_LOCK_ISSUE, "Someone has stolen my lock");
	}
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

	efiAssertVoid(ObdCode::CUSTOM_ERR_6625, nextEventTimeNt.Value > nowNt, "setTimer constraint");

	setHardwareSchedulerTimer(nowNt, nextEventTimeNt.Value);
}

void initSingleTimerExecutorHardware() {
	initMicrosecondTimer();
}

void executorStatistics() {
	if (engineConfiguration->debugMode == DBG_EXECUTOR) {
#if EFI_TUNER_STUDIO
		engine->outputChannels.debugIntField1 = ___engine.scheduler.timerCallbackCounter;
		engine->outputChannels.debugIntField2 = ___engine.scheduler.executeAllPendingActionsInvocationCounter;
		engine->outputChannels.debugIntField3 = ___engine.scheduler.scheduleCounter;
		engine->outputChannels.debugIntField4 = ___engine.scheduler.executeCounter;
		engine->outputChannels.debugIntField5 = ___engine.scheduler.maxExecuteCounter;
#endif /* EFI_TUNER_STUDIO */
	}
}

#endif /* EFI_SIGNAL_EXECUTOR_ONE_TIMER */

