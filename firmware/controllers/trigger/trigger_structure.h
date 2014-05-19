/**
 * @file	trigger_structure.h
 *
 * @date Dec 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TRIGGER_STRUCTURE_H_
#define TRIGGER_STRUCTURE_H_

#include <stdint.h>

#include "rusefi_enums.h"
#include "EfiWave.h"

typedef struct {
	/**
	 * TRUE if we know where we are
	 */
	unsigned char shaft_is_synchronized;

	int current_index;

	uint64_t toothed_previous_duration;
	uint64_t toothed_previous_time;

} trigger_state_s;

typedef enum {
	TV_LOW = 0,
	TV_HIGH = 1
} trigger_value_e;

typedef enum {
	T_PRIMARY = 0,
	T_SECONDARY = 1
} trigger_wheel_e;

class trigger_shape_helper {
	int pinStates0[PWM_PHASE_MAX_COUNT];
	int pinStates1[PWM_PHASE_MAX_COUNT];
public:
	trigger_shape_helper();

	single_wave_s waves[2];
};

class trigger_shape_s {
private:
	trigger_shape_helper h;
	int size;
public:
	trigger_shape_s();
	void addEvent(float angle, trigger_wheel_e waveIndex, trigger_value_e state);
	void reset();
	int getSize();
	multi_wave_s wave;

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
private:
	float switchTimes[PWM_PHASE_MAX_COUNT];
};

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void clearTriggerState(trigger_state_s *state);
void triggerAddEvent(trigger_shape_s *trigger, float angle, trigger_wheel_e waveIndex, trigger_value_e state);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TRIGGER_STRUCTURE_H_ */
