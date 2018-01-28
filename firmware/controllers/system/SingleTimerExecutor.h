/**
 * @file SingleTimerExecutor.h
 *
 * @date: Apr 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef SINGLETIMEREXECUTOR_H_
#define SINGLETIMEREXECUTOR_H_

#include "scheduler.h"
#include "event_queue.h"

class Executor {
public:
	Executor();
	void scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, schfunc_t callback, void *param);
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

void initSignalExecutorImpl(void);
void executorStatistics();

#endif /* SINGLETIMEREXECUTOR_H_ */
