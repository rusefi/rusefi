/*
 * SingleTimerExecutor.h
 *
 *  Created on: Apr 18, 2014
 *      Author: Andrey
 */

#ifndef SINGLETIMEREXECUTOR_H_
#define SINGLETIMEREXECUTOR_H_

#include "signal_executor.h"
#include "event_queue.h"

class Executor {
public:
	Executor();
	void schedule(scheduling_s *scheduling, uint64_t nowUs, int delayUs, schfunc_t callback, void *param);
	EventQueue eq;
};

#endif /* SINGLETIMEREXECUTOR_H_ */
