/**
 * @file	scheduler.h
 *
 * @date October 1, 2020
 */
#include "scheduler.h"

void action_s::execute() {
	efiAssertVoid(CUSTOM_ERR_ASSERT, callback != NULL, "callback==null1");
	callback(param);
}

schfunc_t action_s::getCallback() const {
	return callback;
}

void * action_s::getArgument() const {
	return param;
}

scheduling_pool::scheduling_pool() {
	// Simply call release on the whole pool to add it to the linked list
	for (size_t i = 0; i < efi::size(m_pool); i++) {
		release(&m_pool[i]);
	}
	
	m_inUseCount = 0;
}

scheduling_s* scheduling_pool::get() {
	auto retVal = m_next;

	if (retVal) {
		m_next = retVal->nextScheduling_s;
		retVal->nextScheduling_s = nullptr;
	}

	m_inUseCount++;

	return retVal;
}

void scheduling_pool::release(scheduling_s* element) {
	// Link the element in to the head of the list
	element->nextScheduling_s = m_next;
	m_next = element;
	m_inUseCount--;
}
