/**
 * @file	trigger_structure.cpp
 *
 * @date Jan 20, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
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
	memset(&pinStates, 0, sizeof(pinStates));
	for (int i = 0; i < TRIGGER_CHANNEL_COUNT; i++) {
		waves[i].init(pinStates[i]);
	}
}

TriggerShape::TriggerShape() :
		wave(switchTimesBuffer, NULL) {
	initialize(OM_NONE, false);
	wave.waves = h.waves;
	// todo: false here, true in clear() what a mess!
	useRiseEdge = false;
	useOnlyPrimaryForSync = false;
	gapBothDirections = false;
	isSynchronizationNeeded = false;
	// todo: reuse 'clear' method?
	invertOnAdd = false;
	tdcPosition = 0;
//	skippedToothCount = totalToothCount = 0;
	syncRatioFrom = syncRatioTo = 0;
	secondSyncRatioFrom = 0.000001;
	secondSyncRatioTo = 100000;
	thirdSyncRatioFrom = 0.000001;
	thirdSyncRatioTo = 100000;
	memset(eventAngles, 0, sizeof(eventAngles));
	memset(frontOnlyIndexes, 0, sizeof(frontOnlyIndexes));
	memset(isFrontEvent, 0, sizeof(isFrontEvent));
	memset(triggerIndexByAngle, 0, sizeof(triggerIndexByAngle));
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	memset(&triggerSignals, 0, sizeof(triggerSignals));
#endif
}

int TriggerShape::getSize() const {
	return size;
}

int TriggerShape::getTriggerShapeSynchPointIndex() {
	return triggerShapeSynchPointIndex;
}

void TriggerShape::calculateTriggerSynchPoint(TriggerState *state DECLARE_ENGINE_PARAMETER_S) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	efiAssertVoid(getRemainingStack(chThdSelf()) > 256, "calc s");
#endif
	trigger_config_s const*triggerConfig = &engineConfiguration->trigger;

	triggerShapeSynchPointIndex = findTriggerZeroEventIndex(state, this, triggerConfig PASS_ENGINE_PARAMETER);

	engine->engineCycleEventCount = getLength();

	float firstAngle = getAngle(triggerShapeSynchPointIndex);

	int frontOnlyIndex = 0;

	for (int eventIndex = 0; eventIndex < engine->engineCycleEventCount; eventIndex++) {
		if (eventIndex == 0) {
			// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
			eventAngles[0] = 0;
			// this value would be used in case of front-only
			eventAngles[1] = 0;
			frontOnlyIndexes[0] = 0;
		} else {
			int triggerDefinitionCoordinate = (triggerShapeSynchPointIndex + eventIndex) % engine->engineCycleEventCount;
			int triggerDefinitionIndex = triggerDefinitionCoordinate >= size ? triggerDefinitionCoordinate - size : triggerDefinitionCoordinate;
			float angle = getAngle(triggerDefinitionCoordinate) - firstAngle;
			fixAngle(angle);
			if (engineConfiguration->useOnlyFrontForTrigger) {
				if (isFrontEvent[triggerDefinitionIndex]) {
					frontOnlyIndex += 2;
					eventAngles[frontOnlyIndex] = angle;
					eventAngles[frontOnlyIndex + 1] = angle;
				}
			} else {
				eventAngles[eventIndex] = angle;
			}

			frontOnlyIndexes[eventIndex] = frontOnlyIndex;
		}
	}
}

void TriggerShape::initialize(operation_mode_e operationMode, bool needSecondTriggerInput) {
	tdcPosition = 0;
	setTriggerSynchronizationGap(2);
	// todo: true here, false in constructor() what a mess!
	useRiseEdge = true;

	invertOnAdd = false;
	gapBothDirections = false;

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
	reset();
}

void TriggerState::reset() {
	cycleCallback = NULL;
	shaft_is_synchronized = false;
	toothed_previous_time = 0;
	toothed_previous_duration = 0;
	durationBeforePrevious = 0;
	thirdPreviousDuration = 0;

	totalRevolutionCounter = 0;
	totalTriggerErrorCounter = 0;
	orderingErrorCounter = 0;
	currentDuration = 0;
	curSignal = SHAFT_PRIMARY_DOWN;
	prevSignal = SHAFT_PRIMARY_DOWN;
	prevCycleDuration = 0;
	startOfCycleNt = 0;

	resetRunningCounters();
	resetCurrentCycleState();
	memset(expectedTotalTime, 0, sizeof(expectedTotalTime));
	memset(prevTotalTime, 0, sizeof(prevTotalTime));
	totalEventCountBase = 0;
	isFirstEvent = true;
}

int TriggerState::getCurrentIndex() {
	return currentCycle.current_index;
}

efitime_t TriggerState::getStartOfRevolutionIndex() {
	return totalEventCountBase;
}

void TriggerState::resetRunningCounters() {
	runningRevolutionCounter = 0;
	runningTriggerErrorCounter = 0;
	runningOrderingErrorCounter = 0;
}

efitime_t TriggerState::getTotalEventCounter() {
	return totalEventCountBase + currentCycle.current_index;
}

int TriggerState::getTotalRevolutionCounter() {
	return totalRevolutionCounter;
}

void TriggerState::resetCurrentCycleState() {
	memset(currentCycle.eventCount, 0, sizeof(currentCycle.eventCount));
	memset(currentCycle.timeOfPreviousEventNt, 0, sizeof(currentCycle.timeOfPreviousEventNt));
	memset(currentCycle.totalTimeNt, 0, sizeof(currentCycle.totalTimeNt));
	currentCycle.current_index = 0;
}

/**
 * Trigger event count equals engine cycle event count if we have a cam sensor.
 * Two trigger cycles make one engine cycle in case of a four stroke engine If we only have a cranksensor.
 */
uint32_t TriggerShape::getLength() const {
	return operationMode == FOUR_STROKE_CAM_SENSOR ? getSize() : 2 * getSize();
}

angle_t TriggerShape::getAngle(int index) const {
	// todo: why is this check here? looks like the code below could be used universally
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

void TriggerShape::addEvent(angle_t angle, trigger_wheel_e const waveIndex, trigger_value_e const stateParam, float filterLeft, float filterRight) {
	if (angle > filterLeft && angle < filterRight)
		addEvent(angle, waveIndex, stateParam);
}

operation_mode_e TriggerShape::getOperationMode() {
	return operationMode;
}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
extern bool printTriggerDebug;
#endif

void TriggerShape::addEvent2(angle_t angle, trigger_wheel_e const waveIndex, trigger_value_e const stateParam DECLARE_ENGINE_PARAMETER_S) {
	addEvent(angle, waveIndex, stateParam);
}

void TriggerShape::addEvent(angle_t angle, trigger_wheel_e const waveIndex, trigger_value_e const stateParam) {
	efiAssertVoid(operationMode != OM_NONE, "operationMode not set");

	efiAssertVoid(waveIndex!= T_SECONDARY || needSecondTriggerInput, "secondary needed or not?");

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	if (printTriggerDebug) {
		printf("addEvent %f\r\n", angle);
	}
#endif

	trigger_value_e state;
	if (invertOnAdd) {
		state = (stateParam == TV_FALL) ? TV_RISE : TV_FALL;
	} else {
		state = stateParam;
	}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	int signal = waveIndex * 1000 + stateParam;
	triggerSignals[size] = signal;
#endif

	float engineCycle = getEngineCycle(operationMode);

	/**
	 * While '720' value works perfectly it has not much sense for crank sensor-only scenario.
	 * todo: accept angle as a value in the 0..1 range?
	 */
	angle /= engineCycle;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
// todo: PASS_ENGINE?
	if (!engineConfiguration->useOnlyFrontForTrigger || stateParam == TV_RISE) {
		expectedEventCount[waveIndex]++;
	}
#endif

	efiAssertVoid(angle > 0, "angle should be positive");
	if (size > 0) {
		if (angle <= previousAngle) {
			firmwareError("invalid angle order: %f and %f, size=%d", angle, previousAngle, size);
			return;
		}
	}
	previousAngle = angle;
	if (size == 0) {
		size = 1;
		for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
			single_wave_s *wave = &this->wave.waves[i];

			if (wave->pinStates == NULL) {
				firmwareError("wave pinStates is NULL");
				return;
			}
			wave->pinStates[0] = initialState[i];
		}

		isFrontEvent[0] = TV_RISE == stateParam;
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
	// todo: does this logic actually work? I think it does not!
	for (int i = size - 1; i >= index; i--) {
		for (int j = 0; j < PWM_PHASE_MAX_WAVE_PER_PWM; j++) {
			wave.waves[j].pinStates[i + 1] = wave.getChannelState(j, index);
		}
		wave.setSwitchTime(i + 1, wave.getSwitchTime(i));
	}

	isFrontEvent[index] = TV_RISE == stateParam;

	if (index != size) {
		firmwareError("are we ever here?");
	}

//	int index = size;
	size++;

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		wave.waves[i].pinStates[index] = wave.getChannelState(i, index - 1);
	}
	wave.setSwitchTime(index, angle);
	wave.waves[waveIndex].pinStates[index] = state;
}

angle_t TriggerShape::getCycleDuration() const {
	return (operationMode == FOUR_STROKE_CAM_SENSOR) ? 720 : 360;
}

angle_t TriggerShape::getSwitchAngle(int index) const {
	return getCycleDuration() * wave.getSwitchTime(index);
}

void multi_wave_s::checkSwitchTimes(int size) {
	checkSwitchTimes2(size, switchTimes);
}

void setVwConfiguration(TriggerShape *s) {
	efiAssertVoid(s != NULL, "TriggerShape is NULL");
	operation_mode_e operationMode = FOUR_STROKE_CRANK_SENSOR;

	s->useRiseEdge = true;

	initializeSkippedToothTriggerShapeExt(s, 60, 2,
			operationMode);

	s->isSynchronizationNeeded = true;

	s->initialize(operationMode, false);

	int totalTeethCount = 60;
	int skippedCount = 2;

	float engineCycle = getEngineCycle(operationMode);
	float toothWidth = 0.5;

	addSkippedToothTriggerEvents(T_PRIMARY, s, 60, 2, toothWidth, 0, engineCycle,
			NO_LEFT_FILTER, 690);

	float angleDown = engineCycle / totalTeethCount * (totalTeethCount - skippedCount - 1 + (1 - toothWidth) );
	s->addEvent(0 + angleDown + 12, T_PRIMARY, TV_RISE, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEvent(0 + engineCycle, T_PRIMARY, TV_FALL, NO_LEFT_FILTER, NO_RIGHT_FILTER);

	s->setTriggerSynchronizationGap2(1.6, 4);
}

void setToothedWheelConfiguration(TriggerShape *s, int total, int skipped,
		operation_mode_e operationMode) {
#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

	s->useRiseEdge = true;

	initializeSkippedToothTriggerShapeExt(s, total, skipped,
			operationMode);
#endif
}

void TriggerShape::setTriggerSynchronizationGap2(float syncRatioFrom, float syncRatioTo) {
	isSynchronizationNeeded = true;
	this->syncRatioFrom = syncRatioFrom;
	this->syncRatioTo = syncRatioTo;
}

void TriggerShape::setTriggerSynchronizationGap(float syncRatio) {
	setTriggerSynchronizationGap2(syncRatio * 0.75f, syncRatio * 1.25f);
}

void TriggerShape::setSecondTriggerSynchronizationGap2(float syncRatioFrom, float syncRatioTo) {
	isSynchronizationNeeded = true;
	this->secondSyncRatioFrom = syncRatioFrom;
	this->secondSyncRatioTo = syncRatioTo;
}

void TriggerShape::setThirdTriggerSynchronizationGap(float syncRatio) {
	setThirdTriggerSynchronizationGap2(syncRatio * 0.75f, syncRatio * 1.25f);
}

void TriggerShape::setThirdTriggerSynchronizationGap2(float syncRatioFrom, float syncRatioTo) {
	isSynchronizationNeeded = true;
	this->thirdSyncRatioFrom = syncRatioFrom;
	this->thirdSyncRatioTo = syncRatioTo;
}

void TriggerShape::setSecondTriggerSynchronizationGap(float syncRatio) {
	setSecondTriggerSynchronizationGap2(syncRatio * 0.75f, syncRatio * 1.25f);
}

#define S24 (720.0f / 24 / 2)

static float addAccordPair(TriggerShape *s, float sb, trigger_wheel_e const waveIndex) {
	s->addEvent(sb, waveIndex, TV_RISE);
	sb += S24;
	s->addEvent(sb, waveIndex, TV_FALL);
	sb += S24;

	return sb;
}

#define DIP 7.5f
static float addAccordPair3(TriggerShape *s, float sb) {
	sb += DIP;
	s->addEvent(sb, T_CHANNEL_3, TV_RISE);
	sb += DIP;
	s->addEvent(sb, T_CHANNEL_3, TV_FALL);
	sb += 2 * DIP;
	return sb;
}

/**
 * Thank you Dip!
 * http://forum.pgmfi.org/viewtopic.php?f=2&t=15570start=210#p139007
 */
void configureHondaAccordCDDip(TriggerShape *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	s->initialState[T_SECONDARY] = TV_RISE;
	float sb = 0;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(90, T_SECONDARY, TV_FALL);
	sb = 90;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(180, T_SECONDARY, TV_RISE);
	sb = 180;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(270, T_SECONDARY, TV_FALL);
	sb = 270;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);


	s->addEvent(360.0f - DIP, T_PRIMARY, TV_RISE);
	s->addEvent(360, T_SECONDARY, TV_RISE);
	sb = 360;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(450, T_SECONDARY, TV_FALL);
	sb = 450;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(540, T_SECONDARY, TV_RISE);
	sb = 540;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(630, T_SECONDARY, TV_FALL);
	sb = 630;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(720.0f - DIP, T_PRIMARY, TV_FALL);

//	s->addEvent(720.0f - 12 * sb, T_SECONDARY, TV_FALL);
//	s->addEvent(720.0f, T_SECONDARY, TV_FALL);

	s->addEvent(720.0f, T_SECONDARY, TV_RISE);

	s->isSynchronizationNeeded = false;
}

void configureHondaAccordCD(TriggerShape *s, bool withOneEventSignal, bool withFourEventSignal,
		trigger_wheel_e const oneEventWave,
		trigger_wheel_e const fourEventWave,
		float prefix) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

//	trigger_wheel_e const oneEventWave = T_CHANNEL_3;
//	bool withFourEventSignal = true;
//	trigger_wheel_e const fourEventWave = T_PRIMARY;

	float sb = 5.0f + prefix;

	float tdcWidth = 0.1854 * 720 / 4;

	s->isSynchronizationNeeded = false;

	sb = addAccordPair(s, sb, T_SECONDARY);

	if (withOneEventSignal)
		s->addEvent(sb - S24 / 2, oneEventWave, TV_RISE);

	sb = addAccordPair(s, sb, T_SECONDARY);
	sb = addAccordPair(s, sb, T_SECONDARY);
	if (withOneEventSignal)
		s->addEvent(sb - S24 / 2, oneEventWave, TV_FALL);
	sb = addAccordPair(s, sb, T_SECONDARY);
	sb = addAccordPair(s, sb, T_SECONDARY);
	if (withFourEventSignal) {
		s->addEvent(1 * 180.0f + prefix - tdcWidth, fourEventWave, TV_RISE);
	}
	sb = addAccordPair(s, sb, T_SECONDARY);
	if (withFourEventSignal) {
		s->addEvent(1 * 180.0f + prefix, fourEventWave, TV_FALL);
	}

	sb = addAccordPair(s, sb, T_SECONDARY);
	sb = addAccordPair(s, sb,T_SECONDARY);
	sb = addAccordPair(s, sb, T_SECONDARY);
	sb = addAccordPair(s, sb, T_SECONDARY);
	sb = addAccordPair(s, sb, T_SECONDARY);

	if (withFourEventSignal) {
		s->addEvent(2 * 180.0f + prefix - tdcWidth, fourEventWave, TV_RISE);
	}
	sb = addAccordPair(s, sb, T_SECONDARY);
	if (withFourEventSignal) {
		s->addEvent(2 * 180.0f + prefix, fourEventWave, TV_FALL);
	}

	for (int i = 3; i <= 4; i++) {
		sb = addAccordPair(s, sb, T_SECONDARY);
		sb = addAccordPair(s, sb, T_SECONDARY);
		sb = addAccordPair(s, sb, T_SECONDARY);
		sb = addAccordPair(s, sb, T_SECONDARY);
		sb = addAccordPair(s, sb, T_SECONDARY);

		if (withFourEventSignal) {
			s->addEvent(i * 180.0f + prefix - tdcWidth, fourEventWave, TV_RISE);
		}
		sb = addAccordPair(s, sb, T_SECONDARY);
		if (withFourEventSignal) {
			s->addEvent(i * 180.0f + prefix, fourEventWave, TV_FALL);
		}
	}
}
