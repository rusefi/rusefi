#pragma once

class TriggerScheduler : public EngineModule {
public:
	void schedule(AngleBasedEventBase* event, angle_t angle, action_s action);

	bool scheduleOrQueue(AngleBasedEventNew *event,
			     efitick_t edgeTimestamp,
			     angle_t angle,
			     action_s action,
				 float currentPhase, float nextPhase);

	void scheduleEventsUntilNextTriggerTooth(int rpm,
						 uint32_t trgEventIndex,
						 efitick_t edgeTimestamp,
						 float currentPhase, float nextPhase);

	// For unit tests
	AngleBasedEventBase * getElementAtIndexForUnitTest(int index);

private:
	void schedule(AngleBasedEventBase* event, action_s action);

	bool assertNotInList(AngleBasedEventBase *head, AngleBasedEventBase *element);

	/**
	 * That's the linked list of pending events scheduled in relation to trigger
	 * At the moment we iterate over the whole list while looking for events for specific
	 * trigger index We can make it an array of lists per trigger index, but that would take
	 * some RAM and probably not needed yet.
	 */
	AngleBasedEventBase *m_angleBasedEventsHead = nullptr;
};
