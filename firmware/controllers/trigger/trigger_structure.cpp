/**
 * @file	trigger_structure.cpp
 *
 * @date Jan 20, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
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


#include "global.h"
#include "trigger_structure.h"
#include "trigger_decoder.h"
#include "engine_math.h"
#include "trigger_universal.h"
#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
#include "sensor_chart.h"
#endif /* EFI_SENSOR_CHART */

EXTERN_ENGINE;

trigger_shape_helper::trigger_shape_helper() {
	memset(&pinStates, 0, sizeof(pinStates));
	for (int channelIndex = 0; channelIndex < TRIGGER_CHANNEL_COUNT; channelIndex++) {
		channels[channelIndex].init(pinStates[channelIndex]);
	}
}

TriggerShape::TriggerShape() :
		wave(switchTimesBuffer, NULL) {
	version = 0;
	initialize(OM_NONE, false);
	wave.channels = h.channels;

	memset(triggerIndexByAngle, 0, sizeof(triggerIndexByAngle));
}

void TriggerShape::calculateTriggerSynchPoint(TriggerState *state DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	efiAssertVoid(CUSTOM_ERR_6642, getRemainingStack(chThdGetSelfX()) > 256, "calc s");
#endif
	trigger_config_s const*triggerConfig = &engineConfiguration->trigger;

	triggerShapeSynchPointIndex = findTriggerZeroEventIndex(state, this, triggerConfig PASS_ENGINE_PARAMETER_SUFFIX);

	int length = getLength();
	engine->engineCycleEventCount = length;
	efiAssertVoid(CUSTOM_SHAPE_LEN_ZERO, length > 0, "shapeLength=0");
	if (length >= PWM_PHASE_MAX_COUNT) {
		warning(CUSTOM_ERR_TRIGGER_SHAPE_TOO_LONG, "Count above %d", length);
		shapeDefinitionError = true;
		return;
	}

	float firstAngle = getAngle(triggerShapeSynchPointIndex);
	assertAngleRange(triggerShapeSynchPointIndex, "firstAngle", CUSTOM_ERR_6551);

	int frontOnlyIndex = 0;

	for (int eventIndex = 0; eventIndex < length; eventIndex++) {
		if (eventIndex == 0) {
			// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
			eventAngles[0] = 0;
			// this value would be used in case of front-only
			eventAngles[1] = 0;
			frontOnlyIndexes[0] = 0;
		} else {
			assertAngleRange(triggerShapeSynchPointIndex, "triggerShapeSynchPointIndex", CUSTOM_ERR_6552);
			int triggerDefinitionCoordinate = (triggerShapeSynchPointIndex + eventIndex) % engine->engineCycleEventCount;
			efiAssertVoid(CUSTOM_ERR_6595, engine->engineCycleEventCount != 0, "zero engineCycleEventCount");
			int triggerDefinitionIndex = triggerDefinitionCoordinate >= privateTriggerDefinitionSize ? triggerDefinitionCoordinate - privateTriggerDefinitionSize : triggerDefinitionCoordinate;
			float angle = getAngle(triggerDefinitionCoordinate) - firstAngle;
			efiAssertVoid(CUSTOM_ERR_6596, !cisnan(angle), "trgSyncNaN");
			fixAngle(angle, "trgSync", CUSTOM_ERR_6559);
			if (engineConfiguration->useOnlyRisingEdgeForTrigger) {
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
	isSynchronizationNeeded = true; // that's default value
	this->needSecondTriggerInput = needSecondTriggerInput;
	memset(expectedDutyCycle, 0, sizeof(expectedDutyCycle));
	memset(eventAngles, 0, sizeof(eventAngles));
//	memset(triggerIndexByAngle, 0, sizeof(triggerIndexByAngle));

	setTriggerSynchronizationGap(2);
	for (int index = 1; index < GAP_TRACKING_LENGTH ; index++) {
		// NaN means do not use this gap ratio
		setTriggerSynchronizationGap3(index, NAN, 100000);
	}

	tdcPosition = 0;
	shapeDefinitionError = useOnlyPrimaryForSync = false;
	useRiseEdge = true;
	gapBothDirections = false;

	invertOnAdd = false;

	this->operationMode = operationMode;
	privateTriggerDefinitionSize = 0;
	triggerShapeSynchPointIndex = 0;
	memset(initialState, 0, sizeof(initialState));
	memset(switchTimesBuffer, 0, sizeof(switchTimesBuffer));
	memset(expectedEventCount, 0, sizeof(expectedEventCount));
	wave.reset();
	previousAngle = 0;
	memset(frontOnlyIndexes, 0, sizeof(frontOnlyIndexes));
	memset(isFrontEvent, 0, sizeof(isFrontEvent));
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	memset(&triggerSignals, 0, sizeof(triggerSignals));
#endif
}

int TriggerShape::getSize() const {
	return privateTriggerDefinitionSize;
}

int TriggerShape::getTriggerShapeSynchPointIndex() {
	return triggerShapeSynchPointIndex;
}

efitime_t TriggerState::getStartOfRevolutionIndex() {
	return totalEventCountBase;
}

void TriggerState::resetRunningCounters() {
	runningRevolutionCounter = 0;
	runningTriggerErrorCounter = 0;
	runningOrderingErrorCounter = 0;
}

void TriggerState::runtimeStatistics(efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// empty base implementation
}

TriggerStateWithRunningStatistics::TriggerStateWithRunningStatistics() {
	instantRpm = 0;
	prevInstantRpmValue = 0;
	// avoid ill-defined instant RPM when the data is not gathered yet
	efitime_t nowNt = getTimeNowNt();
	for (int i = 0; i < PWM_PHASE_MAX_COUNT; i++) {
		timeOfLastEvent[i] = nowNt;
	}
}

float TriggerStateWithRunningStatistics::calculateInstantRpm(int *prevIndex, efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	int current_index = currentCycle.current_index; // local copy so that noone changes the value on us
	/**
	 * Here we calculate RPM based on last 90 degrees
	 */
	angle_t currentAngle = TRIGGER_SHAPE(eventAngles[current_index]);
	// todo: make this '90' depend on cylinder count or trigger shape?
	angle_t previousAngle = currentAngle - 90;
	fixAngle(previousAngle, "prevAngle", CUSTOM_ERR_6560);
	// todo: prevIndex should be pre-calculated
	*prevIndex = TRIGGER_SHAPE(triggerIndexByAngle[(int)previousAngle]);

	// now let's get precise angle for that event
	angle_t prevIndexAngle = TRIGGER_SHAPE(eventAngles[*prevIndex]);
	uint32_t time = nowNt - timeOfLastEvent[*prevIndex];
	angle_t angleDiff = currentAngle - prevIndexAngle;
	// todo: angle diff should be pre-calculated
	fixAngle(angleDiff, "angleDiff", CUSTOM_ERR_6561);

	// just for safety
	if (time == 0)
		return prevInstantRpmValue;

	float instantRpm = (60000000.0 / 360 * US_TO_NT_MULTIPLIER) * angleDiff / time;
	instantRpmValue[current_index] = instantRpm;
	timeOfLastEvent[current_index] = nowNt;

	// This fixes early RPM instability based on incomplete data
	if (instantRpm < RPM_LOW_THRESHOLD)
		return prevInstantRpmValue;
	prevInstantRpmValue = instantRpm;

	return instantRpm;
}

void TriggerStateWithRunningStatistics::setLastEventTimeForInstantRpm(efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	timeOfLastEvent[currentCycle.current_index] = nowNt;
}

void TriggerStateWithRunningStatistics::runtimeStatistics(efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (engineConfiguration->debugMode == DBG_INSTANT_RPM) {
		int prevIndex;
		instantRpm = calculateInstantRpm(&prevIndex, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
	}
	if (ENGINE(sensorChartMode) == SC_RPM_ACCEL || ENGINE(sensorChartMode) == SC_DETAILED_RPM) {
		int prevIndex;
		instantRpm = calculateInstantRpm(&prevIndex, nowNt PASS_ENGINE_PARAMETER_SUFFIX);

#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
		angle_t currentAngle = TRIGGER_SHAPE(eventAngles[currentCycle.current_index]);
		if (boardConfiguration->sensorChartMode == SC_DETAILED_RPM) {
			scAddData(currentAngle, instantRpm);
		} else {
			scAddData(currentAngle, instantRpm / instantRpmValue[prevIndex]);
		}
#endif /* EFI_SENSOR_CHART */
	}
}

efitime_t TriggerState::getTotalEventCounter() {
	return totalEventCountBase + currentCycle.current_index;
}

int TriggerState::getTotalRevolutionCounter() {
	return totalRevolutionCounter;
}


/**
 * physical primary trigger duration
 */
angle_t TriggerShape::getCycleDuration() const {
	switch (operationMode) {
	case FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR:
		return 180;
	case FOUR_STROKE_CRANK_SENSOR:
	case TWO_STROKE:
		return 360;
	default:
		return 720;
	}
}

/**
 * Trigger event count equals engine cycle event count if we have a cam sensor.
 * Two trigger cycles make one engine cycle in case of a four stroke engine If we only have a cranksensor.
 */
uint32_t TriggerShape::getLength() const {
	/**
	 * 4 for FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR
	 * 2 for FOUR_STROKE_CRANK_SENSOR
	 * 1 otherwise
	 */
	int multiplier = getEngineCycle(operationMode) / getCycleDuration();
	return multiplier * getSize();
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
	efiAssert(CUSTOM_ERR_ASSERT, privateTriggerDefinitionSize != 0, "shapeSize=0", NAN);
	int crankCycle = index / privateTriggerDefinitionSize;
	int remainder = index % privateTriggerDefinitionSize;

	return getCycleDuration() * crankCycle + getSwitchAngle(remainder);
}

void TriggerShape::addEvent3(angle_t angle, trigger_wheel_e const channelIndex, trigger_value_e const stateParam, float filterLeft, float filterRight DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (angle > filterLeft && angle < filterRight)
		addEvent2(angle, channelIndex, stateParam PASS_ENGINE_PARAMETER_SUFFIX);
}

operation_mode_e TriggerShape::getOperationMode() {
	return operationMode;
}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
extern bool printTriggerDebug;
#endif

void TriggerShape::calculateExpectedEventCounts(bool useOnlyRisingEdgeForTrigger) {
// todo: move the following logic from below here
	//	if (!useOnlyRisingEdgeForTrigger || stateParam == TV_RISE) {
//		expectedEventCount[channelIndex]++;
//	}

}

/**
 * Deprecated - see https://github.com/rusefi/rusefi/issues/635
 */
void TriggerShape::addEvent2(angle_t angle, trigger_wheel_e const channelIndex, trigger_value_e const stateParam DECLARE_ENGINE_PARAMETER_SUFFIX) {
	/**
	 * While '720' value works perfectly it has not much sense for crank sensor-only scenario.
	 */
	addEvent(engineConfiguration->useOnlyRisingEdgeForTrigger, angle / getEngineCycle(operationMode), channelIndex, stateParam);
}

// todo: the whole 'useOnlyRisingEdgeForTrigger' parameter and logic should not be here
// todo: see calculateExpectedEventCounts
// related calculation should be done once trigger is initialized outside of trigger shape scope
void TriggerShape::addEvent(bool useOnlyRisingEdgeForTrigger, angle_t angle, trigger_wheel_e const channelIndex, trigger_value_e const stateParam) {

	efiAssertVoid(CUSTOM_OMODE_UNDEF, operationMode != OM_NONE, "operationMode not set");

	efiAssertVoid(CUSTOM_ERR_6598, channelIndex!= T_SECONDARY || needSecondTriggerInput, "secondary needed or not?");

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	if (printTriggerDebug) {
		printf("addEvent2 %.2f i=%d r/f=%d\r\n", angle, channelIndex, stateParam);
	}
#endif

	trigger_value_e state;
	if (invertOnAdd) {
		state = (stateParam == TV_FALL) ? TV_RISE : TV_FALL;
	} else {
		state = stateParam;
	}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	int signal = channelIndex * 1000 + stateParam;
	triggerSignals[privateTriggerDefinitionSize] = signal;
#endif


	if (!useOnlyRisingEdgeForTrigger || stateParam == TV_RISE) {
		expectedEventCount[channelIndex]++;
	}

	efiAssertVoid(CUSTOM_ERR_6599, angle > 0, "angle should be positive");
	if (privateTriggerDefinitionSize > 0) {
		if (angle <= previousAngle) {
			warning(CUSTOM_ERR_TRG_ANGLE_ORDER, "invalid angle order: new=%.2f and prev=%.2f, size=%d", angle, previousAngle, privateTriggerDefinitionSize);
			shapeDefinitionError = true;
			return;
		}
	}
	previousAngle = angle;
	if (privateTriggerDefinitionSize == 0) {
		privateTriggerDefinitionSize = 1;
		for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
			SingleWave *wave = &this->wave.channels[i];

			if (wave->pinStates == NULL) {
				warning(CUSTOM_ERR_STATE_NULL, "wave pinStates is NULL");
				shapeDefinitionError = true;
				return;
			}
			wave->setState(/* channelIndex */ 0, /* value */ initialState[i]);
		}

		isFrontEvent[0] = TV_RISE == stateParam;
		wave.setSwitchTime(0, angle);
		wave.channels[channelIndex].setState(/* channelIndex */ 0, /* value */ state);
		return;
	}

	int exactMatch = wave.findAngleMatch(angle, privateTriggerDefinitionSize);
	if (exactMatch != EFI_ERROR_CODE) {
		warning(CUSTOM_ERR_SAME_ANGLE, "same angle: not supported");
		shapeDefinitionError = true;
		return;
	}

	int index = wave.findInsertionAngle(angle, privateTriggerDefinitionSize);

	/**
	 * todo: it would be nice to be able to provide trigger angles without sorting them externally
	 * The idea here is to shift existing data - including handling high vs low state of the signals
	 */
	// todo: does this logic actually work? I think it does not! due to broken state handling
/*
	for (int i = size - 1; i >= index; i--) {
		for (int j = 0; j < PWM_PHASE_MAX_WAVE_PER_PWM; j++) {
			wave.waves[j].pinStates[i + 1] = wave.getChannelState(j, index);
		}
		wave.setSwitchTime(i + 1, wave.getSwitchTime(i));
	}
*/
	isFrontEvent[index] = TV_RISE == stateParam;

	if (index != privateTriggerDefinitionSize) {
		firmwareError(ERROR_TRIGGER_DRAMA, "are we ever here?");
	}

	privateTriggerDefinitionSize++;

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		int value = wave.getChannelState(/* channelIndex */i, index - 1);
		wave.channels[i].setState(index, value);
	}
	wave.setSwitchTime(index, angle);
	wave.channels[channelIndex].setState(index, state);
}

angle_t TriggerShape::getSwitchAngle(int index) const {
	return getCycleDuration() * wave.getSwitchTime(index);
}

void setToothedWheelConfiguration(TriggerShape *s, int total, int skipped,
		operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

	s->useRiseEdge = true;

	initializeSkippedToothTriggerShapeExt(s, total, skipped,
			operationMode PASS_ENGINE_PARAMETER_SUFFIX);
#endif
}

void TriggerShape::setTriggerSynchronizationGap2(float syncRatioFrom, float syncRatioTo) {
	setTriggerSynchronizationGap3(0, syncRatioFrom, syncRatioTo);
}

void TriggerShape::setTriggerSynchronizationGap3(int index, float syncRatioFrom, float syncRatioTo) {
	isSynchronizationNeeded = true;
	this->syncronizationRatioFrom[index] = syncRatioFrom;
	this->syncronizationRatioTo[index] = syncRatioTo;
	if (index == 0) {
		// we have a special case here - only sync with one gap has this feature
		this->syncRatioAvg = (int)efiRound((syncRatioFrom + syncRatioTo) * 0.5f, 1.0f);
	}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	if (printTriggerDebug) {
		printf("setTriggerSynchronizationGap3 %d %.2f %.2f\r\n", index, syncRatioFrom, syncRatioTo);
	}
#endif /* EFI_UNIT_TEST */

}

void TriggerShape::setTriggerSynchronizationGap(float syncRatio) {
	setTriggerSynchronizationGap3(0, syncRatio * 0.75f, syncRatio * 1.25f);
}

void TriggerShape::setSecondTriggerSynchronizationGap2(float syncRatioFrom, float syncRatioTo) {
	setTriggerSynchronizationGap3(1, syncRatioFrom, syncRatioTo);
}

void TriggerShape::setThirdTriggerSynchronizationGap(float syncRatio) {
	setTriggerSynchronizationGap3(2, syncRatio * 0.75f, syncRatio * 1.25f);
}

void TriggerShape::setSecondTriggerSynchronizationGap(float syncRatio) {
	setTriggerSynchronizationGap3(1, syncRatio * 0.75f, syncRatio * 1.25f);
}
