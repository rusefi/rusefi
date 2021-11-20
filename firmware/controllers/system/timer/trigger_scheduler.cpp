#include "pch.h"

#include "event_queue.h"
#include "os_access.h"

bool TriggerScheduler::assertNotInList(AngleBasedEvent *head, AngleBasedEvent *element) {
       assertNotInListMethodBody(AngleBasedEvent, head, element, nextToothEvent)
}

/**
 * Schedules 'action' to occur at engine cycle angle 'angle'.
 *
 * If you know when a recent trigger occured, you can pass it in as 'trgEventIndex' and
 * 'edgeTimestamp'.  Otherwise pass in TRIGGER_EVENT_UNDEFINED and the work will be scheduled on
 * the next trigger edge.
 *
 * @return true if event corresponds to current tooth and was time-based scheduler
 *         false if event was put into queue for scheduling at a later tooth
 */
bool TriggerScheduler::scheduleOrQueue(AngleBasedEvent *event,
		uint32_t trgEventIndex,
		efitick_t edgeTimestamp,
		angle_t angle,
		action_s action) {
	event->position.setAngle(angle);

	/**
	 * Here's the status as of Jan 2020:
	 * Once we hit the last trigger tooth prior to needed event, schedule it by time.  We use
	 * as much trigger position angle as possible and only use less precise RPM-based time
	 * calculation for the last portion of the angle, the one between two teeth closest to the
	 * desired angle moment.
	 */
	if (trgEventIndex != TRIGGER_EVENT_UNDEFINED && event->position.triggerEventIndex == trgEventIndex) {
		/**
		 * Spark should be fired before the next trigger event - time-based delay is best precision possible
		 */
		scheduling_s * sDown = &event->scheduling;

		scheduleByAngle(
			sDown,
			edgeTimestamp,
			event->position.angleOffsetFromTriggerEvent,
			action
		);

		return true;
	} else {
		event->action = action;
		/**
		 * Spark should be scheduled in relation to some future trigger event, this way we get better firing precision
		 */
		{
			chibios_rt::CriticalSectionLocker csl;

			// TODO: This is O(n), consider some other way of detecting if in a list,
			// and consider doubly linked or other list tricks.

			if (!assertNotInList(m_angleBasedEventsHead, event)) {
				// Use Append to retain some semblance of event ordering in case of
				// time skew.  Thus on events are always followed by off events.
				LL_APPEND2(m_angleBasedEventsHead, event, nextToothEvent);

				return false;
			}
		}
#if SPARK_EXTREME_LOGGING
		efiPrintf("isPending thus not adding to queue index=%d rev=%d now=%d",
			  trgEventIndex, getRevolutionCounter(), (int)getTimeNowUs());
#endif /* SPARK_EXTREME_LOGGING */
		return false;
	}
}

void TriggerScheduler::scheduleEventsUntilNextTriggerTooth(int rpm,
							   uint32_t trgEventIndex,
							   efitick_t edgeTimestamp) {

	// Not sure why we check ignitionEnabled; it's left over from when this code lived in
	// spark_logic.cpp, but I think it should be removed.
	if (!isValidRpm(rpm) || !engineConfiguration->isIgnitionEnabled) {
		 // this might happen for instance in case of a single trigger event after a pause
		return;
	}

	AngleBasedEvent *current, *tmp, *keephead;
	AngleBasedEvent *keeptail = nullptr;

	{
		chibios_rt::CriticalSectionLocker csl;

		keephead =m_angleBasedEventsHead;
		m_angleBasedEventsHead = nullptr;
	}

	LL_FOREACH_SAFE2(keephead, current, tmp, nextToothEvent)
	{
		if (current->position.triggerEventIndex == trgEventIndex) {
			// time to fire a spark which was scheduled previously

			// Yes this looks like O(n^2), but that's only over the entire engine
			// cycle.  It's really O(mn + nn) where m = # of teeth and n = # events
			// fired per cycle.  The number of teeth outweigh the number of events, at
			// least for 60-2....  So odds are we're only firing an event or two per
			// tooth, which means the outer loop is really only O(n).  And if we are
			// firing many events per teeth, then it's likely the events before this
			// one also fired and thus the call to LL_DELETE2 is closer to O(1).
			LL_DELETE2(keephead, current, nextToothEvent);

			scheduling_s * sDown = &current->scheduling;

#if SPARK_EXTREME_LOGGING
			efiPrintf("time to invoke ind=%d %d %d",
				  trgEventIndex, getRevolutionCounter(), (int)getTimeNowUs());
#endif /* SPARK_EXTREME_LOGGING */

			// In case this event was scheduled by overdwell protection, cancel it so
			// we can re-schedule at the correct time
			engine->executor.cancel(sDown);

			scheduleByAngle(
				sDown,
				edgeTimestamp,
				current->position.angleOffsetFromTriggerEvent,
				current->action
			);
		} else {
			keeptail = current; // Used for fast list concatenation
		}
	}

	if (keephead) {
		chibios_rt::CriticalSectionLocker csl;

		// Put any new entries onto the end of the keep list
		keeptail->nextToothEvent = m_angleBasedEventsHead;
		m_angleBasedEventsHead = keephead;
	}
}

#if EFI_UNIT_TEST
// todo: reduce code duplication with another 'getElementAtIndexForUnitText'
AngleBasedEvent * TriggerScheduler::getElementAtIndexForUnitTest(int index) {
	AngleBasedEvent * current;

	LL_FOREACH2(m_angleBasedEventsHead, current, nextToothEvent)
	{
		if (index == 0)
			return current;
		index--;
	}
	firmwareError(OBD_PCM_Processor_Fault, "getElementAtIndexForUnitText: null");
	return nullptr;
}
#endif /* EFI_UNIT_TEST */
