/**
 * @file event_queue.h
 *
 * @date Apr 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "scheduler.h"
#include "utlist.h"

#ifndef EVENT_SCHEDULER_H_
#define EVENT_SCHEDULER_H_

#define EMPTY_QUEUE 0x0FFFFFFFFFFFFFFFLL

#define QUEUE_LENGTH_LIMIT 1000

template<typename T>
bool assertNotInList(T *head, T*element) {
	// this code is just to validate state, no functional load
	T * current;
	int counter = 0;
	LL_FOREACH(head, current)
	{
		if (++counter > QUEUE_LENGTH_LIMIT) {
			firmwareError("Looped queue?");
			return false;
		}
		if (current == element) {
			/**
			 * for example, this might happen in case of sudden RPM change if event
			 * was not scheduled by angle but was scheduled by time. In case of scheduling
			 * by time with slow RPM the whole next fast revolution might be within the wait period
			 */
			warning(OBD_PCM_Processor_Fault, "re-adding element into event_queue: [%s]", element->name);
			return true;
		}
	}
	return false;
}

class EventQueue {
public:
	EventQueue();

	void insertTask(scheduling_s *scheduling, uint64_t timeX, schfunc_t callback, void *param);

	int executeAll(uint64_t now);

	uint64_t getNextEventTime(uint64_t nowUs);
	void clear(void);
	int size(void);
	scheduling_s *getForUnitText(int index);
	void setLateDelay(int value);
	scheduling_s * getHead();
private:
	bool checkIfPending(scheduling_s *scheduling);
	scheduling_s *head;
	uint64_t lateDelay;
};

#endif /* EVENT_SCHEDULER_H_ */
