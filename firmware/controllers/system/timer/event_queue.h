/**
 * @file event_queue.h
 *
 * @date Apr 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "scheduler.h"
#include "utlist.h"
#include <rusefi/expected.h>

#define QUEUE_LENGTH_LIMIT 1000

/**
 * Execution sorted linked list
 */
class EventQueue {
public:
	// See comment in EventQueue::executeAll for info about lateDelay - it sets the
	// time gap between events for which we will wait instead of rescheduling the next
	// event in a group of events near one another.
	explicit EventQueue(efidur_t lateDelay = 0);

	/**
	 * O(size) - linear search in sorted linked list
	 */
	bool insertTask(scheduling_s *scheduling, efitick_t timeX, action_s const& action);
	void remove(scheduling_s* scheduling);

	int executeAll(efitick_t now);
	scheduling_s* pickOne(efitick_t now);
	void executeAndFree(scheduling_s* current);
	bool executeOne(efitick_t now);

	expected<efitick_t> getNextEventTime(efitick_t nowUs) const;
	void clear();
	int size() const;
	scheduling_s *getElementAtIndexForUnitText(int index);
	scheduling_s * getHead();

	scheduling_s* getFreeScheduling();
	void tryReturnScheduling(scheduling_s* sched);
private:
	void assertListIsSorted() const;
	/**
	 * this list is sorted
	 */
	scheduling_s *m_head = nullptr;
	const efidur_t m_lateDelay;

	scheduling_s* m_freelist = nullptr;
	scheduling_s m_pool[64];
};

