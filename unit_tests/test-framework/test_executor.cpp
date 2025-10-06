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

int TestExecutor::executeAll(efitimeus_t nowUs) {
	return executeAllNt(US2NT(nowUs));
}

int TestExecutor::executeAllNt(efitick_t nowNt) {
	return schedulingQueue.executeAll(nowNt);
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

void TestExecutor::schedule(const char *msg, scheduling_s* scheduling, efitick_t timeNt, action_s const& action) {
	if (m_mockExecutor) {
		m_mockExecutor->schedule(msg, scheduling, timeNt, action);
		return;
	}
	// technical debt: NT -> US -> NT in unit test scheduler #7245
  // by the way we have loss of precision while converting NT to integer US
  // technical debt: looks like our unit tests were all created with us precision?
	efitimeus_t scheduleTime = timeNt;
	extern bool unitTestTaskPrecisionHack;
  	if(unitTestTaskPrecisionHack) {
    	scheduleTime = US2NT(NT2US(timeNt));
   	}
	schedulingQueue.insertTask(scheduling, scheduleTime, action);
}

void TestExecutor::cancel(scheduling_s* s) {
	if (m_mockExecutor) {
		m_mockExecutor->cancel(s);
		return;
	}

	schedulingQueue.remove(s);
}

void TestExecutor::setMockExecutor(Scheduler* exec) {
	m_mockExecutor = exec;
}
