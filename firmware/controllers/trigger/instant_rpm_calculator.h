/**
 * instant_rpm_calculator.h
 */

#pragma once
#include "trigger_structure.h"

class InstantRpmCalculator {
public:
	InstantRpmCalculator();
	float getInstantRpm() const {
		return m_instantRpm;
	}

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	void updateInstantRpm(
			uint32_t current_index,
		TriggerWaveform const & triggerShape, TriggerFormDetails *triggerFormDetails,
		uint32_t index, efitick_t nowNt);
#endif
	/**
	 * Update timeOfLastEvent[] on every trigger event - even without synchronization
	 * Needed for early spin-up RPM detection.
	 */
	void setLastEventTimeForInstantRpm(efitick_t nowNt);

	void movePreSynchTimestamps();

	void resetInstantRpm() {
		setArrayValues(timeOfLastEvent, 0);
		setArrayValues(spinningEvents, 0);
		spinningEventIndex = 0;
		prevInstantRpmValue = 0;
		m_instantRpm = 0;
	}

	/**
	 * timestamp of each trigger wheel tooth
	 */
	uint32_t timeOfLastEvent[PWM_PHASE_MAX_COUNT];

	size_t spinningEventIndex = 0;

	// we might need up to one full trigger cycle of events - which on 60-2 means storage for ~120
	// todo: change the implementation to reuse 'timeOfLastEvent'
	uint32_t spinningEvents[120];
	/**
	 * instant RPM calculated at this trigger wheel tooth
	 */
	float instantRpmValue[PWM_PHASE_MAX_COUNT];
	/**
	 * Stores last non-zero instant RPM value to fix early instability
	 */
	float prevInstantRpmValue = 0;


	float m_instantRpm = 0;
private:
	float calculateInstantRpm(
		TriggerWaveform const & triggerShape, TriggerFormDetails *triggerFormDetails,
		uint32_t index, efitick_t nowNt);

	float m_instantRpmRatio = 0;
};
