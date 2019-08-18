/**
 * @file	trigger_decoder.cpp
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2019
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

#if EFI_SENSOR_CHART
#include "sensor_chart.h"
#endif

TriggerState::TriggerState() {
	resetTriggerState();
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
	triggerCycleCallback = NULL;
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

void TriggerState::resetCurrentCycleState() {
	memset(currentCycle.eventCount, 0, sizeof(currentCycle.eventCount));
	memset(currentCycle.timeOfPreviousEventNt, 0, sizeof(currentCycle.timeOfPreviousEventNt));
	memset(currentCycle.totalTimeNt, 0, sizeof(currentCycle.totalTimeNt));
	currentCycle.current_index = 0;
}

TriggerStateWithRunningStatistics::TriggerStateWithRunningStatistics() :
		//https://en.cppreference.com/w/cpp/language/zero_initialization
		timeOfLastEvent(), instantRpmValue()
		{
}

#if EFI_SHAFT_POSITION_INPUT

EXTERN_ENGINE
;

// todo: this should become a field on some class
// no reason to make this a field on TriggerState since we have two instances of that one
// and only one needs this data structure. we probably need a virtual method of .addError() and
// only TriggerStateWithRunningStatistics would have the field?
static cyclic_buffer<int> errorDetection;

#if ! EFI_PROD_CODE
bool printTriggerDebug = false;
float actualSynchGap;
#endif /* ! EFI_PROD_CODE */

#if EFI_TUNER_STUDIO
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

static Logging * logger;

/**
 * @return TRUE is something is wrong with trigger decoding
 */
bool isTriggerDecoderError(void) {
	return errorDetection.sum(6) > 4;
}

void calculateTriggerSynchPoint(TriggerShape *shape, TriggerState *state DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_PROD_CODE
	efiAssertVoid(CUSTOM_ERR_6642, getCurrentRemainingStack() > 256, "calc s");
#endif
	trigger_config_s const*triggerConfig = &engineConfiguration->trigger;

	shape->triggerShapeSynchPointIndex = findTriggerZeroEventIndex(state, shape, triggerConfig PASS_ENGINE_PARAMETER_SUFFIX);

	int length = shape->getLength();
	engine->engineCycleEventCount = length;
	efiAssertVoid(CUSTOM_SHAPE_LEN_ZERO, length > 0, "shapeLength=0");
	if (length >= PWM_PHASE_MAX_COUNT) {
		warning(CUSTOM_ERR_TRIGGER_SHAPE_TOO_LONG, "Count above %d", length);
		shape->setShapeDefinitionError(true);
		return;
	}

	float firstAngle = shape->getAngle(shape->triggerShapeSynchPointIndex);
	assertAngleRange(shape->triggerShapeSynchPointIndex, "firstAngle", CUSTOM_ERR_6551);

	int riseOnlyIndex = 0;

	for (int eventIndex = 0; eventIndex < length; eventIndex++) {
		if (eventIndex == 0) {
			// explicit check for zero to avoid issues where logical zero is not exactly zero due to float nature
			shape->eventAngles[0] = 0;
			// this value would be used in case of front-only
			shape->eventAngles[1] = 0;
			shape->riseOnlyIndexes[0] = 0;
		} else {
			assertAngleRange(shape->triggerShapeSynchPointIndex, "triggerShapeSynchPointIndex", CUSTOM_ERR_6552);
			unsigned int triggerDefinitionCoordinate = (shape->triggerShapeSynchPointIndex + eventIndex) % engine->engineCycleEventCount;
			efiAssertVoid(CUSTOM_ERR_6595, engine->engineCycleEventCount != 0, "zero engineCycleEventCount");
			int triggerDefinitionIndex = triggerDefinitionCoordinate >= shape->privateTriggerDefinitionSize ? triggerDefinitionCoordinate - shape->privateTriggerDefinitionSize : triggerDefinitionCoordinate;
			float angle = shape->getAngle(triggerDefinitionCoordinate) - firstAngle;
			efiAssertVoid(CUSTOM_ERR_6596, !cisnan(angle), "trgSyncNaN");
			fixAngle(angle, "trgSync", CUSTOM_ERR_6559);
			if (engineConfiguration->useOnlyRisingEdgeForTrigger) {
				if (shape->isRiseEvent[triggerDefinitionIndex]) {
					riseOnlyIndex += 2;
					shape->eventAngles[riseOnlyIndex] = angle;
					shape->eventAngles[riseOnlyIndex + 1] = angle;
				}
			} else {
				shape->eventAngles[eventIndex] = angle;
			}

			shape->riseOnlyIndexes[eventIndex] = riseOnlyIndex;
		}
	}
}

efitime_t TriggerState::getTotalEventCounter() const {
	return totalEventCountBase + currentCycle.current_index;
}

int TriggerState::getTotalRevolutionCounter() const {
	return totalRevolutionCounter;
}

void TriggerStateWithRunningStatistics::movePreSynchTimestamps(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// here we take timestamps of events which happened prior to synchronization and place them
	// at appropriate locations
	for (int i = 0; i < spinningEventIndex;i++) {
		timeOfLastEvent[getTriggerSize() - i] = spinningEvents[i];
	}
}

float TriggerStateWithRunningStatistics::calculateInstantRpm(int *prevIndex, efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	int current_index = currentCycle.current_index; // local copy so that noone changes the value on us
	timeOfLastEvent[current_index] = nowNt;
	/**
	 * Here we calculate RPM based on last 90 degrees
	 */
	angle_t currentAngle = TRIGGER_SHAPE(eventAngles[current_index]);
	// todo: make this '90' depend on cylinder count or trigger shape?
	if (cisnan(currentAngle)) {
		return NOISY_RPM;
	}
	angle_t previousAngle = currentAngle - 90;
	fixAngle(previousAngle, "prevAngle", CUSTOM_ERR_6560);
	// todo: prevIndex should be pre-calculated
	*prevIndex = TRIGGER_SHAPE(triggerIndexByAngle[(int)previousAngle]);

	// now let's get precise angle for that event
	angle_t prevIndexAngle = TRIGGER_SHAPE(eventAngles[*prevIndex]);
	efitick_t time90ago = timeOfLastEvent[*prevIndex];
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
	instantRpmValue[current_index] = instantRpm;

	// This fixes early RPM instability based on incomplete data
	if (instantRpm < RPM_LOW_THRESHOLD)
		return prevInstantRpmValue;
	prevInstantRpmValue = instantRpm;

	return instantRpm;
}

void TriggerStateWithRunningStatistics::setLastEventTimeForInstantRpm(efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (shaft_is_synchronized) {
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

void TriggerStateWithRunningStatistics::runtimeStatistics(efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (engineConfiguration->debugMode == DBG_INSTANT_RPM) {
		int prevIndex;
		instantRpm = calculateInstantRpm(&prevIndex, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
	}
	if (ENGINE(sensorChartMode) == SC_RPM_ACCEL || ENGINE(sensorChartMode) == SC_DETAILED_RPM) {
		int prevIndex;
		instantRpm = calculateInstantRpm(&prevIndex, nowNt PASS_ENGINE_PARAMETER_SUFFIX);

#if EFI_SENSOR_CHART
		angle_t currentAngle = TRIGGER_SHAPE(eventAngles[currentCycle.current_index]);
		if (CONFIGB(sensorChartMode) == SC_DETAILED_RPM) {
			scAddData(currentAngle, instantRpm);
		} else {
			scAddData(currentAngle, instantRpm / instantRpmValue[prevIndex]);
		}
#endif /* EFI_SENSOR_CHART */
	}
}

bool TriggerState::isValidIndex(DECLARE_ENGINE_PARAMETER_SIGNATURE) const {
	return currentCycle.current_index < getTriggerSize();
}

static trigger_wheel_e eventIndex[6] = { T_PRIMARY, T_PRIMARY, T_SECONDARY, T_SECONDARY, T_CHANNEL_3, T_CHANNEL_3 };
static trigger_value_e eventType[6] = { TV_FALL, TV_RISE, TV_FALL, TV_RISE, TV_FALL, TV_RISE };

#define getCurrentGapDuration(nowNt) \
	(isFirstEvent ? 0 : (nowNt) - toothed_previous_time)

#if EFI_UNIT_TEST
#define PRINT_INC_INDEX 		if (printTriggerDebug) {\
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
	if (engineConfiguration->useOnlyRisingEdgeForTrigger) {currentCycle.current_index++;} \
	currentCycle.current_index++; \
	PRINT_INC_INDEX; \
}

#define considerEventForGap() (!TRIGGER_SHAPE(useOnlyPrimaryForSync) || isPrimary)

#define needToSkipFall(type) ((!TRIGGER_SHAPE(gapBothDirections)) && (( TRIGGER_SHAPE(useRiseEdge)) && (type != TV_RISE)))
#define needToSkipRise(type) ((!TRIGGER_SHAPE(gapBothDirections)) && ((!TRIGGER_SHAPE(useRiseEdge)) && (type != TV_FALL)))

#define isLessImportant(type) (needToSkipFall(type) || needToSkipRise(type) || (!considerEventForGap()) )

int TriggerState::getCurrentIndex() const {
	return currentCycle.current_index;
}

void TriggerState::validateCamVvtCounters() {
	// micro-optimized 'totalRevolutionCounter % 256'
	int camVvtValidationIndex = totalRevolutionCounter & 0xFF;
	if (camVvtValidationIndex == 0) {
		vvtCamCounter = 0;
	} else if (camVvtValidationIndex == 0xFE && vvtCamCounter < 60) {
		// magic logic: we expect at least 60 CAM/VVT events for each 256 trigger cycles, otherwise throw a code
		warning(OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, "no CAM signals");
	}
}

void TriggerState::incrementTotalEventCounter() {
	totalRevolutionCounter++;
}

bool TriggerState::isEvenRevolution() const {
	return totalRevolutionCounter & 1;
}

void TriggerState::onSynchronizationLost(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setShaftSynchronized(false);
	// Needed for early instant-RPM detection
	engine->rpmCalculator.setStopSpinning(PASS_ENGINE_PARAMETER_SIGNATURE);
}

bool TriggerState::validateEventCounters(DECLARE_ENGINE_PARAMETER_SIGNATURE) const {
	bool isDecodingError = currentCycle.eventCount[0] != TRIGGER_SHAPE(expectedEventCount[0])
					|| currentCycle.eventCount[1] != TRIGGER_SHAPE(expectedEventCount[1])
					|| currentCycle.eventCount[2] != TRIGGER_SHAPE(expectedEventCount[2]);

#if EFI_UNIT_TEST
			printf("sync point: isDecodingError=%d isInit=%d\r\n", isDecodingError, engine->isInitializingTrigger);
			if (isDecodingError) {
				printf("count: cur=%d exp=%d\r\n", currentCycle.eventCount[0],  TRIGGER_SHAPE(expectedEventCount[0]));
				printf("count: cur=%d exp=%d\r\n", currentCycle.eventCount[1],  TRIGGER_SHAPE(expectedEventCount[1]));
				printf("count: cur=%d exp=%d\r\n", currentCycle.eventCount[2],  TRIGGER_SHAPE(expectedEventCount[2]));
			}
#endif /* EFI_UNIT_TEST */

	return isDecodingError;
}

void TriggerState::handleTriggerError(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->debugMode == DBG_TRIGGER_SYNC) {
#if EFI_TUNER_STUDIO
		tsOutputChannels.debugIntField1 = currentCycle.eventCount[0];
		tsOutputChannels.debugIntField2 = currentCycle.eventCount[1];
		tsOutputChannels.debugIntField3 = currentCycle.eventCount[2];
#endif /* EFI_TUNER_STUDIO */
	}

	warning(CUSTOM_SYNC_COUNT_MISMATCH, "trigger not happy current %d/%d/%d expected %d/%d/%d",
			currentCycle.eventCount[0],
			currentCycle.eventCount[1],
			currentCycle.eventCount[2],
			TRIGGER_SHAPE(expectedEventCount[0]),
			TRIGGER_SHAPE(expectedEventCount[1]),
			TRIGGER_SHAPE(expectedEventCount[2]));
	lastDecodingErrorTime = getTimeNowNt();
	someSortOfTriggerError = true;

	totalTriggerErrorCounter++;
	if (CONFIG(verboseTriggerSynchDetails) || someSortOfTriggerError) {
#if EFI_PROD_CODE
		scheduleMsg(logger, "error: synchronizationPoint @ index %d expected %d/%d/%d got %d/%d/%d",
				currentCycle.current_index, TRIGGER_SHAPE(expectedEventCount[0]),
				TRIGGER_SHAPE(expectedEventCount[1]), TRIGGER_SHAPE(expectedEventCount[2]),
				currentCycle.eventCount[0], currentCycle.eventCount[1], currentCycle.eventCount[2]);
#endif /* EFI_PROD_CODE */
	}
}

void TriggerState::onShaftSynchronization(efitime_t nowNt, trigger_wheel_e triggerWheel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setShaftSynchronized(true);
	// this call would update duty cycle values
	nextTriggerEvent()
	;

	if (triggerCycleCallback != NULL) {
		triggerCycleCallback(this);
	}

	startOfCycleNt = nowNt;
	resetCurrentCycleState();
	incrementTotalEventCounter();
	totalEventCountBase += getTriggerSize();

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
 * This method is invoked every time we have a fall or rise on one of the trigger sensors.
 * This method changes the state of trigger_state_s data structure according to the trigger event
 * @param signal type of event which just happened
 * @param nowNt current time
 */
void TriggerState::decodeTriggerEvent(trigger_event_e const signal, efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	bool useOnlyRisingEdgeForTrigger = CONFIG(useOnlyRisingEdgeForTrigger);
	// todo: use 'triggerShape' instead of TRIGGER_SHAPE in order to decouple this method from engine #635
	TriggerShape *triggerShape = &ENGINE(triggerCentral.triggerShape);

	efiAssertVoid(CUSTOM_ERR_6640, signal <= SHAFT_3RD_RISING, "unexpected signal");

	trigger_wheel_e triggerWheel = eventIndex[signal];
	trigger_value_e type = eventType[signal];

	if (!useOnlyRisingEdgeForTrigger && curSignal == prevSignal) {
		orderingErrorCounter++;
	}

	prevSignal = curSignal;
	curSignal = signal;

	currentCycle.eventCount[triggerWheel]++;

	efiAssertVoid(CUSTOM_OBD_93, toothed_previous_time <= nowNt, "toothed_previous_time after nowNt");

	efitime_t currentDurationLong = getCurrentGapDuration(nowNt);

	/**
	 * For performance reasons, we want to work with 32 bit values. If there has been more then
	 * 10 seconds since previous trigger event we do not really care.
	 */
	toothDurations[0] =
			currentDurationLong > 10 * US2NT(US_PER_SECOND_LL) ? 10 * US2NT(US_PER_SECOND_LL) : currentDurationLong;

	bool isPrimary = triggerWheel == T_PRIMARY;

	if (isLessImportant(type)) {
#if EFI_UNIT_TEST
		if (printTriggerDebug) {
			printf("%s isLessImportant %s now=%lld index=%d\r\n",
					getTrigger_type_e(engineConfiguration->trigger.type),
					getTrigger_event_e(signal),
					nowNt,
					currentCycle.current_index);
		}
#endif /* EFI_UNIT_TEST */

		/**
		 * For less important events we simply increment the index.
		 */
		nextTriggerEvent()
		;
	} else {

#if EFI_UNIT_TEST
		if (printTriggerDebug) {
			printf("%s event %s %d\r\n",
					getTrigger_type_e(engineConfiguration->trigger.type),
					getTrigger_event_e(signal),
					nowNt);
		}
#endif /* EFI_UNIT_TEST */

		isFirstEvent = false;
// todo: skip a number of signal from the beginning

#if EFI_PROD_CODE
//	scheduleMsg(&logger, "from %.2f to %.2f %d %d", triggerConfig->syncRatioFrom, triggerConfig->syncRatioTo, toothDurations[0], shaftPositionState->toothDurations[1]);
//	scheduleMsg(&logger, "ratio %.2f", 1.0 * toothDurations[0]/ shaftPositionState->toothDurations[1]);
#else
		if (printTriggerDebug) {
			printf("ratio %.2f: current=%d previous=%d\r\n", 1.0 * toothDurations[0] / toothDurations[1],
					toothDurations[0], toothDurations[1]);
		}
#endif

		bool isSynchronizationPoint;
		bool wasSynchronized = shaft_is_synchronized;

		DISPLAY(DISPLAY_CONFIG(TRIGGERINPUTPINS1));
		DISPLAY_TEXT(Trigger_1_Fall);
		DISPLAY(DISPLAY_FIELD(HWEVENTCOUNTERS1));
		DISPLAY_TEXT(Rise);
		DISPLAY(DISPLAY_FIELD(HWEVENTCOUNTERS2));
		DISPLAY_TEXT(EOL);

		DISPLAY(DISPLAY_CONFIG(TRIGGERINPUTPINS2));
		DISPLAY_TEXT(Trigger_2_Fall);
		DISPLAY(DISPLAY_FIELD(HWEVENTCOUNTERS3));
		DISPLAY_TEXT(Rise);
		DISPLAY(DISPLAY_FIELD(HWEVENTCOUNTERS4));

		if (triggerShape->isSynchronizationNeeded) {
			// this is getting a little out of hand, any ideas?

			if (CONFIG(debugMode) == DBG_TRIGGER_SYNC) {
				float currentGap = 1.0 * toothDurations[0] / toothDurations[1];
#if EFI_TUNER_STUDIO
				tsOutputChannels.debugFloatField1 = currentGap;
				tsOutputChannels.debugFloatField2 = currentCycle.current_index;
#endif /* EFI_TUNER_STUDIO */
			}

			bool isSync = true;
			for (int i = 0;i<GAP_TRACKING_LENGTH;i++) {
				bool isGapCondition = cisnan(triggerShape->syncronizationRatioFrom[i]) || (toothDurations[i] > toothDurations[i + 1] * TRIGGER_SHAPE(syncronizationRatioFrom[i])
					&& toothDurations[i] < toothDurations[i + 1] * triggerShape->syncronizationRatioTo[i]);

				isSync &= isGapCondition;

			}
			isSynchronizationPoint = isSync;


			/**
			 * todo: technically we can afford detailed logging even with 60/2 as long as low RPM
			 * todo: figure out exact threshold as a function of RPM and tooth count?
			 * Open question what is 'triggerShape->getSize()' for 60/2 is it 58 or 58*2 or 58*4?
			 */
			bool silentTriggerError = triggerShape->getSize() > 40 && CONFIG(silentTriggerError);

#if EFI_PROD_CODE
			if (CONFIG(verboseTriggerSynchDetails) || (someSortOfTriggerError && !silentTriggerError)) {
#else
				if (printTriggerDebug) {
#endif /* EFI_PROD_CODE */

#if EFI_PROD_CODE

				for (int i = 0;i<GAP_TRACKING_LENGTH;i++) {
					float gap = 1.0 * toothDurations[i] / toothDurations[i + 1];
					if (cisnan(gap)) {
						scheduleMsg(logger, "index=%d NaN gap, you have noise issues?",
								i);
					} else {
						scheduleMsg(logger, "time=%d index=%d: gap=%.2f expected from %.2f to %.2f error=%s",
							getTimeNowSeconds(),
							i,
							gap,
							TRIGGER_SHAPE(syncronizationRatioFrom[i]),
							TRIGGER_SHAPE(syncronizationRatioTo[i]),
							boolToString(someSortOfTriggerError));
					}
				}

#else
				float gap = 1.0 * toothDurations[0] / toothDurations[1];
				actualSynchGap = gap;
				for (int i = 0;i<GAP_TRACKING_LENGTH;i++) {
					float gap = 1.0 * toothDurations[i] / toothDurations[i + 1];
					print("index=%d: gap=%.2f expected from %.2f to %.2f error=%s\r\n",
							i,
							gap,
							TRIGGER_SHAPE(syncronizationRatioFrom[i]),
							TRIGGER_SHAPE(syncronizationRatioTo[i]),
							boolToString(someSortOfTriggerError));
				}


#endif /* EFI_PROD_CODE */
			}

		} else {
			/**
			 * We are here in case of a wheel without synchronization - we just need to count events,
			 * synchronization point simply happens once we have the right number of events
			 *
			 * in case of noise the counter could be above the expected number of events, that's why 'more or equals' and not just 'equals'
			 */

#if EFI_UNIT_TEST
			if (printTriggerDebug) {
				printf("sync=%d index=%d size=%d\r\n",
					shaft_is_synchronized,
					currentCycle.current_index,
					getTriggerSize());
			}
#endif /* EFI_UNIT_TEST */
			unsigned int endOfCycleIndex = getTriggerSize() - (CONFIG(useOnlyRisingEdgeForTrigger) ? 2 : 1);


			isSynchronizationPoint = !shaft_is_synchronized || (currentCycle.current_index >= endOfCycleIndex);

#if EFI_UNIT_TEST
			if (printTriggerDebug) {
				printf("isSynchronizationPoint=%d index=%d size=%d\r\n",
						isSynchronizationPoint,
						currentCycle.current_index,
						getTriggerSize());
			}
#endif /* EFI_UNIT_TEST */

		}

#if EFI_UNIT_TEST
		if (printTriggerDebug) {
			printf("%s isSynchronizationPoint=%d index=%d %s\r\n",
					getTrigger_type_e(engineConfiguration->trigger.type),
					isSynchronizationPoint, currentCycle.current_index,
					getTrigger_event_e(signal));
		}
#endif /* EFI_UNIT_TEST */

		if (isSynchronizationPoint) {

			// We only care about trigger shape once we have synchronized trigger. Anything could happen
			// during first revolution and it's fine
			if (wasSynchronized) {

				/**
			 	 * We can check if things are fine by comparing the number of events in a cycle with the expected number of event.
			 	 */
				bool isDecodingError = validateEventCounters(PASS_ENGINE_PARAMETER_SIGNATURE);

				enginePins.triggerDecoderErrorPin.setValue(isDecodingError);

				if (isDecodingError && !engine->isInitializingTrigger) {
					handleTriggerError(PASS_ENGINE_PARAMETER_SIGNATURE);
				}

				errorDetection.add(isDecodingError);

				if (isTriggerDecoderError()) {
					warning(CUSTOM_OBD_TRG_DECODING, "trigger decoding issue. expected %d/%d/%d got %d/%d/%d",
							TRIGGER_SHAPE(expectedEventCount[0]), TRIGGER_SHAPE(expectedEventCount[1]),
							TRIGGER_SHAPE(expectedEventCount[2]), currentCycle.eventCount[0], currentCycle.eventCount[1],
							currentCycle.eventCount[2]);
				}
			}

			onShaftSynchronization(nowNt, triggerWheel PASS_ENGINE_PARAMETER_SUFFIX);

		} else {	/* if (!isSynchronizationPoint) */
			nextTriggerEvent()
			;
		}

		for (int i = GAP_TRACKING_LENGTH; i > 0; i--) {
			toothDurations[i] = toothDurations[i - 1];
		}

		toothed_previous_time = nowNt;
	}
	if (!isValidIndex(PASS_ENGINE_PARAMETER_SIGNATURE) && !engine->isInitializingTrigger) {
		// let's not show a warning if we are just starting to spin
		if (GET_RPM_VALUE != 0) {
			warning(CUSTOM_SYNC_ERROR, "sync error: index #%d above total size %d", currentCycle.current_index, getTriggerSize());
			lastDecodingErrorTime = getTimeNowNt();
			someSortOfTriggerError = true;
		}
	}
	if (someSortOfTriggerError) {
		if (getTimeNowNt() - lastDecodingErrorTime > US2NT(US_PER_SECOND_LL)) {
			someSortOfTriggerError = false;
		}
	}

	runtimeStatistics(nowNt PASS_ENGINE_PARAMETER_SUFFIX);

	// Needed for early instant-RPM detection
	if (!engine->isInitializingTrigger) {
		engine->rpmCalculator.setSpinningUp(nowNt PASS_ENGINE_PARAMETER_SUFFIX);
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
 * This function finds the index of synchronization event within TriggerShape
 */
uint32_t findTriggerZeroEventIndex(TriggerState *state, TriggerShape * shape,
		trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_SUFFIX) {
	UNUSED(triggerConfig);
#if EFI_PROD_CODE
	efiAssert(CUSTOM_ERR_ASSERT, getCurrentRemainingStack() > 128, "findPos", -1);
#endif
	errorDetection.clear();
	efiAssert(CUSTOM_ERR_ASSERT, state != NULL, "NULL state", -1);

	state->resetTriggerState();

	if (shape->shapeDefinitionError) {
		return 0;
	}

	engine->isInitializingTrigger = true;

	// todo: should this variable be declared 'static' to reduce stack usage?
	TriggerStimulatorHelper helper;

	uint32_t syncIndex = helper.findTriggerSyncPoint(shape, state PASS_ENGINE_PARAMETER_SUFFIX);
	if (syncIndex == EFI_ERROR_CODE) {
		engine->isInitializingTrigger = false;
		return syncIndex;
	}
	efiAssert(CUSTOM_ERR_ASSERT, state->getTotalRevolutionCounter() == 1, "findZero_revCounter", EFI_ERROR_CODE);

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
	state->triggerCycleCallback = onFindIndexCallback;

	helper.assertSyncPositionAndSetDutyCycle(syncIndex, state, shape PASS_ENGINE_PARAMETER_SUFFIX);

	engine->isInitializingTrigger = false;
	return syncIndex % shape->getSize();
}

void initTriggerDecoderLogger(Logging *sharedLogger) {
	logger = sharedLogger;
}

void TriggerState::runtimeStatistics(efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	UNUSED(nowNt);
	// empty base implementation
}

 void initTriggerDecoder(void) {
#if EFI_GPIO_HARDWARE
	enginePins.triggerDecoderErrorPin.initPin("trg_err", CONFIGB(triggerErrorPin),
			&CONFIGB(triggerErrorPinMode));
#endif /* EFI_GPIO_HARDWARE */
}

#endif /* EFI_SHAFT_POSITION_INPUT */
