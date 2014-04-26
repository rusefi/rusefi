/**
 * @file event_queue.h
 *
 * @date Apr 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "signal_executor.h"

#ifndef EVENT_SCHEDULER_H_
#define EVENT_SCHEDULER_H_

#define EMPTY_QUEUE 0x0FFFFFFFFFFFFFFFLL

class EventQueue {
public:
	EventQueue();

	void insertTask(scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param);
	void insertTask(scheduling_s *scheduling, uint64_t nowUs, int delayUs, schfunc_t callback, void *param);

	void executeAll(uint64_t now);

	uint64_t getNextEventTime(uint64_t nowUs);
	void clear(void);
private:
	scheduling_s *head;
};

#endif /* EVENT_SCHEDULER_H_ */
