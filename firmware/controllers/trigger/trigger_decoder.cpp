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

#include "global.h"
#include "os_access.h"

#include "obd_error_codes.h"
#include "trigger_decoder.h"
#include "cyclic_buffer.h"
#include "efi_gpio.h"
#include "engine.h"
#include "engine_math.h"
#include "trigger_central.h"
#include "trigger_simulator.h"
#include "perf_trace.h"

#if EFI_SENSOR_CHART
#include "sensor_chart.h"
#endif

TriggerState::TriggerState() {
	resetTriggerState();
}

bool TriggerState::getShaftSynchronized() {
	return shaft_is_synchronized;
}

void TriggerState::setShaftSynchronized(bool value) {
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

void TriggerState::resetTriggerState() {
	setShaftSynchronized(false);
	toothed_previous_time = 0;

	memset(toothDurations, 0, sizeof(toothDurations));

	totalRevolutionCounter = 0;
	totalTriggerErrorCounter = 0;
	orderingErrorCounter = 0;
	lastDecodingErrorTime = US2NT(-10000000LL);
	someSortOfTriggerError = false;

	memset(toothDurations, 0, sizeof(toothDurations));
	curSignal = SHAFT_PRIMARY_FALLING;
	prevSignal = SHAFT_PRIMARY_FALLING;
	startOfCycleNt = 0;

	resetCurrentCycleState();
	memset(expectedTotalTime, 0, sizeof(expectedTotalTime));

	totalEventCountBase = 0;
	isFirstEvent = true;
}

void TriggerState::setTriggerErrorState() {
	lastDecodingErrorTime = getTimeNowNt();
	someSortOfTriggerError = true;
}

void TriggerState::resetCurrentCycleState() {
	memset(currentCycle.eventCount, 0, sizeof(currentCycle.eventCount));
	memset(currentCycle.timeOfPreviousEventNt, 0, sizeof(currentCycle.timeOfPreviousEventNt));
#if EFI_UNIT_TEST
	memcpy(currentCycle.totalTimeNtCopy, currentCycle.totalTimeNt, sizeof(currentCycle.totalTimeNt));
#endif
	memset(currentCycle.totalTimeNt, 0, sizeof(currentCycle.totalTimeNt));
	currentCycle.current_index = 0;
}

TriggerStateWithRunningStatistics::TriggerStateWithRunningStatistics() :
		//https://en.cppreference.com/w/cpp/language/zero_initialization
		timeOfLastEvent(), instantRpmValue()
		{
}

#if EFI_SHAFT_POSITION_INPUT

#if ! EFI_PROD_CODE
bool printTriggerDebug = false;
bool printTriggerTrace = false;
float actualSynchGap;
#endif /* ! EFI_PROD_CODE */

void TriggerWaveform::initializeSyncPoint(TriggerState& state,
			const TriggerConfiguration& triggerConfiguration,
			const trigger_config_s& triggerConfig) {
	triggerShapeSynchPointIndex = state.findTriggerZeroEventIndex(*this,
			triggerConfiguration, triggerConfig);
}

/**
 * Calculate 'shape.triggerShapeSynchPointIndex' value using 'TriggerState *state'
 */
void calculateTriggerSynchPoint(
		TriggerWaveform& shape,
		TriggerState& state
		DECLARE_ENGINE_PARAMETER_SUFFIX) {
	state.resetTriggerState();

#if EFI_PROD_CODE
	efiAssertVoid(CUSTOM_TRIGGER_STACK, getCurrentRemainingStack() > EXPECTED_REMAINING_STACK, "calc s");
#endif
	engine->triggerErrorDetection.clear();
	shape.initializeSyncPoint(state,
			engine->primaryTriggerConfiguration,
			CONFIG(trigger));

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

void prepareEventAngles(TriggerWaveform *shape,
		TriggerFormDetails *details DECLARE_ENGINE_PARAMETER_SUFFIX) {
	int triggerShapeSynchPointIndex = shape->triggerShapeSynchPointIndex;
	if (triggerShapeSynchPointIndex == EFI_ERROR_CODE) {
		return;
	}
	float firstAngle = shape->getAngle(triggerShapeSynchPointIndex);
	assertAngleRange(firstAngle, "firstAngle", CUSTOM_TRIGGER_SYNC_ANGLE);

	int riseOnlyIndex = 0;

	int length = shape->getLength();

	memset(details->eventAngles, 0, sizeof(details->eventAngles));

	// this may be <length for some triggers like symmetrical crank Miata NB
	int triggerShapeLength = shape->privateTriggerDefinitionSize;

	assertAngleRange(shape->triggerShapeSynchPointIndex, "triggerShapeSynchPointIndex", CUSTOM_TRIGGER_SYNC_ANGLE2);
	efiAssertVoid(CUSTOM_TRIGGER_CYCLE, engine->engineCycleEventCount != 0, "zero engineCycleEventCount");

	for (int eventIndex = 0; eventIndex < length; eventIndex++) {
		if (eventIndex == 0) {
			// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
			details->eventAngles[0] = 0;
			// this value would be used in case of front-only
			details->eventAngles[1] = 0;
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
					details->eventAngles[riseOnlyIndex] = angle;
					details->eventAngles[riseOnlyIndex + 1] = angle;
				}
			} else {
				details->eventAngles[eventIndex] = angle;
			}
		}
	}
}

int64_t TriggerState::getTotalEventCounter() const {
	return totalEventCountBase + currentCycle.current_index;
}

int TriggerState::getTotalRevolutionCounter() const {
	return totalRevolutionCounter;
}

void TriggerStateWithRunningStatistics::movePreSynchTimestamps(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
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

float TriggerStateWithRunningStatistics::calculateInstantRpm(TriggerFormDetails *triggerFormDetails, efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	int current_index = currentCycle.current_index; // local copy so that noone changes the value on us
	assertIsInBoundsWithResult(current_index, timeOfLastEvent, "calc timeOfLastEvent", 0);
	timeOfLastEvent[current_index] = nowNt;
	/**
	 * Here we calculate RPM based on last 90 degrees
	 */
	angle_t currentAngle = triggerFormDetails->eventAngles[current_index];
	// todo: make this '90' depend on cylinder count or trigger shape?
	if (cisnan(currentAngle)) {
		return NOISY_RPM;
	}
	angle_t previousAngle = currentAngle - 90;
	fixAngle(previousAngle, "prevAngle", CUSTOM_ERR_TRIGGER_ANGLE_RANGE);
	// todo: prevIndex should be pre-calculated
	int prevIndex = triggerFormDetails->triggerIndexByAngle[(int)previousAngle];

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
	// todo: angle diff should be pre-calculated
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

void TriggerStateWithRunningStatistics::setLastEventTimeForInstantRpm(efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
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

void TriggerStateWithRunningStatistics::updateInstantRpm(TriggerFormDetails *triggerFormDetails, efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	m_instantRpm = calculateInstantRpm(triggerFormDetails, nowNt PASS_ENGINE_PARAMETER_SUFFIX);


#if EFI_SENSOR_CHART
	if (ENGINE(sensorChartMode) == SC_RPM_ACCEL || ENGINE(sensorChartMode) == SC_DETAILED_RPM) {
		angle_t currentAngle = triggerFormDetails->eventAngles[currentCycle.current_index];
		if (CONFIG(sensorChartMode) == SC_DETAILED_RPM) {
			scAddData(currentAngle, m_instantRpm);
		} else {
			scAddData(currentAngle, m_instantRpmRatio);
		}
	}
#endif /* EFI_SENSOR_CHART */
}

bool TriggerState::isValidIndex(const TriggerWaveform& triggerShape) const {
	return currentCycle.current_index < triggerShape.getSize();
}

static trigger_wheel_e eventIndex[6] = { T_PRIMARY, T_PRIMARY, T_SECONDARY, T_SECONDARY, T_CHANNEL_3, T_CHANNEL_3 };
static trigger_value_e eventType[6] = { TV_FALL, TV_RISE, TV_FALL, TV_RISE, TV_FALL, TV_RISE };

#if EFI_UNIT_TEST
#define PRINT_INC_INDEX 		if (printTriggerTrace) {\
		printf("nextTriggerEvent index=%d\r\n", currentCycle.current_index); \
		}
#else
#define PRINT_INC_INDEX {}
#endif /* EFI_UNIT_TEST */

#define nextTriggerEvent() \
 { \
	uint32_t prevTime = currentCycle.timeOfPreviousEventNt[triggerWheel]; \
	if (prevTime != 0) { \
		/* even event - apply the value*/ \
		currentCycle.totalTimeNt[triggerWheel] += (nowNt - prevTime); \
		currentCycle.timeOfPreviousEventNt[triggerWheel] = 0; \
	} else { \
		/* odd event - start accumulation */ \
		currentCycle.timeOfPreviousEventNt[triggerWheel] = nowNt; \
	} \
	if (triggerConfiguration.UseOnlyRisingEdgeForTrigger) {currentCycle.current_index++;} \
	currentCycle.current_index++; \
	PRINT_INC_INDEX; \
}

#define considerEventForGap() (!triggerShape.useOnlyPrimaryForSync || isPrimary)

#define needToSkipFall(type) ((!triggerShape.gapBothDirections) && (( triggerShape.useRiseEdge) && (type != TV_RISE)))
#define needToSkipRise(type) ((!triggerShape.gapBothDirections) && ((!triggerShape.useRiseEdge) && (type != TV_FALL)))

int TriggerState::getCurrentIndex() const {
	return currentCycle.current_index;
}

void TriggerCentral::validateCamVvtCounters() {
	// micro-optimized 'totalRevolutionCounter % 256'
	int camVvtValidationIndex = triggerState.getTotalRevolutionCounter() & 0xFF;
	if (camVvtValidationIndex == 0) {
		vvtCamCounter = 0;
	} else if (camVvtValidationIndex == 0xFE && vvtCamCounter < 60) {
		// magic logic: we expect at least 60 CAM/VVT events for each 256 trigger cycles, otherwise throw a code
		warning(OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, "no CAM signals");
	}
}

bool TriggerState::syncSymmetricalCrank(int divider, int remainder) {
	efiAssert(OBD_PCM_Processor_Fault, remainder < divider, "syncSymmetricalCrank", false);
	bool isSync = false;
	while (getTotalRevolutionCounter() % divider != remainder) {
		/**
		 * we are here if we've detected the cam sensor within the wrong crank phase
		 * let's increase the trigger event counter, that would adjust the state of
		 * virtual crank-based trigger
		 */
		incrementTotalEventCounter();
		isSync = true;
	}
	return isSync;
}

void TriggerState::incrementTotalEventCounter() {
	totalRevolutionCounter++;
}

bool TriggerState::validateEventCounters(const TriggerWaveform& triggerShape) const {
	bool isDecodingError = false;
	for (int i = 0;i < PWM_PHASE_MAX_WAVE_PER_PWM;i++) {
		isDecodingError |= (currentCycle.eventCount[i] != triggerShape.getExpectedEventCount(i));
	}


#if EFI_UNIT_TEST
			printf("sync point: isDecodingError=%d\r\n", isDecodingError);
			if (isDecodingError) {
				for (int i = 0;i < PWM_PHASE_MAX_WAVE_PER_PWM;i++) {
					printf("count: cur=%d exp=%d\r\n", currentCycle.eventCount[i],  triggerShape.getExpectedEventCount(i));
				}
			}
#endif /* EFI_UNIT_TEST */

	return isDecodingError;
}

void TriggerState::onShaftSynchronization(
		const TriggerStateCallback triggerCycleCallback,
		const efitick_t nowNt,
		const TriggerWaveform& triggerShape) {


	if (triggerCycleCallback) {
		triggerCycleCallback(this);
	}

	startOfCycleNt = nowNt;
	resetCurrentCycleState();
	incrementTotalEventCounter();
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
void TriggerState::decodeTriggerEvent(
		const TriggerWaveform& triggerShape,
		const TriggerStateCallback triggerCycleCallback,
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

	efiAssertVoid(CUSTOM_TRIGGER_UNEXPECTED, signal <= SHAFT_3RD_RISING, "unexpected signal");

	trigger_wheel_e triggerWheel = eventIndex[signal];
	trigger_value_e type = eventType[signal];

	if (!useOnlyRisingEdgeForTrigger && curSignal == prevSignal) {
		orderingErrorCounter++;
	}

	prevSignal = curSignal;
	curSignal = signal;

	currentCycle.eventCount[triggerWheel]++;

	if (toothed_previous_time > nowNt) {
		firmwareError(CUSTOM_OBD_93, "toothed_previous_time after nowNt %d %d", toothed_previous_time, nowNt);
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
					getTrigger_type_e(triggerConfiguration.TriggerType),
					getTrigger_event_e(signal),
					(int)nowNt,
					currentCycle.current_index);
		}
#endif /* EFI_UNIT_TEST */

		/**
		 * For less important events we simply increment the index.
		 */
		nextTriggerEvent()
		;
	} else {
#if !EFI_PROD_CODE
		if (printTriggerTrace) {
			printf("%s event %s %d\r\n",
					getTrigger_type_e(triggerConfiguration.TriggerType),
					getTrigger_event_e(signal),
					nowNt);
			printf("decodeTriggerEvent ratio %.2f: current=%d previous=%d\r\n", 1.0 * toothDurations[0] / toothDurations[1],
					toothDurations[0], toothDurations[1]);
		}
#endif

		isFirstEvent = false;
		bool isSynchronizationPoint;
		bool wasSynchronized = getShaftSynchronized();

		DISPLAY_STATE(Trigger_State)
		DISPLAY_TEXT(Current_Gap);
		DISPLAY(DISPLAY_FIELD(currentGap));
		DISPLAY_TEXT(EOL);

		DISPLAY_STATE(Trigger_Central)
		DISPLAY(DISPLAY_CONFIG(TRIGGERINPUTPINS1));
		DISPLAY_TEXT("Trigger 1: Fall");
		DISPLAY(DISPLAY_FIELD(HWEVENTCOUNTERS1));
		DISPLAY_TEXT(", Rise");
		DISPLAY(DISPLAY_FIELD(HWEVENTCOUNTERS2));
		DISPLAY_TEXT(EOL);

		DISPLAY(DISPLAY_CONFIG(TRIGGERINPUTPINS2));
		DISPLAY_TEXT("Trigger 2: Fall");
		DISPLAY(DISPLAY_FIELD(HWEVENTCOUNTERS3));
		DISPLAY_TEXT(", Rise");
		DISPLAY(DISPLAY_FIELD(HWEVENTCOUNTERS4));
		DISPLAY_TEXT(EOL);

		DISPLAY_TEXT(VVT_1);
		DISPLAY(DISPLAY_CONFIG(CAMINPUTS1));
		DISPLAY(DISPLAY_FIELD(vvtEventRiseCounter));
		DISPLAY(DISPLAY_FIELD(vvtEventFallCounter));
		DISPLAY(DISPLAY_FIELD(vvtCamCounter));

		if (triggerShape.isSynchronizationNeeded) {
			currentGap = 1.0 * toothDurations[0] / toothDurations[1];

			if (CONFIG(debugMode) == DBG_TRIGGER_COUNTERS) {
#if EFI_TUNER_STUDIO
				tsOutputChannels.debugFloatField6 = currentGap;
				tsOutputChannels.debugIntField3 = currentCycle.current_index;
#endif /* EFI_TUNER_STUDIO */
			}

			bool isSync = true;
			for (int i = 0; i < triggerShape.gapTrackingLength; i++) {
				bool isGapCondition = cisnan(triggerShape.syncronizationRatioFrom[i]) || (toothDurations[i] > toothDurations[i + 1] * triggerShape.syncronizationRatioFrom[i]
					&& toothDurations[i] < toothDurations[i + 1] * triggerShape.syncronizationRatioTo[i]);

				isSync &= isGapCondition;
			}

			isSynchronizationPoint = isSync;
			if (isSynchronizationPoint) {
				enginePins.debugTriggerSync.toggle();
			}

			/**
			 * todo: technically we can afford detailed logging even with 60/2 as long as low RPM
			 * todo: figure out exact threshold as a function of RPM and tooth count?
			 * Open question what is 'triggerShape.getSize()' for 60/2 is it 58 or 58*2 or 58*4?
			 */
			bool silentTriggerError = triggerShape.getSize() > 40 && CONFIG(silentTriggerError);

#if EFI_UNIT_TEST
			actualSynchGap = 1.0 * toothDurations[0] / toothDurations[1];
#endif /* EFI_UNIT_TEST */

#if EFI_PROD_CODE || EFI_SIMULATOR
			if (triggerConfiguration.VerboseTriggerSynchDetails || (someSortOfTriggerError && !silentTriggerError)) {

				int rpm = GET_RPM();
				floatms_t engineCycleDuration = getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX);
				if (!engineConfiguration->useOnlyRisingEdgeForTrigger) {
					int time = currentCycle.totalTimeNt[0];
					efiPrintf("%s duty %f %d",
						name,
						time / engineCycleDuration,
						time
						);
				}

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
						efiPrintf("%s rpm=%d time=%d eventIndex=%d gapIndex=%d: gap=%.3f expected from %.3f to %.3f error=%s",
								triggerConfiguration.PrintPrefix,
								GET_RPM(),
							/* cast is needed to make sure we do not put 64 bit value to stack*/ (int)getTimeNowSeconds(),
							currentCycle.current_index,
							i,
							gap,
							ratioFrom,
							triggerShape.syncronizationRatioTo[i],
							boolToString(someSortOfTriggerError));
					}
				}
			}
#else
			if (printTriggerTrace) {
				float gap = 1.0 * toothDurations[0] / toothDurations[1];
				for (int i = 0;i<triggerShape.gapTrackingLength;i++) {
					float gap = 1.0 * toothDurations[i] / toothDurations[i + 1];
					printf("index=%d: gap=%.2f expected from %.2f to %.2f error=%s\r\n",
							i,
							gap,
							triggerShape.syncronizationRatioFrom[i],
							triggerShape.syncronizationRatioTo[i],
							boolToString(someSortOfTriggerError));
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
					getTrigger_type_e(triggerConfiguration.TriggerType),
					isSynchronizationPoint, currentCycle.current_index,
					getTrigger_event_e(signal));
		}
#endif /* EFI_UNIT_TEST */

		if (isSynchronizationPoint) {
			if (triggerStateListener) {
				triggerStateListener->OnTriggerSyncronization(wasSynchronized);
			}

			setShaftSynchronized(true);
			// this call would update duty cycle values
			nextTriggerEvent()
			;

			onShaftSynchronization(triggerCycleCallback, nowNt, triggerShape);

		} else {	/* if (!isSynchronizationPoint) */
			nextTriggerEvent()
			;
		}

		for (int i = triggerShape.gapTrackingLength; i > 0; i--) {
			toothDurations[i] = toothDurations[i - 1];
		}

		toothed_previous_time = nowNt;
	}
	if (getShaftSynchronized() && !isValidIndex(triggerShape) && triggerStateListener) {
		triggerStateListener->OnTriggerInvalidIndex(currentCycle.current_index);
		return;
	}
	if (someSortOfTriggerError) {
		if (getTimeNowNt() - lastDecodingErrorTime > NT_PER_SECOND) {
			someSortOfTriggerError = false;
		}
	}


	// Needed for early instant-RPM detection
	if (triggerStateListener) {
		triggerStateListener->OnTriggerStateProperState(nowNt);
	}
}

static void onFindIndexCallback(TriggerState *state) {
	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		// todo: that's not the best place for this intermediate data storage, fix it!
		state->expectedTotalTime[i] = state->currentCycle.totalTimeNt[i];
	}
}

/**
 * Trigger shape is defined in a way which is convenient for trigger shape definition
 * On the other hand, trigger decoder indexing begins from synchronization event.
 *
 * This function finds the index of synchronization event within TriggerWaveform
 */
uint32_t TriggerState::findTriggerZeroEventIndex(
		TriggerWaveform& shape,
		const TriggerConfiguration& triggerConfiguration,
		const trigger_config_s& triggerConfig) {
	UNUSED(triggerConfig);
#if EFI_PROD_CODE
	efiAssert(CUSTOM_ERR_ASSERT, getCurrentRemainingStack() > 128, "findPos", -1);
#endif


	resetTriggerState();

	if (shape.shapeDefinitionError) {
		return 0;
	}


	// todo: should this variable be declared 'static' to reduce stack usage?
	TriggerStimulatorHelper helper;

	uint32_t syncIndex = helper.findTriggerSyncPoint(shape,
			triggerConfiguration,
			*this);
	if (syncIndex == EFI_ERROR_CODE) {
		return syncIndex;
	}
	efiAssert(CUSTOM_ERR_ASSERT, getTotalRevolutionCounter() == 1, "findZero_revCounter", EFI_ERROR_CODE);

#if EFI_UNIT_TEST
	if (printTriggerDebug) {
		printf("findTriggerZeroEventIndex: syncIndex located %d!\r\n", syncIndex);
	}
#endif /* EFI_UNIT_TEST */

	/**
	 * Now that we have just located the synch point, we can simulate the whole cycle
	 * in order to calculate expected duty cycle
	 *
	 * todo: add a comment why are we doing '2 * shape->getSize()' here?
	 */

	helper.assertSyncPositionAndSetDutyCycle(onFindIndexCallback, triggerConfiguration,
			syncIndex, *this, shape);

	return syncIndex % shape.getSize();
}

#endif /* EFI_SHAFT_POSITION_INPUT */
