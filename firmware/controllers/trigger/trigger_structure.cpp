/**
 * @file	trigger_structure.cpp
 *
 * @date Jan 20, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
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

EXTERN_ENGINE;

trigger_shape_helper::trigger_shape_helper() {
	for (int i = 0; i < TRIGGER_CHANNEL_COUNT; i++) {
		waves[i].init(pinStates[i]);
	}
}

TriggerShape::TriggerShape() :
		wave(switchTimesBuffer, NULL) {
	reset(OM_NONE, false);
	wave.waves = h.waves;
	useRiseEdge = false;
	gapBothDirections = false;
	isSynchronizationNeeded = false;
	invertOnAdd = false;
	tdcPosition = 0;
	skippedToothCount = totalToothCount = 0;
	syncRatioFrom = syncRatioTo = 0;
}

int TriggerShape::getSize() const {
	return size;
}

int TriggerShape::getTriggerShapeSynchPointIndex() {
	return triggerShapeSynchPointIndex;
}

void TriggerShape::calculateTriggerSynchPoint(engine_configuration_s *engineConfiguration, Engine *engine) {
	trigger_config_s const*triggerConfig = &engineConfiguration->trigger;
	setTriggerShapeSynchPointIndex(engineConfiguration, findTriggerZeroEventIndex(this, triggerConfig PASS_ENGINE_PARAMETER), engine);
}

void TriggerShape::setTriggerShapeSynchPointIndex(engine_configuration_s *engineConfiguration, int triggerShapeSynchPointIndex, Engine *engine) {
	this->triggerShapeSynchPointIndex = triggerShapeSynchPointIndex;

	engine->engineCycleEventCount = getLength();

	float firstAngle = getAngle(triggerShapeSynchPointIndex);

	for (int i = 0; i < engine->engineCycleEventCount; i++) {
		if (i == 0) {
			// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
			eventAngles[i] = 0;
		} else {
			float angle = getAngle((triggerShapeSynchPointIndex + i) % engine->engineCycleEventCount) - firstAngle;
			fixAngle(angle);
			eventAngles[i] = angle;
		}
	}
}

void TriggerShape::clear() {
	tdcPosition = 0;
	setTriggerSynchronizationGap(2);
	useRiseEdge = true;

	invertOnAdd = false;
	gapBothDirections = false;
}

void TriggerShape::reset(operation_mode_e operationMode, bool needSecondTriggerInput) {
	this->operationMode = operationMode;
	size = 0;
	this->needSecondTriggerInput = needSecondTriggerInput;
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
	orderingErrorCounter = 0;

	resetRunningCounters();
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

void TriggerState::resetRunningCounters() {
	runningRevolutionCounter = 0;
	runningTriggerErrorCounter = 0;
	runningOrderingErrorCounter = 0;
}

uint64_t TriggerState::getTotalEventCounter() {
	return totalEventCountBase + current_index;
}

int TriggerState::getTotalRevolutionCounter() {
	return totalRevolutionCounter;
}

void TriggerState::clear() {
	memset(eventCount, 0, sizeof(eventCount));
	memset(eventCountExt, 0, sizeof(eventCountExt));
	memset(timeOfPreviousEventNt, 0, sizeof(timeOfPreviousEventNt));
	memset(totalTimeNt, 0, sizeof(totalTimeNt));
	current_index = 0;
}

/**
 * Trigger event count equals engine cycle event count if we have a cam sensor.
 * Two trigger cycles make one engine cycle in case of a four stroke engine If we only have a cranksensor.
 */
uint32_t TriggerShape::getLength() const {
	return operationMode == FOUR_STROKE_CAM_SENSOR ? getSize() : 2 * getSize();
}

float TriggerShape::getAngle(int index) const {
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

void TriggerShape::addEvent(float angle, trigger_wheel_e const waveIndex, trigger_value_e const stateParam) {
	efiAssertVoid(operationMode != OM_NONE, "operationMode not set");

	efiAssertVoid(waveIndex!= T_SECONDARY || needSecondTriggerInput, "secondary needed or not?");

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

int TriggerShape::getCycleDuration() const {
	return (operationMode == FOUR_STROKE_CAM_SENSOR) ? 720 : 360;
}

float TriggerShape::getSwitchAngle(int index) const {
	return getCycleDuration() * wave.getSwitchTime(index);
}

void multi_wave_s::checkSwitchTimes(int size) {
	checkSwitchTimes2(size, switchTimes);
}

void setToothedWheelConfiguration(TriggerShape *s, int total, int skipped,
		engine_configuration_s const *engineConfiguration) {
#if EFI_ENGINE_CONTROL

	s->useRiseEdge = true;

	initializeSkippedToothTriggerShapeExt(s, total, skipped,
			getOperationMode(engineConfiguration));
#endif
}

void TriggerShape::setTriggerSynchronizationGap2(float syncRatioFrom, float syncRatioTo) {
	isSynchronizationNeeded = true;
	this->syncRatioFrom = syncRatioFrom;
	this->syncRatioTo = syncRatioTo;
}

void TriggerShape::setTriggerSynchronizationGap(float synchRatio) {
	setTriggerSynchronizationGap2(synchRatio * 0.75f, synchRatio * 1.25f);
}

#define S24 (720.0f / 24 / 2)

static float addAccordPair(TriggerShape *s, float sb) {
	s->addEvent(sb, T_SECONDARY, TV_HIGH);
	sb += S24;
	s->addEvent(sb, T_SECONDARY, TV_LOW);
	sb += S24;

	return sb;
}

#define DIP 7.5f
static float addAccordPair3(TriggerShape *s, float sb) {
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
void configureHondaAccordCDDip(TriggerShape *s) {
	s->reset(FOUR_STROKE_CAM_SENSOR, true);

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
}

void configureHondaAccordCD(TriggerShape *s, bool with3rdSignal) {
	s->reset(FOUR_STROKE_CAM_SENSOR, true);

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
}
