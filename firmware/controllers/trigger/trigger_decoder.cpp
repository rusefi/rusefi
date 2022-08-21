/**
 * @file	trigger_decoder.cpp
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 *
 *
 * enable trigger_details
 * DBG_TRIGGER_COUNTERS = 5
 * set debug_mode 5
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

#include "os_access.h"

#include "obd_error_codes.h"
#include "trigger_decoder.h"
#include "cyclic_buffer.h"
#include "trigger_central.h"
#include "trigger_simulator.h"

#if EFI_SENSOR_CHART
#include "sensor_chart.h"
#endif

TriggerDecoderBase::TriggerDecoderBase(const char* name)
	: name(name)
{
	resetTriggerState();
}

bool TriggerDecoderBase::getShaftSynchronized() {
	return shaft_is_synchronized;
}

void TriggerDecoderBase::setShaftSynchronized(bool value) {
	if (value) {
		if (!shaft_is_synchronized) {
			// just got synchronized
			mostRecentSyncTime = getTimeNowNt();
		}
	} else {
		// sync loss
		mostRecentSyncTime = 0;
	}
	shaft_is_synchronized = value;
}

void TriggerDecoderBase::resetTriggerState() {
	setShaftSynchronized(false);
	toothed_previous_time = 0;

	memset(toothDurations, 0, sizeof(toothDurations));

	totalRevolutionCounter = 0;
	totalTriggerErrorCounter = 0;
	orderingErrorCounter = 0;
	m_timeSinceDecodeError.init();

	prevSignal = SHAFT_PRIMARY_FALLING;
	startOfCycleNt = 0;

	resetCurrentCycleState();

	totalEventCountBase = 0;
	isFirstEvent = true;
}

void TriggerDecoderBase::setTriggerErrorState() {
	m_timeSinceDecodeError.reset();
	totalTriggerErrorCounter++;
}

void TriggerDecoderBase::resetCurrentCycleState() {
	memset(currentCycle.eventCount, 0, sizeof(currentCycle.eventCount));
	currentCycle.current_index = 0;
}

#if EFI_SHAFT_POSITION_INPUT

PrimaryTriggerDecoder::PrimaryTriggerDecoder(const char* name)
	: TriggerDecoderBase(name)
	//https://en.cppreference.com/w/cpp/language/zero_initialization
	, timeOfLastEvent()
	, instantRpmValue()
{
}

#if ! EFI_PROD_CODE
bool printTriggerDebug = false;
bool printTriggerTrace = false;
// todo: migrate to triggerSyncGapRatio or triggerActualSyncGapRatio?
float actualSynchGap;
#endif /* ! EFI_PROD_CODE */

void TriggerWaveform::initializeSyncPoint(TriggerDecoderBase& state,
			const TriggerConfiguration& triggerConfiguration) {
	triggerShapeSynchPointIndex = state.findTriggerZeroEventIndex(*this, triggerConfiguration);
}

/**
 * Calculate 'shape.triggerShapeSynchPointIndex' value using 'TriggerDecoderBase *state'
 */
void calculateTriggerSynchPoint(
		TriggerWaveform& shape,
		TriggerDecoderBase& state) {
	state.resetTriggerState();

#if EFI_PROD_CODE
	efiAssertVoid(CUSTOM_TRIGGER_STACK, getCurrentRemainingStack() > EXPECTED_REMAINING_STACK, "calc s");
#endif
	engine->triggerErrorDetection.clear();
	shape.initializeSyncPoint(state, engine->primaryTriggerConfiguration);

	int length = shape.getLength();
	engine->engineCycleEventCount = length;

	efiAssertVoid(CUSTOM_SHAPE_LEN_ZERO, length > 0, "shapeLength=0");
	if (shape.getSize() >= PWM_PHASE_MAX_COUNT) {
		// todo: by the time we are here we had already modified a lot of RAM out of bounds!
		firmwareError(CUSTOM_ERR_TRIGGER_WAVEFORM_TOO_LONG, "Trigger length above maximum: %d", length);
		shape.setShapeDefinitionError(true);
		return;
	}

	if (shape.getSize() == 0) {
		firmwareError(CUSTOM_ERR_TRIGGER_ZERO, "triggerShape size is zero");
	}
}

void TriggerFormDetails::prepareEventAngles(TriggerWaveform *shape) {
	int triggerShapeSynchPointIndex = shape->triggerShapeSynchPointIndex;
	if (triggerShapeSynchPointIndex == EFI_ERROR_CODE) {
		return;
	}
	angle_t firstAngle = shape->getAngle(triggerShapeSynchPointIndex);
	assertAngleRange(firstAngle, "firstAngle", CUSTOM_TRIGGER_SYNC_ANGLE);

	int riseOnlyIndex = 0;

	size_t length = shape->getLength();

	memset(eventAngles, 0, sizeof(eventAngles));

	// this may be <length for some triggers like symmetrical crank Miata NB
	size_t triggerShapeLength = shape->getSize();

	assertAngleRange(shape->triggerShapeSynchPointIndex, "triggerShapeSynchPointIndex", CUSTOM_TRIGGER_SYNC_ANGLE2);
	efiAssertVoid(CUSTOM_TRIGGER_CYCLE, engine->engineCycleEventCount != 0, "zero engineCycleEventCount");

	for (size_t eventIndex = 0; eventIndex < length; eventIndex++) {
		if (eventIndex == 0) {
			// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
			eventAngles[0] = 0;
			// this value would be used in case of front-only
			eventAngles[1] = 0;
		} else {
			// Rotate the trigger around so that the sync point is at position 0
			auto wrappedIndex = (shape->triggerShapeSynchPointIndex + eventIndex) % length;

			// Compute this tooth's position within the trigger definition
			// (wrap, as the trigger def may be smaller than total trigger length)
			auto triggerDefinitionIndex = wrappedIndex % triggerShapeLength;

			// Compute the relative angle of this tooth to the sync point's tooth
			float angle = shape->getAngle(wrappedIndex) - firstAngle;

			efiAssertVoid(CUSTOM_TRIGGER_CYCLE, !cisnan(angle), "trgSyncNaN");
			// Wrap the angle back in to [0, 720)
			fixAngle(angle, "trgSync", CUSTOM_TRIGGER_SYNC_ANGLE_RANGE);

			if (engineConfiguration->useOnlyRisingEdgeForTrigger) {
				efiAssertVoid(OBD_PCM_Processor_Fault, triggerDefinitionIndex < triggerShapeLength, "trigger shape fail");
				assertIsInBounds(triggerDefinitionIndex, shape->isRiseEvent, "isRise");

				// In case this is a rising event, replace the following fall event with the rising as well
				if (shape->isRiseEvent[triggerDefinitionIndex]) {
					riseOnlyIndex += 2;
					eventAngles[riseOnlyIndex] = angle;
					eventAngles[riseOnlyIndex + 1] = angle;
				}
			} else {
				eventAngles[eventIndex] = angle;
			}
		}
	}
}

int64_t TriggerDecoderBase::getTotalEventCounter() const {
	return totalEventCountBase + currentCycle.current_index;
}

int TriggerDecoderBase::getTotalRevolutionCounter() const {
	return totalRevolutionCounter;
}

void PrimaryTriggerDecoder::resetTriggerState() {
	TriggerDecoderBase::resetTriggerState();

	memset(timeOfLastEvent, 0, sizeof(timeOfLastEvent));
	memset(spinningEvents, 0, sizeof(spinningEvents));
	spinningEventIndex = 0;
	prevInstantRpmValue = 0;
	m_instantRpm = 0;

	resetHasFullSync();
}

void PrimaryTriggerDecoder::movePreSynchTimestamps() {
	// here we take timestamps of events which happened prior to synchronization and place them
	// at appropriate locations
	auto triggerSize = getTriggerSize();

	int eventsToCopy = minI(spinningEventIndex, triggerSize);

	size_t firstSrc;
	size_t firstDst;

	if (eventsToCopy >= triggerSize) {
		// Only copy one trigger length worth of events, filling the whole buffer
		firstSrc = spinningEventIndex - triggerSize;
		firstDst = 0;
	} else {
		// There is less than one full cycle, copy to the end of the buffer
		firstSrc = 0;
		firstDst = triggerSize - spinningEventIndex;
	}

	memcpy(timeOfLastEvent + firstDst, spinningEvents + firstSrc, eventsToCopy * sizeof(timeOfLastEvent[0]));
}

float PrimaryTriggerDecoder::calculateInstantRpm(
	TriggerWaveform const & triggerShape, TriggerFormDetails *triggerFormDetails,
	uint32_t current_index, efitick_t nowNt) {

	assertIsInBoundsWithResult(current_index, timeOfLastEvent, "calc timeOfLastEvent", 0);

	// Record the time of this event so we can calculate RPM from it later
	timeOfLastEvent[current_index] = nowNt;

	// Determine where we currently are in the revolution
	angle_t currentAngle = triggerFormDetails->eventAngles[current_index];
	if (cisnan(currentAngle)) {
		// todo: huh? dead code? how can we get NAN from eventAngles table?
		return NOISY_RPM;
	}

	// Hunt for a tooth ~90 degrees ago to compare to the current time
	angle_t previousAngle = currentAngle - 90;
	fixAngle(previousAngle, "prevAngle", CUSTOM_ERR_TRIGGER_ANGLE_RANGE);
	int prevIndex = triggerShape.findAngleIndex(triggerFormDetails, previousAngle);

	// now let's get precise angle for that event
	angle_t prevIndexAngle = triggerFormDetails->eventAngles[prevIndex];
	efitick_t time90ago = timeOfLastEvent[prevIndex];
	if (time90ago == 0) {
		return prevInstantRpmValue;
	}
	// we are OK to subtract 32 bit value from more precise 64 bit since the result would 32 bit which is
	// OK for small time differences like this one
	uint32_t time = nowNt - time90ago;
	angle_t angleDiff = currentAngle - prevIndexAngle;

	// Wrap the angle in to the correct range (ie, could be -630 when we want +90)
	fixAngle(angleDiff, "angleDiff", CUSTOM_ERR_6561);

	// just for safety
	if (time == 0)
		return prevInstantRpmValue;

	float instantRpm = (60000000.0 / 360 * US_TO_NT_MULTIPLIER) * angleDiff / time;
	assertIsInBoundsWithResult(current_index, instantRpmValue, "instantRpmValue", 0);
	instantRpmValue[current_index] = instantRpm;

	// This fixes early RPM instability based on incomplete data
	if (instantRpm < RPM_LOW_THRESHOLD) {
		return prevInstantRpmValue;
	}

	prevInstantRpmValue = instantRpm;

	m_instantRpmRatio = instantRpm / instantRpmValue[prevIndex];

	return instantRpm;
}

void PrimaryTriggerDecoder::setLastEventTimeForInstantRpm(efitick_t nowNt) {
	if (getShaftSynchronized()) {
		return;
	}
	// here we remember tooth timestamps which happen prior to synchronization
	if (spinningEventIndex >= PRE_SYNC_EVENTS) {
		// too many events while trying to find synchronization point
		// todo: better implementation would be to shift here or use cyclic buffer so that we keep last
		// 'PRE_SYNC_EVENTS' events
		return;
	}
	spinningEvents[spinningEventIndex++] = nowNt;
}

void PrimaryTriggerDecoder::updateInstantRpm(
	TriggerWaveform const & triggerShape, TriggerFormDetails *triggerFormDetails,
	uint32_t index, efitick_t nowNt) {

	m_instantRpm = calculateInstantRpm(triggerShape, triggerFormDetails, index,
					   nowNt);


#if EFI_SENSOR_CHART
	if (engine->sensorChartMode == SC_RPM_ACCEL || engine->sensorChartMode == SC_DETAILED_RPM) {
		angle_t currentAngle = triggerFormDetails->eventAngles[currentCycle.current_index];
		if (engineConfiguration->sensorChartMode == SC_DETAILED_RPM) {
			scAddData(currentAngle, m_instantRpm);
		} else {
			scAddData(currentAngle, m_instantRpmRatio);
		}
	}
#endif /* EFI_SENSOR_CHART */
}

bool TriggerDecoderBase::isValidIndex(const TriggerWaveform& triggerShape) const {
	return currentCycle.current_index < triggerShape.getSize();
}

static trigger_wheel_e eventIndex[4] = { T_PRIMARY, T_PRIMARY, T_SECONDARY, T_SECONDARY };
static trigger_value_e eventType[4] = { TV_FALL, TV_RISE, TV_FALL, TV_RISE };

#if EFI_UNIT_TEST
#define PRINT_INC_INDEX 		if (printTriggerTrace) {\
		printf("nextTriggerEvent index=%d\r\n", currentCycle.current_index); \
		}
#else
#define PRINT_INC_INDEX {}
#endif /* EFI_UNIT_TEST */

#define nextTriggerEvent() \
 { \
	if (useOnlyRisingEdgeForTrigger) {currentCycle.current_index++;} \
	currentCycle.current_index++; \
	PRINT_INC_INDEX; \
}

#define considerEventForGap() (!triggerShape.useOnlyPrimaryForSync || isPrimary)

#define needToSkipFall(type) ((!triggerShape.gapBothDirections) && (( triggerShape.useRiseEdge) && (type != TV_RISE)))
#define needToSkipRise(type) ((!triggerShape.gapBothDirections) && ((!triggerShape.useRiseEdge) && (type != TV_FALL)))

int TriggerDecoderBase::getCurrentIndex() const {
	return currentCycle.current_index;
}

void TriggerCentral::validateCamVvtCounters() {
	// micro-optimized 'totalRevolutionCounter % 256'
	int camVvtValidationIndex = triggerState.getTotalRevolutionCounter() & 0xFF;
	if (camVvtValidationIndex == 0) {
		vvtCamCounter = 0;
	} else if (camVvtValidationIndex == 0xFE && vvtCamCounter < 60) {
		// magic logic: we expect at least 60 CAM/VVT events for each 256 trigger cycles, otherwise throw a code
		warning(OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, "No Camshaft Position Sensor signals");
	}
}

angle_t PrimaryTriggerDecoder::syncEnginePhase(int divider, int remainder, angle_t engineCycle) {
	efiAssert(OBD_PCM_Processor_Fault, remainder < divider, "syncEnginePhase", false);
	angle_t totalShift = 0;
	while (getTotalRevolutionCounter() % divider != remainder) {
		/**
		 * we are here if we've detected the cam sensor within the wrong crank phase
		 * let's increase the trigger event counter, that would adjust the state of
		 * virtual crank-based trigger
		 */
		incrementTotalEventCounter();
		totalShift += engineCycle / divider;
	}

	// Allow injection/ignition to happen, we've now fully sync'd the crank based on new cam information
	m_hasSynchronizedPhase = true;

	if (totalShift > 0) {
		camResyncCounter++;
	}

	return totalShift;
}

void TriggerDecoderBase::incrementTotalEventCounter() {
	totalRevolutionCounter++;
}

void PrimaryTriggerDecoder::onTriggerError() {
	// On trigger error, we've lost full sync
	resetHasFullSync();
}

bool TriggerDecoderBase::validateEventCounters(const TriggerWaveform& triggerShape) const {
	// We can check if things are fine by comparing the number of events in a cycle with the expected number of event.
	bool isDecodingError = false;
	for (int i = 0;i < PWM_PHASE_MAX_WAVE_PER_PWM;i++) {
		isDecodingError |= (currentCycle.eventCount[i] != triggerShape.getExpectedEventCount(i));
	}

#if EFI_UNIT_TEST
	printf("validateEventCounters: isDecodingError=%d\n", isDecodingError);
	if (isDecodingError) {
		for (int i = 0;i < PWM_PHASE_MAX_WAVE_PER_PWM;i++) {
			printf("count: cur=%d exp=%d\n", currentCycle.eventCount[i],  triggerShape.getExpectedEventCount(i));
		}
	}
#endif /* EFI_UNIT_TEST */

	return isDecodingError;
}

void TriggerDecoderBase::onShaftSynchronization(
		bool wasSynchronized,
		const efitick_t nowNt,
		const TriggerWaveform& triggerShape) {
	startOfCycleNt = nowNt;
	resetCurrentCycleState();

	if (wasSynchronized) {
		incrementTotalEventCounter();
	} else {
		// We have just synchronized, this is the zeroth revolution
		totalRevolutionCounter = 0;
	}

	totalEventCountBase += triggerShape.getSize();

#if EFI_UNIT_TEST
	if (printTriggerDebug) {
		printf("onShaftSynchronization index=%d %d\r\n",
				currentCycle.current_index,
				totalRevolutionCounter);
	}
#endif /* EFI_UNIT_TEST */
}

/**
 * @brief Trigger decoding happens here
 * VR falls are filtered out and some VR noise detection happens prior to invoking this method, for
 * Hall this method is invoked every time we have a fall or rise on one of the trigger sensors.
 * This method changes the state of trigger_state_s data structure according to the trigger event
 * @param signal type of event which just happened
 * @param nowNt current time
 */
expected<TriggerDecodeResult> TriggerDecoderBase::decodeTriggerEvent(
		const char *msg,
		const TriggerWaveform& triggerShape,
		TriggerStateListener* triggerStateListener,
		const TriggerConfiguration& triggerConfiguration,
		const trigger_event_e signal,
		const efitick_t nowNt) {
	ScopePerf perf(PE::DecodeTriggerEvent);
	
	if (previousEventTimer.getElapsedSecondsAndReset(nowNt) > 1) {
		/**
		 * We are here if there is a time gap between now and previous shaft event - that means the engine is not running.
		 * That means we have lost synchronization since the engine is not running :)
		 */
		setShaftSynchronized(false);
		if (triggerStateListener) {
			triggerStateListener->OnTriggerSynchronizationLost();
		}
	}

	bool useOnlyRisingEdgeForTrigger = triggerConfiguration.UseOnlyRisingEdgeForTrigger;

	efiAssert(CUSTOM_TRIGGER_UNEXPECTED, signal <= SHAFT_SECONDARY_RISING, "unexpected signal", unexpected);

	trigger_wheel_e triggerWheel = eventIndex[signal];
	trigger_value_e type = eventType[signal];

	// Check that we didn't get the same edge twice in a row - that should be impossible
	if (!useOnlyRisingEdgeForTrigger && prevSignal == signal) {
		orderingErrorCounter++;
	}

	prevSignal = signal;

	currentCycle.eventCount[triggerWheel]++;

	if (toothed_previous_time > nowNt) {
		firmwareError(CUSTOM_OBD_93, "[%s] toothed_previous_time after nowNt prev=%d now=%d", msg, toothed_previous_time, nowNt);
	}

	efitick_t currentDurationLong = isFirstEvent ? 0 : nowNt - toothed_previous_time;

	/**
	 * For performance reasons, we want to work with 32 bit values. If there has been more then
	 * 10 seconds since previous trigger event we do not really care.
	 */
	toothDurations[0] =
			currentDurationLong > 10 * NT_PER_SECOND ? 10 * NT_PER_SECOND : currentDurationLong;

	bool isPrimary = triggerWheel == T_PRIMARY;

	if (needToSkipFall(type) || needToSkipRise(type) || (!considerEventForGap())) {
#if EFI_UNIT_TEST
		if (printTriggerTrace) {
			printf("%s isLessImportant %s now=%d index=%d\r\n",
					getTrigger_type_e(triggerConfiguration.TriggerType.type),
					getTrigger_event_e(signal),
					(int)nowNt,
					currentCycle.current_index);
		}
#endif /* EFI_UNIT_TEST */

		// For less important events we simply increment the index.
		nextTriggerEvent();
	} else {
#if !EFI_PROD_CODE
		if (printTriggerTrace) {
			printf("%s event %s %d\r\n",
					getTrigger_type_e(triggerConfiguration.TriggerType.type),
					getTrigger_event_e(signal),
					nowNt);
			printf("decodeTriggerEvent ratio %.2f: current=%d previous=%d\r\n", 1.0 * toothDurations[0] / toothDurations[1],
					toothDurations[0], toothDurations[1]);
		}
#endif

		isFirstEvent = false;
		bool isSynchronizationPoint;
		bool wasSynchronized = getShaftSynchronized();

		if (triggerShape.isSynchronizationNeeded) {
			triggerSyncGapRatio = (float)toothDurations[0] / toothDurations[1];

			isSynchronizationPoint = isSyncPoint(triggerShape, triggerConfiguration.TriggerType.type);
			if (isSynchronizationPoint) {
				enginePins.debugTriggerSync.toggle();
			}

			/**
			 * todo: technically we can afford detailed logging even with 60/2 as long as low RPM
			 * todo: figure out exact threshold as a function of RPM and tooth count?
			 * Open question what is 'triggerShape.getSize()' for 60/2 is it 58 or 58*2 or 58*4?
			 */
			bool silentTriggerError = triggerShape.getSize() > 40 && engineConfiguration->silentTriggerError;

#if EFI_UNIT_TEST
			actualSynchGap = triggerSyncGapRatio;
#endif /* EFI_UNIT_TEST */

#if EFI_PROD_CODE || EFI_SIMULATOR
			bool verbose = engine->isEngineSnifferEnabled && triggerConfiguration.VerboseTriggerSynchDetails;

			if (verbose || (someSortOfTriggerError() && !silentTriggerError)) {

				int rpm = Sensor::getOrZero(SensorType::Rpm);
				floatms_t engineCycleDuration = getEngineCycleDuration(rpm);

				for (int i = 0;i<triggerShape.gapTrackingLength;i++) {
					float ratioFrom = triggerShape.syncronizationRatioFrom[i];
					if (cisnan(ratioFrom)) {
						// we do not track gap at this depth
						continue;
					}

					float gap = 1.0 * toothDurations[i] / toothDurations[i + 1];
					if (cisnan(gap)) {
						efiPrintf("index=%d NaN gap, you have noise issues?",
								i);
					} else {
						float ratioTo = triggerShape.syncronizationRatioTo[i];

						bool gapOk = isInRange(ratioFrom, gap, ratioTo);

						efiPrintf("%srpm=%d time=%d eventIndex=%d gapIndex=%d: %s gap=%.3f expected from %.3f to %.3f error=%s",
								triggerConfiguration.PrintPrefix,
								(int)Sensor::getOrZero(SensorType::Rpm),
							/* cast is needed to make sure we do not put 64 bit value to stack*/ (int)getTimeNowSeconds(),
							currentCycle.current_index,
							i,
							gapOk ? "Y" : "n",
							gap,
							ratioFrom,
							ratioTo,
							boolToString(someSortOfTriggerError()));
					}
				}
			}
#else
			if (printTriggerTrace) {
				float gap = 1.0 * toothDurations[0] / toothDurations[1];
				for (int i = 0;i<triggerShape.gapTrackingLength;i++) {
					float gap = 1.0 * toothDurations[i] / toothDurations[i + 1];
					printf("%sindex=%d: gap=%.2f expected from %.2f to %.2f error=%s\r\n",
							triggerConfiguration.PrintPrefix,
							i,
							gap,
							triggerShape.syncronizationRatioFrom[i],
							triggerShape.syncronizationRatioTo[i],
							boolToString(someSortOfTriggerError()));
				}
			}
#endif /* EFI_PROD_CODE */
		} else {
			/**
			 * We are here in case of a wheel without synchronization - we just need to count events,
			 * synchronization point simply happens once we have the right number of events
			 *
			 * in case of noise the counter could be above the expected number of events, that's why 'more or equals' and not just 'equals'
			 */

			unsigned int endOfCycleIndex = triggerShape.getSize() - (triggerConfiguration.UseOnlyRisingEdgeForTrigger ? 2 : 1);

			isSynchronizationPoint = !getShaftSynchronized() || (currentCycle.current_index >= endOfCycleIndex);

#if EFI_UNIT_TEST
			if (printTriggerTrace) {
				printf("decodeTriggerEvent sync=%d isSynchronizationPoint=%d index=%d size=%d\r\n",
						getShaftSynchronized(),
					isSynchronizationPoint,
					currentCycle.current_index,
					triggerShape.getSize());
			}
#endif /* EFI_UNIT_TEST */
		}
#if EFI_UNIT_TEST
		if (printTriggerTrace) {
			printf("decodeTriggerEvent %s isSynchronizationPoint=%d index=%d %s\r\n",
					getTrigger_type_e(triggerConfiguration.TriggerType.type),
					isSynchronizationPoint, currentCycle.current_index,
					getTrigger_event_e(signal));
		}
#endif /* EFI_UNIT_TEST */

		if (isSynchronizationPoint) {
			bool isDecodingError = validateEventCounters(triggerShape);

			if (triggerStateListener) {
				triggerStateListener->OnTriggerSyncronization(wasSynchronized, isDecodingError);
			}

			// If we got a sync point, but the wrong number of events since the last sync point
			// One of two things has happened:
			//  - We missed a tooth, and this is the real sync point
			//  - Due to some mistake in timing, we found what looks like a sync point but actually isn't
			// In either case, we should wait for another sync point before doing anything to try and run an engine,
			// so we clear the synchronized flag.
			if (wasSynchronized && isDecodingError) {
				setTriggerErrorState();

				// Something wrong, no longer synchronized
				setShaftSynchronized(false);

				// This is a decoding error
				onTriggerError();
			} else {
				// If this was the first sync point OR no decode error, we're synchronized!
				setShaftSynchronized(true);
			}

			// this call would update duty cycle values
			nextTriggerEvent();

			onShaftSynchronization(wasSynchronized, nowNt, triggerShape);
		} else {	/* if (!isSynchronizationPoint) */
			nextTriggerEvent();
		}

		for (int i = triggerShape.gapTrackingLength; i > 0; i--) {
			toothDurations[i] = toothDurations[i - 1];
		}

		toothed_previous_time = nowNt;
	}

	if (getShaftSynchronized() && !isValidIndex(triggerShape)) {
		// We've had too many events since the last sync point, we should have seen a sync point by now.
		// This is a trigger error.

		// let's not show a warning if we are just starting to spin
		if (Sensor::getOrZero(SensorType::Rpm) != 0) {
			warning(CUSTOM_SYNC_ERROR, "sync error for %s: index #%d above total size %d", name, currentCycle.current_index, triggerShape.getSize());
			setTriggerErrorState();
		}

		onTriggerError();

		setShaftSynchronized(false);

		return unexpected;
	}

	// Needed for early instant-RPM detection
	if (triggerStateListener) {
		triggerStateListener->OnTriggerStateProperState(nowNt);
	}

	triggerStateIndex = currentCycle.current_index;

	if (getShaftSynchronized()) {
		return TriggerDecodeResult{ currentCycle.current_index };
	} else {
		return unexpected;
	}
}

bool TriggerDecoderBase::isSyncPoint(const TriggerWaveform& triggerShape, trigger_type_e triggerType) const {
	// Miata NB needs a special decoder.
	// The problem is that the crank wheel only has 4 teeth, also symmetrical, so the pattern
	// is long-short-long-short for one crank rotation.
	// A quick acceleration can result in two successive "short gaps", so we see 
	// long-short-short-short-long instead of the correct long-short-long-short-long
	// This logic expands the lower bound on a "long" tooth, then compares the last
	// tooth to the current one.

	// Instead of detecting short/long, this logic first checks for "maybe short" and "maybe long",
	// then simply tests longer vs. shorter instead of absolute value.
	if (triggerType == TT_MIATA_VVT) {
		auto secondGap = (float)toothDurations[1] / toothDurations[2];

		bool currentGapOk = isInRange(triggerShape.syncronizationRatioFrom[0], (float)triggerSyncGapRatio, triggerShape.syncronizationRatioTo[0]);
		bool secondGapOk  = isInRange(triggerShape.syncronizationRatioFrom[1], secondGap,  triggerShape.syncronizationRatioTo[1]);

		// One or both teeth was impossible range, this is not the sync point
		if (!currentGapOk || !secondGapOk) {
			return false;
		}

		// If both teeth are in the range of possibility, return whether this gap is
		// shorter than the last or not.  If it is, this is the sync point.
		return triggerSyncGapRatio < secondGap;
	}

	for (int i = 0; i < triggerShape.gapTrackingLength; i++) {
		auto from = triggerShape.syncronizationRatioFrom[i];
		auto to = triggerShape.syncronizationRatioTo[i];

		if (cisnan(from)) {
			// don't check this gap, skip it
			continue;
		}

		// This is transformed to avoid a division and use a cheaper multiply instead
		// toothDurations[i] / toothDurations[i+1] > from
		// is an equivalent comparison to
		// toothDurations[i] > toothDurations[i+1] * from
		bool isGapCondition = 
			  (toothDurations[i] > toothDurations[i + 1] * from
			&& toothDurations[i] < toothDurations[i + 1] * to);

		if (!isGapCondition) {
			return false;
		}
	}

	return true;
}

/**
 * Trigger shape is defined in a way which is convenient for trigger shape definition
 * On the other hand, trigger decoder indexing begins from synchronization event.
 *
 * This function finds the index of synchronization event within TriggerWaveform
 */
uint32_t TriggerDecoderBase::findTriggerZeroEventIndex(
		TriggerWaveform& shape,
		const TriggerConfiguration& triggerConfiguration) {
#if EFI_PROD_CODE
	efiAssert(CUSTOM_ERR_ASSERT, getCurrentRemainingStack() > 128, "findPos", -1);
#endif


	resetTriggerState();

	if (shape.shapeDefinitionError) {
		return 0;
	}

	TriggerStimulatorHelper helper;

	uint32_t syncIndex = helper.findTriggerSyncPoint(shape,
			triggerConfiguration,
			*this);
	if (syncIndex == EFI_ERROR_CODE) {
		return syncIndex;
	}

	// Assert that we found the sync point on the very first revolution
	efiAssert(CUSTOM_ERR_ASSERT, getTotalRevolutionCounter() == 0, "findZero_revCounter", EFI_ERROR_CODE);

#if EFI_UNIT_TEST
	if (printTriggerDebug) {
		printf("findTriggerZeroEventIndex: syncIndex located %d!\r\n", syncIndex);
	}
#endif /* EFI_UNIT_TEST */

	helper.assertSyncPosition(triggerConfiguration,
			syncIndex, *this, shape);

	return syncIndex % shape.getSize();
}

#endif /* EFI_SHAFT_POSITION_INPUT */
