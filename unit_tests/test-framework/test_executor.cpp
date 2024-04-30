/*
 * test_executor.cpp
 *
 * @date Dec 8, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "test_executor.h"

bool debugSignalExecutor = false;

TestExecutor::~TestExecutor() {
	// Flush the queue and reset all scheduling_s at the end of a test's execution
	clear();
}

void TestExecutor::scheduleForLater(const char *msg, scheduling_s *scheduling, int delayUs, action_s action) {
	if (debugSignalExecutor) {
		printf("scheduleTask %d\r\n", delayUs);
	}

	if (m_mockExecutor) {
		m_mockExecutor->scheduleForLater(msg, scheduling, delayUs, action);
		return;
	}

	scheduleByTimestamp(msg, scheduling, getTimeNowUs() + delayUs, action);
}

int TestExecutor::executeAll(efitimeus_t nowUs) {
	return schedulingQueue.executeAll(US2NT(nowUs));
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

void TestExecutor::scheduleByTimestamp(const char *msg, scheduling_s *scheduling, efitimeus_t timeUs, action_s action) {
	if (timeUs < 0) {
		throw std::runtime_error("Negative timeUs not expected.");
	}

	if (debugSignalExecutor) {
		printf("scheduleByTime %d\r\n", timeUs);
	}

	if (m_mockExecutor) {
		m_mockExecutor->scheduleByTimestamp(msg, scheduling, timeUs, action);
		return;
	}

	schedulingQueue.insertTask(scheduling, US2NT(timeUs), action);
}

void TestExecutor::scheduleByTimestampNt(const char *msg, scheduling_s* scheduling, efitick_t timeNt, action_s action) {
	if (m_mockExecutor) {
		m_mockExecutor->scheduleByTimestampNt(msg, scheduling, timeNt, action);
		return;
	}
  // by the way we have loss of precision while converting NT to integer US
	scheduleByTimestamp(msg, scheduling, NT2US(timeNt), action);
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
