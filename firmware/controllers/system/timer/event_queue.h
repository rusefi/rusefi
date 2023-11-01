/**
 * @file event_queue.h
 *
 * @date Apr 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "scheduler.h"
#include "utlist.h"
#include <rusefi/expected.h>

#pragma once

#define QUEUE_LENGTH_LIMIT 1000

// templates do not accept field names so we use a macro here
#define assertNotInListMethodBody(head, element, field)                     \
	/* this code is just to validate state, no functional load*/            \
	decltype(head) current;                                                 \
	int counter = 0;                                                        \
	LL_FOREACH2(head, current, field) {                                     \
		if (++counter > QUEUE_LENGTH_LIMIT) {                               \
			firmwareError(ObdCode::CUSTOM_ERR_LOOPED_QUEUE, "Looped queue?");        \
			return false;                                                   \
		}                                                                   \
		if (current == element) {                                           \
			/**                                                                                     \
			 * for example, this might happen in case of sudden RPM change if event                 \
			 * was not scheduled by angle but was scheduled by time. In case of scheduling          \
			 * by time with slow RPM the whole next fast revolution might be within the wait period \
			 */                                                                                     \
			warning(ObdCode::CUSTOM_RE_ADDING_INTO_EXECUTION_QUEUE, "re-adding element into event_queue");   \
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
	explicit EventQueue(efitick_t lateDelay = 0);

	/**
	 * O(size) - linear search in sorted linked list
	 */
	bool insertTask(scheduling_s *scheduling, efitick_t timeX, action_s action);
	void remove(scheduling_s* scheduling);

	int executeAll(efitick_t now);
	bool executeOne(efitick_t now);

	expected<efitick_t> getNextEventTime(efitick_t nowUs) const;
	void clear(void);
	int size(void) const;
	scheduling_s *getElementAtIndexForUnitText(int index);
	scheduling_s * getHead();
	void assertListIsSorted() const;

	scheduling_s* getFreeScheduling();
	void tryReturnScheduling(scheduling_s* sched);
private:
	/**
	 * this list is sorted
	 */
	scheduling_s *m_head = nullptr;
	const efitick_t m_lateDelay;

	scheduling_s* m_freelist = nullptr; 
	scheduling_s m_pool[64];
};

