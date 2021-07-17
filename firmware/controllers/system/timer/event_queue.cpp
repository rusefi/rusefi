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

#include "os_access.h"
#include "event_queue.h"
#include "efitime.h"
#include "os_util.h"
#include "perf_trace.h"

#if EFI_UNIT_TEST
extern int timeNowUs;
extern bool verboseMode;
#endif /* EFI_UNIT_TEST */


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

	if (scheduling->action) {
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
expected<efitime_t> EventQueue::getNextEventTime(efitime_t nowX) const {
	if (head != NULL) {
		if (head->momentX <= nowX) {
			/**
			 * We are here if action timestamp is in the past. We should rarely be here since this 'getNextEventTime()' is
			 * always invoked by 'scheduleTimerCallback' which is always invoked right after 'executeAllPendingActions' - but still,
			 * for events which are really close to each other we would end up here.
			 *
			 * looks like we end up here after 'writeconfig' (which freezes the firmware) - we are late
			 * for the next scheduled event
			 */
			return nowX + lateDelay;
		} else {
			return head->momentX;
		}
	}

	return unexpected;
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

	bool didExecute;
	do {
		didExecute = executeOne(now);
		executionCounter += didExecute ? 1 : 0;
	} while (didExecute);

	return executionCounter;
}

bool EventQueue::executeOne(efitime_t now) {
	// Read the head every time - a previously executed event could
	// have inserted something new at the head
	scheduling_s* current = head;

	// Queue is empty - bail
	if (!current) {
		return false;
	}

	// If the next event is far in the future, we'll reschedule
	// and execute it next time.
	// We do this when the next event is close enough that the overhead of
	// resetting the timer and scheduling an new interrupt is greater than just
	// waiting for the time to arrive.  On current CPUs, this is reasonable to set
	// around 10 microseconds.
	if (current->momentX > now + lateDelay) {
		return false;
	}

	// near future - spin wait for the event to happen and avoid the
	// overhead of rescheduling the timer.
	// yes, that's a busy wait but that's what we need here
	while (current->momentX > getTimeNowNt()) {
		UNIT_TEST_BUSY_WAIT_CALLBACK();
	}

	// step the head forward, unlink this element, clear scheduled flag
	head = current->nextScheduling_s;
	current->nextScheduling_s = nullptr;

	// Grab the action but clear it in the event so we can reschedule from the action's execution
	auto action = current->action;
	current->action = {};

#if EFI_UNIT_TEST
	printf("QUEUE: execute current=%d param=%d\r\n", (long)current, (long)action.getArgument());
#endif

	// Execute the current element
	{
		ScopePerf perf2(PE::EventQueueExecuteCallback);
		action.execute();
	}

#if EFI_UNIT_TEST
	// (tests only) Ensure we didn't break anything
	assertListIsSorted();
#endif

	return true;
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
	// Flush the queue, resetting all scheduling_s as though we'd executed them
	while(head) {
		auto x = head;
		// link next element to head
		head = x->nextScheduling_s;

		// Reset this element
		x->momentX = 0;
		x->nextScheduling_s = nullptr;
		x->action = {};
	}

	head = nullptr;
}
