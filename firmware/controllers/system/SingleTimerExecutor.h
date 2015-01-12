/**
 * @file SingleTimerExecutor.h
 *
 * @date: Apr 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef SINGLETIMEREXECUTOR_H_
#define SINGLETIMEREXECUTOR_H_

#include "scheduler.h"
#include "event_queue.h"

class Executor {
public:
	Executor();
	void schedule(scheduling_s *scheduling, uint64_t nowUs, int delayUs, schfunc_t callback, void *param);
	void schedule2(scheduling_s *scheduling, uint64_t timeUs, schfunc_t callback, void *param);
	void onTimerCallback();
private:
	EventQueue queue;
	bool reentrantFlag;
	void doExecute();
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initSignalExecutorImpl(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SINGLETIMEREXECUTOR_H_ */
