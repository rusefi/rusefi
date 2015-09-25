/**
 * @file	trigger_decoder.cpp
 *
 * @date Dec 24, 2013
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

#if EFI_SHAFT_POSITION_INPUT || defined(__DOXYGEN__)

#include "obd_error_codes.h"
#include "trigger_decoder.h"
#include "cyclic_buffer.h"
#include "trigger_mazda.h"
#include "trigger_chrysler.h"
#include "trigger_gm.h"
#include "trigger_bmw.h"
#include "trigger_mitsubishi.h"
#include "trigger_subaru.h"
#include "trigger_nissan.h"
#include "auto_generated_enums.h"
#include "trigger_structure.h"
#include "efiGpio.h"
#include "engine.h"
#include "engine_math.h"
#include "trigger_central.h"
#include "trigger_simulator.h"

#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
#include "sensor_chart.h"
#endif

static OutputPin triggerDecoderErrorPin;

EXTERN_ENGINE
;

static cyclic_buffer<int> errorDetection;

#if ! EFI_PROD_CODE || defined(__DOXYGEN__)
bool printTriggerDebug = false;
float actualSynchGap;
#endif /* ! EFI_PROD_CODE */

static Logging * logger;

efitick_t lastDecodingErrorTime = US2NT(-10000000LL);
// the boolean flag is a performance optimization so that complex comparison is avoided if no error
bool_t someSortOfTriggerError = false;

/**
 * @return TRUE is something is wrong with trigger decoding
 */
bool_t isTriggerDecoderError(void) {
	return errorDetection.sum(6) > 4;
}

bool_t TriggerState::isValidIndex(DECLARE_ENGINE_PARAMETER_F) {
	return currentCycle.current_index < TRIGGER_SHAPE(size);
}

float TriggerState::getTriggerDutyCycle(int index) {
	float time = prevTotalTime[index];

	return 100 * time / prevCycleDuration;
}

static trigger_wheel_e eventIndex[6] = { T_PRIMARY, T_PRIMARY, T_SECONDARY, T_SECONDARY, T_CHANNEL_3, T_CHANNEL_3 };
//static trigger_value_e eventType[6] = { TV_LOW, TV_HIGH, TV_LOW, TV_HIGH, TV_LOW, TV_HIGH };

#define getCurrentGapDuration(nowNt) \
	(isFirstEvent ? 0 : (nowNt) - toothed_previous_time)

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
	if (engineConfiguration->useOnlyFrontForTrigger) {currentCycle.current_index++;} \
	currentCycle.current_index++; \
}

#define nextRevolution() { \
	if (cycleCallback != NULL) { \
		cycleCallback(this); \
	} \
	memcpy(prevTotalTime, currentCycle.totalTimeNt, sizeof(prevTotalTime)); \
	prevCycleDuration = nowNt - startOfCycleNt; \
	startOfCycleNt = nowNt; \
	resetCurrentCycleState(); \
	totalRevolutionCounter++; \
	runningRevolutionCounter++; \
	totalEventCountBase += TRIGGER_SHAPE(size); \
}

/**
 * @brief Trigger decoding happens here
 * This method is invoked every time we have a fall or rise on one of the trigger sensors.
 * This method changes the state of trigger_state_s data structure according to the trigger event
 * @param signal type of event which just happened
 * @param nowNt current time
 */
void TriggerState::decodeTriggerEvent(trigger_event_e const signal, efitime_t nowNt DECLARE_ENGINE_PARAMETER_S) {
	efiAssertVoid(signal <= SHAFT_3RD_UP, "unexpected signal");

	trigger_wheel_e triggerWheel = eventIndex[signal];

	if (!engineConfiguration->useOnlyFrontForTrigger && curSignal == prevSignal) {
		orderingErrorCounter++;
	}

	prevSignal = curSignal;
	curSignal = signal;

	currentCycle.eventCount[triggerWheel]++;

	efitime_t currentDurationLong = getCurrentGapDuration(nowNt);

	/**
	 * For performance reasons, we want to work with 32 bit values. If there has been more then
	 * 10 seconds since previous trigger event we do not really care.
	 */
	currentDuration =
			currentDurationLong > 10 * US2NT(US_PER_SECOND_LL) ? 10 * US2NT(US_PER_SECOND_LL) : currentDurationLong;

	bool_t isPrimary = triggerWheel == T_PRIMARY;

	if (isLessImportant(signal)) {
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (printTriggerDebug) {
			printf("%s isLessImportant %s %d\r\n",
					getTrigger_type_e(engineConfiguration->trigger.type),
					getTrigger_event_e(signal),
					nowNt);
		}
#endif

		/**
		 * For less important events we simply increment the index.
		 */
		nextTriggerEvent()
		;
		if (TRIGGER_SHAPE(gapBothDirections) && considerEventForGap()) {
			isFirstEvent = false;
			durationBeforePrevious = toothed_previous_duration;
			toothed_previous_duration = currentDuration;
			toothed_previous_time = nowNt;
		}
	} else {

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (printTriggerDebug) {
			printf("%s event %s %d\r\n",
					getTrigger_type_e(engineConfiguration->trigger.type),
					getTrigger_event_e(signal),
					nowNt);
		}
#endif

		isFirstEvent = false;
// todo: skip a number of signal from the beginning

#if EFI_PROD_CODE || defined(__DOXYGEN__)
//	scheduleMsg(&logger, "from %f to %f %d %d", triggerConfig->syncRatioFrom, triggerConfig->syncRatioTo, currentDuration, shaftPositionState->toothed_previous_duration);
//	scheduleMsg(&logger, "ratio %f", 1.0 * currentDuration/ shaftPositionState->toothed_previous_duration);
#else
		if (toothed_previous_duration != 0) {
//		printf("ratio %f: cur=%d pref=%d\r\n", 1.0 * currentDuration / shaftPositionState->toothed_previous_duration,
//				currentDuration, shaftPositionState->toothed_previous_duration);
		}
#endif

		bool_t isSynchronizationPoint;

		if (TRIGGER_SHAPE(isSynchronizationNeeded)) {
			/**
			 * Here I prefer to have two multiplications instead of one division, that's a micro-optimization
			 */
			isSynchronizationPoint = currentDuration > toothed_previous_duration * TRIGGER_SHAPE(syncRatioFrom)
					&& currentDuration < toothed_previous_duration * TRIGGER_SHAPE(syncRatioTo)
					&& toothed_previous_duration > durationBeforePrevious * TRIGGER_SHAPE(secondSyncRatioFrom)
					&& toothed_previous_duration < durationBeforePrevious * TRIGGER_SHAPE(secondSyncRatioTo)
			;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
			if (engineConfiguration->isPrintTriggerSynchDetails || someSortOfTriggerError) {
#else
				if (printTriggerDebug) {
#endif /* EFI_PROD_CODE */
				float gap = 1.0 * currentDuration / toothed_previous_duration;
				float prevGap = 1.0 * toothed_previous_duration / durationBeforePrevious;
#if EFI_PROD_CODE || defined(__DOXYGEN__)
				scheduleMsg(logger, "gap=%f/%f @ %d while expected %f/%f and %f/%f", gap, prevGap, currentCycle.current_index, TRIGGER_SHAPE(syncRatioFrom), TRIGGER_SHAPE(syncRatioTo), TRIGGER_SHAPE(secondSyncRatioFrom), TRIGGER_SHAPE(secondSyncRatioTo));
#else
				actualSynchGap = gap;
				print("current gap %f/%f c=%d prev=%d\r\n", gap, prevGap, currentDuration, toothed_previous_duration);
#endif /* EFI_PROD_CODE */
			}

		} else {
			/**
			 * in case of noise the counter could be above the expected number of events
			 */
			int d = engineConfiguration->useOnlyFrontForTrigger ? 2 : 1;
			isSynchronizationPoint = !shaft_is_synchronized || (currentCycle.current_index >= TRIGGER_SHAPE(size) - d);

		}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (printTriggerDebug) {
			printf("%s isSynchronizationPoint=%d index=%d %s\r\n",
					getTrigger_type_e(engineConfiguration->trigger.type),
					isSynchronizationPoint, currentCycle.current_index,
					getTrigger_event_e(signal));
		}
#endif

		if (isSynchronizationPoint) {

			/**
			 * We can check if things are fine by comparing the number of events in a cycle with the expected number of event.
			 */
			bool isDecodingError = currentCycle.eventCount[0] != TRIGGER_SHAPE(expectedEventCount[0])
					|| currentCycle.eventCount[1] != TRIGGER_SHAPE(expectedEventCount[1])
					|| currentCycle.eventCount[2] != TRIGGER_SHAPE(expectedEventCount[2]);

			triggerDecoderErrorPin.setValue(isDecodingError);
			if (isDecodingError) {
				lastDecodingErrorTime = getTimeNowNt();
				someSortOfTriggerError = true;

				totalTriggerErrorCounter++;
				if (engineConfiguration->isPrintTriggerSynchDetails || someSortOfTriggerError) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
					scheduleMsg(logger, "error: synchronizationPoint @ index %d expected %d/%d/%d got %d/%d/%d",
							currentCycle.current_index, TRIGGER_SHAPE(expectedEventCount[0]),
							TRIGGER_SHAPE(expectedEventCount[1]), TRIGGER_SHAPE(expectedEventCount[2]),
							currentCycle.eventCount[0], currentCycle.eventCount[1], currentCycle.eventCount[2]);
#endif /* EFI_PROD_CODE */
				}
			}

			errorDetection.add(isDecodingError);

			if (isTriggerDecoderError()) {
				warning(OBD_PCM_Processor_Fault, "trigger decoding issue. expected %d/%d/%d got %d/%d/%d",
						TRIGGER_SHAPE(expectedEventCount[0]), TRIGGER_SHAPE(expectedEventCount[1]),
						TRIGGER_SHAPE(expectedEventCount[2]), currentCycle.eventCount[0], currentCycle.eventCount[1],
						currentCycle.eventCount[2]);
			}

			shaft_is_synchronized = true;
			// this call would update duty cycle values
			nextTriggerEvent()
			;

			nextRevolution();
		} else {
			nextTriggerEvent()
			;
		}

		durationBeforePrevious = toothed_previous_duration;
		toothed_previous_duration = currentDuration;
		toothed_previous_time = nowNt;
	}
	if (!isValidIndex(PASS_ENGINE_PARAMETER_F)) {
		warning(OBD_PCM_Processor_Fault, "unexpected eventIndex=%d while size %d", currentCycle.current_index, TRIGGER_SHAPE(size));
		lastDecodingErrorTime = getTimeNowNt();
		someSortOfTriggerError = true;
	}
	if (someSortOfTriggerError) {
		if (getTimeNowNt() - lastDecodingErrorTime > US2NT(US_PER_SECOND_LL)) {
			someSortOfTriggerError = false;
		}
	}

	if (boardConfiguration->sensorChartMode == SC_RPM_ACCEL || boardConfiguration->sensorChartMode == SC_DETAILED_RPM) {
		angle_t currentAngle = TRIGGER_SHAPE(eventAngles[currentCycle.current_index]);
		// todo: make this '90' depend on cylinder count?
		angle_t prevAngle = currentAngle - 90;
		fixAngle(prevAngle);
		// todo: prevIndex should be pre-calculated
		int prevIndex = TRIGGER_SHAPE(triggerIndexByAngle[(int)prevAngle]);
		// now let's get precise angle for that event
		prevAngle = TRIGGER_SHAPE(eventAngles[prevIndex]);
		uint32_t time = nowNt - timeOfLastEvent[prevIndex];
		angle_t angleDiff = currentAngle - prevAngle;
		// todo: angle diff should be pre-calculated
		fixAngle(angleDiff);

		float r = (60000000.0 / 360 * US_TO_NT_MULTIPLIER) * angleDiff / time;

#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
		if (boardConfiguration->sensorChartMode == SC_DETAILED_RPM) {
			scAddData(currentAngle, r);
		} else {
			scAddData(currentAngle, r / instantRpmValue[prevIndex]);
		}
#endif
		instantRpmValue[currentCycle.current_index] = r;
		timeOfLastEvent[currentCycle.current_index] = nowNt;
	}
}

float getEngineCycle(operation_mode_e operationMode) {
	return operationMode == TWO_STROKE ? 360 : 720;
}

void addSkippedToothTriggerEvents(trigger_wheel_e wheel, TriggerShape *s, int totalTeethCount, int skippedCount,
		float toothWidth, float offset, float engineCycle, float filterLeft, float filterRight) {
	efiAssertVoid(totalTeethCount > 0, "total count");
	efiAssertVoid(skippedCount >= 0, "skipped count");

	for (int i = 0; i < totalTeethCount - skippedCount - 1; i++) {
		float angleDown = engineCycle / totalTeethCount * (i + (1 - toothWidth));
		float angleUp = engineCycle / totalTeethCount * (i + 1);
		s->addEvent(offset + angleDown, wheel, TV_HIGH, filterLeft, filterRight);
		s->addEvent(offset + angleUp, wheel, TV_LOW, filterLeft, filterRight);
	}

	float angleDown = engineCycle / totalTeethCount * (totalTeethCount - skippedCount - 1 + (1 - toothWidth));
	s->addEvent(offset + angleDown, wheel, TV_HIGH, filterLeft, filterRight);
	s->addEvent(offset + engineCycle, wheel, TV_LOW, filterLeft, filterRight);
}

void initializeSkippedToothTriggerShapeExt(TriggerShape *s, int totalTeethCount, int skippedCount,
		operation_mode_e operationMode) {
	efiAssertVoid(totalTeethCount > 0, "totalTeethCount is zero");

//	s->totalToothCount = totalTeethCount;
//	s->skippedToothCount = skippedCount;

	s->setTriggerSynchronizationGap(skippedCount + 1);
	s->isSynchronizationNeeded = (skippedCount != 0);

	efiAssertVoid(s != NULL, "TriggerShape is NULL");
	s->reset(operationMode, false);

	addSkippedToothTriggerEvents(T_PRIMARY, s, totalTeethCount, skippedCount, 0.5, 0, getEngineCycle(operationMode),
	NO_LEFT_FILTER, NO_RIGHT_FILTER);
}

static void configureOnePlusOne(TriggerShape *s, operation_mode_e operationMode) {
	float engineCycle = getEngineCycle(operationMode);

	s->reset(FOUR_STROKE_CAM_SENSOR, true);

	s->addEvent(180, T_PRIMARY, TV_HIGH);
	s->addEvent(360, T_PRIMARY, TV_LOW);

	s->addEvent(540, T_SECONDARY, TV_HIGH);
	s->addEvent(720, T_SECONDARY, TV_LOW);

	s->isSynchronizationNeeded = false;
}

static void configureOnePlus60_2(TriggerShape *s, operation_mode_e operationMode) {
	s->reset(FOUR_STROKE_CAM_SENSOR, true);

	int totalTeethCount = 60;
	int skippedCount = 2;

	s->addEvent(2, T_PRIMARY, TV_HIGH);
	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 360, 2, 20);
	s->addEvent(20, T_PRIMARY, TV_LOW);
	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 360, 20, NO_RIGHT_FILTER);

	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 360, 360, NO_LEFT_FILTER,
	NO_RIGHT_FILTER);

	s->isSynchronizationNeeded = false;
}

/**
 * External logger is needed because at this point our logger is not yet initialized
 */
void TriggerShape::initializeTriggerShape(Logging *logger DECLARE_ENGINE_PARAMETER_S) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	efiAssertVoid(getRemainingStack(chThdSelf()) > 256, "init t");
	scheduleMsg(logger, "initializeTriggerShape()");
#endif
	TriggerShape *triggerShape = this;

	const trigger_config_s *triggerConfig = &engineConfiguration->trigger;

	clear();

	switch (triggerConfig->type) {

	case TT_TOOTHED_WHEEL:
		initializeSkippedToothTriggerShapeExt(triggerShape, triggerConfig->customTotalToothCount,
				triggerConfig->customSkippedToothCount, engineConfiguration->operationMode);
		break;

	case TT_MAZDA_MIATA_NA:
		initializeMazdaMiataNaShape(triggerShape);
		break;

	case TT_MAZDA_MIATA_NB:
		initializeMazdaMiataNbShape(triggerShape);
		break;

	case TT_DODGE_NEON_1995:
		configureNeon1995TriggerShape(triggerShape);
		break;

	case TT_DODGE_STRATUS:
		configureDodgeStratusTriggerShape(triggerShape);
		break;

	case TT_DODGE_NEON_2003:
		configureNeon2003TriggerShape(triggerShape);
		break;

	case TT_FORD_ASPIRE:
		configureFordAspireTriggerShape(triggerShape);
		break;

	case TT_GM_7X:
		// todo: fix this configureGmTriggerShape(triggerShape);
		configureFordAspireTriggerShape(triggerShape);
		break;

	case TT_MAZDA_DOHC_1_4:
		configureMazdaProtegeLx(triggerShape);
		break;

	case TT_ONE_PLUS_ONE:
		configureOnePlusOne(triggerShape, engineConfiguration->operationMode);
		break;

	case TT_ONE_PLUS_TOOTHED_WHEEL_60_2:
		configureOnePlus60_2(triggerShape, engineConfiguration->operationMode);
		break;

	case TT_ONE:
		setToothedWheelConfiguration(triggerShape, 1, 0, engineConfiguration->operationMode);
		break;

	case TT_MAZDA_SOHC_4:
		configureMazdaProtegeSOHC(triggerShape);
		break;

	case TT_MINI_COOPER_R50:
		configureMiniCooperTriggerShape(triggerShape);
		break;

	case TT_TOOTHED_WHEEL_60_2:
		setToothedWheelConfiguration(triggerShape, 60, 2, engineConfiguration->operationMode);
		break;

	case TT_60_2_VW:
		setVwConfiguration(triggerShape);
		break;

	case TT_TOOTHED_WHEEL_36_1:
		setToothedWheelConfiguration(triggerShape, 36, 1, engineConfiguration->operationMode);
		break;

	case TT_HONDA_ACCORD_CD_TWO_WIRES:
		configureHondaAccordCD(triggerShape, false, true, T_CHANNEL_3, T_PRIMARY, 0);
		break;

	case TT_HONDA_ACCORD_CD:
		configureHondaAccordCD(triggerShape, true, true, T_CHANNEL_3, T_PRIMARY, 0);
		break;

	case TT_HONDA_ACCORD_1_24:
		configureHondaAccordCD(triggerShape, true, false, T_PRIMARY, T_PRIMARY, 10);
		break;

	case TT_HONDA_ACCORD_CD_DIP:
		configureHondaAccordCDDip(triggerShape);
		break;

	case TT_MITSU:
		initializeMitsubishi4g18(triggerShape);
		break;

	case TT_DODGE_RAM:
		initDodgeRam(triggerShape);
		break;

	case TT_36_2_2_2:
		initialize36_2_2_2(triggerShape);
		break;

	case TT_NISSAN:
		initializeNissan(triggerShape);
		break;

	default:
		firmwareError("initializeTriggerShape() not implemented: %d", triggerConfig->type);
		;
		return;
	}
	wave.checkSwitchTimes(getSize());
	calculateTriggerSynchPoint(&engine->triggerCentral.triggerState PASS_ENGINE_PARAMETER);
}

static void onFindIndex(TriggerState *state) {
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
		trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_S) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	efiAssert(getRemainingStack(chThdSelf()) > 128, "findPos", -1);
#endif
	errorDetection.clear();
	efiAssert(state != NULL, "NULL state", -1);

	state->reset();

	// todo: should this variable be declared 'static' to reduce stack usage?
	TriggerStimulatorHelper helper;

	uint32_t index = helper.doFindTrigger(shape, triggerConfig, state PASS_ENGINE_PARAMETER);
	if (index == EFI_ERROR_CODE) {
		return index;
	}
	efiAssert(state->getTotalRevolutionCounter() == 1, "totalRevolutionCounter", EFI_ERROR_CODE);

	/**
	 * Now that we have just located the synch point, we can simulate the whole cycle
	 * in order to calculate expected duty cycle
	 *
	 * todo: add a comment why are we doing '2 * shape->getSize()' here?
	 */
	state->cycleCallback = onFindIndex;

	helper.assertSyncPositionAndSetDutyCycle(index, state, shape, triggerConfig PASS_ENGINE_PARAMETER);

	return index % shape->getSize();
}

void initTriggerDecoderLogger(Logging *sharedLogger) {
	logger = sharedLogger;
}

void initTriggerDecoder(void) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
	outputPinRegisterExt2("trg_err", &triggerDecoderErrorPin, boardConfiguration->triggerErrorPin,
			&boardConfiguration->triggerErrorPinMode);
#endif
}

#endif /* EFI_SHAFT_POSITION_INPUT */
