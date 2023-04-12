#include "pch.h"

#include "event_queue.h"

bool TriggerScheduler::assertNotInList(AngleBasedEvent *head, AngleBasedEvent *element) {
       assertNotInListMethodBody(head, element, nextToothEvent)
}

void TriggerScheduler::schedule(AngleBasedEvent* event, angle_t angle, action_s action) {
	event->setAngle(angle);

	schedule(event, action);
}

/**
 * Schedules 'action' to occur at engine cycle angle 'angle'.
 *
 * @return true if event corresponds to current tooth and was time-based scheduler
 *         false if event was put into queue for scheduling at a later tooth
 */
bool TriggerScheduler::scheduleOrQueue(AngleBasedEvent *event,
		efitick_t edgeTimestamp,
		angle_t angle,
		action_s action,
		float currentPhase, float nextPhase) {
	event->setAngle(angle);

	if (event->shouldSchedule(currentPhase, nextPhase)) {
		// if we're due now, just schedule the event
		scheduleByAngle(
			&event->scheduling,
			edgeTimestamp,
			event->getAngleFromNow(currentPhase),
			action
		);

		return true;
	} else {
		// If not due now, add it to the queue to be scheduled later
		schedule(event, action);

		return false;
	}
}

void TriggerScheduler::schedule(AngleBasedEvent* event, action_s action) {
	event->action = action;

	{
		chibios_rt::CriticalSectionLocker csl;

		// TODO: This is O(n), consider some other way of detecting if in a list,
		// and consider doubly linked or other list tricks.

		if (!assertNotInList(m_angleBasedEventsHead, event)) {
			// Use Append to retain some semblance of event ordering in case of
			// time skew.  Thus on events are always followed by off events.
			LL_APPEND2(m_angleBasedEventsHead, event, nextToothEvent);
		}
	}
}

void TriggerScheduler::scheduleEventsUntilNextTriggerTooth(int rpm,
							   efitick_t edgeTimestamp, float currentPhase, float nextPhase) {

	if (!isValidRpm(rpm)) {
		 // this might happen for instance in case of a single trigger event after a pause
		return;
	}

	AngleBasedEvent *current, *tmp, *keephead;
	AngleBasedEvent *keeptail = nullptr;

	{
		chibios_rt::CriticalSectionLocker csl;

		keephead = m_angleBasedEventsHead;
		m_angleBasedEventsHead = nullptr;
	}

	LL_FOREACH_SAFE2(keephead, current, tmp, nextToothEvent)
	{
		if (current->shouldSchedule(currentPhase, nextPhase)) {
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
			efiPrintf("time to invoke [%.1f, %.1f) %d %d",
				  currentPhase, nextPhase, getRevolutionCounter(), (int)getTimeNowUs());
#endif /* SPARK_EXTREME_LOGGING */

			// In case this event was scheduled by overdwell protection, cancel it so
			// we can re-schedule at the correct time
			engine->executor.cancel(sDown);

			scheduleByAngle(
				sDown,
				edgeTimestamp,
				current->getAngleFromNow(currentPhase),
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

void AngleBasedEvent::setAngle(angle_t angle) {
	this->enginePhase = angle;
}

bool AngleBasedEvent::shouldSchedule(float currentPhase, float nextPhase) const {
	return isPhaseInRange(this->enginePhase, currentPhase, nextPhase);
}

float AngleBasedEvent::getAngleFromNow(float currentPhase) const {
	float angleOffset = this->enginePhase - currentPhase;
	if (angleOffset < 0) {
		angleOffset += engine->engineState.engineCycle;
	}

	return angleOffset;
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
	firmwareError(ObdCode::OBD_PCM_Processor_Fault, "getElementAtIndexForUnitText: null");
	return nullptr;
}
#endif /* EFI_UNIT_TEST */
