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
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "os_access.h"
#include "event_queue.h"
#include "efitime.h"
#include "os_util.h"
#include "perf_trace.h"

#if EFI_UNIT_TEST
extern bool verboseMode;
#endif /* EFI_UNIT_TEST */

uint32_t maxSchedulingPrecisionLoss = 0;

EventQueue::EventQueue() {
	head = nullptr;
	setLateDelay(100);
}

bool EventQueue::checkIfPending(scheduling_s *scheduling) {
	assertNotInListMethodBody(scheduling_s, head, scheduling, nextScheduling_s);
}

/**
 * @return true if inserted into the head of the list
 */
bool EventQueue::insertTask(scheduling_s *scheduling, efitime_t timeX, action_s action) {
	ScopePerf perf(PE::EventQueueInsertTask);

#if EFI_UNIT_TEST
	assertListIsSorted();
#endif /* EFI_UNIT_TEST */
	efiAssert(CUSTOM_ERR_ASSERT, action.getCallback() != NULL, "NULL callback", false);

// please note that simulator does not use this code at all - simulator uses signal_executor_sleep

	if (scheduling->isScheduled) {
#if EFI_UNIT_TEST
		if (verboseMode) {
			printf("Already scheduled was %d\r\n", (int)scheduling->momentX);
			printf("Already scheduled now %d\r\n", (int)timeX);
		}
#endif /* EFI_UNIT_TEST */
		return false;
	}

	scheduling->momentX = timeX;
	scheduling->action = action;
	scheduling->isScheduled = true;

	if (head == NULL || timeX < head->momentX) {
		// here we insert into head of the linked list
		LL_PREPEND2(head, scheduling, nextScheduling_s);
#if EFI_UNIT_TEST
		assertListIsSorted();
#endif /* EFI_UNIT_TEST */
		return true;
	} else {
		// here we know we are not in the head of the list, let's find the position - linear search
		scheduling_s *insertPosition = head;
		while (insertPosition->nextScheduling_s != NULL && insertPosition->nextScheduling_s->momentX < timeX) {
			insertPosition = insertPosition->nextScheduling_s;
		}

		scheduling->nextScheduling_s = insertPosition->nextScheduling_s;
		insertPosition->nextScheduling_s = scheduling;
#if EFI_UNIT_TEST
		assertListIsSorted();
#endif /* EFI_UNIT_TEST */
		return false;
	}
}

/**
 * On this layer it does not matter which units are used - us, ms ot nt.
 *
 * This method is always invoked under a lock
 * @return Get the timestamp of the soonest pending action, skipping all the actions in the past
 */
efitime_t EventQueue::getNextEventTime(efitime_t nowX) const {
	
	if (head != NULL) {
		if (head->momentX <= nowX) {
			/**
			 * We are here if action timestamp is in the past
			 *
			 * looks like we end up here after 'writeconfig' (which freezes the firmware) - we are late
			 * for the next scheduled event
			 */
			efitime_t aBitInTheFuture = nowX + lateDelay;
			return aBitInTheFuture;
		} else {
			return head->momentX;
		}
	}
	return EMPTY_QUEUE;
}

/**
 * See also maxPrecisionCallbackDuration for total hw callback time
 */
uint32_t maxEventCallbackDuration = 0;

/**
 * Invoke all pending actions prior to specified timestamp
 * @return number of executed actions
 */
int EventQueue::executeAll(efitime_t now) {
	ScopePerf perf(PE::EventQueueExecuteAll);

	int executionCounter = 0;

#if EFI_UNIT_TEST
	assertListIsSorted();
#endif

	while (true) {
		// Read the head every time - a previously executed event could
		// have inserted something new at the head
		scheduling_s* current = head;

		// Queue is empty - bail
		if (!current) {
			break;
		}

		// Only execute events that occured in the past.
		// The list is sorted, so as soon as we see an event
		// in the future, we're done.
		if (current->momentX > now) {
			break;
		}

		executionCounter++;

		// step the head forward, unlink this element, clear scheduled flag
		head = current->nextScheduling_s;
		current->nextScheduling_s = nullptr;
		current->isScheduled = false;

#if EFI_UNIT_TEST
		printf("QUEUE: execute current=%d param=%d\r\n", (long)current, (long)current->action.getArgument());
#endif

		// Execute the current element
		{
			ScopePerf perf2(PE::EventQueueExecuteCallback);
			current->action.execute();
		}

#if EFI_UNIT_TEST
	// (tests only) Ensure we didn't break anything
	assertListIsSorted();
#endif
	}

	return executionCounter;
}

int EventQueue::size(void) const {
	scheduling_s *tmp;
	int result;
	LL_COUNT2(head, tmp, result, nextScheduling_s);
	return result;
}

void EventQueue::assertListIsSorted() const {
	scheduling_s *current = head;
	while (current != NULL && current->nextScheduling_s != NULL) {
		efiAssertVoid(CUSTOM_ERR_6623, current->momentX <= current->nextScheduling_s->momentX, "list order");
		current = current->nextScheduling_s;
	}
}

void EventQueue::setLateDelay(int value) {
	lateDelay = value;
}

scheduling_s * EventQueue::getHead() {
	return head;
}

// todo: reduce code duplication with another 'getElementAtIndexForUnitText'
scheduling_s *EventQueue::getElementAtIndexForUnitText(int index) {
	scheduling_s * current;

	LL_FOREACH2(head, current, nextScheduling_s)
	{
		if (index == 0)
			return current;
		index--;
	}
#if EFI_UNIT_TEST
	firmwareError(OBD_PCM_Processor_Fault, "getForUnitText: null");
#endif /* EFI_UNIT_TEST */
	return NULL;
}

void EventQueue::clear(void) {
	head = nullptr;
}
