/**
 * @file	trigger_structure.cpp
 *
 * @date Jan 20, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
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

#include "pch.h"

#include "trigger_universal.h"
#include "trigger_structure.h"

#include "engine_state.h"

void wrapAngle(angle_t& angle, const char* msg, ObdCode code) {
	if (std::isnan(angle)) {
		firmwareError(ObdCode::CUSTOM_ERR_ANGLE, "a NaN %s", msg);
		angle = 0;
	}

	assertAngleRange(angle, msg, code);
	float engineCycle = getEngineState()->engineCycle;

	while (angle < 0) {
		angle += engineCycle;
	}

	while (angle >= engineCycle) {
		angle -= engineCycle;
	}
}

TriggerWaveform::TriggerWaveform() {
	initialize(OM_NONE, SyncEdge::Rise);
}

void TriggerWaveform::initialize(operation_mode_e p_operationMode, SyncEdge p_syncEdge) {
	operationMode = p_operationMode;
	syncEdge = p_syncEdge;

	// These defaults are also encoded in TriggerWheelInfo.java
	// If they are changed here, they also need to be changed there.
	isSynchronizationNeeded = true;
	isSecondWheelCam = false;
	needSecondTriggerInput = false;
	shapeWithoutTdc = false;
	useOnlyPrimaryForSync = false;
	knownOperationMode = true;

	// If RiseOnly, ignore falling edges completely.
	useOnlyRisingEdges = syncEdge == SyncEdge::RiseOnly;

	setTriggerSynchronizationGap(2);
	for (int gapIndex = 1; gapIndex < GAP_TRACKING_LENGTH ; gapIndex++) {
		// NaN means do not use this gap ratio
		setTriggerSynchronizationGap3(gapIndex, NAN, 100000);
	}
	gapTrackingLength = 1;

	tdcPosition = 0;
	shapeDefinitionError = false;

	triggerShapeSynchPointIndex = 0;
	setArrayValues(expectedEventCount, 0);
	wave.reset();
	wave.waveCount = TRIGGER_INPUT_PIN_COUNT;
	wave.phaseCount = 0;
	previousAngle = 0;
	setArrayValues(isRiseEvent, 0);
#if EFI_UNIT_TEST
	memset(triggerSignalIndeces, 0, sizeof(triggerSignalIndeces));
	memset(&triggerSignalStates, 0, sizeof(triggerSignalStates));
#endif // EFI_UNIT_TEST
}

size_t TriggerWaveform::getSize() const {
	return wave.phaseCount;
}

int TriggerWaveform::getTriggerWaveformSynchPointIndex() const {
	return triggerShapeSynchPointIndex;
}

/**
 * physical primary trigger duration
 * @see getEngineCycle
 * @see getCrankDivider
 */
angle_t TriggerWaveform::getCycleDuration() const {
	switch (operationMode) {
	case FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR:
		return FOUR_STROKE_CYCLE_DURATION / SYMMETRICAL_CRANK_SENSOR_DIVIDER;
	case FOUR_STROKE_THREE_TIMES_CRANK_SENSOR:
		return FOUR_STROKE_CYCLE_DURATION / SYMMETRICAL_THREE_TIMES_CRANK_SENSOR_DIVIDER;
	case FOUR_STROKE_SIX_TIMES_CRANK_SENSOR:
		return FOUR_STROKE_CYCLE_DURATION / SYMMETRICAL_SIX_TIMES_CRANK_SENSOR_DIVIDER;
	case FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR:
		return FOUR_STROKE_CYCLE_DURATION / SYMMETRICAL_TWELVE_TIMES_CRANK_SENSOR_DIVIDER;
	case FOUR_STROKE_CRANK_SENSOR:
	case TWO_STROKE:
		return TWO_STROKE_CYCLE_DURATION;
	case OM_NONE:
  case FOUR_STROKE_CAM_SENSOR:
		return FOUR_STROKE_CYCLE_DURATION;
	}
	criticalError("unreachable getCycleDuration");
	return 0;
}

bool TriggerWaveform::needsDisambiguation() const {
	switch (getWheelOperationMode()) {
		case FOUR_STROKE_CRANK_SENSOR:
		case FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR:
		case FOUR_STROKE_THREE_TIMES_CRANK_SENSOR:
		case FOUR_STROKE_SIX_TIMES_CRANK_SENSOR:
		case FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR:
			return true;
		case FOUR_STROKE_CAM_SENSOR:
		case TWO_STROKE:
		case OM_NONE:
			return false;
	/* let's NOT handle default in order to benefit from -Werror=switch	*/
	}
	criticalError("unreachable needsDisambiguation");
	return true;
}

/**
 * Trigger event count equals engine cycle event count if we have a cam sensor.
 * Two trigger cycles make one engine cycle in case of a four stroke engine If we only have a cranksensor.
 *
 * 'engine->engineCycleEventCount' hold a pre-calculated copy of this value as a performance optimization
 */
size_t TriggerWaveform::getLength() const {
	/**
	 * 24 for FOUR_STROKE_TWELVE_TIMES_CRANK_SENSOR
	 * 6 for FOUR_STROKE_THREE_TIMES_CRANK_SENSOR
	 * 4 for FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR
	 * 2 for FOUR_STROKE_CRANK_SENSOR
	 * 1 otherwise
	 */
	int multiplier = getEngineCycle(operationMode) / getCycleDuration();
	return multiplier * getSize();
}

angle_t TriggerWaveform::getAngle(int index) const {
	/**
	 * FOUR_STROKE_CRANK_SENSOR magic:
	 * We have two crank shaft revolutions for each engine cycle
	 * See also trigger_central.cpp
	 * See also getEngineCycleEventCount()
	 */
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, wave.phaseCount != 0, "shapeSize=0", NAN);
	int crankCycle = index / wave.phaseCount;
	int remainder = index % wave.phaseCount;

	auto cycleStartAngle = getCycleDuration() * crankCycle;
	auto positionWithinCycle = getSwitchAngle(remainder);

	return cycleStartAngle + positionWithinCycle;
}

void TriggerWaveform::addEventClamped(angle_t angle, TriggerValue const stateParam, TriggerWheel const channelIndex, float filterLeft, float filterRight) {
	if (angle > filterLeft && angle < filterRight) {
#if EFI_UNIT_TEST
//		printf("addEventClamped %f %s\r\n", angle, getTrigger_value_e(stateParam));
#endif /* EFI_UNIT_TEST */
		addEvent(angle / getEngineCycle(operationMode), stateParam, channelIndex);
	}
}

/**
 * See also Engine#getOperationMode which accounts for additional settings which are
 * needed to resolve precise mode for vague wheels
 */
operation_mode_e TriggerWaveform::getWheelOperationMode() const {
	return operationMode;
}

/**
 * At the time of this writing, this only differs from getWheelOperationMode for 36-2-2-2,
 * which is initialized as FOUR_STROKE_CRANK_SENSOR, but we pretend it's not when used on a rotary.
 */
operation_mode_e TriggerWaveform::getAmbiguousOperationMode() {
	if (!knownOperationMode) {
		return lookupOperationMode();
	}
	return operationMode;
}

#if EFI_UNIT_TEST
extern bool printTriggerDebug;
#endif

size_t TriggerWaveform::getExpectedEventCount(TriggerWheel channelIndex) const {
	return expectedEventCount[(int)channelIndex];
}

void TriggerWaveform::calculateExpectedEventCounts() {
	if (!useOnlyRisingEdges) {
		for (size_t i = 0; i < efi::size(expectedEventCount); i++) {
			if (getExpectedEventCount((TriggerWheel)i) % 2 != 0) {
				firmwareError(ObdCode::ERROR_TRIGGER_DRAMA, "Trigger: should be even number of events index=%d count=%d", i, getExpectedEventCount((TriggerWheel)i));
			}
		}
	}

	bool isSingleToothOnPrimaryChannel = useOnlyRisingEdges ? getExpectedEventCount(TriggerWheel::T_PRIMARY) == 1 : getExpectedEventCount(TriggerWheel::T_PRIMARY) == 2;
	// todo: next step would be to set 'isSynchronizationNeeded' automatically based on the logic we have here
	if (!shapeWithoutTdc && isSingleToothOnPrimaryChannel != !isSynchronizationNeeded) {
		firmwareError(ObdCode::ERROR_TRIGGER_DRAMA, "shapeWithoutTdc isSynchronizationNeeded isSingleToothOnPrimaryChannel constraint violation");
	}
	if (isSingleToothOnPrimaryChannel) {
		useOnlyPrimaryForSync = true;
	} else {
		if (getExpectedEventCount(TriggerWheel::T_SECONDARY) == 0 && useOnlyPrimaryForSync) {
			firmwareError(ObdCode::ERROR_TRIGGER_DRAMA, "why would you set useOnlyPrimaryForSync with only one trigger wheel?");
		}
	}

// todo: move the following logic from below here
	//	if (!useOnlyRisingEdgeForTrigger || stateParam == TriggerValue::RISE) {
//		expectedEventCount[channelIndex]++;
//	}

}

/**
 * See header for documentation
 */
void TriggerWaveform::addEvent720(angle_t angle, TriggerValue const state, TriggerWheel const channelIndex) {
	addEvent(angle / FOUR_STROKE_CYCLE_DURATION, state, channelIndex);
}

void TriggerWaveform::addEvent360(angle_t angle, TriggerValue const state, TriggerWheel const channelIndex) {
	efiAssertVoid(ObdCode::CUSTOM_OMODE_UNDEF, operationMode == FOUR_STROKE_CAM_SENSOR || operationMode == FOUR_STROKE_CRANK_SENSOR, "Not a mode for 360");
#define CRANK_MODE_MULTIPLIER 2.0f
	addEvent(CRANK_MODE_MULTIPLIER * angle / FOUR_STROKE_CYCLE_DURATION, state, channelIndex);
}

void TriggerWaveform::addToothRiseFall(angle_t angle, angle_t width, TriggerWheel const channelIndex) {
    addEvent360(angle - width, TriggerValue::RISE, channelIndex);
    addEvent360(angle, TriggerValue::FALL, channelIndex);
}

void TriggerWaveform::addToothFallRise(angle_t angle, angle_t width, TriggerWheel const channelIndex) {
    addEvent360(angle - width, TriggerValue::FALL, channelIndex);
    addEvent360(angle, TriggerValue::RISE, channelIndex);
}

void TriggerWaveform::addEventAngle(angle_t angle, TriggerValue const state, TriggerWheel const channelIndex) {
	addEvent(angle / getCycleDuration(), state, channelIndex);
}

void TriggerWaveform::addEvent(angle_t angle, TriggerValue const state, TriggerWheel const channelIndex) {
	efiAssertVoid(ObdCode::CUSTOM_OMODE_UNDEF, operationMode != OM_NONE, "operationMode not set");

	if (channelIndex == TriggerWheel::T_SECONDARY) {
		needSecondTriggerInput = true;
	}

#if EFI_UNIT_TEST
	if (printTriggerDebug) {
		printf("addEvent2 %.2f i=%d front=%d\r\n", angle, channelIndex, state);
	}
#endif

#if EFI_UNIT_TEST
	assertIsInBounds(wave.phaseCount, triggerSignalIndeces, "trigger shape overflow");
	triggerSignalIndeces[wave.phaseCount] = channelIndex;
	triggerSignalStates[wave.phaseCount] = state;
#endif // EFI_UNIT_TEST


	// todo: the whole 'useOnlyRisingEdgeForTrigger' parameter and logic should not be here
	// todo: see calculateExpectedEventCounts
	// related calculation should be done once trigger is initialized outside of trigger shape scope
	if (!useOnlyRisingEdges || state == TriggerValue::RISE) {
		expectedEventCount[(int)channelIndex]++;
	}

	if (angle <= 0 || angle > 1) {
		firmwareError(ObdCode::CUSTOM_ERR_6599, "angle should be positive not above 1: index=%d angle %f", (int)channelIndex, angle);
		return;
	}
	if (wave.phaseCount > 0) {
		if (angle <= previousAngle) {
			warning(ObdCode::CUSTOM_ERR_TRG_ANGLE_ORDER, "invalid angle order %s %s: new=%.2f/%f and prev=%.2f/%f, size=%d",
					getTriggerWheel(channelIndex),
					getTrigger_value_e(state),
					angle, angle * getCycleDuration(),
					previousAngle, previousAngle * getCycleDuration(),
					wave.phaseCount);
			setShapeDefinitionError(true);
			return;
		}
	}
	previousAngle = angle;
	if (wave.phaseCount == 0) {
		wave.phaseCount = 1;
		for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
			wave.setChannelState(i, /* switchIndex */ 0, TriggerValue::FALL);
		}

		isRiseEvent[0] = TriggerValue::RISE == state;
		wave.setSwitchTime(0, angle);
		wave.setChannelState((int)channelIndex, /* channelIndex */ 0, /* value */ state);
		return;
	}

	if (wave.findAngleMatch(angle)) {
		warning(ObdCode::CUSTOM_ERR_SAME_ANGLE, "same angle: not supported");
		setShapeDefinitionError(true);
		return;
	}

	int index = wave.findInsertionAngle(angle);

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
	isRiseEvent[index] = TriggerValue::RISE == state;

	if ((unsigned)index != wave.phaseCount) {
		firmwareError(ObdCode::ERROR_TRIGGER_DRAMA, "are we ever here?");
	}

	wave.phaseCount++;

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		pin_state_t value = wave.getChannelState(/* channelIndex */i, index - 1);
		wave.setChannelState(i, index, value);
	}
	wave.setSwitchTime(index, angle);
	wave.setChannelState((int)channelIndex, index, state);
}

angle_t TriggerWaveform::getSwitchAngle(int index) const {
	return getCycleDuration() * wave.getSwitchTime(index);
}

void TriggerWaveform::setTriggerSynchronizationGap2(float syncRatioFrom, float syncRatioTo) {
	setTriggerSynchronizationGap3(/*gapIndex*/0, syncRatioFrom, syncRatioTo);
}

void TriggerWaveform::setTriggerSynchronizationGap3(int gapIndex, float syncRatioFrom, float syncRatioTo) {
	isSynchronizationNeeded = true;
	criticalAssertVoid(gapIndex >= 0 && gapIndex < GAP_TRACKING_LENGTH, "gapIndex out of range");
	synchronizationRatioFrom[gapIndex] = syncRatioFrom;
	synchronizationRatioTo[gapIndex] = syncRatioTo;
	if (gapIndex == 0) {
		// we have a special case here - only sync with one gap has this feature
		this->syncRatioAvg = (int)efiRound((syncRatioFrom + syncRatioTo) * 0.5f, 1.0f);
	}
	gapTrackingLength = maxI(1 + gapIndex, gapTrackingLength);

#if EFI_UNIT_TEST
	if (printTriggerDebug) {
		printf("setTriggerSynchronizationGap3 %d %.2f %.2f\r\n", gapIndex, syncRatioFrom, syncRatioTo);
	}
#endif /* EFI_UNIT_TEST */

}

uint16_t TriggerWaveform::findAngleIndex(TriggerFormDetails *details, angle_t targetAngle) const {
	size_t engineCycleEventCount = getLength();

	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, engineCycleEventCount != 0 && engineCycleEventCount <= 0xFFFF,
		  "engineCycleEventCount", 0);

	uint32_t left = 0;
	uint32_t right = engineCycleEventCount - 1;

	/**
	 * Let's find the last trigger angle which is less or equal to the desired angle
	 * todo: extract binary search as template method?
	 */
	do {
		uint32_t middle = (left + right) / 2;

		if (details->eventAngles[middle] <= targetAngle) {
			left = middle + 1;
		} else {
			right = middle - 1;
		}
	} while (left <= right);
	left -= 1;
	if (useOnlyRisingEdges) {
		left &= ~1U;
	}
	return left;
}

TriggerWheel TriggerWaveform::getWheel(size_t index) {
#if EFI_UNIT_TEST
	return triggerSignalIndeces[index];
#else
  UNUSED(index);
#endif
	return TriggerWheel::T_PRIMARY;
}

void TriggerWaveform::setShapeDefinitionError(bool value) {
	shapeDefinitionError = value;
}

void TriggerWaveform::setTriggerSynchronizationGap(float syncRatio) {
	setTriggerSynchronizationGap4(/*gapIndex*/0, syncRatio);
}

void TriggerWaveform::setSecondTriggerSynchronizationGap(float syncRatio) {
	setTriggerSynchronizationGap4(/*gapIndex*/1, syncRatio);
}

void TriggerWaveform::setSecondTriggerSynchronizationGap2(float syncRatioFrom, float syncRatioTo) {
	setTriggerSynchronizationGap3(/*gapIndex*/1, syncRatioFrom, syncRatioTo);
}

void TriggerWaveform::setThirdTriggerSynchronizationGap(float syncRatio) {
	setTriggerSynchronizationGap4(/*gapIndex*/2, syncRatio);
}

PUBLIC_API_WEAK void customTrigger(operation_mode_e triggerOperationMode, TriggerWaveform *s, trigger_type_e type) {
  if (type == trigger_type_e::TT_CUSTOM_1 || type == trigger_type_e::TT_CUSTOM_2) {
    initializeSkippedToothTrigger(s, 1, 0, triggerOperationMode, SyncEdge::Rise);
    return;
  }
		s->setShapeDefinitionError(true);
		warning(ObdCode::CUSTOM_ERR_NO_SHAPE, "initializeTriggerWaveform() not implemented: %d", type);
}

void TriggerWaveform::setEvents(const ComplexTriggerSignal* events, size_t length) {
	// The ComplexTriggerSignal list passed should always begin at angle zero.
	// Within a given channel, events passed should have no overlap,
	// but there can be overlap between different channels.
	// Instead of adding a fall just after adding its matching rise,
	// we will store it, and check before adding the next rise whether or not it needs to be added.
	ComplexTriggerSignal pendingFall[2] = {{TriggerWheel::T_PRIMARY, 0.0f, 0.0f, 0, 0.0f}, {TriggerWheel::T_PRIMARY, 0.0f, 0.0f, 0, 0.0f}};
	angle_t full = operationMode == FOUR_STROKE_CAM_SENSOR ? 360 : getCycleDuration();
  for (size_t i = 0; i < length; i++) {
    ComplexTriggerSignal e = events[i];
		for (int t = 0; t < e.count; t++) {
			double mult = (e.count > 1) ? (t / (e.count - 1.0)) : 0.0;
			angle_t angle = e.angle + ((e.lastAngle - e.angle) * mult);
			addPendingFall(pendingFall, angle);
			addEvent(angle / full, TriggerValue::RISE, e.wheel);

			angle_t fallAngle = angle + e.width;
			if (full - fallAngle < 0.1f) {
				fallAngle = full;
			}
			// Store the pending fall
			pendingFall[(int)e.wheel] = {e.wheel, fallAngle, 0.0f, 1, 0.0f};
		}
	}
	addPendingFall(pendingFall, full);
}

void TriggerWaveform::addPendingFall(ComplexTriggerSignal* pendingFall, angle_t angle) {
	angle_t full = operationMode == FOUR_STROKE_CAM_SENSOR ? 360 : getCycleDuration();

	// The indices stored here point to elements in pendingFall
	int indices[] = {0, 1};

	// If the event on channel 0 happens after the event on channel 1,
	// swap the elements in `indices`.
	if (pendingFall[0].count > 0 && pendingFall[1].count > 0) {
		if (pendingFall[0].angle > pendingFall[1].angle) {
			indices[0] = 1;
			indices[1] = 0;
		}
	}

	for (int i : indices) {
		if (pendingFall[i].count > 0 && pendingFall[i].angle <= angle) {
			addEvent(pendingFall[i].angle / full, TriggerValue::FALL, pendingFall[i].wheel);
			// Reset
			pendingFall[i].count = 0;
		}
	}
}

void TriggerWaveform::initializePre(trigger_type_e type) {
#if EFI_PROD_CODE
	efiAssertVoid(ObdCode::CUSTOM_ERR_6641, hasLotsOfRemainingStack(), "init t");
	efiPrintf("initializeTriggerWaveform(%s/%d)", getTrigger_type_e(type), (int)type);
#endif
	shapeDefinitionError = false;
}

void TriggerWaveform::initializePost(trigger_type_e type, bool isCrankWheel) {
	if (isCrankWheel && !needSecondTriggerInput &&
#if EFI_UNIT_TEST
			engineConfiguration != nullptr &&
#endif
			engineConfiguration->triggerInputPins[1] != Gpio::Unassigned) {
#ifndef HARDWARE_CI
		criticalError("Single-channel trigger %s selected while two inputs were configured", getTrigger_type_e(type));
#endif
	}
	calculateExpectedEventCounts();
	version++;
	if (!shapeDefinitionError) {
		wave.checkSwitchTimes(getCycleDuration());
	}
}
