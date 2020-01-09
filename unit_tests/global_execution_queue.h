/*
 * @file global_execution_queue.h
 *
 *  Created on: Jan 9, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#ifndef GLOBAL_EXECUTION_QUEUE_H_
#define GLOBAL_EXECUTION_QUEUE_H_

#include "scheduler.h"
#include "event_queue.h"

class TestExecutor : public ExecutorInterface {
public:
	void scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, action_s action) override;
	void scheduleByTimestampNt(scheduling_s *scheduling, efitick_t timeNt, action_s action) override;
	void scheduleForLater(scheduling_s *scheduling, int delayUs, action_s action) override;
	void clear();
	int executeAll(efitime_t now);
	int size();
	scheduling_s* getForUnitTest(int index);
private:
	EventQueue schedulingQueue;
};

#endif /* GLOBAL_EXECUTION_QUEUE_H_ */
