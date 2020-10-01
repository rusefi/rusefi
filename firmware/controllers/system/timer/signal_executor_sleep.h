/*
 * @file    signal_executir_sleep.h

 * @date Oct 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "scheduler.h"

class SleepExecutor : public ExecutorInterface {
public:
	void scheduleByTimestamp(efitimeus_t timeUs, action_s action) override;
	void scheduleByTimestampNt(efitick_t timeNt, action_s action) override;
	void scheduleForLater(int delayUs, action_s action) override;
};
