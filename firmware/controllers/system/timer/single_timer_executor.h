/**
 * @file single_timer_executor.h
 *
 * @date: Apr 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "scheduler.h"
#include "event_queue.h"

class SingleTimerExecutor : public ExecutorInterface {
public:
	SingleTimerExecutor();
	void scheduleByTimestamp(efitimeus_t timeUs, action_s action) override;
	void scheduleByTimestampNt(efitime_t timeNt, action_s action) override;
	void scheduleForLater(int delayUs, action_s action) override;
	void onTimerCallback();
	int timerCallbackCounter = 0;
	int scheduleCounter = 0;
	int executeAllPendingActionsInvocationCounter = 0;
private:
	EventQueue queue;
	bool reentrantFlag = false;
	void executeAllPendingActions();
	void scheduleTimerCallback();
};

void initSingleTimerExecutorHardware(void);
void executorStatistics();

