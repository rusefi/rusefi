/**
 * @file	trigger_decoder.h
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TRIGGER_DECODER_H_
#define TRIGGER_DECODER_H_

#include "trigger_structure.h"
#include "engine_configuration.h"
#include "ec2.h"

class TriggerState;

typedef void (*TriggerStateCallback)(TriggerState *);

class TriggerState {
public:
	TriggerState();
	int getCurrentIndex();
	int getTotalRevolutionCounter();
	uint64_t getTotalEventCounter();
	uint64_t getStartOfRevolutionIndex();
	void nextRevolution(int triggerEventCount, uint64_t nowUs);
	void nextTriggerEvent(trigger_wheel_e triggerWheel, uint64_t nowUs);
	void decodeTriggerEvent(trigger_shape_s const*triggerShape, trigger_config_s const*triggerConfig, trigger_event_e const signal, uint64_t nowUs);

	TriggerStateCallback cycleCallback;

	/**
	 * TRUE if we know where we are
	 */
	unsigned char shaft_is_synchronized;

	uint64_t toothed_previous_duration;
	uint64_t toothed_previous_time;
private:
	void clear();
	/**
	 * index within trigger revolution, from 0 to trigger event count
	 */
	int current_index;
	/**
	 * Number of actual events within current trigger cycle
	 * see trigger_shape_s
	 */
	int eventCount[PWM_PHASE_MAX_WAVE_PER_PWM];
	uint64_t timeOfPreviousEvent[PWM_PHASE_MAX_WAVE_PER_PWM];
	int totalTime[PWM_PHASE_MAX_WAVE_PER_PWM];
	uint64_t totalEventCountBase;
	int totalRevolutionCounter;
	bool isFirstEvent;
};

class TriggerStimulatorHelper {
public:
	TriggerStimulatorHelper();
	void nextStep(TriggerState *state, trigger_shape_s * shape, int i, trigger_config_s const*triggerConfig);
private:
	bool primaryWheelState;
	bool secondaryWheelState;
};

void initializeSkippedToothTriggerShapeExt(trigger_shape_s *s, int totalTeethCount, int skippedCount, operation_mode_e operationMode);
int findTriggerZeroEventIndex(trigger_shape_s * shape, trigger_config_s const*triggerConfig);
void initializeTriggerShape(Logging *logger, engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2);
void initTriggerDecoder(void);

#ifdef __cplusplus
extern "C"
{
#endif

int isTriggerDecoderError(void);

#ifdef __cplusplus
}
#endif

#endif /* TRIGGER_DECODER_H_ */
