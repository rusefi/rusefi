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
#include "engine_configuration.h"

class trigger_shape_s;

#define TRIGGER_CHANNEL_COUNT 3

class trigger_shape_helper {
	int pinStates[TRIGGER_CHANNEL_COUNT][PWM_PHASE_MAX_COUNT];
public:
	trigger_shape_helper();

	single_wave_s waves[TRIGGER_CHANNEL_COUNT];
};

class trigger_shape_s {
public:
	trigger_shape_s();
	int isSynchronizationNeeded;

	int totalToothCount;
	int skippedToothCount;

	float dutyCycle[PWM_PHASE_MAX_WAVE_PER_PWM];

	float syncRatioFrom;
	float syncRatioTo;

	int useRiseEdge;

	/**
	 * This is used for signal validation
	 */
	int expectedEventCount[PWM_PHASE_MAX_WAVE_PER_PWM];

	void addEvent(float angle, trigger_wheel_e const waveIndex, trigger_value_e const state);
	void reset(operation_mode_e operationMode);
	int getSize() const;
	multi_wave_s wave;

	/**
	 * Total count of shaft events per CAM or CRANK shaft revolution.
	 * TODO this should be migrated to CRANKshaft revolution, this would go together
	 * TODO with eliminating RPM_MULT magic constant
	 *
	 * todo: somehow changing the type of this to uint32_t breaks unit tests? WHY?
	 */
	int shaftPositionEventCount;

	/**
	 * this one is per CRANKshaft revolution
	 */
	uint32_t getLength() const;

	// todo: add a runtime validation which would verify that this field was set properly
	// tood: maybe even automate this flag calculation?
	int initialState[PWM_PHASE_MAX_WAVE_PER_PWM];

	int getTriggerShapeSynchPointIndex();

	void calculateTriggerSynchPoint(trigger_config_s const*triggerConfig);

	void setTriggerShapeSynchPointIndex(int triggerShapeSynchPointIndex);
	/**
	 * These angles are in event coordinates - with synchronization point located at angle zero.
	 * These values are pre-calculated for performance reasons.
	 */
	float eventAngles[PWM_PHASE_MAX_COUNT];

private:
	trigger_shape_helper h;
	int size;
	/**
	 * index of synchronization event within trigger_shape_s
	 * See findTriggerZeroEventIndex()
	 */
	int triggerShapeSynchPointIndex;
	/**
	 * Values are in the 0..1 range
	 */
	float switchTimesBuffer[PWM_PHASE_MAX_COUNT];
	/**
	 * These angles are in trigger DESCRIPTION coordinates - i.e. the way you add events while declaring trigger shape
	 */
	float getSwitchAngle(int index) const;

	float previousAngle;
	/**
	 * this is part of performance optimization
	 */
	operation_mode_e operationMode;

	/**
	 * This private method should only be used to prepare the array of pre-calculated values
	 * See eventAngles array
	 */
	float getAngle(int phaseIndex) const;

	int getCycleDuration() const;
};

void setTriggerSynchronizationGap(trigger_shape_s *s, float synchGap);
void setToothedWheelConfiguration(trigger_shape_s *s, int total, int skipped, engine_configuration_s const *engineConfiguration);

#endif /* TRIGGER_STRUCTURE_H_ */
