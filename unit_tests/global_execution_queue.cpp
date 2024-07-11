/*
 * global_execution_queue.cpp
 *
 * @date Dec 8, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "global_execution_queue.h"

bool debugSignalExecutor = false;

TestExecutor::~TestExecutor() {
	// Flush the queue and reset all scheduling_s at the end of a test's execution
	clear();
}

int TestExecutor::executeAll(efitick_t now) {
	return schedulingQueue.executeAll(now);
}

void TestExecutor::clear() {
	schedulingQueue.clear();
}

int TestExecutor::size() {
	return schedulingQueue.size();
}

scheduling_s* TestExecutor::getHead() {
	return schedulingQueue.getHead();
}

scheduling_s* TestExecutor::getForUnitTest(int index) {
	return schedulingQueue.getElementAtIndexForUnitText(index);
}

void TestExecutor::scheduleByTimestampNt(const char *msg, scheduling_s* scheduling, efitick_t timeNt, action_s action) {
	if (m_mockExecutor) {
		m_mockExecutor->scheduleByTimestampNt(msg, scheduling, timeNt, action);
		return;
	}

	schedulingQueue.insertTask(scheduling, NT2US(timeNt), action);
}

void TestExecutor::cancel(scheduling_s* s) {
	if (m_mockExecutor) {
		m_mockExecutor->cancel(s);
		return;
	}

	schedulingQueue.remove(s);
}

void TestExecutor::setMockExecutor(ExecutorInterface* exec) {
	m_mockExecutor = exec;
}
