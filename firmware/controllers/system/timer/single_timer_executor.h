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
	void schedule(const char *msg, scheduling_s *scheduling, efitick_t timeNt, action_s action) override;
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
};

void initSingleTimerExecutorHardware(void);
void executorStatistics();

