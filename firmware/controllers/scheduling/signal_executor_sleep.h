/*
 * @file    signal_executir_sleep.h

 * @date Oct 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef SIGNAL_EXECUTOR_SLEEP_H_
#define SIGNAL_EXECUTOR_SLEEP_H_

#include "scheduler.h"

class SleepExecutor : public ExecutorInterface {
public:
	void scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, schfunc_t callback, void *param) override;
	void scheduleForLater(scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) override;
};

#endif /* SIGNAL_EXECUTOR_SLEEP_H_ */
