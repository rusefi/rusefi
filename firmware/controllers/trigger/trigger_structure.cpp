/**
 * @file	trigger_structure.cpp
 *
 * @date Jan 20, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "main.h"
#include "trigger_structure.h"
#include "error_handling.h"
#include "trigger_decoder.h"

trigger_shape_helper::trigger_shape_helper() {
	waves[0].init(pinStates0);
	waves[1].init(pinStates1);
}

trigger_shape_s::trigger_shape_s() :
		wave(switchTimes, NULL) {
	reset(OM_NONE);
	wave.waves = h.waves;
}

int trigger_shape_s::getSize() {
	return size;
}

int trigger_shape_s::getTriggerShapeSynchPointIndex() {
	return triggerShapeSynchPointIndex;
}

// todo: clean-up!
int getEngineCycleEventCount2(operation_mode_e mode, trigger_shape_s * s);
float fixAngle(float angle);

void trigger_shape_s::setTriggerShapeSynchPointIndex(int triggerShapeSynchPointIndex) {
	this->triggerShapeSynchPointIndex = triggerShapeSynchPointIndex;

	int engineCycleEventCount = getEngineCycleEventCount2(operationMode, this);

	float firstAngle = getAngle(triggerShapeSynchPointIndex);

	for (int i = 0; i < engineCycleEventCount; i++) {
		if (i == 0) {
			// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
			eventAngles[i] = 0;
		} else {
			eventAngles[i] = fixAngle(getAngle((triggerShapeSynchPointIndex + i) % engineCycleEventCount) - firstAngle);
		}
	}
}

void trigger_shape_s::reset(operation_mode_e operationMode) {
	this->operationMode = operationMode;
	size = 0;
	shaftPositionEventCount = 0;
	triggerShapeSynchPointIndex = 0;
	memset(initialState, 0, sizeof(initialState));
	memset(switchTimes, 0, sizeof(switchTimes));
	wave.reset();
	previousAngle = 0;
}

int multi_wave_s::getChannelState(int channelIndex, int phaseIndex) const {
	return waves[channelIndex].pinStates[phaseIndex];
}

void multi_wave_s::setSwitchTime(int index, float value) {
	switchTimes[index] = value;
}

TriggerState::TriggerState() {
	clear();
	totalEventCountBase = 0;
	isFirstEvent = true;
}

int TriggerState::getCurrentIndex() {
	return current_index;
}

uint64_t TriggerState::getStartOfRevolutionIndex() {
	return totalEventCountBase;
}

uint64_t TriggerState::getTotalEventCounter() {
	return totalEventCountBase + current_index;
}

void TriggerState::nextRevolution(int triggerEventCount) {
	current_index = 0;
	totalRevolutionCounter++;
	totalEventCountBase += triggerEventCount;
}

int TriggerState::getTotalRevolutionCounter() {
	return totalRevolutionCounter;
}

void TriggerState::nextTriggerEvent() {
	current_index++;
}

void TriggerState::clear() {
	shaft_is_synchronized = FALSE;
	toothed_previous_time = 0;
	toothed_previous_duration = 0;
	current_index = 0;
	totalRevolutionCounter = 0;
}

float trigger_shape_s::getAngle(int index) const {
	if (operationMode == FOUR_STROKE_CAM_SENSOR)
		return switchAngles[index];
	/**
	 * FOUR_STROKE_CRANK_SENSOR magic:
	 * We have two crank shaft revolutions for each engine cycle
	 * See also trigger_central.cpp
	 * See also getEngineCycleEventCount()
	 */
	int triggerEventCounter = size;

	if (index < triggerEventCounter) {
		return switchAngles[index];
	} else {
		return 360 + switchAngles[index - triggerEventCounter];
	}
}

void trigger_shape_s::addEvent(float angle, trigger_wheel_e waveIndex, trigger_value_e state) {
	efiAssertVoid(operationMode != OM_NONE, "operationMode not set");
	/**
	 * While '720' value works perfectly it has not much sense for crank sensor-only scenario.
	 * todo: accept angle as a value in the 0..1 range?
	 */
	angle /= 720;
	efiAssertVoid(angle > previousAngle, "invalid angle order");
	previousAngle = angle;
	if (size == 0) {
		size = 1;
		for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
			single_wave_s *wave = &this->wave.waves[i];

			if (wave == NULL) {
				firmwareError("wave is NULL");
				return;
			}
			if (wave->pinStates == NULL) {
				firmwareError("wave pinStates is NULL");
				return;
			}
			wave->pinStates[0] = initialState[i];
		}

		setSwitchTime(0, angle);
		wave.waves[waveIndex].pinStates[0] = state;
		return;
	}

//	if(angle!=trigger->wave.switchTimes[trigger->currentIndex])

	int index = size++;

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++)
		wave.waves[i].pinStates[index] = wave.getChannelState(i, index - 1);
	setSwitchTime(index, angle);
	wave.waves[waveIndex].pinStates[index] = state;
}

void trigger_shape_s::setSwitchTime(int index, float angle) {
	int cycleDuration = (operationMode == FOUR_STROKE_CAM_SENSOR) ? 720 : 360;
	switchAngles[index] = cycleDuration * angle;
	wave.setSwitchTime(index, angle);
}

void multi_wave_s::checkSwitchTimes(int size) {
	checkSwitchTimes2(size, switchTimes);
}

void setToothedWheelConfiguration(trigger_shape_s *s, int total, int skipped, engine_configuration_s const *engineConfiguration) {
	s->isSynchronizationNeeded = (skipped != 0);

	s->totalToothCount = total;
	s->skippedToothCount = skipped;
	s->needSecondTriggerInput = false;
	s->useRiseEdge = TRUE;

	initializeSkippedToothTriggerShapeExt(s, s->totalToothCount,
			s->skippedToothCount,
			getOperationMode(engineConfiguration));
}

void setTriggerSynchronizationGap(trigger_shape_s *s, float synchGap) {
	s->isSynchronizationNeeded = TRUE;
	s->syncRatioFrom = synchGap * 0.75;
	s->syncRatioTo = synchGap * 1.25;
}
