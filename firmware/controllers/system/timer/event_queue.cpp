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

#ifndef EFI_UNIT_TEST_VERBOSE_ACTION
#define EFI_UNIT_TEST_VERBOSE_ACTION 0
#elif EFI_UNIT_TEST_VERBOSE_ACTION
#include <iostream>
#endif

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
		m_freelist = retVal->next;
		retVal->next = nullptr;

#if EFI_PROD_CODE
		getTunerStudioOutputChannels()->schedulingUsedCount++;
#endif
	}

	return retVal;
}

void EventQueue::tryReturnScheduling(scheduling_s* sched) {
	// Only return this scheduling to the free list if it's from the correct pool
	if (sched >= &m_pool[0] && sched <= &m_pool[efi::size(m_pool) - 1]) {
		sched->next = m_freelist;
		m_freelist = sched;

#if EFI_PROD_CODE
		getTunerStudioOutputChannels()->schedulingUsedCount--;
#endif
	}
}

/**
 * @return true if inserted into the head of the list
 */
bool EventQueue::insertTask(scheduling_s *scheduling, efitick_t timeNt, action_s const& action) {
	ScopePerf perf(PE::EventQueueInsertTask);

	if (!scheduling) {
		scheduling = getFreeScheduling();

		// If still null, the free list is empty and all schedulings in the pool have been expended.
		if (!scheduling) {
			// TODO: should we warn or error here?
// todo: look into why units tests fail here
#if EFI_PROD_CODE
      criticalError("No slots in scheduling pool");
#endif
			return false;
		}
	}

	assertListIsSorted();
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, action.getCallback() != nullptr, "NULL callback", false);

// please note that simulator does not use this code at all - simulator uses signal_executor_sleep

	if (scheduling->action) {
#if EFI_UNIT_TEST
		if (verboseMode) {
			printf("Already scheduled was %d\r\n", (int)scheduling->getMomentNt());
			printf("Already scheduled now %d\r\n", (int)timeNt);
		}
#endif /* EFI_UNIT_TEST */
		return false;
	}

	scheduling->setMomentNt(timeNt);
	scheduling->action = action;

	if (!m_head || timeNt < m_head->getMomentNt()) {
		// here we insert into head of the linked list
		LL_PREPEND(m_head, scheduling);
		assertListIsSorted();
		return true;
	} else {
		// here we know we are not in the head of the list, let's find the position - linear search
		scheduling_s *insertPosition = m_head;
		while (insertPosition->next != nullptr && insertPosition->next->getMomentNt() < timeNt) {
			insertPosition = insertPosition->next;
		}

		scheduling->next = insertPosition->next;
		insertPosition->next = scheduling;
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
		m_head = m_head->next;
		scheduling->next = nullptr;
		scheduling->action = {};
	} else {
		auto prev = m_head;	// keep track of the element before the one to remove, so we can link around it
		auto current = prev->next;

		// Find our element
		while (current && current != scheduling) {
			prev = current;
			current = current->next;
		}

		// Walked off the end, this is an error since this *should* have been scheduled
		if (!current) {
			firmwareError(ObdCode::OBD_PCM_Processor_Fault, "EventQueue::remove didn't find element");
			return;
		}

		efiAssertVoid(ObdCode::OBD_PCM_Processor_Fault, current == scheduling, "current not equal to scheduling");

		// Link around the removed item
		prev->next = current->next;

		// Clean the item to remove
		current->next = nullptr;
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

scheduling_s* EventQueue::pickOne(efitick_t now) {
	// Read the head every time - a previously executed event could
	// have inserted something new at the head
	scheduling_s* current = m_head;

	// Queue is empty - bail
	if (!current) {
		return nullptr;
	}

	// If the next event is far in the future, we'll reschedule
	// and execute it next time.
	// We do this when the next event is close enough that the overhead of
	// resetting the timer and scheduling an new interrupt is greater than just
	// waiting for the time to arrive.  On current CPUs, this is reasonable to set
	// around 10 microseconds.
	if (current->getMomentNt() > now + m_lateDelay) {
		return nullptr;
	}

	// step the head forward, unlink this element
	// TODO: LL_DELETE(m_head, current);
	m_head = current->next;
	current->next = nullptr;

	return current;
}

void EventQueue::executeAndFree(scheduling_s* current) {
	// Grab the action but clear it in the event so we can reschedule from the action's execution
	auto const action{ std::move(current->action) };

#if EFI_DETAILED_LOGGING
	printf("QUEUE: execute current=%d param=%d\r\n", reinterpret_cast<uintptr_t>(current), action.getArgumentRaw());
#endif

	tryReturnScheduling(current);

	// Execute the current element
	{
		ScopePerf perf2(PE::EventQueueExecuteCallback);
#if EFI_DETAILED_LOGGING && EFI_UNIT_TEST_VERBOSE_ACTION
		std::cout << "EventQueue::executeAndFree: " << action.getCallbackName() << "(" << reinterpret_cast<uintptr_t>(action.getCallback()) << ") with raw arg = " << action.getArgumentRaw() << std::endl;
#endif
		action.execute();

#if EFI_UNIT_TEST
		// std::cout << "Executed at " << now << std::endl;
#endif
	}

	assertListIsSorted();
}

bool EventQueue::executeOne(efitick_t now) {
	scheduling_s* current = pickOne(now);

	if (!current) {
		return false;
	}

	// now it is time to execute
	executeAndFree(current);

	return true;
}

int EventQueue::size() const {
	scheduling_s *tmp;
	int result;
	LL_COUNT(m_head, tmp, result);
	return result;
}

void EventQueue::assertListIsSorted() const {
#if EFI_UNIT_TEST || EFI_SIMULATOR
	int counter = 0;
	scheduling_s *current = m_head;
	while (current != nullptr && current->next != nullptr) {
		efiAssertVoid(ObdCode::CUSTOM_ERR_6623, current->getMomentNt() <= current->next->getMomentNt(), "list order");
		current = current->next;
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

	LL_FOREACH(m_head, current)
	{
		if (index == 0) {
			return current;
		}
		index--;
	}

	return nullptr;
}

void EventQueue::clear() {
	// Flush the queue, resetting all scheduling_s as though we'd executed them
	while(m_head) {
		auto x = m_head;
		// link next element to head
		m_head = x->next;

		// Reset this element
		x->setMomentNt(0);
		x->next = nullptr;
		x->action = {};
	}

	m_head = nullptr;
}
