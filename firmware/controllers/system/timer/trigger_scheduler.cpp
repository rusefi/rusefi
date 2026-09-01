#include "pch.h"

#include "event_queue.h"
#include "angle_clock.h"
#include "trigger_central.h"

bool TriggerScheduler::assertNotInList(AngleBasedEvent *head, AngleBasedEvent *element) {
	/* this code is just to validate state, no functional load*/
	decltype(head) current;
	int counter = 0;
	LL_FOREACH2(head, current, nextToothEvent) {
		if (++counter > QUEUE_LENGTH_LIMIT) {
			firmwareError(ObdCode::CUSTOM_ERR_LOOPED_QUEUE, "Looped queue?");
			return false;
		}

		if (current == element) {
			/**
			 * for example, this might happen in case of sudden RPM change if event
			 * was not scheduled by angle but was scheduled by time. In case of scheduling
			 * by time with slow RPM the whole next fast revolution might be within the wait
			 */
			warning(ObdCode::CUSTOM_RE_ADDING_INTO_EXECUTION_QUEUE, "re-adding element into event_queue");
			return true;
		}
	}

	return false;
}

TRIGGER_RAM_CODE void TriggerScheduler::schedule(const char *msg, AngleBasedEvent* event, angle_t angle, action_s const& action) {
	event->setAngle(angle);

	schedule(msg, event, action);
}

/**
 * Schedules 'action' to occur at engine cycle angle 'angle'.
 *
 * @return true if event corresponds to current tooth and was time-based scheduler
 *         false if event was put into queue for scheduling at a later tooth
 */
TRIGGER_RAM_CODE bool TriggerScheduler::scheduleOrQueue(const char *msg, AngleBasedEvent *event,
		efitick_t edgeTimestamp,
		angle_t angle,
		action_s action,
		float currentPhase, float nextPhase) {
	event->setAngle(angle);

    // *kludge* naming mess: if (shouldSchedule) { scheduleByAngle } else { schedule } see header for more details
	if (event->shouldSchedule(currentPhase, nextPhase)) {
		// if we're due now, just schedule the event
		scheduleByAngle(
			&event->eventScheduling,
			edgeTimestamp,
			event->getAngleFromNow(currentPhase),
			action
		);

		return true;
	} else {
		// If not due now, add it to the queue to be scheduled later
		schedule(msg, event, action);

		return false;
	}
}

TRIGGER_RAM_CODE void TriggerScheduler::schedule(const char *msg, AngleBasedEvent* event, action_s const& action) {
	if (event->getAngle() < 0) {
	    // at the moment we expect API consumer to wrap angle. shall we do the wrapping in the enginePhase setter?
	    // i.e. what is the best level to take care of the range constraint?
		criticalError("Negative angle %s %f", msg, event->getAngle());
	}

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

void TriggerScheduler::cancel(AngleBasedEvent* event) {
	chibios_rt::CriticalSectionLocker csl;

	LL_DELETE2(m_angleBasedEventsHead, event, nextToothEvent);

#if EFI_ANGLE_CLOCK
	// The event may already be armed on the hardware angle clock (armed one
	// tooth ahead and removed from this queue). Kill the TMR4 spark channel
	// directly by cylinder index - O(1) and no action-pointer search needed.
	if (event->cylinderIndex >= 0) {
		angleClockCancelSpark(event->cylinderIndex);
	}
#endif // EFI_ANGLE_CLOCK
}

TRIGGER_RAM_CODE void TriggerScheduler::scheduleEventsUntilNextTriggerTooth(float rpm,
							   efitick_t edgeTimestamp, float currentPhase, float nextPhase,
							   float nextNextPhase) {
#if !EFI_ANGLE_CLOCK
	UNUSED(nextNextPhase);
#endif // !EFI_ANGLE_CLOCK

	if (rpm == 0) {
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

			scheduling_s * sDown = &current->eventScheduling;

#if SPARK_EXTREME_LOGGING
			efiPrintf("time to invoke [%.1f, %.1f) %d %d",
				  currentPhase, nextPhase, getRevolutionCounter(), time2print(getTimeNowUs()));
#endif /* SPARK_EXTREME_LOGGING */

			// In case this event was scheduled by overdwell protection, cancel it so
			// we can re-schedule at the correct time
			// [tag:overdwell]
			engine->scheduler.cancel(sDown);

#if EFI_ANGLE_CLOCK
			// Current-tooth spark: try TMR4 first. With scheduleEventsUntilNextTriggerTooth
			// now running at ~8 µs elapsed (before scheduleDwellEarlyIfDue), the arm
			// succeeds for remaining > 0.5° at 7000 rpm. Only sparks right at the
			// tooth edge (remaining < 0.5°) still fall back to TIM5.
			if (!angleClockArmSpark(current->cylinderIndex, current->getAngle(),
								current->action, currentPhase, nextPhase)) {
				scheduleByAngle(sDown, edgeTimestamp,
							  current->getAngleFromNow(currentPhase), current->action);
			}
#else
			scheduleByAngle(
				sDown,
				edgeTimestamp,
				current->getAngleFromNow(currentPhase),
				current->action
			);
#endif // EFI_ANGLE_CLOCK
#if EFI_ANGLE_CLOCK
	} else if (nextNextPhase != nextPhase && current->shouldSchedule(nextPhase, nextNextPhase)) {
		// Due during the NEXT tooth: arm it on the hardware angle clock
		// one full tooth early, so a late handoff (decode tails up to
		// ~1 ms) cannot shift it. The nextNextPhase != nextPhase guard
		// rejects single-tooth triggers, where every event angle maps to
		// the same phase and the range test is meaningless.
		LL_DELETE2(keephead, current, nextToothEvent);

		scheduling_s * sDown = &current->eventScheduling;

		// The overdwell rescue (armed on sDown by scheduleSparkEvent) is
		// KEPT on the success path: it stays the coil safety net if this
		// armed fire is lost (stale target, channels busy, desync refresh
		// cancels it). If the angle-clock fire executes first, the rescue
		// later no-ops on IgnitionEvent::sparkFiredSinceCharge; if the arm
		// is lost, the rescue discharges the coil at 1.5x dwell and cancels
		// the armed compare via TriggerScheduler::cancel.
		float angleFromNow = current->getAngleFromNow(currentPhase);

		// Arm in the ANGLE domain with the freshest tooth data - the
		// per-tooth refresh re-anchors the tick every tooth, so the error
		// never exceeds one tooth of acceleration at ANY rpm (the 90-degree
		// rpm average lags by revolutions at the catch and fired events
		// ms-late - the fuse incident).
		if (!angleClockArmSpark(current->cylinderIndex, current->getAngle(), current->action, currentPhase, nextPhase)) {
			// Arm failed (stale target, tick passed, or all channels busy):
			// fall back to a time-based fire on the SAME basis the proven
			// time-based path uses (oneDegreeUs, the 90-degree-window rpm
			// average). The eventScheduling struct is free here (the overdwell
			// rescue lives on the event's dwellStartTimer, armed at the actual
			// charge); the rescue still bounds the charge if this fire loses
			// the race against it (see overFireSparkAndPrepareNextSchedule).
			engine->scheduler.schedule("fire", sDown, sumTickAndFloat(edgeTimestamp, angleFromNow * US2NT(engine->rpmCalculator.oneDegreeUs)), current->action);
		}
#endif // EFI_ANGLE_CLOCK
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
	criticalError("getElementAtIndexForUnitText: null");
	return nullptr;
}
#endif /* EFI_UNIT_TEST */
