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

#include "pch.h"

#include "event_queue.h"
#include "efitime.h"

#if EFI_UNIT_TEST
extern bool verboseMode;
#endif /* EFI_UNIT_TEST */

EventQueue::EventQueue(efidur_t lateDelay)
	: m_lateDelay(lateDelay)
{
	for (size_t i = 0; i < efi::size(m_pool); i++) {
		tryReturnScheduling(&m_pool[i]);
	}

#if EFI_PROD_CODE
	getTunerStudioOutputChannels()->schedulingUsedCount = 0;
#endif
}

scheduling_s* EventQueue::getFreeScheduling() {
	auto retVal = m_freelist;

	if (retVal) {
		m_freelist = retVal->nextScheduling_s;
		retVal->nextScheduling_s = nullptr;

#if EFI_PROD_CODE
		getTunerStudioOutputChannels()->schedulingUsedCount++;
#endif
	}

	return retVal;
}

void EventQueue::tryReturnScheduling(scheduling_s* sched) {
	// Only return this scheduling to the free list if it's from the correct pool
	if (sched >= &m_pool[0] && sched <= &m_pool[efi::size(m_pool) - 1]) {
		sched->nextScheduling_s = m_freelist;
		m_freelist = sched;

#if EFI_PROD_CODE
		getTunerStudioOutputChannels()->schedulingUsedCount--;
#endif
	}
}

/**
 * @return true if inserted into the head of the list
 */
bool EventQueue::insertTask(scheduling_s *scheduling, efitick_t timeX, action_s action) {
	ScopePerf perf(PE::EventQueueInsertTask);

	if (!scheduling) {
		scheduling = getFreeScheduling();

		// If still null, the free list is empty and all schedulings in the pool have been expended.
		if (!scheduling) {
			// TODO: should we warn or error here?

			return false;
		}
	}

	assertListIsSorted();
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, action.getCallback() != NULL, "NULL callback", false);

// please note that simulator does not use this code at all - simulator uses signal_executor_sleep

	if (scheduling->action) {
#if EFI_UNIT_TEST
		if (verboseMode) {
			printf("Already scheduled was %d\r\n", (int)scheduling->getMomentRaw());
			printf("Already scheduled now %d\r\n", (int)timeX);
		}
#endif /* EFI_UNIT_TEST */
		return false;
	}

	scheduling->setMomentX(timeX);
	scheduling->action = action;

	if (!m_head || timeX < m_head->getMomentNt()) {
		// here we insert into head of the linked list
		LL_PREPEND2(m_head, scheduling, nextScheduling_s);
		assertListIsSorted();
		return true;
	} else {
		// here we know we are not in the head of the list, let's find the position - linear search
		scheduling_s *insertPosition = m_head;
		while (insertPosition->nextScheduling_s != NULL && insertPosition->nextScheduling_s->getMomentNt() < timeX) {
			insertPosition = insertPosition->nextScheduling_s;
		}

		scheduling->nextScheduling_s = insertPosition->nextScheduling_s;
		insertPosition->nextScheduling_s = scheduling;
		assertListIsSorted();
		return false;
	}
}

void EventQueue::remove(scheduling_s* scheduling) {
	assertListIsSorted();

	// Special case: event isn't scheduled, so don't cancel it
	if (!scheduling->action) {
		return;
	}

	// Special case: empty list, nothing to do
	if (!m_head) {
		return;
	}

	// Special case: is the item to remove at the head?
	if (scheduling == m_head) {
		m_head = m_head->nextScheduling_s;
		scheduling->nextScheduling_s = nullptr;
		scheduling->action = {};
	} else {
		auto prev = m_head;	// keep track of the element before the one to remove, so we can link around it
		auto current = prev->nextScheduling_s;

		// Find our element
		while (current && current != scheduling) {
			prev = current;
			current = current->nextScheduling_s;
		}

		// Walked off the end, this is an error since this *should* have been scheduled
		if (!current) {
			firmwareError(ObdCode::OBD_PCM_Processor_Fault, "EventQueue::remove didn't find element");
			return;
		}

		efiAssertVoid(ObdCode::OBD_PCM_Processor_Fault, current == scheduling, "current not equal to scheduling");

		// Link around the removed item
		prev->nextScheduling_s = current->nextScheduling_s;

		// Clean the item to remove
		current->nextScheduling_s = nullptr;
		current->action = {};
	}

	assertListIsSorted();
}

/**
 * On this layer it does not matter which units are used - us, ms ot nt.
 *
 * This method is always invoked under a lock
 * @return Get the timestamp of the soonest pending action, skipping all the actions in the past
 */
expected<efitick_t> EventQueue::getNextEventTime(efitick_t nowNt) const {
	if (m_head) {
		if (m_head->getMomentNt() <= nowNt) {
			/**
			 * We are here if action timestamp is in the past. We should rarely be here since this 'getNextEventTime()' is
			 * always invoked by 'scheduleTimerCallback' which is always invoked right after 'executeAllPendingActions' - but still,
			 * for events which are really close to each other we would end up here.
			 *
			 * looks like we end up here after 'writeconfig' (which freezes the firmware) - we are late
			 * for the next scheduled event
			 */
			return nowNt + m_lateDelay;
		} else {
			return m_head->getMomentNt();
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
int EventQueue::executeAll(efitick_t now) {
	ScopePerf perf(PE::EventQueueExecuteAll);

	int executionCounter = 0;

	assertListIsSorted();

	bool didExecute;
	do {
		didExecute = executeOne(now);
		executionCounter += didExecute ? 1 : 0;
	} while (didExecute);

	return executionCounter;
}

bool EventQueue::executeOne(efitick_t now) {
	// Read the head every time - a previously executed event could
	// have inserted something new at the head
	scheduling_s* current = m_head;

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
	if (current->getMomentNt() > now + m_lateDelay) {
		return false;
	}

#if EFI_UNIT_TEST
//	efitick_t spinDuration = current->getMomentNt() - getTimeNowNt();
//	if (spinDuration > 0) {
//		throw std::runtime_error("Time Spin in unit test");
//	}
#endif

	// near future - spin wait for the event to happen and avoid the
	// overhead of rescheduling the timer.
	// yes, that's a busy wait but that's what we need here
	while (current->getMomentNt() > getTimeNowNt()) {
#if EFI_UNIT_TEST
  // todo: remove this hack see https://github.com/rusefi/rusefi/issues/6457
extern bool unitTestBusyWaitHack;
    if (unitTestBusyWaitHack) {
	    break;
	  }
#endif
		UNIT_TEST_BUSY_WAIT_CALLBACK();
	}

	// step the head forward, unlink this element, clear scheduled flag
	m_head = current->nextScheduling_s;
	current->nextScheduling_s = nullptr;

	// Grab the action but clear it in the event so we can reschedule from the action's execution
	auto action = current->action;
	current->action = {};

	tryReturnScheduling(current);
	current = nullptr;

#if EFI_DEFAILED_LOGGING
	printf("QUEUE: execute current=%d param=%d\r\n", (uintptr_t)current, (uintptr_t)action.getArgument());
#endif

	// Execute the current element
	{
		ScopePerf perf2(PE::EventQueueExecuteCallback);
		action.execute();
	}

	assertListIsSorted();
	return true;
}

int EventQueue::size(void) const {
	scheduling_s *tmp;
	int result;
	LL_COUNT2(m_head, tmp, result, nextScheduling_s);
	return result;
}

void EventQueue::assertListIsSorted() const {
#if EFI_UNIT_TEST || EFI_SIMULATOR
	int counter = 0;
	scheduling_s *current = m_head;
	while (current != NULL && current->nextScheduling_s != NULL) {
		efiAssertVoid(ObdCode::CUSTOM_ERR_6623, current->getMomentNt() <= current->nextScheduling_s->getMomentNt(), "list order");
		current = current->nextScheduling_s;
		if (counter++ > 1'000'000'000)
			criticalError("EventQueue: looks like a loop?!");
	}
#endif // EFI_UNIT_TEST || EFI_SIMULATOR
}

scheduling_s * EventQueue::getHead() {
	return m_head;
}

// todo: reduce code duplication with another 'getElementAtIndexForUnitText'
scheduling_s *EventQueue::getElementAtIndexForUnitText(int index) {
	scheduling_s * current;

	LL_FOREACH2(m_head, current, nextScheduling_s)
	{
		if (index == 0)
			return current;
		index--;
	}

	return NULL;
}

void EventQueue::clear(void) {
	// Flush the queue, resetting all scheduling_s as though we'd executed them
	while(m_head) {
		auto x = m_head;
		// link next element to head
		m_head = x->nextScheduling_s;

		// Reset this element
		x->setMomentX(0);
		x->nextScheduling_s = nullptr;
		x->action = {};
	}

	m_head = nullptr;
}
