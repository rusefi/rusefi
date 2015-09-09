/**
 * @file	trigger_decoder.h
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef TRIGGER_DECODER_H_
#define TRIGGER_DECODER_H_

#include "main.h"
#include "trigger_structure.h"
#include "engine_configuration.h"

#define NO_LEFT_FILTER -1
#define NO_RIGHT_FILTER 1000

class TriggerState;

typedef void (*TriggerStateCallback)(TriggerState *);

typedef struct {
	/**
	 * Here we accumulate the amount of time this signal was ON within current trigger cycle
	 */
	int totalTimeNt[PWM_PHASE_MAX_WAVE_PER_PWM];
	/**
	 * index within trigger revolution, from 0 to trigger event count
	 */
	uint32_t current_index;
	/**
	 * Number of actual events within current trigger cycle
	 * see TriggerShape
	 */
	uint32_t eventCount[PWM_PHASE_MAX_WAVE_PER_PWM];
	efitime_t timeOfPreviousEventNt[PWM_PHASE_MAX_WAVE_PER_PWM];

} current_cycle_state_s;

class TriggerState {
public:
	TriggerState();
	int getCurrentIndex();
	int getTotalRevolutionCounter();
	efitime_t getTotalEventCounter();
	efitime_t getStartOfRevolutionIndex();
	void decodeTriggerEvent(trigger_event_e const signal, efitime_t nowUs DECLARE_ENGINE_PARAMETER_S);

	float getTriggerDutyCycle(int index);
	TriggerStateCallback cycleCallback;

	/**
	 * TRUE if we know where we are
	 */
	bool shaft_is_synchronized;

	uint32_t toothed_previous_duration;
	/**
	 * this could be a local variable, but it's better for debugging to have it as a field
	 */
	uint32_t currentDuration;
	efitime_t toothed_previous_time;

	current_cycle_state_s currentCycle;
	/**
	 * Total time result for previous trigger cycle
	 */
	uint32_t prevTotalTime[PWM_PHASE_MAX_WAVE_PER_PWM];
	int expectedTotalTime[PWM_PHASE_MAX_WAVE_PER_PWM];
	/**
	 * how many times since ECU reboot we had unexpected number of teeth in trigger cycle
	 */
	uint32_t totalTriggerErrorCounter;
	uint32_t runningTriggerErrorCounter;
	uint32_t orderingErrorCounter;
	uint32_t runningOrderingErrorCounter;

	void resetRunningCounters();

	uint32_t runningRevolutionCounter;
private:
	void resetCurrentCycleState();

	trigger_event_e curSignal;
	trigger_event_e prevSignal;
	efitime_t totalEventCountBase;
	uint32_t totalRevolutionCounter;
	bool isFirstEvent;
	efitime_t prevCycleDuration;
	efitick_t startOfCycleNt;
};

class TriggerStimulatorHelper {
public:
	TriggerStimulatorHelper();
	void nextStep(TriggerState *state, TriggerShape * shape, int i, trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_S);
};

float getEngineCycle(operation_mode_e operationMode);
void addSkippedToothTriggerEvents(trigger_wheel_e wheel, TriggerShape *s,
		int totalTeethCount, int skippedCount,
		float toothWidth,
		float offset, float engineCycle, float filterLeft, float filterRight);
void initializeSkippedToothTriggerShapeExt(TriggerShape *s, int totalTeethCount, int skippedCount, operation_mode_e operationMode);
uint32_t findTriggerZeroEventIndex(TriggerShape * shape, trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_S);

class Engine;

void initTriggerDecoder(void);
void initTriggerDecoderLogger(Logging *sharedLogger);

bool_t isTriggerDecoderError(void);

#define considerEventForGap() (!TRIGGER_SHAPE(useOnlyPrimaryForSync) || isPrimary)

#define isLessImportant(signal) ((TRIGGER_SHAPE(useRiseEdge) && signal != SHAFT_PRIMARY_UP) \
		|| (!TRIGGER_SHAPE(useRiseEdge) && signal != SHAFT_PRIMARY_DOWN) \
		|| (!considerEventForGap()) \
	)

#endif /* TRIGGER_DECODER_H_ */
