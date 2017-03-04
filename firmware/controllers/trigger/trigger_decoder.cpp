/**
 * @file	trigger_decoder.cpp
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
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
#include "trigger_toyota.h"
#include "trigger_rover.h"
#include "trigger_honda.h"
#include "trigger_structure.h"
#include "efiGpio.h"
#include "engine.h"
#include "engine_math.h"
#include "trigger_central.h"
#include "trigger_simulator.h"
#include "trigger_universal.h"

#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
#include "sensor_chart.h"
#endif


EXTERN_ENGINE
;

static cyclic_buffer<int> errorDetection;
static bool isInitializingTrigger = false; // #286 miata NA config - sync error on startup

#if ! EFI_PROD_CODE || defined(__DOXYGEN__)
bool printTriggerDebug = false;
float actualSynchGap;
#endif /* ! EFI_PROD_CODE */

static Logging * logger;

efitick_t lastDecodingErrorTime = US2NT(-10000000LL);
// the boolean flag is a performance optimization so that complex comparison is avoided if no error
bool someSortOfTriggerError = false;

/**
 * @return TRUE is something is wrong with trigger decoding
 */
bool isTriggerDecoderError(void) {
	return errorDetection.sum(6) > 4;
}

bool TriggerState::isValidIndex(DECLARE_ENGINE_PARAMETER_F) {
	return currentCycle.current_index < TRIGGER_SHAPE(size);
}

float TriggerState::getTriggerDutyCycle(int index) {
	float time = prevTotalTime[index];

	return 100 * time / prevCycleDuration;
}

static trigger_wheel_e eventIndex[6] = { T_PRIMARY, T_PRIMARY, T_SECONDARY, T_SECONDARY, T_CHANNEL_3, T_CHANNEL_3 };
static trigger_value_e eventType[6] = { TV_FALL, TV_RISE, TV_FALL, TV_RISE, TV_FALL, TV_RISE };

#define getCurrentGapDuration(nowNt) \
	(isFirstEvent ? 0 : (nowNt) - toothed_previous_time)

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
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

#define nextRevolution() { \
	if (cycleCallback != NULL) { \
		cycleCallback(this); \
	} \
	memcpy(prevTotalTime, currentCycle.totalTimeNt, sizeof(prevTotalTime)); \
	prevCycleDuration = nowNt - startOfCycleNt; \
	startOfCycleNt = nowNt; \
	resetCurrentCycleState(); \
	intTotalEventCounter(); \
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
	efiAssertVoid(signal <= SHAFT_3RD_RISING, "unexpected signal");

	trigger_wheel_e triggerWheel = eventIndex[signal];
	trigger_value_e type = eventType[signal];

	if (!engineConfiguration->useOnlyRisingEdgeForTrigger && curSignal == prevSignal) {
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

	bool isPrimary = triggerWheel == T_PRIMARY;

	if (isLessImportant(type)) {
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (printTriggerDebug) {
			printf("%s isLessImportant %s now=%d index=%d\r\n",
					getTrigger_type_e(engineConfiguration->trigger.type),
					getTrigger_event_e(signal),
					nowNt,
					currentCycle.current_index);
		}
#endif

		/**
		 * For less important events we simply increment the index.
		 */
		nextTriggerEvent()
		;
		if (TRIGGER_SHAPE(gapBothDirections) && considerEventForGap()) {
			isFirstEvent = false;
			thirdPreviousDuration = durationBeforePrevious;
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
		if (printTriggerDebug) {
			printf("ratio %f: current=%d previous=%d\r\n", 1.0 * currentDuration / toothed_previous_duration,
				currentDuration, toothed_previous_duration);
		}
#endif

		bool isSynchronizationPoint;

		if (TRIGGER_SHAPE(isSynchronizationNeeded)) {
			/**
			 * Here I prefer to have two multiplications instead of one division, that's a micro-optimization
			 */
			isSynchronizationPoint =
					   currentDuration > toothed_previous_duration * TRIGGER_SHAPE(syncRatioFrom)
					&& currentDuration < toothed_previous_duration * TRIGGER_SHAPE(syncRatioTo)
					&& toothed_previous_duration > durationBeforePrevious * TRIGGER_SHAPE(secondSyncRatioFrom)
					&& toothed_previous_duration < durationBeforePrevious * TRIGGER_SHAPE(secondSyncRatioTo)
// this is getting a little out of hand, any ideas?
					&& durationBeforePrevious > thirdPreviousDuration * TRIGGER_SHAPE(thirdSyncRatioFrom)
					&& durationBeforePrevious < thirdPreviousDuration * TRIGGER_SHAPE(thirdSyncRatioTo)
;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
			if (engineConfiguration->isPrintTriggerSynchDetails || someSortOfTriggerError) {
#else
				if (printTriggerDebug) {
#endif /* EFI_PROD_CODE */
				float gap = 1.0 * currentDuration / toothed_previous_duration;
				float prevGap = 1.0 * toothed_previous_duration / durationBeforePrevious;
				float gap3 = 1.0 * durationBeforePrevious / thirdPreviousDuration;
#if EFI_PROD_CODE || defined(__DOXYGEN__)
				scheduleMsg(logger, "%d: gap=%f/%f/%f @ %d while expected %f/%f and %f/%f error=%d",
						getTimeNowSeconds(),
						gap, prevGap, gap3,
						currentCycle.current_index,
						TRIGGER_SHAPE(syncRatioFrom), TRIGGER_SHAPE(syncRatioTo),
						TRIGGER_SHAPE(secondSyncRatioFrom), TRIGGER_SHAPE(secondSyncRatioTo), someSortOfTriggerError);
#else
				actualSynchGap = gap;
				print("current gap %f/%f/%f c=%d prev=%d\r\n", gap, prevGap, gap3, currentDuration, toothed_previous_duration);
#endif /* EFI_PROD_CODE */
			}

		} else {
			/**
			 * We are here in case of a wheel without synchronization - we just need to count events,
			 * synchronization point simply happens once we have the right number of events
			 *
			 * in case of noise the counter could be above the expected number of events, that's why 'more or equals' and not just 'equals'
			 */

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (printTriggerDebug) {
			printf("sync=%d index=%d size=%d\r\n",
					shaft_is_synchronized,
					currentCycle.current_index,
					TRIGGER_SHAPE(size));
		}
#endif
			int endOfCycleIndex = TRIGGER_SHAPE(size) - (engineConfiguration->useOnlyRisingEdgeForTrigger ? 2 : 1);


			isSynchronizationPoint = !shaft_is_synchronized || (currentCycle.current_index >= endOfCycleIndex);

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (printTriggerDebug) {
			printf("isSynchronizationPoint=%d index=%d size=%d\r\n",
					isSynchronizationPoint,
					currentCycle.current_index,
					TRIGGER_SHAPE(size));
		}
#endif


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

			enginePins.triggerDecoderErrorPin.setValue(isDecodingError);
			if (isDecodingError && !isInitializingTrigger) {
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
				warning(CUSTOM_OBD_TRG_DECODING, "trigger decoding issue. expected %d/%d/%d got %d/%d/%d",
						TRIGGER_SHAPE(expectedEventCount[0]), TRIGGER_SHAPE(expectedEventCount[1]),
						TRIGGER_SHAPE(expectedEventCount[2]), currentCycle.eventCount[0], currentCycle.eventCount[1],
						currentCycle.eventCount[2]);
			}

			shaft_is_synchronized = true;
			// this call would update duty cycle values
			nextTriggerEvent()
			;

			nextRevolution();

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
			if (printTriggerDebug) {
				printf("index=%d %d\r\n",
						currentCycle.current_index,
						runningRevolutionCounter);
			}
#endif /* EFI_UNIT_TEST */
		} else {
			nextTriggerEvent()
			;
		}

		thirdPreviousDuration = durationBeforePrevious;
		durationBeforePrevious = toothed_previous_duration;
		toothed_previous_duration = currentDuration;
		toothed_previous_time = nowNt;
	}
	if (!isValidIndex(PASS_ENGINE_PARAMETER_F) && !isInitializingTrigger) {
		// let's not show a warning if we are just starting to spin
		if (engine->rpmCalculator.rpmValue != 0) {
			warning(CUSTOM_SYNC_ERROR, "sync error: index #%d above total size %d", currentCycle.current_index, TRIGGER_SHAPE(size));
			lastDecodingErrorTime = getTimeNowNt();
			someSortOfTriggerError = true;
		}
	}
	if (someSortOfTriggerError) {
		if (getTimeNowNt() - lastDecodingErrorTime > US2NT(US_PER_SECOND_LL)) {
			someSortOfTriggerError = false;
		}
	}

	if (ENGINE(sensorChartMode) == SC_RPM_ACCEL || ENGINE(sensorChartMode) == SC_DETAILED_RPM) {
		angle_t currentAngle = TRIGGER_SHAPE(eventAngles[currentCycle.current_index]);
		// todo: make this '90' depend on cylinder count?
		angle_t prevAngle = currentAngle - 90;
		fixAngle(prevAngle, "prevAngle");
		// todo: prevIndex should be pre-calculated
		int prevIndex = TRIGGER_SHAPE(triggerIndexByAngle[(int)prevAngle]);
		// now let's get precise angle for that event
		prevAngle = TRIGGER_SHAPE(eventAngles[prevIndex]);
// todo: re-implement this as a subclass. we need two instances of
//		uint32_t time = nowNt - timeOfLastEvent[prevIndex];
		angle_t angleDiff = currentAngle - prevAngle;
		// todo: angle diff should be pre-calculated
		fixAngle(angleDiff, "angleDiff");

//		float r = (60000000.0 / 360 * US_TO_NT_MULTIPLIER) * angleDiff / time;

#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
		if (boardConfiguration->sensorChartMode == SC_DETAILED_RPM) {
//			scAddData(currentAngle, r);
		} else {
//			scAddData(currentAngle, r / instantRpmValue[prevIndex]);
		}
#endif
//		instantRpmValue[currentCycle.current_index] = r;
//		timeOfLastEvent[currentCycle.current_index] = nowNt;
	}
}

void configure3_1_cam(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_S) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);


	const float crankW = 360 / 3 / 2;


	trigger_wheel_e crank = T_SECONDARY;

	s->addEvent2(10, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(50, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);


	float a = 2 * crankW;

	// #1/3
	s->addEvent2(a += crankW, crank, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(a += crankW, crank, TV_FALL PASS_ENGINE_PARAMETER);
	// #2/3
	s->addEvent2(a += crankW, crank, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(a += crankW, crank, TV_FALL PASS_ENGINE_PARAMETER);
	// #3/3
	a += crankW;
	a += crankW;

	// 2nd #1/3
	s->addEvent2(a += crankW, crank, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(a += crankW, crank, TV_FALL PASS_ENGINE_PARAMETER);

	// 2nd #2/3
	s->addEvent2(a += crankW, crank, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(a += crankW, crank, TV_FALL PASS_ENGINE_PARAMETER);

	s->isSynchronizationNeeded = false;
}

void configureOnePlusOne(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_S) {
	float engineCycle = getEngineCycle(operationMode);

	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	s->addEvent2(180, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(360, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);

	s->addEvent2(540, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER);
	s->addEvent2(720, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER);

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}

void configureOnePlus60_2(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_S) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	int totalTeethCount = 60;
	int skippedCount = 2;

	s->addEvent2(2, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER);
	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 360, 2, 20 PASS_ENGINE_PARAMETER);
	s->addEvent2(20, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER);
	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 360, 20, NO_RIGHT_FILTER PASS_ENGINE_PARAMETER);

	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 360, 360, NO_LEFT_FILTER,
	NO_RIGHT_FILTER PASS_ENGINE_PARAMETER);

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}

static TriggerState initState CCM_OPTIONAL;

/**
 * External logger is needed because at this point our logger is not yet initialized
 */
void TriggerShape::initializeTriggerShape(Logging *logger DECLARE_ENGINE_PARAMETER_S) {
	const trigger_config_s *triggerConfig = &engineConfiguration->trigger;
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	efiAssertVoid(getRemainingStack(chThdSelf()) > 256, "init t");
	scheduleMsg(logger, "initializeTriggerShape(%s/%d)", getTrigger_type_e(triggerConfig->type), (int) triggerConfig->type);
#endif
	TriggerShape *triggerShape = this;


	switch (triggerConfig->type) {

	case TT_TOOTHED_WHEEL:
		initializeSkippedToothTriggerShapeExt(this, triggerConfig->customTotalToothCount,
				triggerConfig->customSkippedToothCount, engineConfiguration->operationMode PASS_ENGINE_PARAMETER);
		break;

	case TT_MAZDA_MIATA_NA:
		initializeMazdaMiataNaShape(this PASS_ENGINE_PARAMETER);
		break;

	case TT_MAZDA_MIATA_NB1:
		initializeMazdaMiataNb1Shape(this PASS_ENGINE_PARAMETER);
		break;

	case TT_MAZDA_MIATA_VVT_TEST:
		initializeMazdaMiataVVtTestShape(this PASS_ENGINE_PARAMETER);
		break;

	case TT_MIATA_VVT:
		initializeMazdaMiataNb2Crank(this PASS_ENGINE_PARAMETER);
		break;

	case TT_DODGE_NEON_1995:
		configureNeon1995TriggerShape(this PASS_ENGINE_PARAMETER);
		break;

	case TT_DODGE_STRATUS:
		configureDodgeStratusTriggerShape(this PASS_ENGINE_PARAMETER);
		break;

	case TT_DODGE_NEON_2003_CAM:
		configureNeon2003TriggerShapeCam(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_DODGE_NEON_2003_CRANK:
		configureNeon2003TriggerShapeCam(this PASS_ENGINE_PARAMETER);
//		configureNeon2003TriggerShapeCrank(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_FORD_ASPIRE:
		configureFordAspireTriggerShape(this PASS_ENGINE_PARAMETER);
		break;

	case TT_GM_7X:
		// todo: fix this configureGmTriggerShape(triggerShape);
		configureFordAspireTriggerShape(this PASS_ENGINE_PARAMETER);
		break;

	case TT_MAZDA_DOHC_1_4:
		configureMazdaProtegeLx(this PASS_ENGINE_PARAMETER);
		break;

	case TT_ONE_PLUS_ONE:
		configureOnePlusOne(this, engineConfiguration->operationMode PASS_ENGINE_PARAMETER);
		break;

	case TT_3_1_CAM:
		configure3_1_cam(this, engineConfiguration->operationMode PASS_ENGINE_PARAMETER);
		break;

	case TT_ONE_PLUS_TOOTHED_WHEEL_60_2:
		configureOnePlus60_2(this, engineConfiguration->operationMode PASS_ENGINE_PARAMETER);
		break;

	case TT_ONE:
		setToothedWheelConfiguration(triggerShape, 1, 0, engineConfiguration->operationMode PASS_ENGINE_PARAMETER);
		break;

	case TT_MAZDA_SOHC_4:
		configureMazdaProtegeSOHC(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_MINI_COOPER_R50:
		configureMiniCooperTriggerShape(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_TOOTHED_WHEEL_60_2:
		setToothedWheelConfiguration(triggerShape, 60, 2, engineConfiguration->operationMode PASS_ENGINE_PARAMETER);
		break;

	case TT_60_2_VW:
		setVwConfiguration(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_TOOTHED_WHEEL_36_1:
		setToothedWheelConfiguration(triggerShape, 36, 1, engineConfiguration->operationMode PASS_ENGINE_PARAMETER);
		break;

	case TT_HONDA_4_24_1:
		configureHonda_1_4_24(triggerShape, true, true, T_CHANNEL_3, T_PRIMARY, 0 PASS_ENGINE_PARAMETER);
		break;

	case TT_HONDA_4_24:
		configureHonda_1_4_24(this, false, true, T_NONE, T_PRIMARY, 0 PASS_ENGINE_PARAMETER);
		break;

	case TT_HONDA_1_24:
		configureHonda_1_4_24(triggerShape, true, false, T_PRIMARY, T_NONE, 10 PASS_ENGINE_PARAMETER);
		break;

	case TT_HONDA_ACCORD_1_24_SHIFTED:
		configureHondaAccordShifted(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_HONDA_1_4_24:
		configureHondaAccordCDDip(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_HONDA_CBR_600:
		configureHondaCbr600(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_HONDA_CBR_600_CUSTOM:
		configureHondaCbr600custom(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_MITSUBISHI:
		initializeMitsubishi4g18(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_DODGE_RAM:
		initDodgeRam(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_JEEP_18_2_2_2:
		initJeep18_2_2_2(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_SUBARU_7_6:
		initializeSubaru7_6(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_36_2_2_2:
		initialize36_2_2_2(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_2JZ_3_34:
		initialize2jzGE3_34(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_2JZ_1_12:
		initialize2jzGE1_12(triggerShape PASS_ENGINE_PARAMETER);
		break;

	case TT_NISSAN:
		initializeNissan(this PASS_ENGINE_PARAMETER);
		break;

	case TT_ROVER_K:
		initializeRoverK(this PASS_ENGINE_PARAMETER);
		break;

	case TT_GM_LS_24:
		initGmLS24(this PASS_ENGINE_PARAMETER);
		break;

	default:
		firmwareError(CUSTOM_ERR_NO_SHAPE, "initializeTriggerShape() not implemented: %d", triggerConfig->type);
		return;
	}
	wave.checkSwitchTimes(getSize());
	/**
	 * this instance is used only to initialize 'this' TriggerShape instance
	 * #192 BUG real hardware trigger events could be coming even while we are initializing trigger
	 */
	initState.reset();
	calculateTriggerSynchPoint(&initState PASS_ENGINE_PARAMETER);
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
	isInitializingTrigger = true;
	errorDetection.clear();
	efiAssert(state != NULL, "NULL state", -1);

	state->reset();

	if (shape->shapeDefinitionError) {
		return 0;
	}

	// todo: should this variable be declared 'static' to reduce stack usage?
	TriggerStimulatorHelper helper;

	uint32_t syncIndex = helper.doFindTrigger(shape, triggerConfig, state PASS_ENGINE_PARAMETER);
	if (syncIndex == EFI_ERROR_CODE) {
		isInitializingTrigger = false;
		return syncIndex;
	}
	efiAssert(state->getTotalRevolutionCounter() == 1, "totalRevolutionCounter", EFI_ERROR_CODE);

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	if (printTriggerDebug) {
		printf("syncIndex located %d\r\n", syncIndex);
	}
#endif /* EFI_UNIT_TEST */

	/**
	 * Now that we have just located the synch point, we can simulate the whole cycle
	 * in order to calculate expected duty cycle
	 *
	 * todo: add a comment why are we doing '2 * shape->getSize()' here?
	 */
	state->cycleCallback = onFindIndex;

	helper.assertSyncPositionAndSetDutyCycle(syncIndex, state, shape, triggerConfig PASS_ENGINE_PARAMETER);

	isInitializingTrigger = false;
	return syncIndex % shape->getSize();
}

void initTriggerDecoderLogger(Logging *sharedLogger) {
	logger = sharedLogger;
}

void initTriggerDecoder(void) {
#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
	outputPinRegisterExt2("trg_err", &enginePins.triggerDecoderErrorPin, boardConfiguration->triggerErrorPin,
			&boardConfiguration->triggerErrorPinMode);
#endif
}

#endif /* EFI_SHAFT_POSITION_INPUT */
