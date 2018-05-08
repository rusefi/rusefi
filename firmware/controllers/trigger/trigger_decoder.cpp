/**
 * @file	trigger_decoder.cpp
 *
 * @date Dec 24, 2013
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
#include "rfiutil.h"

#if EFI_SENSOR_CHART || defined(__DOXYGEN__)
#include "sensor_chart.h"
#endif


EXTERN_ENGINE
;

static TriggerState initState CCM_OPTIONAL;

static cyclic_buffer<int> errorDetection;
static bool isInitializingTrigger = false; // #286 miata NA config - sync error on startup

#if ! EFI_PROD_CODE || defined(__DOXYGEN__)
bool printTriggerDebug = false;
float actualSynchGap;
#endif /* ! EFI_PROD_CODE */

#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_UNIT_TEST */

static Logging * logger;

efitick_t lastDecodingErrorTime = US2NT(-10000000LL);
// the boolean flag is a performance optimization so that complex comparison is avoided if no error
static bool someSortOfTriggerError = false;

/**
 * @return TRUE is something is wrong with trigger decoding
 */
bool isTriggerDecoderError(void) {
	return errorDetection.sum(6) > 4;
}

bool TriggerState::isValidIndex(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return currentCycle.current_index < TRIGGER_SHAPE(size);
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

#define considerEventForGap() (!TRIGGER_SHAPE(useOnlyPrimaryForSync) || isPrimary)

#define needToSkipFall(type) ((!TRIGGER_SHAPE(gapBothDirections)) && (( TRIGGER_SHAPE(useRiseEdge)) && (type != TV_RISE)))
#define needToSkipRise(type) ((!TRIGGER_SHAPE(gapBothDirections)) && ((!TRIGGER_SHAPE(useRiseEdge)) && (type != TV_FALL)))

#define isLessImportant(type) (needToSkipFall(type) || needToSkipRise(type) || (!considerEventForGap()) )

TriggerState::TriggerState() {
	reset();
}

void TriggerState::reset() {
	triggerCycleCallback = NULL;
	shaft_is_synchronized = false;
	toothed_previous_time = 0;
	toothed_previous_duration = 0;
	durationBeforePrevious = 0;
	thirdPreviousDuration = 0;

	totalRevolutionCounter = 0;
	totalTriggerErrorCounter = 0;
	orderingErrorCounter = 0;
	currentDuration = 0;
	curSignal = SHAFT_PRIMARY_FALLING;
	prevSignal = SHAFT_PRIMARY_FALLING;
	startOfCycleNt = 0;

	resetRunningCounters();
	resetCurrentCycleState();
	memset(expectedTotalTime, 0, sizeof(expectedTotalTime));

	totalEventCountBase = 0;
	isFirstEvent = true;
}

int TriggerState::getCurrentIndex() {
	return currentCycle.current_index;
}

void TriggerState::incrementTotalEventCounter() {
	totalRevolutionCounter++;
}

bool TriggerState::isEvenRevolution() {
	return totalRevolutionCounter & 1;
}

void TriggerState::resetCurrentCycleState() {
	memset(currentCycle.eventCount, 0, sizeof(currentCycle.eventCount));
	memset(currentCycle.timeOfPreviousEventNt, 0, sizeof(currentCycle.timeOfPreviousEventNt));
	memset(currentCycle.totalTimeNt, 0, sizeof(currentCycle.totalTimeNt));
	currentCycle.current_index = 0;
}

void TriggerState::onSynchronizationLost(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	shaft_is_synchronized = false;
	// Needed for early instant-RPM detection
	engine->rpmCalculator.setStopSpinning(PASS_ENGINE_PARAMETER_SIGNATURE);
}

/**
 * @brief Trigger decoding happens here
 * This method is invoked every time we have a fall or rise on one of the trigger sensors.
 * This method changes the state of trigger_state_s data structure according to the trigger event
 * @param signal type of event which just happened
 * @param nowNt current time
 */
void TriggerState::decodeTriggerEvent(trigger_event_e const signal, efitime_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efiAssertVoid(signal <= SHAFT_3RD_RISING, "unexpected signal");

	trigger_wheel_e triggerWheel = eventIndex[signal];
	trigger_value_e type = eventType[signal];

	if (!CONFIG(useOnlyRisingEdgeForTrigger) && curSignal == prevSignal) {
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

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (printTriggerDebug) {
			printf("%s event %s %d\r\n",
					getTrigger_type_e(engineConfiguration->trigger.type),
					getTrigger_event_e(signal),
					nowNt);
		}
#endif /* EFI_UNIT_TEST */

		isFirstEvent = false;
// todo: skip a number of signal from the beginning

#if EFI_PROD_CODE || defined(__DOXYGEN__)
//	scheduleMsg(&logger, "from %.2f to %.2f %d %d", triggerConfig->syncRatioFrom, triggerConfig->syncRatioTo, currentDuration, shaftPositionState->toothed_previous_duration);
//	scheduleMsg(&logger, "ratio %.2f", 1.0 * currentDuration/ shaftPositionState->toothed_previous_duration);
#else
		if (printTriggerDebug) {
			printf("ratio %.2f: current=%d previous=%d\r\n", 1.0 * currentDuration / toothed_previous_duration,
				currentDuration, toothed_previous_duration);
		}
#endif

		bool isSynchronizationPoint;

		if (TRIGGER_SHAPE(isSynchronizationNeeded)) {
			// this is getting a little out of hand, any ideas?

			if (CONFIG(debugMode) == DBG_TRIGGER_SYNC) {
				float currentGap = 1.0 * currentDuration / toothed_previous_duration;
#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
				tsOutputChannels.debugFloatField1 = currentGap;
				tsOutputChannels.debugFloatField2 = currentCycle.current_index;
#endif /* EFI_UNIT_TEST */
			}

			bool primaryGap = currentDuration > toothed_previous_duration * TRIGGER_SHAPE(syncRatioFrom)
				&& currentDuration < toothed_previous_duration * TRIGGER_SHAPE(syncRatioTo);

			bool secondaryGap = cisnan(TRIGGER_SHAPE(secondSyncRatioFrom)) || (toothed_previous_duration > durationBeforePrevious * TRIGGER_SHAPE(secondSyncRatioFrom)
			&& toothed_previous_duration < durationBeforePrevious * TRIGGER_SHAPE(secondSyncRatioTo));

			bool thirdGap = cisnan(TRIGGER_SHAPE(thirdSyncRatioFrom)) || (durationBeforePrevious > thirdPreviousDuration * TRIGGER_SHAPE(thirdSyncRatioFrom)
			&& durationBeforePrevious < thirdPreviousDuration * TRIGGER_SHAPE(thirdSyncRatioTo));

			/**
			 * Here I prefer to have two multiplications instead of one division, that's a micro-optimization
			 */
			isSynchronizationPoint = primaryGap
					&& secondaryGap
					&& thirdGap;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
			if (CONFIG(isPrintTriggerSynchDetails) || (someSortOfTriggerError && !CONFIG(silentTriggerError))) {
#else
				if (printTriggerDebug) {
#endif /* EFI_PROD_CODE */
				float gap = 1.0 * currentDuration / toothed_previous_duration;
				float prevGap = 1.0 * toothed_previous_duration / durationBeforePrevious;
				float gap3 = 1.0 * durationBeforePrevious / thirdPreviousDuration;
#if EFI_PROD_CODE || defined(__DOXYGEN__)
				scheduleMsg(logger, "%d: cur=%.2f/prev=%.2f/3rd=%.2f @ %d while expected from %.2f to %.2f and 2nd from %.2f to %.2f and 3rd from %.2f to %.2f error=%d",
						getTimeNowSeconds(),
						gap, prevGap, gap3,
						currentCycle.current_index,
						TRIGGER_SHAPE(syncRatioFrom), TRIGGER_SHAPE(syncRatioTo),
						TRIGGER_SHAPE(secondSyncRatioFrom), TRIGGER_SHAPE(secondSyncRatioTo),
						TRIGGER_SHAPE(thirdSyncRatioFrom), TRIGGER_SHAPE(thirdSyncRatioTo),
						someSortOfTriggerError);
#else
				actualSynchGap = gap;
				print("current gap %.2f/%.2f/%.2f c=%d prev=%d\r\n", gap, prevGap, gap3, currentDuration, toothed_previous_duration);
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
#endif /* EFI_UNIT_TEST */
			int endOfCycleIndex = TRIGGER_SHAPE(size) - (CONFIG(useOnlyRisingEdgeForTrigger) ? 2 : 1);


			isSynchronizationPoint = !shaft_is_synchronized || (currentCycle.current_index >= endOfCycleIndex);

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
			if (printTriggerDebug) {
				printf("isSynchronizationPoint=%d index=%d size=%d\r\n",
						isSynchronizationPoint,
						currentCycle.current_index,
						TRIGGER_SHAPE(size));
			}
#endif /* EFI_UNIT_TEST */

		}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		if (printTriggerDebug) {
			printf("%s isSynchronizationPoint=%d index=%d %s\r\n",
					getTrigger_type_e(engineConfiguration->trigger.type),
					isSynchronizationPoint, currentCycle.current_index,
					getTrigger_event_e(signal));
		}
#endif /* EFI_UNIT_TEST */

		if (isSynchronizationPoint) {

			/**
			 * We can check if things are fine by comparing the number of events in a cycle with the expected number of event.
			 */
			bool isDecodingError = currentCycle.eventCount[0] != TRIGGER_SHAPE(expectedEventCount[0])
					|| currentCycle.eventCount[1] != TRIGGER_SHAPE(expectedEventCount[1])
					|| currentCycle.eventCount[2] != TRIGGER_SHAPE(expectedEventCount[2]);

			enginePins.triggerDecoderErrorPin.setValue(isDecodingError);
			if (isDecodingError && !isInitializingTrigger) {
				if (engineConfiguration->debugMode == DBG_TRIGGER_SYNC) {
#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)

					tsOutputChannels.debugIntField1 = currentCycle.eventCount[0];
					tsOutputChannels.debugIntField2 = currentCycle.eventCount[1];
					tsOutputChannels.debugIntField3 = currentCycle.eventCount[2];
#endif /* EFI_UNIT_TEST */
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
				if (CONFIG(isPrintTriggerSynchDetails) || someSortOfTriggerError) {
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


			if (triggerCycleCallback != NULL) {
				triggerCycleCallback(this);
			}

			startOfCycleNt = nowNt;
			resetCurrentCycleState();
			incrementTotalEventCounter();
			runningRevolutionCounter++;
			totalEventCountBase += TRIGGER_SHAPE(size);


#if EFI_UNIT_TEST || defined(__DOXYGEN__)
			if (printTriggerDebug) {
				printf("index=%d %d\r\n",
						currentCycle.current_index,
						runningRevolutionCounter);
			}
#endif /* EFI_UNIT_TEST */
		} else {	/* if (!isSynchronizationPoint) */
			nextTriggerEvent()
			;
		}

		thirdPreviousDuration = durationBeforePrevious;
		durationBeforePrevious = toothed_previous_duration;
		toothed_previous_duration = currentDuration;
		toothed_previous_time = nowNt;
	}
	if (!isValidIndex(PASS_ENGINE_PARAMETER_SIGNATURE) && !isInitializingTrigger) {
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

	runtimeStatistics(nowNt PASS_ENGINE_PARAMETER_SUFFIX);

	// Needed for early instant-RPM detection
	if (!isInitializingTrigger) {
		engine->rpmCalculator.setSpinningUp(nowNt PASS_ENGINE_PARAMETER_SUFFIX);
	}
}

/**
 * External logger is needed because at this point our logger is not yet initialized
 */
void TriggerShape::initializeTriggerShape(Logging *logger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	const trigger_config_s *triggerConfig = &engineConfiguration->trigger;
#if !EFI_UNIT_TEST
	// we have a confusing threading model so some synchronization would not hurt
	bool alreadyLocked = lockAnyContext();
#endif /* EFI_UNIT_TEST */

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 256, "init t");
	scheduleMsg(logger, "initializeTriggerShape(%s/%d)", getTrigger_type_e(triggerConfig->type), (int) triggerConfig->type);
#endif

	shapeDefinitionError = false;

	switch (triggerConfig->type) {

	case TT_TOOTHED_WHEEL:
		initializeSkippedToothTriggerShapeExt(this, triggerConfig->customTotalToothCount,
				triggerConfig->customSkippedToothCount, engineConfiguration->operationMode PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_MAZDA_MIATA_NA:
		initializeMazdaMiataNaShape(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_MAZDA_MIATA_NB1:
		initializeMazdaMiataNb1Shape(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_MAZDA_MIATA_VVT_TEST:
		initializeMazdaMiataVVtTestShape(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_MIATA_VVT:
		initializeMazdaMiataNb2Crank(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_DODGE_NEON_1995:
		configureNeon1995TriggerShape(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_DODGE_NEON_1995_ONLY_CRANK:
		configureNeon1995TriggerShapeOnlyCrank(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_DODGE_STRATUS:
		configureDodgeStratusTriggerShape(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_DODGE_NEON_2003_CAM:
		configureNeon2003TriggerShapeCam(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_DODGE_NEON_2003_CRANK:
		configureNeon2003TriggerShapeCam(this PASS_ENGINE_PARAMETER_SUFFIX);
//		configureNeon2003TriggerShapeCrank(triggerShape PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_FORD_ASPIRE:
		configureFordAspireTriggerShape(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_GM_7X:
		configureGmTriggerShape(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_MAZDA_DOHC_1_4:
		configureMazdaProtegeLx(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_ONE_PLUS_ONE:
		configureOnePlusOne(this, engineConfiguration->operationMode PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_3_1_CAM:
		configure3_1_cam(this, engineConfiguration->operationMode PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_ONE_PLUS_TOOTHED_WHEEL_60_2:
		configureOnePlus60_2(this, engineConfiguration->operationMode PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_ONE:
		setToothedWheelConfiguration(this, 1, 0, engineConfiguration->operationMode PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_MAZDA_SOHC_4:
		configureMazdaProtegeSOHC(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_MINI_COOPER_R50:
		configureMiniCooperTriggerShape(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_TOOTHED_WHEEL_60_2:
		setToothedWheelConfiguration(this, 60, 2, engineConfiguration->operationMode PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_60_2_VW:
		setVwConfiguration(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_TOOTHED_WHEEL_36_1:
		setToothedWheelConfiguration(this, 36, 1, engineConfiguration->operationMode PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_HONDA_4_24_1:
		configureHonda_1_4_24(this, true, true, T_CHANNEL_3, T_PRIMARY, 0 PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_HONDA_4_24:
		configureHonda_1_4_24(this, false, true, T_NONE, T_PRIMARY, 0 PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_HONDA_1_24:
		configureHonda_1_4_24(this, true, false, T_PRIMARY, T_NONE, 10 PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_HONDA_ACCORD_1_24_SHIFTED:
		configureHondaAccordShifted(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_HONDA_1_4_24:
		configureHondaAccordCDDip(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_HONDA_CBR_600:
		configureHondaCbr600(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_HONDA_CBR_600_CUSTOM:
		configureHondaCbr600custom(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_MITSUBISHI:
		initializeMitsubishi4g18(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_DODGE_RAM:
		initDodgeRam(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_JEEP_18_2_2_2:
		initJeep18_2_2_2(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_SUBARU_7_6:
		initializeSubaru7_6(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_36_2_2_2:
		initialize36_2_2_2(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_2JZ_3_34:
		initialize2jzGE3_34(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_2JZ_1_12:
		initialize2jzGE1_12(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_NISSAN_SR20VE:
		initializeNissanSR20VE_4(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_NISSAN_SR20VE_360:
		initializeNissanSR20VE_4_360(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_ROVER_K:
		initializeRoverK(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	case TT_GM_LS_24:
		initGmLS24(this PASS_ENGINE_PARAMETER_SUFFIX);
		break;

	default:
		shapeDefinitionError = true;
		warning(CUSTOM_ERR_NO_SHAPE, "initializeTriggerShape() not implemented: %d", triggerConfig->type);
	}
	if (!shapeDefinitionError) {
		wave.checkSwitchTimes(getSize());
		/**
	 	 * this instance is used only to initialize 'this' TriggerShape instance
	 	 * #192 BUG real hardware trigger events could be coming even while we are initializing trigger
	 	 */
		initState.reset();
		calculateTriggerSynchPoint(&initState PASS_ENGINE_PARAMETER_SUFFIX);

		if (engine->triggerCentral.triggerShape.getSize() == 0) {
			firmwareError(CUSTOM_ERR_TRIGGER_ZERO, "triggerShape size is zero");
		}
		engine->engineCycleEventCount = getLength();
	}
	version++;

#if !EFI_UNIT_TEST
	if (!alreadyLocked) {
		unlockAnyContext();
	}
#endif

	// Moved here from mainTriggerCallback()
	prepareOutputSignals(PASS_ENGINE_PARAMETER_SIGNATURE);
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
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	efiAssert(getRemainingStack(chThdGetSelfX()) > 128, "findPos", -1);
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

	uint32_t syncIndex = helper.findTriggerSyncPoint(shape, state PASS_ENGINE_PARAMETER_SUFFIX);
	if (syncIndex == EFI_ERROR_CODE) {
		isInitializingTrigger = false;
		return syncIndex;
	}
	efiAssert(state->getTotalRevolutionCounter() == 1, "findZero_revCounter", EFI_ERROR_CODE);

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
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

	isInitializingTrigger = false;
	return syncIndex % shape->getSize();
}

void initTriggerDecoderLogger(Logging *sharedLogger) {
	logger = sharedLogger;
}

void initTriggerDecoder(void) {
#if EFI_GPIO_HARDWARE || defined(__DOXYGEN__)
	enginePins.triggerDecoderErrorPin.initPin("trg_err", boardConfiguration->triggerErrorPin,
			&boardConfiguration->triggerErrorPinMode);
#endif /* EFI_GPIO_HARDWARE */
}

#endif /* EFI_SHAFT_POSITION_INPUT */
