/*
 * @file spark_logic.cpp
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "spark_logic.h"

// dependency injection
#include "engine_state.h"
#include "rpm_calculator_api.h"
// end of injection

#include "utlist.h"
#include "event_queue.h"
#include "tooth_logger.h"

#include "knock_logic.h"

#if EFI_ENGINE_CONTROL

#if EFI_UNIT_TEST
extern bool verboseMode;
#endif /* EFI_UNIT_TEST */

static const char *prevSparkName = nullptr;

static void fireSparkBySettingPinLow(IgnitionEvent *event, IgnitionOutputPin *output) {
#if SPARK_EXTREME_LOGGING
	efiPrintf("spark goes low  %d %s %d current=%d cnt=%d id=%d", getRevolutionCounter(), output->name, (int)getTimeNowUs(),
			output->currentLogicValue, output->outOfOrder, event->sparkId);
#endif /* SPARK_EXTREME_LOGGING */

	/**
	 * there are two kinds of 'out-of-order'
	 * 1) low goes before high, everything is fine after words
	 *
	 * 2) we have an un-matched low followed by legit pairs
	 */

	output->signalFallSparkId = event->sparkId;

	if (!output->currentLogicValue && !event->wasSparkLimited) {
		warning(ObdCode::CUSTOM_OUT_OF_ORDER_COIL, "out-of-order coil off %s", output->getName());
		output->outOfOrder = true;
	}
	output->setLow();
}

/**
 * @param cylinderIndex from 0 to cylinderCount, not cylinder number
 */
static int getIgnitionPinForIndex(int cylinderIndex, ignition_mode_e ignitionMode) {
	switch (ignitionMode) {
	case IM_ONE_COIL:
		return 0;
	case IM_WASTED_SPARK: {
		if (engineConfiguration->cylindersCount == 1) {
			// we do not want to divide by zero
			return 0;
		}
		return cylinderIndex % (engineConfiguration->cylindersCount / 2);
	}
	case IM_INDIVIDUAL_COILS:
		return cylinderIndex;
	case IM_TWO_COILS:
		return cylinderIndex % 2;

	default:
		firmwareError(ObdCode::CUSTOM_OBD_IGNITION_MODE, "Invalid ignition mode getIgnitionPinForIndex(): %d", engineConfiguration->ignitionMode);
		return 0;
	}
}

static void prepareCylinderIgnitionSchedule(angle_t dwellAngleDuration, floatms_t sparkDwell, IgnitionEvent *event) {
	// todo: clean up this implementation? does not look too nice as is.

	// let's save planned duration so that we can later compare it with reality
	event->sparkDwell = sparkDwell;

	angle_t sparkAngle =
		// Negate because timing *before* TDC, and we schedule *after* TDC
		- getEngineState()->timingAdvance[event->cylinderNumber]
		// Offset by this cylinder's position in the cycle
		+ getCylinderAngle(event->cylinderIndex, event->cylinderNumber)
		// Pull any extra timing for knock retard
		+ engine->module<KnockController>()->getKnockRetard();

	efiAssertVoid(ObdCode::CUSTOM_SPARK_ANGLE_1, !cisnan(sparkAngle), "sparkAngle#1");

	auto ignitionMode = getCurrentIgnitionMode();
	engine->outputChannels.currentIgnitionMode = static_cast<uint8_t>(ignitionMode);

	const int index = getIgnitionPinForIndex(event->cylinderIndex, ignitionMode);
	const int coilIndex = ID2INDEX(getCylinderId(index));
	IgnitionOutputPin *output = &enginePins.coils[coilIndex];

	IgnitionOutputPin *secondOutput;

	// If wasted spark, find the paired coil in addition to "main" output for this cylinder
	if (ignitionMode == IM_WASTED_SPARK) {
		int secondIndex = index + engineConfiguration->cylindersCount / 2;
		int secondCoilIndex = ID2INDEX(getCylinderId(secondIndex));
		secondOutput = &enginePins.coils[secondCoilIndex];
	} else {
		secondOutput = nullptr;
	}

	event->outputs[0] = output;
	event->outputs[1] = secondOutput;

	wrapAngle2(sparkAngle, "findAngle#2", ObdCode::CUSTOM_ERR_6550, getEngineCycle(getEngineRotationState()->getOperationMode()));
	event->sparkAngle = sparkAngle;
	// Stash which cylinder we're scheduling so that knock sensing knows which
	// cylinder just fired
	event->cylinderNumber = coilIndex;

	angle_t dwellStartAngle = sparkAngle - dwellAngleDuration;
	efiAssertVoid(ObdCode::CUSTOM_ERR_6590, !cisnan(dwellStartAngle), "findAngle#5");

	assertAngleRange(dwellStartAngle, "findAngle dwellStartAngle", ObdCode::CUSTOM_ERR_6550);
	wrapAngle2(dwellStartAngle, "findAngle#7", ObdCode::CUSTOM_ERR_6550, getEngineCycle(getEngineRotationState()->getOperationMode()));
	event->dwellAngle = dwellStartAngle;
}

static void chargeTrailingSpark(IgnitionOutputPin* pin) {
#if SPARK_EXTREME_LOGGING
	efiPrintf("chargeTrailingSpark %s", pin->name);
#endif /* SPARK_EXTREME_LOGGING */
	pin->setHigh();
}

static void fireTrailingSpark(IgnitionOutputPin* pin) {
#if SPARK_EXTREME_LOGGING
	efiPrintf("fireTrailingSpark %s", pin->name);
#endif /* SPARK_EXTREME_LOGGING */
	pin->setLow();
}

static void overFireSparkAndPrepareNextSchedule(IgnitionEvent *event) {
#if SPARK_EXTREME_LOGGING
	efiPrintf("overFireSparkAndPrepareNextSchedule %s", event->outputs[0]->name);
#endif /* SPARK_EXTREME_LOGGING */
	fireSparkAndPrepareNextSchedule(event);
}

void fireSparkAndPrepareNextSchedule(IgnitionEvent *event) {
	for (int i = 0; i< MAX_OUTPUTS_FOR_IGNITION;i++) {
		IgnitionOutputPin *output = event->outputs[i];

		if (output) {
			fireSparkBySettingPinLow(event, output);
		}
	}

	efitick_t nowNt = getTimeNowNt();

#if EFI_TOOTH_LOGGER
	LogTriggerCoilState(nowNt, false);
#endif // EFI_TOOTH_LOGGER

#if !EFI_UNIT_TEST
if (engineConfiguration->debugMode == DBG_DWELL_METRIC) {
#if EFI_TUNER_STUDIO
	uint32_t actualDwellDurationNt = getTimeNowLowerNt() - event->actualStartOfDwellNt;
	/**
	 * ratio of desired dwell duration to actual dwell duration gives us some idea of how good is input trigger jitter
	 */
	float ratio = NT2US(actualDwellDurationNt) / 1000.0 / event->sparkDwell;

	// todo: smarted solution for index to field mapping
	switch (event->cylinderIndex) {
	case 0:
		engine->outputChannels.debugFloatField1 = ratio;
		break;
	case 1:
		engine->outputChannels.debugFloatField2 = ratio;
		break;
	case 2:
		engine->outputChannels.debugFloatField3 = ratio;
		break;
	case 3:
		engine->outputChannels.debugFloatField4 = ratio;
		break;
	}
#endif

	}
#endif /* EFI_UNIT_TEST */
	// now that we've just fired a coil let's prepare the new schedule for the next engine revolution

	angle_t dwellAngleDuration = engine->ignitionState.dwellAngle;
	floatms_t sparkDwell = engine->ignitionState.sparkDwell;
	if (cisnan(dwellAngleDuration) || cisnan(sparkDwell)) {
		// we are here if engine has just stopped
		return;
	}

	// If there are more sparks to fire, schedule them
	if (event->sparksRemaining > 0) {
		event->sparksRemaining--;

		efitick_t nextDwellStart = nowNt + engine->engineState.multispark.delay;
		efitick_t nextFiring = nextDwellStart + engine->engineState.multispark.dwell;
#if SPARK_EXTREME_LOGGING
	efiPrintf("schedule multispark");
#endif /* SPARK_EXTREME_LOGGING */

		// We can schedule both of these right away, since we're going for "asap" not "particular angle"
		engine->executor.scheduleByTimestampNt("dwell", &event->dwellStartTimer, nextDwellStart, { &turnSparkPinHigh, event });
		engine->executor.scheduleByTimestampNt("firing", &event->sparkEvent.scheduling, nextFiring, { fireSparkAndPrepareNextSchedule, event });
	} else {
		if (engineConfiguration->enableTrailingSparks) {
#if SPARK_EXTREME_LOGGING
	efiPrintf("scheduleByAngle TrailingSparks");
#endif /* SPARK_EXTREME_LOGGING */

			// Trailing sparks are enabled - schedule an event for the corresponding trailing coil
			scheduleByAngle(
				&event->trailingSparkFire, nowNt, engine->engineState.trailingSparkAngle,
				{ &fireTrailingSpark, &enginePins.trailingCoils[event->cylinderNumber] }
			);
		}

		// If all events have been scheduled, prepare for next time.
		prepareCylinderIgnitionSchedule(dwellAngleDuration, sparkDwell, event);
	}

	engine->onSparkFireKnockSense(event->cylinderNumber, nowNt);
}

static void startDwellByTurningSparkPinHigh(IgnitionEvent *event, IgnitionOutputPin *output) {
	// todo: no reason for this to be disabled in unit_test mode?!
#if ! EFI_UNIT_TEST

	if (Sensor::getOrZero(SensorType::Rpm) > 2 * engineConfiguration->cranking.rpm) {
		const char *outputName = output->getName();
		if (prevSparkName == outputName && getCurrentIgnitionMode() != IM_ONE_COIL) {
			warning(ObdCode::CUSTOM_OBD_SKIPPED_SPARK, "looks like skipped spark event %d %s", getRevolutionCounter(), outputName);
		}
		prevSparkName = outputName;
	}
#endif /* EFI_UNIT_TEST */


#if SPARK_EXTREME_LOGGING
	efiPrintf("spark goes high %d %s %d current=%d cnt=%d id=%d", getRevolutionCounter(), output->name, (int)getTimeNowUs(),
			output->currentLogicValue, output->outOfOrder, event->sparkId);
#endif /* SPARK_EXTREME_LOGGING */

	if (output->outOfOrder) {
		output->outOfOrder = false;
		if (output->signalFallSparkId == event->sparkId) {
			// let's save this coil if things do not look right
			return;
		}
	}

	output->setHigh();
}

void turnSparkPinHigh(IgnitionEvent *event) {
	event->actualStartOfDwellNt = getTimeNowLowerNt();

	efitick_t nowNt = getTimeNowNt();

#if EFI_TOOTH_LOGGER
	LogTriggerCoilState(nowNt, true);
#endif // EFI_TOOTH_LOGGER

	for (int i = 0; i< MAX_OUTPUTS_FOR_IGNITION;i++) {
		IgnitionOutputPin *output = event->outputs[i];
		if (output != NULL) {
			startDwellByTurningSparkPinHigh(event, output);
		}
	}

	if (engineConfiguration->enableTrailingSparks) {
		IgnitionOutputPin *output = &enginePins.trailingCoils[event->cylinderNumber];
		// Trailing sparks are enabled - schedule an event for the corresponding trailing coil
		scheduleByAngle(
			&event->trailingSparkCharge, nowNt, engine->engineState.trailingSparkAngle,
			{ &chargeTrailingSpark, output }
		);
	}
}

static void scheduleSparkEvent(bool limitedSpark, IgnitionEvent *event,
		int rpm, efitick_t edgeTimestamp, float currentPhase, float nextPhase) {

	angle_t sparkAngle = event->sparkAngle;
	const floatms_t dwellMs = engine->ignitionState.sparkDwell;
	if (cisnan(dwellMs) || dwellMs <= 0) {
		warning(ObdCode::CUSTOM_DWELL, "invalid dwell to handle: %.2f at %d", dwellMs, rpm);
		return;
	}
	if (cisnan(sparkAngle)) {
		warning(ObdCode::CUSTOM_ADVANCE_SPARK, "NaN advance");
		return;
	}

	float angleOffset = event->dwellAngle - currentPhase;
	if (angleOffset < 0) {
		angleOffset += engine->engineState.engineCycle;
	}

	/**
	 * By the way 32-bit value should hold at least 400 hours of events at 6K RPM x 12 events per revolution
	 */
	event->sparkId = engine->engineState.sparkCounter++;
	event->wasSparkLimited = limitedSpark;

	efitick_t chargeTime = 0;

	/**
	 * The start of charge is always within the current trigger event range, so just plain time-based scheduling
	 */
	if (!limitedSpark) {
#if SPARK_EXTREME_LOGGING
		efiPrintf("scheduling sparkUp %d %s now=%d %d later id=%d", getRevolutionCounter(), event->getOutputForLoggins()->name, (int)getTimeNowUs(), (int)angleOffset,
				event->sparkId);
#endif /* SPARK_EXTREME_LOGGING */


	/**
		 * Note how we do not check if spark is limited or not while scheduling 'spark down'
		 * This way we make sure that coil dwell started while spark was enabled would fire and not burn
		 * the coil.
		 */
		chargeTime = scheduleByAngle(&event->dwellStartTimer, edgeTimestamp, angleOffset, { &turnSparkPinHigh, event });

		event->sparksRemaining = engine->engineState.multispark.count;
	} else {
		// don't fire multispark if spark is cut completely!
		event->sparksRemaining = 0;
	}

	/**
	 * Spark event is often happening during a later trigger event timeframe
	 */

	efiAssertVoid(ObdCode::CUSTOM_ERR_6591, !cisnan(sparkAngle), "findAngle#4");
	assertAngleRange(sparkAngle, "findAngle#a5", ObdCode::CUSTOM_ERR_6549);

	bool scheduled = engine->module<TriggerScheduler>()->scheduleOrQueue(
		&event->sparkEvent, edgeTimestamp, sparkAngle,
		{ fireSparkAndPrepareNextSchedule, event },
		currentPhase, nextPhase);

	if (scheduled) {
#if SPARK_EXTREME_LOGGING
		efiPrintf("scheduling sparkDown %d %s now=%d later id=%d", getRevolutionCounter(), event->getOutputForLoggins()->name, (int)getTimeNowUs(), event->sparkId);
#endif /* FUEL_MATH_EXTREME_LOGGING */
	} else {
#if SPARK_EXTREME_LOGGING
		efiPrintf("to queue sparkDown %d %s now=%d for id=%d angle=%.1f", getRevolutionCounter(), event->getOutputForLoggins()->name, (int)getTimeNowUs(), event->sparkId, sparkAngle);
#endif /* SPARK_EXTREME_LOGGING */

		if (!limitedSpark && engine->enableOverdwellProtection) {
			// auto fire spark at 1.5x nominal dwell
			efitick_t fireTime = chargeTime + MSF2NT(1.5f * dwellMs);
			engine->executor.scheduleByTimestampNt("overdwell", &event->sparkEvent.scheduling, fireTime, { overFireSparkAndPrepareNextSchedule, event });
		}
	}

#if EFI_UNIT_TEST
	if (verboseMode) {
		printf("spark dwell@ %.1f spark@ %.2f id=%d\r\n", event->dwellAngle,
			event->sparkEvent.enginePhase,
			event->sparkId);
	}
#endif
}

void initializeIgnitionActions() {
	IgnitionEventList *list = &engine->ignitionEvents;
	angle_t dwellAngle = engine->ignitionState.dwellAngle;
	floatms_t sparkDwell = engine->ignitionState.sparkDwell;
	if (cisnan(engine->engineState.timingAdvance[0]) || cisnan(dwellAngle)) {
		// error should already be reported
		// need to invalidate previous ignition schedule
		list->isReady = false;
		return;
	}
	efiAssertVoid(ObdCode::CUSTOM_ERR_6592, engineConfiguration->cylindersCount > 0, "cylindersCount");

	for (size_t cylinderIndex = 0; cylinderIndex < engineConfiguration->cylindersCount; cylinderIndex++) {
		list->elements[cylinderIndex].cylinderIndex = cylinderIndex;
		prepareCylinderIgnitionSchedule(dwellAngle, sparkDwell, &list->elements[cylinderIndex]);
	}
	list->isReady = true;
}

static void prepareIgnitionSchedule() {
	ScopePerf perf(PE::PrepareIgnitionSchedule);
	
	/**
	 * TODO: warning. there is a bit of a hack here, todo: improve.
	 * currently output signals/times dwellStartTimer from the previous revolutions could be
	 * still used because they have crossed the revolution boundary
	 * but we are already re-purposing the output signals, but everything works because we
	 * are not affecting that space in memory. todo: use two instances of 'ignitionSignals'
	 */
	operation_mode_e operationMode = getEngineRotationState()->getOperationMode();
	float maxAllowedDwellAngle = (int) (getEngineCycle(operationMode) / 2); // the cast is about making Coverity happy

	if (getCurrentIgnitionMode() == IM_ONE_COIL) {
		maxAllowedDwellAngle = getEngineCycle(operationMode) / engineConfiguration->cylindersCount / 1.1;
	}

	if (engine->ignitionState.dwellAngle == 0) {
		warning(ObdCode::CUSTOM_ZERO_DWELL, "dwell is zero?");
	}
	if (engine->ignitionState.dwellAngle > maxAllowedDwellAngle) {
		warning(ObdCode::CUSTOM_DWELL_TOO_LONG, "dwell angle too long: %.2f", engine->ignitionState.dwellAngle);
	}

	// todo: add some check for dwell overflow? like 4 times 6 ms while engine cycle is less then that

	initializeIgnitionActions();
}

void onTriggerEventSparkLogic(int rpm, efitick_t edgeTimestamp, float currentPhase, float nextPhase) {
	ScopePerf perf(PE::OnTriggerEventSparkLogic);

	if (!isValidRpm(rpm) || !engineConfiguration->isIgnitionEnabled) {
		 // this might happen for instance in case of a single trigger event after a pause
		return;
	}

	LimpState limitedSparkState = getLimpManager()->allowIgnition();

	// todo: eliminate state copy logic by giving limpManager it's owm limp_manager.txt and leveraging LiveData
	engine->outputChannels.sparkCutReason = (int8_t)limitedSparkState.reason;
	bool limitedSpark = !limitedSparkState.value;

	if (!engine->ignitionEvents.isReady) {
		prepareIgnitionSchedule();
	}


	/**
	 * Ignition schedule is defined once per revolution
	 * See initializeIgnitionActions()
	 */


//	scheduleSimpleMsg(&logger, "eventId spark ", eventIndex);
	if (engine->ignitionEvents.isReady) {
		for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
			IgnitionEvent *event = &engine->ignitionEvents.elements[i];

			if (!isPhaseInRange(event->dwellAngle, currentPhase, nextPhase)) {
				continue;
			}

			if (i == 0 && engineConfiguration->artificialTestMisfire && (getRevolutionCounter() % ((int)engineConfiguration->scriptSetting[5]) == 0)) {
				// artificial misfire on cylinder #1 for testing purposes
				// enable artificialMisfire
				// set_fsio_setting 6 20
				warning(ObdCode::CUSTOM_ARTIFICIAL_MISFIRE, "artificial misfire on cylinder #1 for testing purposes %d", engine->engineState.sparkCounter);
				continue;
			}
#if EFI_LAUNCH_CONTROL
			if (engine->softSparkLimiter.shouldSkip()) {
				continue;
			}
#endif // EFI_LAUNCH_CONTROL

#if EFI_ANTILAG_SYSTEM && EFI_LAUNCH_CONTROL
			if (engine->ALSsoftSparkLimiter.shouldSkip()) {
				continue;
			}
			auto ALSSkipRatio = engineConfiguration->ALSSkipRatio;
            engine->ALSsoftSparkLimiter.setTargetSkipRatio(ALSSkipRatio);
#endif // EFI_ANTILAG_SYSTEM

			scheduleSparkEvent(limitedSpark, event, rpm, edgeTimestamp, currentPhase, nextPhase);
		}
	}
}

/**
 * Number of sparks per physical coil
 * @see getNumberOfInjections
 */
int getNumberOfSparks(ignition_mode_e mode) {
	switch (mode) {
	case IM_ONE_COIL:
		return engineConfiguration->cylindersCount;
	case IM_TWO_COILS:
		return engineConfiguration->cylindersCount / 2;
	case IM_INDIVIDUAL_COILS:
		return 1;
	case IM_WASTED_SPARK:
		return 2;
	default:
		firmwareError(ObdCode::CUSTOM_ERR_IGNITION_MODE, "Unexpected ignition_mode_e %d", mode);
		return 1;
	}
}

/**
 * @see getInjectorDutyCycle
 */
percent_t getCoilDutyCycle(int rpm) {
	floatms_t totalPerCycle = engine->ignitionState.sparkDwell * getNumberOfSparks(getCurrentIgnitionMode());
	floatms_t engineCycleDuration = getCrankshaftRevolutionTimeMs(rpm) * (getEngineRotationState()->getOperationMode() == TWO_STROKE ? 1 : 2);
	return 100 * totalPerCycle / engineCycleDuration;
}

#endif // EFI_ENGINE_CONTROL
