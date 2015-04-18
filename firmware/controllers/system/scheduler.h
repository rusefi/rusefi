/**
 * @file	scheduler.h
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "main.h"

typedef void (*schfunc_t)(void *);

class scheduling_s {
public:
	scheduling_s();

#if EFI_SIGNAL_EXECUTOR_SLEEP
	VirtualTimer timer;
#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */

	volatile uint64_t momentX;
	schfunc_t callback;
	void *param;
	scheduling_s *next;
	bool_t isScheduled;
};

void scheduleTask(const char *prefix, scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param);
void scheduleByTime(const char *prefix, scheduling_s *scheduling, efitimeus_t time, schfunc_t callback, void *param);

#endif /* SCHEDULER_H_ */
