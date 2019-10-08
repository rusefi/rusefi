/**
 * @file	scheduler.h
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "global.h"

typedef void (*schfunc_t)(void *);

class scheduling_s {
public:
#if EFI_SIGNAL_EXECUTOR_SLEEP
	virtual_timer_t timer;
#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */

	volatile efitime_t momentX = 0;
	bool isScheduled = false;

	scheduling_s *next = nullptr;

	schfunc_t callback = nullptr;
	void *param = nullptr;
};

class ExecutorInterface {
public:
	/**
	 * see also scheduleByAngle
	 */
	virtual void scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, schfunc_t callback, void *param) = 0;
	virtual void scheduleForLater(scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) = 0;
};

#endif /* SCHEDULER_H_ */
