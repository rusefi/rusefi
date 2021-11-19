/**
 * @file	trigger_decoder.h
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "trigger_structure.h"
#include "engine_configuration.h"
#include "trigger_state_generated.h"
#include "timer.h"

class TriggerState;

struct TriggerStateListener {
#if EFI_SHAFT_POSITION_INPUT
	virtual void OnTriggerStateProperState(efitick_t nowNt) = 0;
	virtual void OnTriggerSyncronization(bool wasSynchronized) = 0;
	virtual void OnTriggerInvalidIndex(int currentIndex) = 0;
	virtual void OnTriggerSynchronizationLost() = 0;
#endif // EFI_SHAFT_POSITION_INPUT
};

class TriggerConfiguration {
public:
	explicit TriggerConfiguration(const char* printPrefix) : PrintPrefix(printPrefix) {}
	void update();

	const char* const PrintPrefix;
	bool UseOnlyRisingEdgeForTrigger;
	bool VerboseTriggerSynchDetails;
	trigger_type_e TriggerType;

protected:
	virtual bool isUseOnlyRisingEdgeForTrigger() const = 0;
	virtual bool isVerboseTriggerSynchDetails() const = 0;
	virtual trigger_type_e getType() const = 0;
};

typedef void (*TriggerStateCallback)(TriggerState *);

typedef struct {
	/**
	 * index within trigger revolution, from 0 to trigger event count
	 */
	uint32_t current_index;
	/**
	 * Number of actual events of each channel within current trigger cycle, these
	 * values are used to detect trigger signal errors.
	 * see TriggerWaveform
	 */
	size_t eventCount[PWM_PHASE_MAX_WAVE_PER_PWM];
	/**
	 * This array is used to calculate duty cycle of each trigger channel.
	 * Current implementation is a bit funny - it does not really consider if an event
	 * is a rise or a fall, it works based on the event order within synchronization cycle.
	 *
	 * 32 bit value is good enough here, overflows will happen but they would work just fine.
	 */
	uint32_t timeOfPreviousEventNt[PWM_PHASE_MAX_WAVE_PER_PWM];
	/**
	 * Here we accumulate the amount of time this signal was ON within current trigger cycle
	 */
	uint32_t totalTimeNt[PWM_PHASE_MAX_WAVE_PER_PWM];

#if EFI_UNIT_TEST
	uint32_t totalTimeNtCopy[PWM_PHASE_MAX_WAVE_PER_PWM];
#endif // EFI_UNIT_TEST
} current_cycle_state_s;

/**
 * @see TriggerWaveform for trigger wheel shape definition
 */
class TriggerState : public trigger_state_s {
public:
	TriggerState();
	/**
	 * current trigger processing index, between zero and #size
	 */
	int getCurrentIndex() const;
	int getTotalRevolutionCounter() const;
	/**
	 * this is important for crank-based virtual trigger and VVT magic
	 */
	void incrementTotalEventCounter();
	angle_t syncSymmetricalCrank(int divider, int remainder, angle_t engineCycle);

	efitime_t getTotalEventCounter() const;

	void decodeTriggerEvent(
			const TriggerWaveform& triggerShape,
			const TriggerStateCallback triggerCycleCallback,
			TriggerStateListener* triggerStateListener,
			const TriggerConfiguration& triggerConfiguration,
			const trigger_event_e signal,
			const efitime_t nowUs);

	bool validateEventCounters(const TriggerWaveform& triggerShape) const;
	void onShaftSynchronization(
			const TriggerStateCallback triggerCycleCallback,
			const efitick_t nowNt,
			const TriggerWaveform& triggerShape);

	bool isValidIndex(const TriggerWaveform& triggerShape) const;

	/**
	 * TRUE if we know where we are
	 */
	bool shaft_is_synchronized;
	efitick_t mostRecentSyncTime;

	Timer previousEventTimer;

	void setTriggerErrorState();

	efitick_t lastDecodingErrorTime;
	// the boolean flag is a performance optimization so that complex comparison is avoided if no error
	bool someSortOfTriggerError;

	/**
	 * current duration at index zero and previous durations are following
	 */
	uint32_t toothDurations[GAP_TRACKING_LENGTH + 1];

	efitick_t toothed_previous_time;

	current_cycle_state_s currentCycle;
	const char *name = nullptr;

	int expectedTotalTime[PWM_PHASE_MAX_WAVE_PER_PWM];

	/**
	 * how many times since ECU reboot we had unexpected number of teeth in trigger cycle
	 */
	uint32_t totalTriggerErrorCounter;
	uint32_t orderingErrorCounter;

	void resetTriggerState();
	void setShaftSynchronized(bool value);
	bool getShaftSynchronized();

	/**
	 * this is start of real trigger cycle
	 * for virtual double trigger see timeAtVirtualZeroNt
	 */
	efitick_t startOfCycleNt;

	uint32_t findTriggerZeroEventIndex(
			TriggerWaveform& shape,
			const TriggerConfiguration& triggerConfiguration,
			const trigger_config_s& triggerConfig
			);

private:
	void resetCurrentCycleState();

	trigger_event_e curSignal;
	trigger_event_e prevSignal;
	int64_t totalEventCountBase;
	uint32_t totalRevolutionCounter;
	bool isFirstEvent;
};

// we only need 90 degrees of events so /4 or maybe even /8 should work?
#define PRE_SYNC_EVENTS (PWM_PHASE_MAX_COUNT / 4)


/**
 * the reason for sub-class is simply to save RAM but not having statistics in the trigger initialization instance
 */
class TriggerStateWithRunningStatistics : public TriggerState {
public:
	TriggerStateWithRunningStatistics();

	float getInstantRpm() const {
		return m_instantRpm;
	}

	/**
	 * timestamp of each trigger wheel tooth
	 */
	uint32_t timeOfLastEvent[PWM_PHASE_MAX_COUNT];

	int spinningEventIndex = 0;
	// todo: change the implementation to reuse 'timeOfLastEvent'
	uint32_t spinningEvents[PRE_SYNC_EVENTS];
	/**
	 * instant RPM calculated at this trigger wheel tooth
	 */
	float instantRpmValue[PWM_PHASE_MAX_COUNT];
	/**
	 * Stores last non-zero instant RPM value to fix early instability
	 */
	float prevInstantRpmValue = 0;
	void movePreSynchTimestamps();

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	void updateInstantRpm(
		TriggerWaveform const & triggerShape, TriggerFormDetails *triggerFormDetails,
		uint32_t index, efitick_t nowNt);
#endif
	/**
	 * Update timeOfLastEvent[] on every trigger event - even without synchronization
	 * Needed for early spin-up RPM detection.
	 */
	void setLastEventTimeForInstantRpm(efitick_t nowNt);

private:
	float calculateInstantRpm(
		TriggerWaveform const & triggerShape, TriggerFormDetails *triggerFormDetails,
		uint32_t index, efitick_t nowNt);

	float m_instantRpm = 0;
	float m_instantRpmRatio = 0;

};

angle_t getEngineCycle(operation_mode_e operationMode);

class Engine;

void calculateTriggerSynchPoint(
	TriggerWaveform& shape,
	TriggerState& state);

void prepareEventAngles(TriggerWaveform *shape, TriggerFormDetails *details);
