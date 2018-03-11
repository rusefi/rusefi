/**
 * @file	trigger_decoder.h
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TRIGGER_DECODER_H_
#define TRIGGER_DECODER_H_

#include "main.h"
#include "trigger_structure.h"
#include "engine_configuration.h"

class TriggerState;

typedef void (*TriggerStateCallback)(TriggerState *);

typedef struct {
	/**
	 * index within trigger revolution, from 0 to trigger event count
	 */
	uint32_t current_index;
	/**
	 * Number of actual events of each channel within current trigger cycle, these
	 * values are used to detect trigger signal errors.
	 * see TriggerShape
	 */
	uint32_t eventCount[PWM_PHASE_MAX_WAVE_PER_PWM];
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
} current_cycle_state_s;

/**
 * @see TriggerShape for trigger wheel shape deginition
 */
class TriggerState {
public:
	TriggerState();
	/**
	 * current trigger processing index, between zero and #size
	 */
	int getCurrentIndex();
	int getTotalRevolutionCounter();
	/**
	 * this is important for crank-based virtual trigger and VVT magic
	 */
	bool isEvenRevolution();
	void incrementTotalEventCounter();
	efitime_t getTotalEventCounter();
	efitime_t getStartOfRevolutionIndex();
	void decodeTriggerEvent(trigger_event_e const signal, efitime_t nowUs DECLARE_ENGINE_PARAMETER_SUFFIX);
	/**
	 * Resets synchronization flag and alerts rpm_calculator to reset engine spinning flag.
	 */
	void onSynchronizationLost(DECLARE_ENGINE_PARAMETER_SIGNATURE);

	bool isValidIndex(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	float getTriggerDutyCycle(int index);
	TriggerStateCallback triggerCycleCallback;

	/**
	 * TRUE if we know where we are
	 */
	bool shaft_is_synchronized;

	uint32_t toothed_previous_duration; // todo: unify Camel_notation, what a mess :(
	uint32_t durationBeforePrevious; // this one is before 'toothed_previous_duration'
	uint32_t thirdPreviousDuration; // this one is before durationBeforePrevious, todo: better field names?

	/**
	 * this could be a local variable, but it's better for debugging to have it as a field
	 */
	uint32_t currentDuration;
	efitime_t toothed_previous_time;

	current_cycle_state_s currentCycle;

	int expectedTotalTime[PWM_PHASE_MAX_WAVE_PER_PWM];

	/**
	 * how many times since ECU reboot we had unexpected number of teeth in trigger cycle
	 */
	uint32_t totalTriggerErrorCounter;
	uint32_t runningTriggerErrorCounter;
	uint32_t orderingErrorCounter;
	uint32_t runningOrderingErrorCounter;

	void reset();
	void resetRunningCounters();

	virtual void runtimeStatistics(efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX);

	uint32_t runningRevolutionCounter;
	/**
	 * this is start of real trigger cycle
	 * for virtual double trigger see timeAtVirtualZeroNt
	 */
	efitick_t startOfCycleNt;
private:
	void resetCurrentCycleState();

	trigger_event_e curSignal;
	trigger_event_e prevSignal;
	efitime_t totalEventCountBase;
	uint32_t totalRevolutionCounter;
	bool isFirstEvent;

};


/**
 * the reason for sub-class is simply to save RAM but not having statisics in the trigger initialization instance
 */
class TriggerStateWithRunningStatistics : public TriggerState {
public:
	TriggerStateWithRunningStatistics();
	float instantRpm;
	/**
	 * timestamp of each trigger wheel tooth
	 */
	uint32_t timeOfLastEvent[PWM_PHASE_MAX_COUNT];
	/**
	 * instant RPM calculated at this trigger wheel tooth
	 */
	float instantRpmValue[PWM_PHASE_MAX_COUNT];
	/**
	 * Stores last non-zero instant RPM value to fix early instability
	 */
	float prevInstantRpmValue;
	float calculateInstantRpm(int *prevIndex, efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX);
	virtual void runtimeStatistics(efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX);
	/**
	 * Update timeOfLastEvent[] on every trigger event - even without synchronization
	 * Needed for early spin-up RPM detection.
	 */
	void setLastEventTimeForInstantRpm(efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX);
};

angle_t getEngineCycle(operation_mode_e operationMode);
uint32_t findTriggerZeroEventIndex(TriggerState *state, TriggerShape * shape, trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_SUFFIX);

class Engine;

void initTriggerDecoder(void);
void initTriggerDecoderLogger(Logging *sharedLogger);

bool isTriggerDecoderError(void);


#endif /* TRIGGER_DECODER_H_ */
