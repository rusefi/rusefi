/*
 * @file    signal_executir_sleep.h

 * @date Oct 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "scheduler.h"

class SleepExecutor : public ExecutorInterface {
public:
	void scheduleByTimestamp(const char *msg, scheduling_s *scheduling, efitimeus_t timeUs, action_s action) override;
	void scheduleByTimestampNt(const char *msg, scheduling_s *scheduling, efitick_t timeNt, action_s action) override;
	void scheduleForLater(scheduling_s *scheduling, int delayUs, action_s action) override;
	void cancel(scheduling_s* s) override;
};
