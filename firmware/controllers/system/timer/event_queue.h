/**
 * @file event_queue.h
 *
 * @date Apr 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "scheduler.h"
#include "utlist.h"
#include "expected.h"

#pragma once

#define QUEUE_LENGTH_LIMIT 1000

// templates do not accept field names so we use a macro here
#define assertNotInListMethodBody(T, head, element, field)                  \
	/* this code is just to validate state, no functional load*/            \
	T * current;                                                            \
	int counter = 0;                                                        \
	LL_FOREACH2(head, current, field) {                                     \
		if (++counter > QUEUE_LENGTH_LIMIT) {                               \
			firmwareError(CUSTOM_ERR_LOOPED_QUEUE, "Looped queue?");        \
			return false;                                                   \
		}                                                                   \
		if (current == element) {                                           \
			/**                                                                                     \
			 * for example, this might happen in case of sudden RPM change if event                 \
			 * was not scheduled by angle but was scheduled by time. In case of scheduling          \
			 * by time with slow RPM the whole next fast revolution might be within the wait period \
			 */                                                                                     \
			warning(CUSTOM_RE_ADDING_INTO_EXECUTION_QUEUE, "re-adding element into event_queue");   \
			return true;                                                    \
		} \
	} \
	return false;


/**
 * Execution sorted linked list
 */
class EventQueue {
public:
	// See comment in EventQueue::executeAll for info about lateDelay - it sets the 
	// time gap between events for which we will wait instead of rescheduling the next
	// event in a group of events near one another.
	EventQueue(efitime_t lateDelay = 0) : lateDelay(lateDelay) {}

	/**
	 * O(size) - linear search in sorted linked list
	 */
	bool insertTask(scheduling_s *scheduling, efitime_t timeX, action_s action);
	void remove(scheduling_s* scheduling);

	int executeAll(efitime_t now);
	bool executeOne(efitime_t now);

	expected<efitime_t> getNextEventTime(efitime_t nowUs) const;
	void clear(void);
	int size(void) const;
	scheduling_s *getElementAtIndexForUnitText(int index);
	scheduling_s * getHead();
	void assertListIsSorted() const;
private:
	/**
	 * this list is sorted
	 */
	scheduling_s *head = nullptr;
	const efitime_t lateDelay;
};

