/*
 * @file global_execution_queue.h
 *
 *  Created on: Jan 9, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "scheduler.h"
#include "event_queue.h"

class TestExecutor : public Scheduler {
public:
	~TestExecutor();

	void schedule(const char *msg, scheduling_s *scheduling, efitick_t timeNt, action_s action) override;
	void cancel(scheduling_s* scheduling) override;

	void clear();
	int executeAll(efitick_t now);
	int size();
	scheduling_s * getHead();
	scheduling_s * getForUnitTest(int index);

	void setMockExecutor(Scheduler* exec);
private:
	EventQueue schedulingQueue;
	Scheduler* m_mockExecutor = nullptr;
};
