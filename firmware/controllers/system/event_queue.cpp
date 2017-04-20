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
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "main.h"
#include "event_queue.h"
#include "efitime.h"
#include "efilib2.h"

int maxHowFarOff = 0;

scheduling_s::scheduling_s() {
	callback = NULL;
	next = NULL;
	param = NULL;
	isScheduled = false;
	momentX = 0;
}

EventQueue::EventQueue() {
	head = NULL;
	setLateDelay(100);
}

bool EventQueue::checkIfPending(scheduling_s *scheduling) {
	return assertNotInList<scheduling_s>(head, scheduling);
}

bool EventQueue::insertTask(scheduling_s *scheduling, efitime_t timeX, schfunc_t callback, void *param) {
	return insertTask(false, "*", scheduling, timeX, callback, param);
}
/**
 * @return true if inserted into the head of the list
 */
bool EventQueue::insertTask(const bool monitorReuse, const char *prefix, scheduling_s *scheduling, efitime_t timeX, schfunc_t callback, void *param) {
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	assertListIsSorted();
#endif /* EFI_UNIT_TEST */
	efiAssert(callback != NULL, "NULL callback", false);

// please note that simulator does not use this code at all - simulator uses signal_executor_sleep

	if (scheduling->isScheduled) {
		if (monitorReuse) {
			warning(CUSTOM_OBD_SCH_REUSE, "reuse [%s]", prefix);
		}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		printf("Already scheduled was %d\r\n", (int)scheduling->momentX);
		printf("Already scheduled now %d\r\n", (int)timeX);
#endif /* EFI_UNIT_TEST || EFI_SIMULATOR */
		return false;
	}

	scheduling->momentX = timeX;
	scheduling->callback = callback;
	scheduling->param = param;
	scheduling->isScheduled = true;

	if (head == NULL || timeX < head->momentX) {
		LL_PREPEND(head, scheduling);
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		assertListIsSorted();
#endif /* EFI_UNIT_TEST */
		return true;
	} else {
		scheduling_s *insertPosition = head;
		while (insertPosition->next != NULL && insertPosition->next->momentX < timeX) {
			insertPosition = insertPosition->next;
		}

		scheduling->next = insertPosition->next;
		insertPosition->next = scheduling;
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
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
efitime_t EventQueue::getNextEventTime(efitime_t nowX) {
	
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

static scheduling_s * longScheduling;
uint32_t maxEventQueueTime = 0;
uint32_t lastEventQueueTime;

/**
 * Invoke all pending actions prior to specified timestamp
 * @return number of executed actions
 */
int EventQueue::executeAll(efitime_t now) {
	scheduling_s * current, *tmp;

	scheduling_s * executionList = NULL;
	scheduling_s * lastInExecutionList = NULL;

	int listIterationCounter = 0;
	int executionCounter = 0;
	// we need safe iteration because we are removing elements inside the loop
	LL_FOREACH_SAFE(head, current, tmp)
	{
		efiAssert(current->callback != NULL, "callback==null1", 0);
		if (++listIterationCounter > QUEUE_LENGTH_LIMIT) {
			firmwareError(CUSTOM_LIST_LOOP, "Is this list looped?");
			return false;
		}
		if (current->momentX <= now) {
			executionCounter++;
			efiAssert(head == current, "removing from head", -1);
			//LL_DELETE(head, current);
			head = head->next;
			if (executionList == NULL) {
				lastInExecutionList = executionList = current;
			} else {
				lastInExecutionList->next = current;
				lastInExecutionList = current;
			}
			current->next = NULL;
		} else {
			/**
			 * The list is sorted. Once we find one action in the future, all the remaining ones
			 * are also in the future.
			 */
			break;
		}
	}
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	assertListIsSorted();
#endif

	/*
	 * we need safe iteration here because 'callback' might change change 'current->next'
	 * while re-inserting it into the queue from within the callback
	 */
	LL_FOREACH_SAFE(executionList, current, tmp)
	{
		efiAssert(current->callback != NULL, "callback==null2", 0);
		uint32_t before = GET_TIMESTAMP();
		current->isScheduled = false;
		int howFarOff = now - current->momentX;
		maxHowFarOff = maxI(maxHowFarOff, howFarOff);
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		printf("execute current=%d param=%d\r\n", (long)current, (long)current->param);
#endif
		current->callback(current->param);
		// even with overflow it's safe to subtract here
		lastEventQueueTime = GET_TIMESTAMP() - before;
		if (lastEventQueueTime > maxEventQueueTime)
			maxEventQueueTime = lastEventQueueTime;
		if (lastEventQueueTime > 2000) {
			longScheduling = current;
			lastEventQueueTime++;
		}
	}
	return executionCounter;
}

int EventQueue::size(void) {
	scheduling_s *tmp;
	int result;
	LL_COUNT(head, tmp, result);
	return result;
}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
void EventQueue::assertListIsSorted() {
	scheduling_s *current = head;
	while (current != NULL && current->next != NULL) {
		efiAssertVoid(current->momentX <= current->next->momentX, "list order");
		current = current->next;
	}
}
#endif

void EventQueue::setLateDelay(int value) {
	lateDelay = value;
}

scheduling_s * EventQueue::getHead() {
	return head;
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
