/**
 * @file event_queue.cpp
 *
 * @date Apr 17, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "event_queue.h"
#include "efitime.h"
#include "utlist.h"

EventQueue::EventQueue() {
	head = NULL;
}

void EventQueue::schedule(scheduling_s *scheduling, uint64_t nowUs, int delayUs,
		schfunc_t callback, void *param) {
	if (callback == NULL)
		firmwareError("NULL callback");
	uint64_t time = nowUs + delayUs;

	scheduling->momentUs = time;
#if EFI_SIGNAL_EXECUTOR_ONE_TIMER
	scheduling->callback = callback;
	scheduling->param = param;
#endif

	scheduling_s * elt;
	LL_FOREACH(head, elt)
	{
		if (elt == scheduling) {
			firmwareError("re-adding element");
			return;
		}
	}

	LL_PREPEND(head, scheduling);
}

void EventQueue::schedule(scheduling_s *scheduling, int delayUs,
		schfunc_t callback, void *param) {
	schedule(scheduling, getTimeNowUs(), delayUs, callback, param);
}

uint64_t EventQueue::getNextEventTime(void) {
	scheduling_s * elt;
	// this is a large value which is expected to be larger than any real time
	uint64_t result = EMPTY_QUEUE;

	LL_FOREACH(head, elt)
	{
		if (elt->momentUs < result)
			result = elt->momentUs;

	}
	return result;
}

void EventQueue::execute(uint64_t now) {
	scheduling_s * elt, *tmp;

//	DL_FOREACH_SAFE()

// here we need safe iteration because we are removing elements
	LL_FOREACH_SAFE(head, elt, tmp)
	{
		if (elt->momentUs < now) {
			LL_DELETE(head, elt);
#if EFI_SIGNAL_EXECUTOR_ONE_TIMER
			elt->callback(elt->param);
#endif /* EFI_SIGNAL_EXECUTOR_ONE_TIMER */
		}
	}
}
