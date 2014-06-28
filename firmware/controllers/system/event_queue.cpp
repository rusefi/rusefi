/**
 * @file event_queue.cpp
 * This is a data structure which keeps track of all pending events
 * Implemented as a linked list, which is fine since the number of
 * pending events is pretty low
 * todo: MAYBE migrate to a better data structure, but that's low priority
 *
 * this data structure is NOT thread safe
 *
 * @date Apr 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "event_queue.h"
#include "efitime.h"

EventQueue::EventQueue() {
	head = NULL;
}

bool_t EventQueue::checkIfPending(scheduling_s *scheduling) {
	return assertNotInList<scheduling_s>(head, scheduling);
}

void EventQueue::insertTask(scheduling_s *scheduling, uint64_t nowUs, int delayUs, schfunc_t callback, void *param) {
	if (callback == NULL)
		firmwareError("NULL callback");
	uint64_t time = nowUs + delayUs;

	int alreadyPending = checkIfPending(scheduling);
	if (alreadyPending || hasFirmwareError())
		return;

	scheduling->momentUs = time;
	scheduling->callback = callback;
	scheduling->param = param;

	LL_PREPEND(head, scheduling);
}

//void EventQueue::insertTask(scheduling_s *scheduling, int delayUs, schfunc_t callback, void *param) {
//	insertTask(scheduling, getTimeNowUs(), delayUs, callback, param);
//}

/**
 * Get the timestamp of the soonest pending action
 */
uint64_t EventQueue::getNextEventTime(uint64_t nowUs) {
	scheduling_s * current;
	// this is a large value which is expected to be larger than any real time
	uint64_t result = EMPTY_QUEUE;

	int counter = 0;
	LL_FOREACH(head, current)
	{
		if (++counter > QUEUE_LENGTH_LIMIT) {
			firmwareError("Is this list looped #2?");
			return EMPTY_QUEUE;
		}
		efiAssert(current->momentUs > nowUs, "executeAll should have been called", EMPTY_QUEUE);
		if (current->momentUs < result)
			result = current->momentUs;
	}
	return result;
}

/**
 * Invoke all pending actions prior to specified timestamp
 */
void EventQueue::executeAll(uint64_t now) {
	scheduling_s * current, *tmp;

	scheduling_s * executionList = NULL;

	int counter = 0;

	// we need safe iteration because we are removing elements inside the loop
	LL_FOREACH_SAFE(head, current, tmp)
	{
		if (++counter > QUEUE_LENGTH_LIMIT) {
			firmwareError("Is this list looped?");
			return;
		}
		if (current->momentUs <= now) {
			LL_DELETE(head, current);
			LL_PREPEND(executionList, current);
		}
	}

	/*
	 * we need safe iteration here because 'callback' might change change 'current->next'
	 * while re-inserting it into the queue from within the callback
	 */
	LL_FOREACH_SAFE(executionList, current, tmp)
		current->callback(current->param);
}

int EventQueue::size(void) {
	scheduling_s *tmp;
	int result;
	LL_COUNT(head, tmp, result);
	return result;
}

scheduling_s *EventQueue::getForUnitText(int index) {
	scheduling_s * current;

	LL_FOREACH(head, current)
	{
		if (index == 0)
			return current;
		index--;
	}
	return NULL;
}

void EventQueue::clear(void) {
	head = NULL;
}
