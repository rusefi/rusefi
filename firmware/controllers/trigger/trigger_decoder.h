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

class TriggerState {
public:
	TriggerState();
	int getCurrentIndex();
	int getTotalRevolutionCounter();
	/**
	 * this is important for crank-based virtual trigger and VVT magic
	 */
	bool isEvenRevolution();
	void intTotalEventCounter();
	efitime_t getTotalEventCounter();
	efitime_t getStartOfRevolutionIndex();
	void decodeTriggerEvent(trigger_event_e const signal, efitime_t nowUs DECLARE_ENGINE_PARAMETER_S);

	bool isValidIndex(DECLARE_ENGINE_PARAMETER_F);
	float getTriggerDutyCycle(int index);
	TriggerStateCallback cycleCallback;

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
	/**
	 * Total time result for previous trigger cycle
	 * See totalTimeNt
	 */
	uint32_t prevTotalTime[PWM_PHASE_MAX_WAVE_PER_PWM];
	int expectedTotalTime[PWM_PHASE_MAX_WAVE_PER_PWM];

	// we need two instances of TriggerState
	// todo: re-imiplement as a sub-class to reduce memory consumption
//	uint32_t timeOfLastEvent[PWM_PHASE_MAX_COUNT];
//	float instantRpmValue[PWM_PHASE_MAX_COUNT];

	/**
	 * how many times since ECU reboot we had unexpected number of teeth in trigger cycle
	 */
	uint32_t totalTriggerErrorCounter;
	uint32_t runningTriggerErrorCounter;
	uint32_t orderingErrorCounter;
	uint32_t runningOrderingErrorCounter;

	void reset();
	void resetRunningCounters();

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
	efitime_t prevCycleDuration;
};

angle_t getEngineCycle(operation_mode_e operationMode);
uint32_t findTriggerZeroEventIndex(TriggerState *state, TriggerShape * shape, trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_S);

class Engine;

void initTriggerDecoder(void);
void initTriggerDecoderLogger(Logging *sharedLogger);

bool isTriggerDecoderError(void);

#define considerEventForGap() (!TRIGGER_SHAPE(useOnlyPrimaryForSync) || isPrimary)

#define isLessImportant(type) (!TRIGGER_SHAPE(gapBothDirections) && (TRIGGER_SHAPE(useRiseEdge) && type != TV_RISE) \
		|| (!TRIGGER_SHAPE(gapBothDirections) && !TRIGGER_SHAPE(useRiseEdge) && type != TV_FALL) \
		|| (!considerEventForGap()) \
	)

#endif /* TRIGGER_DECODER_H_ */
