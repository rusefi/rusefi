#pragma once

#define TRIGGER_EVENT_UNDEFINED INT32_MAX

class TriggerScheduler : public EngineModule {
public:
	bool scheduleOrQueue(AngleBasedEvent *event,
			     uint32_t trgEventIndex,
			     efitick_t edgeTimestamp,
			     angle_t angle,
			     action_s action);

	void scheduleEventsUntilNextTriggerTooth(int rpm,
						 uint32_t trgEventIndex,
						 efitick_t edgeTimestamp);

	// For unit tests
	AngleBasedEvent * getElementAtIndexForUnitTest(int index);

private:
	bool assertNotInList(AngleBasedEvent *head, AngleBasedEvent *element);

	/**
	 * That's the linked list of pending events scheduled in relation to trigger
	 * At the moment we iterate over the whole list while looking for events for specific
	 * trigger index We can make it an array of lists per trigger index, but that would take
	 * some RAM and probably not needed yet.
	 */
	AngleBasedEvent *m_angleBasedEventsHead = nullptr;
};
