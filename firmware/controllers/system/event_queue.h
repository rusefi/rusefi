/**
 * @file event_queue.h
 *
 * @date Apr 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
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
			return FALSE;
		}
		if (current == element) {
			warning(OBD_PCM_Processor_Fault, "re-adding element into event_queue: [%s]", element->name);
			return TRUE;
		}
	}
	return FALSE;
}

class EventQueue {
public:
	EventQueue();

	void insertTask(scheduling_s *scheduling, uint64_t timeUs, schfunc_t callback, void *param);

	bool executeAll(uint64_t now);

	uint64_t getNextEventTime(uint64_t nowUs);
	void clear(void);
	int size(void);
	scheduling_s *getForUnitText(int index);
private:
	bool checkIfPending(scheduling_s *scheduling);
	scheduling_s *head;
};

#endif /* EVENT_SCHEDULER_H_ */
