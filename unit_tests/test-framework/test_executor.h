/*
 * @file test_executor.h
 * implementation of ExecutorInterface used by unit tests
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

	bool schedule(const char *msg, scheduling_s *scheduling, efitick_t timeNt, action_s const& action) override;
	bool scheduleTogether(const char *msg, const TimedAction *actions, size_t count) override;
	void cancel(scheduling_s* scheduling) override;

	void clear();
	int executeAll(efitimeus_t nowUs);
	int executeAllNt(efitick_t nowNt);
	int size();
	scheduling_s * getHead();
	scheduling_s * getForUnitTest(int index);
	int getMaxUsedPoolSlots() const { return schedulingQueue.getMaxUsedPoolSlots(); }
	int getUsedPoolSlots() const { return schedulingQueue.getUsedPoolSlots(); }
	int getAvailablePoolSlots() const { return schedulingQueue.getAvailablePoolSlots(); }

	void setMockExecutor(Scheduler* exec);
private:
	EventQueue schedulingQueue;
	Scheduler* m_mockExecutor = nullptr;
};
