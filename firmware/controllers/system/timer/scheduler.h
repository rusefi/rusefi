/**
 * @file	scheduler.h
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2019
 */
#pragma once

#include "global.h"

typedef void (*schfunc_t)(void *);

class action_s {
public:
	void setAction(schfunc_t callback, void *param);
	void execute();
	schfunc_t getCallback() const;
	void * getArgument() const;

private:
	schfunc_t callback = nullptr;
	void *param = nullptr;
};

/**
 * This structure holds information about an event scheduled in the future: when to execute what callback with what parameters
 */
class scheduling_s {
public:
#if EFI_SIGNAL_EXECUTOR_SLEEP
	virtual_timer_t timer;
#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */

	/**
	 * timestamp represented as 64-bit value of ticks since MCU start
	 */
	volatile efitime_t momentX = 0;
	bool isScheduled = false;

	/**
	 * Scheduler implementation uses a sorted linked list of these scheduling records.
	 */
	scheduling_s *nextScheduling_s = nullptr;

	action_s action;
};

class ExecutorInterface {
public:
	/**
	 * see also scheduleByAngle
	 */
	virtual void scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, schfunc_t callback, void *param) = 0;
	virtual void scheduleForLater(scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) = 0;
};
