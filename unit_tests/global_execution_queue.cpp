/*
 * global_execution_queue.cpp
 *
 * @date Dec 8, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "signal_executor.h"
#include "global_execution_queue.h"

bool_t debugSignalExecutor = false;
extern bool verboseMode;

void TestExecutor::scheduleForLater(scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) {
	if (debugSignalExecutor) {
		printf("scheduleTask %d\r\n", delayUs);
	}
	scheduleByTimestamp(scheduling, getTimeNowUs() + delayUs, callback, param);
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

scheduling_s* TestExecutor::getForUnitTest(int index) {
	return schedulingQueue.getForUnitText(index);
}

void TestExecutor::scheduleByTimestamp(scheduling_s *scheduling, efitimeus_t timeUs, schfunc_t callback, void *param) {
	if (debugSignalExecutor) {
		printf("scheduleByTime %d\r\n", timeUs);
	}
	schedulingQueue.insertTask(scheduling, timeUs, callback, param);
}
