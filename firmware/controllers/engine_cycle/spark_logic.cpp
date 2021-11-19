/*
 * @file spark_logic.cpp
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "spark_logic.h"
#include "os_access.h"

#include "utlist.h"
#include "event_queue.h"
#include "tooth_logger.h"

#include "knock_logic.h"

#if EFI_ENGINE_CONTROL

#if EFI_UNIT_TEST
extern bool verboseMode;
#endif /* EFI_UNIT_TEST */

#if EFI_PRINTF_FUEL_DETAILS || FUEL_MATH_EXTREME_LOGGING
	extern bool printFuelDebug;
#endif // EFI_PRINTF_FUEL_DETAILS

static cyclic_buffer<int> ignitionErrorDetection;

static const char *prevSparkName = nullptr;

int isIgnitionTimingError(void) {
	return ignitionErrorDetection.sum(6) > 4;
}

static void fireSparkBySettingPinLow(IgnitionEvent *event, IgnitionOutputPin *output) {
	efitick_t nowNt = getTimeNowNt();
	engine->mostRecentTimeBetweenSparkEvents = nowNt - engine->mostRecentSparkEvent;
	engine->mostRecentSparkEvent = nowNt;

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

	if (!output->currentLogicValue) {
		warning(CUSTOM_OUT_OF_ORDER_COIL, "out-of-order coil off %s", output->getName());
		output->outOfOrder = true;
	}
	output->setLow();
}

// todo: make this a class method?
#define assertPinAssigned(output) { \
		if (!output->isInitialized()) { \
			warning(CUSTOM_OBD_COIL_PIN_NOT_ASSIGNED, "Pin Not Assigned check configuration #%s", (output)->getName()); \
		} \
}

static void prepareCylinderIgnitionSchedule(angle_t dwellAngleDuration, floatms_t sparkDwell, IgnitionEvent *event) {
	// todo: clean up this implementation? does not look too nice as is.

	// let's save planned duration so that we can later compare it with reality
	event->sparkDwell = sparkDwell;

	// change of sign here from 'before TDC' to 'after TDC'
	angle_t ignitionPositionWithinEngineCycle = engine->ignitionPositionWithinEngineCycle[event->cylinderIndex];
	assertAngleRange(ignitionPositionWithinEngineCycle, "aPWEC", CUSTOM_ERR_6566);
	// this correction is usually zero (not used)
	float perCylinderCorrection = engineConfiguration->timing_offset_cylinder[event->cylinderIndex];
	const angle_t sparkAngle = -engine->engineState.timingAdvance + engine->knockController.getKnockRetard() + ignitionPositionWithinEngineCycle + perCylinderCorrection;
	efiAssertVoid(CUSTOM_SPARK_ANGLE_9, !cisnan(sparkAngle), "findAngle#9");

	efiAssertVoid(CUSTOM_SPARK_ANGLE_1, !cisnan(sparkAngle), "sparkAngle#1");
	const int index = engine->ignitionPin[event->cylinderIndex];
	const int coilIndex = ID2INDEX(getCylinderId(index));
	IgnitionOutputPin *output = &enginePins.coils[coilIndex];

	IgnitionOutputPin *secondOutput;
	if (getCurrentIgnitionMode() == IM_WASTED_SPARK && engineConfiguration->twoWireBatchIgnition) {
		int secondIndex = index + engineConfiguration->specs.cylindersCount / 2;
		int secondCoilIndex = ID2INDEX(getCylinderId(secondIndex));
		secondOutput = &enginePins.coils[secondCoilIndex];
		assertPinAssigned(secondOutput);
	} else {
		secondOutput = nullptr;
	}

	assertPinAssigned(output);

	event->outputs[0] = output;
	event->outputs[1] = secondOutput;
	event->sparkAngle = sparkAngle;
	// Stash which cylinder we're scheduling so that knock sensing knows which
	// cylinder just fired
	event->cylinderNumber = coilIndex;

	angle_t dwellStartAngle = sparkAngle - dwellAngleDuration;
	efiAssertVoid(CUSTOM_ERR_6590, !cisnan(dwellStartAngle), "findAngle#5");
	assertAngleRange(dwellStartAngle, "findAngle#a6", CUSTOM_ERR_6550);
	event->dwellPosition.setAngle(dwellStartAngle);

#if FUEL_MATH_EXTREME_LOGGING
	if (printFuelDebug) {
		printf("addIgnitionEvent %s ind=%d\n", output->name, event->dwellPosition.triggerEventIndex);
	}
	//	efiPrintf("addIgnitionEvent %s ind=%d", output->name, event->dwellPosition->eventIndex);
#endif /* FUEL_MATH_EXTREME_LOGGING */
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
		tsOutputChannels.debugFloatField1 = ratio;
		break;
	case 1:
		tsOutputChannels.debugFloatField2 = ratio;
		break;
	case 2:
		tsOutputChannels.debugFloatField3 = ratio;
		break;
	case 3:
		tsOutputChannels.debugFloatField4 = ratio;
		break;
	}
#endif

	}
#endif /* EFI_UNIT_TEST */
	// now that we've just fired a coil let's prepare the new schedule for the next engine revolution

	angle_t dwellAngleDuration = engine->engineState.dwellAngle;
	floatms_t sparkDwell = engine->engineState.sparkDwell;
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

	if (GET_RPM() > 2 * engineConfiguration->cranking.rpm) {
		const char *outputName = output->getName();
		if (prevSparkName == outputName && getCurrentIgnitionMode() != IM_ONE_COIL) {
			warning(CUSTOM_OBD_SKIPPED_SPARK, "looks like skipped spark event %d %s", getRevolutionCounter(), outputName);
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

static void scheduleSparkEvent(bool limitedSpark, uint32_t trgEventIndex, IgnitionEvent *event,
		int rpm, efitick_t edgeTimestamp) {

	angle_t sparkAngle = event->sparkAngle;
	const floatms_t dwellMs = engine->engineState.sparkDwell;
	if (cisnan(dwellMs) || dwellMs <= 0) {
		warning(CUSTOM_DWELL, "invalid dwell to handle: %.2f at %d", dwellMs, rpm);
		return;
	}
	if (cisnan(sparkAngle)) {
		warning(CUSTOM_ADVANCE_SPARK, "NaN advance");
		return;
	}

	float angleOffset = event->dwellPosition.angleOffsetFromTriggerEvent;
	int isIgnitionError = angleOffset < 0;
	ignitionErrorDetection.add(isIgnitionError);
	if (isIgnitionError) {
#if EFI_PROD_CODE
		efiPrintf("Negative spark delay=%.1f deg", angleOffset);
#endif /* EFI_PROD_CODE */
		return;
	}

	event->sparkId = engine->globalSparkIdCounter++;

	efitick_t chargeTime = 0;

	/**
	 * The start of charge is always within the current trigger event range, so just plain time-based scheduling
	 */
	if (!limitedSpark) {
#if SPARK_EXTREME_LOGGING
		efiPrintf("scheduling sparkUp ind=%d %d %s now=%d %d later id=%d", trgEventIndex, getRevolutionCounter(), event->getOutputForLoggins()->name, (int)getTimeNowUs(), (int)angleOffset,
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

	efiAssertVoid(CUSTOM_ERR_6591, !cisnan(sparkAngle), "findAngle#4");
	assertAngleRange(sparkAngle, "findAngle#a5", CUSTOM_ERR_6549);

	bool scheduled = engine->module<TriggerScheduler>()->scheduleOrQueue(
		&event->sparkEvent, trgEventIndex, edgeTimestamp, sparkAngle,
		{ fireSparkAndPrepareNextSchedule, event });

	if (scheduled) {
#if SPARK_EXTREME_LOGGING
		efiPrintf("scheduling sparkDown ind=%d %d %s now=%d later id=%d", trgEventIndex, getRevolutionCounter(), event->getOutputForLoggins()->name, (int)getTimeNowUs(), event->sparkId);
#endif /* FUEL_MATH_EXTREME_LOGGING */
	} else {
#if SPARK_EXTREME_LOGGING
		efiPrintf("to queue sparkDown ind=%d %d %s now=%d for id=%d", trgEventIndex, getRevolutionCounter(), event->getOutputForLoggins()->name, (int)getTimeNowUs(), event->sparkEvent.position.triggerEventIndex);
#endif /* SPARK_EXTREME_LOGGING */

		if (!limitedSpark && engine->enableOverdwellProtection) {
			// auto fire spark at 1.5x nominal dwell
			efitick_t fireTime = chargeTime + MSF2NT(1.5f * dwellMs);
			engine->executor.scheduleByTimestampNt("overdwell", &event->sparkEvent.scheduling, fireTime, { overFireSparkAndPrepareNextSchedule, event });
		}
	}

#if EFI_UNIT_TEST
	if (verboseMode) {
		printf("spark dwell@ %d/%d spark@ %d/%d id=%d\r\n", event->dwellPosition.triggerEventIndex, (int)event->dwellPosition.angleOffsetFromTriggerEvent,
			event->sparkEvent.position.triggerEventIndex, (int)event->sparkEvent.position.angleOffsetFromTriggerEvent,
			event->sparkId);
	}
#endif
}

void initializeIgnitionActions() {
	IgnitionEventList *list = &engine->ignitionEvents;
	angle_t dwellAngle = engine->engineState.dwellAngle;
	floatms_t sparkDwell = engine->engineState.sparkDwell;
	if (cisnan(engine->engineState.timingAdvance) || cisnan(dwellAngle)) {
		// error should already be reported
		// need to invalidate previous ignition schedule
		list->isReady = false;
		return;
	}
	efiAssertVoid(CUSTOM_ERR_6592, engineConfiguration->specs.cylindersCount > 0, "cylindersCount");

	for (size_t cylinderIndex = 0; cylinderIndex < engineConfiguration->specs.cylindersCount; cylinderIndex++) {
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
	operation_mode_e operationMode = engine->getOperationMode();
	float maxAllowedDwellAngle = (int) (getEngineCycle(operationMode) / 2); // the cast is about making Coverity happy

	if (getCurrentIgnitionMode() == IM_ONE_COIL) {
		maxAllowedDwellAngle = getEngineCycle(operationMode) / engineConfiguration->specs.cylindersCount / 1.1;
	}

	if (engine->engineState.dwellAngle == 0) {
		warning(CUSTOM_ZERO_DWELL, "dwell is zero?");
	}
	if (engine->engineState.dwellAngle > maxAllowedDwellAngle) {
		warning(CUSTOM_DWELL_TOO_LONG, "dwell angle too long: %.2f", engine->engineState.dwellAngle);
	}

	// todo: add some check for dwell overflow? like 4 times 6 ms while engine cycle is less then that

	initializeIgnitionActions();
}

void onTriggerEventSparkLogic(bool limitedSpark, uint32_t trgEventIndex, int rpm, efitick_t edgeTimestamp
		) {

	ScopePerf perf(PE::OnTriggerEventSparkLogic);

	if (!isValidRpm(rpm) || !engineConfiguration->isIgnitionEnabled) {
		 // this might happen for instance in case of a single trigger event after a pause
		return;
	}

	if (!engine->ignitionEvents.isReady) {
		prepareIgnitionSchedule();
	}


	/**
	 * Ignition schedule is defined once per revolution
	 * See initializeIgnitionActions()
	 */


//	scheduleSimpleMsg(&logger, "eventId spark ", eventIndex);
	if (engine->ignitionEvents.isReady) {
		for (size_t i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
			IgnitionEvent *event = &engine->ignitionEvents.elements[i];
			if (event->dwellPosition.triggerEventIndex != trgEventIndex)
				continue;

			if (i == 0 && engineConfiguration->artificialTestMisfire && (getRevolutionCounter() % ((int)engineConfiguration->scriptSetting[5]) == 0)) {
				// artificial misfire on cylinder #1 for testing purposes
				// enable artificialMisfire
				// set_fsio_setting 6 20
				warning(CUSTOM_ARTIFICIAL_MISFIRE, "artificial misfire on cylinder #1 for testing purposes %d", engine->globalSparkIdCounter);
				continue;
			}
#if EFI_LAUNCH_CONTROL
			if (engine->softSparkLimiter.shouldSkip()) {
				continue;
			}
#endif // EFI_LAUNCH_CONTROL

			scheduleSparkEvent(limitedSpark, trgEventIndex, event, rpm, edgeTimestamp);
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
		return engineConfiguration->specs.cylindersCount;
	case IM_TWO_COILS:
		return engineConfiguration->specs.cylindersCount / 2;
	case IM_INDIVIDUAL_COILS:
		return 1;
	case IM_WASTED_SPARK:
		return 2;
	default:
		firmwareError(CUSTOM_ERR_IGNITION_MODE, "Unexpected ignition_mode_e %d", mode);
		return 1;
	}
}

/**
 * @see getInjectorDutyCycle
 */
percent_t getCoilDutyCycle(int rpm) {
	floatms_t totalPerCycle = engine->engineState.sparkDwell * getNumberOfSparks(getCurrentIgnitionMode());
	floatms_t engineCycleDuration = getCrankshaftRevolutionTimeMs(rpm) * (engine->getOperationMode() == TWO_STROKE ? 1 : 2);
	return 100 * totalPerCycle / engineCycleDuration;
}

#endif // EFI_ENGINE_CONTROL
