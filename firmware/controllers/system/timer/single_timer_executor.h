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
	void scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, action_s action) override;
	void scheduleByTimestampNt(scheduling_s *scheduling, efitime_t timeNt, action_s action) override;
	void scheduleForLater(scheduling_s *scheduling, int delayUs, action_s action) override;
	void onTimerCallback();
	int timerCallbackCounter;
	int scheduleCounter;
	int doExecuteCounter;
private:
	EventQueue queue;
	bool reentrantFlag;
	void doExecute();
	void scheduleTimerCallback();
};

void initSingleTimerExecutorHardware(void);
void executorStatistics();

