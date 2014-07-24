/**
 * @file	scheduler.h
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "main.h"

typedef void (*schfunc_t)(void *);

typedef struct scheduling_struct scheduling_s;
struct scheduling_struct {
#if EFI_SIGNAL_EXECUTOR_SLEEP
	VirtualTimer timer;
#endif /* EFI_SIGNAL_EXECUTOR_SLEEP */

	volatile uint64_t momentUs;
	schfunc_t callback;
	void *param;
	scheduling_s *next;
	const char *name;
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void scheduleTask(const char *prefix, scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SCHEDULER_H_ */
