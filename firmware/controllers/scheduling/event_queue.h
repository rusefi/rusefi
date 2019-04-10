/**
 * @file event_queue.h
 *
 * @date Apr 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "scheduler.h"
#include "utlist.h"

#ifndef EVENT_SCHEDULER_H_
#define EVENT_SCHEDULER_H_

/**
 * this is a large value which is expected to be larger than any real time
 */
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
			firmwareError(CUSTOM_ERR_LOOPED_QUEUE, "Looped queue?");
			return false;
		}
		if (current == element) {
			/**
			 * for example, this might happen in case of sudden RPM change if event
			 * was not scheduled by angle but was scheduled by time. In case of scheduling
			 * by time with slow RPM the whole next fast revolution might be within the wait period
			 */
			warning(CUSTOM_RE_ADDING_INTO_EXECUTION_QUEUE, "re-adding element into event_queue");
			return true;
		}
	}
	return false;
}

/**
 * Execution sorted linked list
 */
class EventQueue {
public:
	EventQueue();

	/**
	 * O(size) - linear search in sorted linked list
	 */
	bool insertTask(scheduling_s *scheduling, efitime_t timeX, schfunc_t callback, void *param);

	int executeAll(efitime_t now);

	efitime_t getNextEventTime(efitime_t nowUs);
	void clear(void);
	int size(void);
	scheduling_s *getForUnitText(int index);
	void setLateDelay(int value);
	scheduling_s * getHead();
	void assertListIsSorted();
private:
	bool checkIfPending(scheduling_s *scheduling);
	/**
	 * this list is sorted
	 */
	scheduling_s *head;
	efitime_t lateDelay;
};

#endif /* EVENT_SCHEDULER_H_ */
