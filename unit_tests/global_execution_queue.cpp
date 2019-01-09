/*
 * global_execution_queue.cpp
 *
 * @date Dec 8, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "signal_executor.h"
#include "event_queue.h"
#include "global_execution_queue.h"

// this global instance is used by integration tests via 'scheduleByTimestamp' global methods below
EventQueue schedulingQueue;

bool_t debugSignalExecutor = false;

TestExecutor::TestExecutor() {

}

void TestExecutor::scheduleForLater(scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) {
	if (debugSignalExecutor) {
		printf("scheduleTask %d\r\n", delayUs);
	}
	scheduleByTimestamp(scheduling, getTimeNowUs() + delayUs, callback, param);
}


void TestExecutor::clear() {
	schedulingQueue.clear();
}

int TestExecutor::size() {
	return schedulingQueue.size();
}

void TestExecutor::scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, schfunc_t callback, void *param) {
	if (debugSignalExecutor) {
		printf("scheduleByTime %d\r\n", timeUs);
	}
	schedulingQueue.insertTask(scheduling, timeUs, callback, param);
}

void initSignalExecutorImpl(void) {
}

