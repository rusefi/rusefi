#pragma once

// TriggerScheduler here is an intermediate tooth-based scheduler working on top of time-base scheduler
// *kludge*: individual event for *Trigger*Scheduler is called *Angle*BasedEvent. Shall we rename to ToothSchedule and ToothBasedEvent?
class TriggerScheduler : public EngineModule {
public:
    // *kludge* we have three methods with *schedule* in the name meaning three different things
    // this method just places event into the collection of tooth-based events
	void schedule(const char *msg, AngleBasedEvent* event, angle_t angle, action_s action);

    // 'schedule' means 'delegates to time-based scheduler' and 'queue' here matches the 'schedule' method above
	bool scheduleOrQueue(const char *msg, AngleBasedEvent *event,
			     efitick_t edgeTimestamp,
			     angle_t angle,
			     action_s action,
				 float currentPhase, float nextPhase);

    // scheduleForActualTimeBasedExecution using underlying time-base scheduler
	void scheduleEventsUntilNextTriggerTooth(float rpm,
						 efitick_t edgeTimestamp,
						 float currentPhase, float nextPhase);

#if EFI_UNIT_TEST
	AngleBasedEvent * getElementAtIndexForUnitTest(int index);
#endif // EFI_UNIT_TEST

private:
	void schedule(const char *msg, AngleBasedEvent* event, action_s action);

	bool assertNotInList(AngleBasedEvent *head, AngleBasedEvent *element);

	/**
	 * That's the linked list of pending events scheduled in relation to trigger
	 * At the moment we iterate over the whole list while looking for events for specific
	 * trigger index We can make it an array of lists per trigger index, but that would take
	 * some RAM and probably not needed yet.
	 */
	AngleBasedEvent *m_angleBasedEventsHead = nullptr;
};
