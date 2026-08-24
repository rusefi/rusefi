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

	// max minus min instant RPM seen during the most recent complete engine cycle
	// (720 deg for a four stroke engine): 0 if RPM is perfectly steady, 100 if RPM
	// went 2000..2100 within the cycle
	float getInstantRpmRange() const {
		return m_rpmRangeLastCycle;
	}

#if EFI_SHAFT_POSITION_INPUT
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
		m_cycleMinRpm = 0;
		m_cycleMaxRpm = 0;
		m_rpmRangeLastCycle = 0;
		m_lastRangeIndex = 0;
	}

	void offsetIndices(int indexOffset);

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

	// min/max instant RPM accumulated within the engine cycle currently in progress
	float m_cycleMinRpm = 0;
	float m_cycleMaxRpm = 0;
	// see getInstantRpmRange()
	float m_rpmRangeLastCycle = 0;
	// previous tooth index, used to detect the start of a new engine cycle
	uint32_t m_lastRangeIndex = 0;
};
