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
#include "engine_math.h"

trigger_shape_helper::trigger_shape_helper() {
	for (int i = 0; i < TRIGGER_CHANNEL_COUNT; i++) {
		waves[i].init(pinStates[i]);
	}
}

trigger_shape_s::trigger_shape_s() :
		wave(switchTimesBuffer, NULL) {
	reset(OM_NONE);
	wave.waves = h.waves;
	useRiseEdge = false;
	gapBothDirections = false;
	isSynchronizationNeeded = false;
	invertOnAdd = false;
}

void trigger_shape_s::assignSize() {
	shaftPositionEventCount = getSize();
}

int trigger_shape_s::getSize() const {
	return size;
}

int trigger_shape_s::getTriggerShapeSynchPointIndex() {
	return triggerShapeSynchPointIndex;
}

// todo: clean-up!
int getEngineCycleEventCount2(operation_mode_e mode, trigger_shape_s * s);

void trigger_shape_s::calculateTriggerSynchPoint(engine_configuration_s *engineConfiguration, Engine *engine) {
	trigger_config_s const*triggerConfig = &engineConfiguration->triggerConfig;
	setTriggerShapeSynchPointIndex(engineConfiguration, findTriggerZeroEventIndex(this, triggerConfig), engine);
}

void trigger_shape_s::setTriggerShapeSynchPointIndex(engine_configuration_s *engineConfiguration, int triggerShapeSynchPointIndex, Engine *engine) {
	this->triggerShapeSynchPointIndex = triggerShapeSynchPointIndex;

	int engineCycleEventCount = getEngineCycleEventCount2(operationMode, this);

	float firstAngle = getAngle(triggerShapeSynchPointIndex);

	for (int i = 0; i < engineCycleEventCount; i++) {
		if (i == 0) {
			// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
			eventAngles[i] = 0;
		} else {
			eventAngles[i] = fixAngle(getAngle((triggerShapeSynchPointIndex + i) % engineCycleEventCount) - firstAngle PASS_ENGINE_PARAMETER);
		}
	}
}

void trigger_shape_s::reset(operation_mode_e operationMode) {
	this->operationMode = operationMode;
	size = 0;
	shaftPositionEventCount = 0;
	triggerShapeSynchPointIndex = 0;
	memset(initialState, 0, sizeof(initialState));
	memset(switchTimesBuffer, 0, sizeof(switchTimesBuffer));
	memset(expectedEventCount, 0, sizeof(expectedEventCount));
	wave.reset();
	previousAngle = 0;
}

int multi_wave_s::getChannelState(int channelIndex, int phaseIndex) const {
	return waves[channelIndex].pinStates[phaseIndex];
}

int multi_wave_s::waveIndertionAngle(float angle, int size) const {
	for (int i = size - 1; i >= 0; i--) {
		if (angle > switchTimes[i])
			return i + 1;
	}
	return 0;
}

int multi_wave_s::findAngleMatch(float angle, int size) const {
	for (int i = 0; i < size; i++) {
		if (isSameF(switchTimes[i], angle))
			return i;
	}
	return EFI_ERROR_CODE;
}

void multi_wave_s::setSwitchTime(int index, float value) {
	switchTimes[index] = value;
}

TriggerState::TriggerState() {
	cycleCallback = NULL;
	shaft_is_synchronized = false;
	toothed_previous_time = 0;
	toothed_previous_duration = 0;
	totalRevolutionCounter = 0;
	totalTriggerErrorCounter = 0;
	clear();
	memset(expectedTotalTime, 0, sizeof(expectedTotalTime));
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

void TriggerState::nextRevolution(int triggerEventCount, uint64_t nowNt) {
	if (cycleCallback != NULL) {
		cycleCallback(this);
	}
	memcpy(prevTotalTime, totalTimeNt, sizeof(prevTotalTime));
	prevCycleDuration = nowNt - startOfCycleNt;
	startOfCycleNt = nowNt;
	clear();
	totalRevolutionCounter++;
	totalEventCountBase += triggerEventCount;
}

int TriggerState::getTotalRevolutionCounter() {
	return totalRevolutionCounter;
}

void TriggerState::nextTriggerEvent(trigger_wheel_e triggerWheel, uint64_t nowNt) {
	uint64_t prevTime = timeOfPreviousEventNt[triggerWheel];
	if (prevTime != 0) {
		// even event - apply the value
		totalTimeNt[triggerWheel] += (nowNt - prevTime);
		timeOfPreviousEventNt[triggerWheel] = 0;
	} else {
		// odd event - start accumulation
		timeOfPreviousEventNt[triggerWheel] = nowNt;
	}

	current_index++;
}

void TriggerState::clear() {
	memset(eventCount, 0, sizeof(eventCount));
	memset(eventCountExt, 0, sizeof(eventCountExt));
	memset(timeOfPreviousEventNt, 0, sizeof(timeOfPreviousEventNt));
	memset(totalTimeNt, 0, sizeof(totalTimeNt));
	current_index = 0;
}

uint32_t trigger_shape_s::getLength() const {
	return operationMode == FOUR_STROKE_CAM_SENSOR ? shaftPositionEventCount : 2 * shaftPositionEventCount;
}

float trigger_shape_s::getAngle(int index) const {
	if (operationMode == FOUR_STROKE_CAM_SENSOR) {
		return getSwitchAngle(index);
	}
	/**
	 * FOUR_STROKE_CRANK_SENSOR magic:
	 * We have two crank shaft revolutions for each engine cycle
	 * See also trigger_central.cpp
	 * See also getEngineCycleEventCount()
	 */
	int triggerEventCounter = size;

	if (index < triggerEventCounter) {
		return getSwitchAngle(index);
	} else {
		return 360 + getSwitchAngle(index - triggerEventCounter);
	}
}

void trigger_shape_s::addEvent(float angle, trigger_wheel_e const waveIndex, trigger_value_e const stateParam) {
	efiAssertVoid(operationMode != OM_NONE, "operationMode not set");

	trigger_value_e state;
	if (invertOnAdd) {
		state = (stateParam == TV_LOW) ? TV_HIGH : TV_LOW;
	} else {
		state = stateParam;
	}

	/**
	 * While '720' value works perfectly it has not much sense for crank sensor-only scenario.
	 * todo: accept angle as a value in the 0..1 range?
	 */
	angle /= 720;

	expectedEventCount[waveIndex]++;

	efiAssertVoid(angle > 0, "angle should be positive");
	if (size > 0) {
		if (angle <= previousAngle) {
			firmwareError("invalid angle order: %f and %f", angle, previousAngle);
			return;
		}
	}
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

		wave.setSwitchTime(0, angle);
		wave.waves[waveIndex].pinStates[0] = state;
		return;
	}

	int exactMatch = wave.findAngleMatch(angle, size);
	if (exactMatch != EFI_ERROR_CODE) {
		firmwareError("same angle: not supported");
		return;
	}

	int index = wave.waveIndertionAngle(angle, size);

	// shifting existing data
	for (int i = size - 1; i >= index; i--) {
		for (int j = 0; j < PWM_PHASE_MAX_WAVE_PER_PWM; j++) {
			wave.waves[j].pinStates[i + 1] = wave.getChannelState(j, index);
		}
		wave.setSwitchTime(i + 1, wave.getSwitchTime(i));
	}

//	int index = size;
	size++;

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		wave.waves[i].pinStates[index] = wave.getChannelState(i, index - 1);
	}
	wave.setSwitchTime(index, angle);
	wave.waves[waveIndex].pinStates[index] = state;
}

int trigger_shape_s::getCycleDuration() const {
	return (operationMode == FOUR_STROKE_CAM_SENSOR) ? 720 : 360;
}

float trigger_shape_s::getSwitchAngle(int index) const {
	return getCycleDuration() * wave.getSwitchTime(index);
}

void multi_wave_s::checkSwitchTimes(int size) {
	checkSwitchTimes2(size, switchTimes);
}

void setToothedWheelConfiguration(trigger_shape_s *s, int total, int skipped,
		engine_configuration_s const *engineConfiguration) {
	s->isSynchronizationNeeded = (skipped != 0);

	s->totalToothCount = total;
	s->skippedToothCount = skipped;
	// todo: move to into configuration definition s->needSecondTriggerInput = false;
	s->useRiseEdge = true;

	initializeSkippedToothTriggerShapeExt(s, s->totalToothCount, s->skippedToothCount,
			getOperationMode(engineConfiguration));
}

void setTriggerSynchronizationGap2(trigger_shape_s *s, float syncGapFrom, float syncRatioTo) {
	s->isSynchronizationNeeded = true;
	s->syncRatioFrom = syncGapFrom;
	s->syncRatioTo = syncRatioTo;
}

void setTriggerSynchronizationGap(trigger_shape_s *s, float synchGap) {
	setTriggerSynchronizationGap2(s, synchGap * 0.75f, synchGap * 1.25f);
}

#define S24 (720.0f / 24 / 2)

static float addAccordPair(trigger_shape_s *s, float sb) {
	s->addEvent(sb, T_SECONDARY, TV_HIGH);
	sb += S24;
	s->addEvent(sb, T_SECONDARY, TV_LOW);
	sb += S24;

	return sb;
}

#define DIP 7.5f
static float addAccordPair3(trigger_shape_s *s, float sb) {
	sb += DIP;
	s->addEvent(sb, T_CHANNEL_3, TV_HIGH);
	sb += DIP;
	s->addEvent(sb, T_CHANNEL_3, TV_LOW);
	sb += 2 * DIP;
	return sb;
}

/**
 * Thank you Dip!
 * http://forum.pgmfi.org/viewtopic.php?f=2&t=15570start=210#p139007
 */
void configureHondaAccordCDDip(trigger_shape_s *s) {
	s->reset(FOUR_STROKE_CAM_SENSOR);

	s->initialState[T_SECONDARY] = TV_HIGH;
	float sb = 0;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(90, T_SECONDARY, TV_LOW);
	sb = 90;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(180, T_SECONDARY, TV_HIGH);
	sb = 180;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(270, T_SECONDARY, TV_LOW);
	sb = 270;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);


	s->addEvent(360.0f - DIP, T_PRIMARY, TV_HIGH);
	s->addEvent(360, T_SECONDARY, TV_HIGH);
	sb = 360;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(450, T_SECONDARY, TV_LOW);
	sb = 450;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(540, T_SECONDARY, TV_HIGH);
	sb = 540;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(630, T_SECONDARY, TV_LOW);
	sb = 630;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(720.0f - DIP, T_PRIMARY, TV_LOW);

//	s->addEvent(720.0f - 12 * sb, T_SECONDARY, TV_LOW);
//	s->addEvent(720.0f, T_SECONDARY, TV_LOW);

	s->addEvent(720.0f, T_SECONDARY, TV_HIGH);

	s->isSynchronizationNeeded = false;

	s->assignSize();
}

void configureHondaAccordCD(trigger_shape_s *s, bool with3rdSignal) {
	s->reset(FOUR_STROKE_CAM_SENSOR);

	float sb = 5.0f;

	float tdcWidth = 0.1854 * 720 / 4;

	s->isSynchronizationNeeded = false;

	sb = addAccordPair(s, sb);

	if (with3rdSignal)
		s->addEvent(sb - S24 / 2, T_CHANNEL_3, TV_HIGH);

	sb = addAccordPair(s, sb);
	sb = addAccordPair(s, sb);
	if (with3rdSignal)
		s->addEvent(sb - S24 / 2, T_CHANNEL_3, TV_LOW);
	sb = addAccordPair(s, sb);
	sb = addAccordPair(s, sb);
	s->addEvent(1 * 180.0f - tdcWidth, T_PRIMARY, TV_HIGH);
	sb = addAccordPair(s, sb);
	s->addEvent(1 * 180.0f, T_PRIMARY, TV_LOW);

	sb = addAccordPair(s, sb);
	sb = addAccordPair(s, sb);
	sb = addAccordPair(s, sb);
	sb = addAccordPair(s, sb);
	sb = addAccordPair(s, sb);

	s->addEvent(2 * 180.0f - tdcWidth, T_PRIMARY, TV_HIGH);
	sb = addAccordPair(s, sb);
	s->addEvent(2 * 180.0f, T_PRIMARY, TV_LOW);

	for (int i = 3; i <= 4; i++) {
		sb = addAccordPair(s, sb);
		sb = addAccordPair(s, sb);
		sb = addAccordPair(s, sb);
		sb = addAccordPair(s, sb);
		sb = addAccordPair(s, sb);

		s->addEvent(i * 180.0f - tdcWidth, T_PRIMARY, TV_HIGH);
		sb = addAccordPair(s, sb);
		s->addEvent(i * 180.0f, T_PRIMARY, TV_LOW);
	}

	s->assignSize();
}
