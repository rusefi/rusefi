/**
 * @file single_timer_executor.h
 *
 * @date: Apr 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "scheduler.h"
#include "event_queue.h"

class SingleTimerExecutor final : public Scheduler {
public:
	SingleTimerExecutor();
	bool schedule(const char *msg, scheduling_s *scheduling, efitick_t timeNt, action_s const& action) override;
	bool scheduleTogether(const char *msg, const TimedAction *actions, size_t count) override;
	void cancel(scheduling_s* scheduling) override;

	void onTimerCallback();
	int timerCallbackCounter = 0;
	int scheduleCounter = 0;
	int maxExecuteCounter = 0;
	int executeCounter;
	int executeAllPendingActionsInvocationCounter = 0;
private:
	EventQueue queue;
	bool reentrantFlag = false;
	void executeAllPendingActions();
	void scheduleTimerCallback();
	bool isTooFarIntoFuture(const char *msg, efitick_t nt) const;
	void onInserted(EventQueue::InsertResult result);
};

void initSingleTimerExecutorHardware();
void executorStatistics();

