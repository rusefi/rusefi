/**
 * @file	trigger_structure.h
 *
 * @date Dec 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TRIGGER_STRUCTURE_H_
#define TRIGGER_STRUCTURE_H_

#include "rusefi_enums.h"
#include "stdint.h"

#define PWM_PHASE_MAX_COUNT 150
#define PWM_PHASE_MAX_WAVE_PER_PWM 2

typedef enum {
	SHAFT_PRIMARY_UP = 0,
	SHAFT_PRIMARY_DOWN = 1,
	SHAFT_SECONDARY_UP = 2,
	SHAFT_SECONDARY_DOWN = 3,
} ShaftEvents;

typedef struct {
	/**
	 * TRUE if we know where we are
	 */
	unsigned char shaft_is_synchronized;

	int current_index;

	uint64_t toothed_previous_duration;
	uint64_t toothed_previous_time;

} trigger_state_s;

/**
 * @brief   PWM configuration for the specific output pin
 */
typedef struct {
	int pinStates[PWM_PHASE_MAX_COUNT];
} single_wave_s;

typedef struct {
	/**
	 * Number of events in the cycle
	 */
	int phaseCount;
	/**
	 * Number of signal wires
	 */
	int waveCount;
	single_wave_s waves[PWM_PHASE_MAX_WAVE_PER_PWM];
	/**
	 * values in the (0..1] range which refer to points within the period at at which pin state should be changed
	 * So, in the simplest case we turn pin off at 0.3 and turn it on at 1 - that would give us a 70% duty cycle PWM
	 */
	float switchTimes[PWM_PHASE_MAX_COUNT];
} multi_wave_s;

typedef enum {
	TV_LOW = 0,
	TV_HIGH = 1
} trigger_value_e;

typedef enum {
	T_PRIMARY = 0,
	T_SECONDARY = 1
} trigger_wheel_e;

/**
 * @brief Trigger wheel(s) configuration
 */
typedef struct {
	trigger_type_e triggerType;

	int isSynchronizationNeeded;

	int totalToothCount;
	int skippedToothCount;

	float syncRatioFrom;
	float syncRatioTo;

	int useRiseEdge;

} trigger_config_s;

typedef struct {
	multi_wave_s wave;
	int size;

	/**
	 * Total count of shaft events per CAM or CRANK shaft revolution.
	 * TODO this should be migrated to CRANKshaft revolution, this would go together
	 * TODO with eliminating RPM_MULT magic constant
	 */
	int shaftPositionEventCount;

	// todo: add a runtime validation which would verify that this field was set properly
	// tood: maybe even automate this flag calculation?
	int initialState[PWM_PHASE_MAX_WAVE_PER_PWM];

	int triggerShapeSynchPointIndex;
} trigger_shape_s;


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void clearTriggerState(trigger_state_s *state);
void triggerShapeInit(trigger_shape_s *trigger);
void triggerAddEvent(trigger_shape_s *trigger, float angle, trigger_wheel_e waveIndex, trigger_value_e state);

void checkSwitchTimes(int size, float *switchTimes);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TRIGGER_STRUCTURE_H_ */
