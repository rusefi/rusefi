/*
 * global_execution_queue.h
 *
 *  Created on: Jan 9, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef GLOBAL_EXECUTION_QUEUE_H_
#define GLOBAL_EXECUTION_QUEUE_H_

#include "scheduler.h"

class TestExecutor : public ExecutorInterface {
public:
	TestExecutor();
	void scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, schfunc_t callback, void *param);
	void scheduleForLater(scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param);
	void clear();
	int size();
	scheduling_s* getForUnitTest(int index);
};

#endif /* GLOBAL_EXECUTION_QUEUE_H_ */
