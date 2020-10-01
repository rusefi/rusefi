/*
 * global_execution_queue.cpp
 *
 * @date Dec 8, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global_execution_queue.h"

bool_t debugSignalExecutor = false;
extern bool verboseMode;

TestExecutor::~TestExecutor() {
	// Flush the queue and reset all scheduling_s at the end of a test's execution
	clear();
}

void TestExecutor::scheduleForLater(int delayUs, action_s action) {
	if (debugSignalExecutor) {
		printf("scheduleTask %d\r\n", delayUs);
	}

	if (m_mockExecutor) {
		m_mockExecutor->scheduleForLater(delayUs, action);
		return;
	}

	scheduleByTimestamp(getTimeNowUs() + delayUs, action);
}

int TestExecutor::executeAll(efitime_t now) {
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

void TestExecutor::scheduleByTimestamp(efitimeus_t timeUs, action_s action) {
	if (debugSignalExecutor) {
		printf("scheduleByTime %d\r\n", timeUs);
	}

	if (m_mockExecutor) {
		m_mockExecutor->scheduleByTimestamp(timeUs, action);
		return;
	}

	schedulingQueue.insertTask(timeUs, action);
}

void TestExecutor::scheduleByTimestampNt(efitick_t timeNt, action_s action) {
	if (m_mockExecutor) {
		m_mockExecutor->scheduleByTimestampNt(timeNt, action);
		return;
	}

	scheduleByTimestamp(NT2US(timeNt), action);
}

void TestExecutor::setMockExecutor(ExecutorInterface* exec) {
	m_mockExecutor = exec;
}
