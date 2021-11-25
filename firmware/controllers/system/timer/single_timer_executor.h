/**
 * @file single_timer_executor.h
 *
 * @date: Apr 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "scheduler.h"
#include "event_queue.h"

class SingleTimerExecutor final : public ExecutorInterface {
public:
	SingleTimerExecutor();
	void scheduleByTimestamp(const char *msg, scheduling_s *scheduling, efitimeus_t timeUs, action_s action) override;
	void scheduleByTimestampNt(const char *msg, scheduling_s *scheduling, efitime_t timeNt, action_s action) override;
	void scheduleForLater(scheduling_s *scheduling, int delayUs, action_s action) override;
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

