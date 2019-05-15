/*
 * @file spark_logic.cpp
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "engine_math.h"
#include "utlist.h"
#include "event_queue.h"
#include "rfiutil.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio_configuration.h"
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

EXTERN_ENGINE;

static cyclic_buffer<int> ignitionErrorDetection;
static Logging *logger;

static const char *prevSparkName = NULL;

IgnitionEventList::IgnitionEventList() {
	isReady = false;
}

int isInjectionEnabled(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// todo: is this worth a method? should this be inlined?
	return CONFIG(isInjectionEnabled);
}

int isIgnitionTimingError(void) {
	return ignitionErrorDetection.sum(6) > 4;
}

static void fireSparkBySettingPinLow(IgnitionEvent *event, IgnitionOutputPin *output) {
#if SPARK_EXTREME_LOGGING
	scheduleMsg(logger, "spark goes low  %d %s %d current=%d cnt=%d id=%d", getRevolutionCounter(), output->name, (int)getTimeNowUs(),
			output->currentLogicValue, output->outOfOrder, event->sparkId);
#endif /* FUEL_MATH_EXTREME_LOGGING */

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
#if EFI_PROD_CODE
	if (CONFIG(dizzySparkOutputPin) != GPIO_UNASSIGNED) {
		enginePins.dizzyOutput.setLow();
	}
#endif /* EFI_PROD_CODE */
}

// todo: make this a class method?
#define assertPinAssigned(output) { \
		if (!output->isInitialized()) { \
			warning(CUSTOM_OBD_COIL_PIN_NOT_ASSIGNED, "no_pin_cl #%s", (output)->getName()); \
		} \
}

static void prepareCylinderIgnitionSchedule(angle_t dwellAngle, floatms_t sparkDwell, IgnitionEvent *event DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// todo: clean up this implementation? does not look too nice as is.

	// let's save planned duration so that we can later compare it with reality
	event->sparkDwell = sparkDwell;

	// change of sign here from 'before TDC' to 'after TDC'
	angle_t ignitionPositionWithinEngineCycle = ENGINE(ignitionPositionWithinEngineCycle[event->cylinderIndex]);
	assertAngleRange(ignitionPositionWithinEngineCycle, "aPWEC", CUSTOM_ERR_6566);
	cfg_float_t_1f timing_offset_cylinder = CONFIG(timing_offset_cylinder[event->cylinderIndex]);
	const angle_t localAdvance = -ENGINE(engineState.timingAdvance) + ignitionPositionWithinEngineCycle + timing_offset_cylinder;
	efiAssertVoid(CUSTOM_ERR_6689, !cisnan(localAdvance), "findAngle#9");

	efiAssertVoid(CUSTOM_ERR_6589, !cisnan(localAdvance), "localAdvance#1");
	const int index = ENGINE(ignitionPin[event->cylinderIndex]);
	const int coilIndex = ID2INDEX(getCylinderId(index PASS_ENGINE_PARAMETER_SUFFIX));
	IgnitionOutputPin *output = &enginePins.coils[coilIndex];

	IgnitionOutputPin *secondOutput;
	if (getCurrentIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE) == IM_WASTED_SPARK && CONFIG(twoWireBatchIgnition)) {
		int secondIndex = index + CONFIG(specs.cylindersCount) / 2;
		int secondCoilIndex = ID2INDEX(getCylinderId(secondIndex PASS_ENGINE_PARAMETER_SUFFIX));
		secondOutput = &enginePins.coils[secondCoilIndex];
		assertPinAssigned(secondOutput);
	} else {
		secondOutput = NULL;
	}

	assertPinAssigned(output);

	event->outputs[0] = output;
	event->outputs[1] = secondOutput;
	event->advance = localAdvance;

	angle_t a = localAdvance - dwellAngle;
	efiAssertVoid(CUSTOM_ERR_6590, !cisnan(a), "findAngle#5");
	assertAngleRange(a, "findAngle#a6", CUSTOM_ERR_6550);
	TRIGGER_SHAPE(findTriggerPosition(&event->dwellPosition, a PASS_CONFIG_PARAM(engineConfiguration->globalTriggerAngleOffset)));

#if FUEL_MATH_EXTREME_LOGGING
	printf("addIgnitionEvent %s ind=%d\n", output->name, event->dwellPosition.eventIndex);
	//	scheduleMsg(logger, "addIgnitionEvent %s ind=%d", output->name, event->dwellPosition->eventIndex);
#endif /* FUEL_MATH_EXTREME_LOGGING */
}

void fireSparkAndPrepareNextSchedule(IgnitionEvent *event) {
	for (int i = 0; i< MAX_OUTPUTS_FOR_IGNITION;i++) {
		IgnitionOutputPin *output = event->outputs[i];
		if (output != NULL) {
			fireSparkBySettingPinLow(event, output);
		}
	}
#if !EFI_UNIT_TEST
if (engineConfiguration->debugMode == DBG_DWELL_METRIC) {
	uint32_t actualDwellDurationNt = getTimeNowLowerNt() - event->startOfDwell;
	/**
	 * ratio of desired dwell duration to actual dwell duration gives us some idea of how good is input trigger jitter
	 */
	float ratio = NT2US(actualDwellDurationNt) / 1000.0 / event->sparkDwell;

#if EFI_TUNER_STUDIO
	// todo: smarted solution for index to field mapping
	if (event->cylinderIndex == 0) {
		tsOutputChannels.debugFloatField1 = ratio;
	} else if (event->cylinderIndex == 1) {
		tsOutputChannels.debugFloatField2 = ratio;
	} else if (event->cylinderIndex == 2) {
		tsOutputChannels.debugFloatField3 = ratio;
	} else if (event->cylinderIndex == 3) {
		tsOutputChannels.debugFloatField4 = ratio;
	}
#endif

}
#endif /* EFI_UNIT_TEST */
#if EFI_UNIT_TEST
	Engine *engine = event->engine;
	EXPAND_Engine;
#endif /* EFI_UNIT_TEST */
	// now that we've just fired a coil let's prepare the new schedule for the next engine revolution

	angle_t dwellAngle = ENGINE(engineState.dwellAngle);
	floatms_t sparkDwell = ENGINE(engineState.sparkDwell);
	if (cisnan(dwellAngle) || cisnan(sparkDwell)) {
		// we are here if engine has just stopped
		return;
	}
 	prepareCylinderIgnitionSchedule(dwellAngle, sparkDwell, event PASS_ENGINE_PARAMETER_SUFFIX);
}

static void startDwellByTurningSparkPinHigh(IgnitionEvent *event, IgnitionOutputPin *output) {

#if ! EFI_UNIT_TEST

	if (GET_RPM_VALUE > 2 * engineConfiguration->cranking.rpm) {
		const char *outputName = output->getName();
		if (prevSparkName == outputName && getCurrentIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE) != IM_ONE_COIL) {
			warning(CUSTOM_OBD_SKIPPED_SPARK, "looks like skipped spark event %d %s", getRevolutionCounter(), outputName);
		}
		prevSparkName = outputName;
	}
#endif /* EFI_UNIT_TEST */


#if SPARK_EXTREME_LOGGING
	scheduleMsg(logger, "spark goes high %d %s %d current=%d cnt=%d id=%d", getRevolutionCounter(), output->name, (int)getTimeNowUs(),
			output->currentLogicValue, output->outOfOrder, event->sparkId);
#endif /* FUEL_MATH_EXTREME_LOGGING */

	if (output->outOfOrder) {
		output->outOfOrder = false;
		if (output->signalFallSparkId == event->sparkId) {
			// let's save this coil if things do not look right
			return;
		}
	}

	output->setHigh();
#if EFI_PROD_CODE
	if (CONFIG(dizzySparkOutputPin) != GPIO_UNASSIGNED) {
		enginePins.dizzyOutput.setHigh();
	}
#endif /* EFI_PROD_CODE */
}

void turnSparkPinHigh(IgnitionEvent *event) {
	event->startOfDwell = getTimeNowLowerNt();
	for (int i = 0; i< MAX_OUTPUTS_FOR_IGNITION;i++) {
		IgnitionOutputPin *output = event->outputs[i];
		if (output != NULL) {
			startDwellByTurningSparkPinHigh(event, output);
		}
	}
}

static ALWAYS_INLINE void handleSparkEvent(bool limitedSpark, uint32_t trgEventIndex, IgnitionEvent *iEvent,
		int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {

	angle_t advance = iEvent->advance;
	const floatms_t dwellMs = ENGINE(engineState.sparkDwell);
	if (cisnan(dwellMs) || dwellMs <= 0) {
		warning(CUSTOM_DWELL, "invalid dwell to handle: %.2f at %d", dwellMs, rpm);
		return;
	}
	if (cisnan(advance)) {
		warning(CUSTOM_ERR_6688, "NaN advance");
		return;
	}

	floatus_t chargeDelayUs = ENGINE(rpmCalculator.oneDegreeUs) * iEvent->dwellPosition.angleOffset;
	int isIgnitionError = chargeDelayUs < 0;
	ignitionErrorDetection.add(isIgnitionError);
	if (isIgnitionError) {
#if EFI_PROD_CODE
		scheduleMsg(logger, "Negative spark delay=%.2f", chargeDelayUs);
#endif /* EFI_PROD_CODE */
		chargeDelayUs = 0;
		return;
	}

	iEvent->sparkId = engine->globalSparkIdCoutner++;

	/**
	 * We are alternating two event lists in order to avoid a potential issue around revolution boundary
	 * when an event is scheduled within the next revolution.
	 */
	scheduling_s * sUp = &iEvent->signalTimerUp;
	scheduling_s * sDown = &iEvent->signalTimerDown;


	/**
	 * The start of charge is always within the current trigger event range, so just plain time-based scheduling
	 */
	if (!limitedSpark) {
#if SPARK_EXTREME_LOGGING
		scheduleMsg(logger, "scheduling sparkUp ind=%d %d %s now=%d %d later id=%d", trgEventIndex, getRevolutionCounter(), iEvent->getOutputForLoggins()->name, (int)getTimeNowUs(), (int)chargeDelayUs,
				iEvent->sparkId);
#endif /* FUEL_MATH_EXTREME_LOGGING */


	/**
		 * Note how we do not check if spark is limited or not while scheduling 'spark down'
		 * This way we make sure that coil dwell started while spark was enabled would fire and not burn
		 * the coil.
		 */
		engine->executor.scheduleForLater(sUp, chargeDelayUs, (schfunc_t) &turnSparkPinHigh, iEvent);
	}
	/**
	 * Spark event is often happening during a later trigger event timeframe
	 * TODO: improve precision
	 */

	efiAssertVoid(CUSTOM_ERR_6591, !cisnan(advance), "findAngle#4");
	assertAngleRange(advance, "findAngle#a5", CUSTOM_ERR_6549);
	TRIGGER_SHAPE(findTriggerPosition(&iEvent->sparkPosition, advance PASS_CONFIG_PARAM(engineConfiguration->globalTriggerAngleOffset)));

#if EFI_UNIT_TEST
	printf("spark dwell@ %d/%d spark@ %d/%d id=%d\r\n", iEvent->dwellPosition.eventIndex, (int)iEvent->dwellPosition.angleOffset,
			iEvent->sparkPosition.eventIndex, (int)iEvent->sparkPosition.angleOffset,
			iEvent->sparkId);
#endif

	/**
	 * todo: extract a "scheduleForAngle" method with best implementation into a separate utility method
	 *
	 * Here's the status as of Nov 2018:
	 * "scheduleForLater" uses time only and for best precision it's best to use "scheduleForLater" only
	 * once we hit the last trigger tooth prior to needed event. This case we use as much trigger position angle as possible
	 * and only use less precise RPM-based time calculation for the last portion of the angle, the one between two teeth closest to the
	 * desired angle moment.
	 *
	 * At the moment we only have time-based scheduler. I believe what needs to be added is a trigger-event based scheduler on top of the
	 * time-based schedule. This case we would be firing events with best possible angle precision.
	 *
	 */
	if (iEvent->sparkPosition.eventIndex == trgEventIndex) {
		/**
		 * Spark should be fired before the next trigger event - time-based delay is best precision possible
		 */
		float timeTillIgnitionUs = ENGINE(rpmCalculator.oneDegreeUs) * iEvent->sparkPosition.angleOffset;

#if SPARK_EXTREME_LOGGING
		scheduleMsg(logger, "scheduling sparkDown ind=%d %d %s now=%d %d later id=%d", trgEventIndex, getRevolutionCounter(), iEvent->getOutputForLoggins()->name, (int)getTimeNowUs(), (int)timeTillIgnitionUs, iEvent->sparkId);
#endif /* FUEL_MATH_EXTREME_LOGGING */

		engine->executor.scheduleForLater(sDown, (int) timeTillIgnitionUs, (schfunc_t) &fireSparkAndPrepareNextSchedule, iEvent);
	} else {
#if SPARK_EXTREME_LOGGING
		scheduleMsg(logger, "to queue sparkDown ind=%d %d %s %d for %d", trgEventIndex, getRevolutionCounter(), iEvent->getOutputForLoggins()->name, (int)getTimeNowUs(), iEvent->sparkPosition.eventIndex);
#endif /* FUEL_MATH_EXTREME_LOGGING */
		/**
		 * Spark should be scheduled in relation to some future trigger event, this way we get better firing precision
		 */
		bool isPending = assertNotInList<IgnitionEvent>(ENGINE(iHead), iEvent);
		if (isPending) {
#if SPARK_EXTREME_LOGGING
			scheduleMsg(logger, "not adding to queue sparkDown ind=%d %d %s %d", trgEventIndex, getRevolutionCounter(), iEvent->getOutputForLoggins()->name, (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */
			return;
		}

		LL_APPEND(ENGINE(iHead), iEvent);
	}
}

static void initializeIgnitionActions(IgnitionEventList *list DECLARE_ENGINE_PARAMETER_SUFFIX) {
	angle_t dwellAngle = ENGINE(engineState.dwellAngle);
	floatms_t sparkDwell = ENGINE(engineState.sparkDwell);
	if (cisnan(ENGINE(engineState.timingAdvance)) || cisnan(dwellAngle)) {
		// error should already be reported
		// need to invalidate previous ignition schedule
		list->isReady = false;
		return;
	}
	efiAssertVoid(CUSTOM_ERR_6592, engineConfiguration->specs.cylindersCount > 0, "cylindersCount");

	for (int cylinderIndex = 0; cylinderIndex < CONFIG(specs.cylindersCount); cylinderIndex++) {
		list->elements[cylinderIndex].cylinderIndex = cylinderIndex;
#if EFI_UNIT_TEST
		list->elements[cylinderIndex].engine = engine;
#endif /* EFI_UNIT_TEST */
		prepareCylinderIgnitionSchedule(dwellAngle, sparkDwell, &list->elements[cylinderIndex] PASS_ENGINE_PARAMETER_SUFFIX);
	}
	list->isReady = true;
}

static ALWAYS_INLINE void prepareIgnitionSchedule(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

	engine->m.beforeIgnitionSch = getTimeNowLowerNt();
	/**
	 * TODO: warning. there is a bit of a hack here, todo: improve.
	 * currently output signals/times signalTimerUp from the previous revolutions could be
	 * still used because they have crossed the revolution boundary
	 * but we are already re-purposing the output signals, but everything works because we
	 * are not affecting that space in memory. todo: use two instances of 'ignitionSignals'
	 */
	float maxAllowedDwellAngle = (int) (getEngineCycle(engineConfiguration->operationMode) / 2); // the cast is about making Coverity happy

	if (getCurrentIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE) == IM_ONE_COIL) {
		maxAllowedDwellAngle = getEngineCycle(engineConfiguration->operationMode) / engineConfiguration->specs.cylindersCount / 1.1;
	}

	if (engine->engineState.dwellAngle == 0) {
		warning(CUSTOM_ZERO_DWELL, "dwell is zero?");
	}
	if (engine->engineState.dwellAngle > maxAllowedDwellAngle) {
		warning(CUSTOM_DWELL_TOO_LONG, "dwell angle too long: %.2f", engine->engineState.dwellAngle);
	}

	// todo: add some check for dwell overflow? like 4 times 6 ms while engine cycle is less then that

	IgnitionEventList *list = &engine->ignitionEvents;

	initializeIgnitionActions(list PASS_ENGINE_PARAMETER_SUFFIX);
	engine->m.ignitionSchTime = getTimeNowLowerNt() - engine->m.beforeIgnitionSch;
}


static void scheduleAllSparkEventsUntilNextTriggerTooth(uint32_t trgEventIndex DECLARE_ENGINE_PARAMETER_SUFFIX) {
	IgnitionEvent *current, *tmp;

	LL_FOREACH_SAFE(ENGINE(iHead), current, tmp)
	{
		if (current->sparkPosition.eventIndex == trgEventIndex) {
			// time to fire a spark which was scheduled previously
			LL_DELETE(ENGINE(iHead), current);

			scheduling_s * sDown = &current->signalTimerDown;

#if SPARK_EXTREME_LOGGING
	scheduleMsg(logger, "time to sparkDown ind=%d %d %s %d", trgEventIndex, getRevolutionCounter(), current->getOutputForLoggins()->name, (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */


			float timeTillIgnitionUs = ENGINE(rpmCalculator.oneDegreeUs) * current->sparkPosition.angleOffset;
			engine->executor.scheduleForLater(sDown, (int) timeTillIgnitionUs, (schfunc_t) &fireSparkAndPrepareNextSchedule, current);
		}
	}
}

void onTriggerEventSparkLogic(bool limitedSpark, uint32_t trgEventIndex, int rpm
		 DECLARE_ENGINE_PARAMETER_SUFFIX) {

	if (!isValidRpm(rpm) || !CONFIG(isIgnitionEnabled)) {
		 // this might happen for instance in case of a single trigger event after a pause
		return;
	}

	if (!ENGINE(ignitionEvents.isReady)) {
		prepareIgnitionSchedule(PASS_ENGINE_PARAMETER_SIGNATURE);
	}


	/**
	 * Ignition schedule is defined once per revolution
	 * See initializeIgnitionActions()
	 */
	scheduleAllSparkEventsUntilNextTriggerTooth(trgEventIndex PASS_ENGINE_PARAMETER_SUFFIX);


//	scheduleSimpleMsg(&logger, "eventId spark ", eventIndex);
	if (ENGINE(ignitionEvents.isReady)) {
		for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
			IgnitionEvent *event = &ENGINE(ignitionEvents.elements[i]);
			if (event->dwellPosition.eventIndex != trgEventIndex)
				continue;
			handleSparkEvent(limitedSpark, trgEventIndex, event, rpm PASS_ENGINE_PARAMETER_SUFFIX);
		}
	}
}

void initSparkLogic(Logging *sharedLogger) {
	logger = sharedLogger;
}

/**
 * Number of sparks per physical coil
 * @see getNumberOfInjections
 */
int getNumberOfSparks(ignition_mode_e mode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch (mode) {
	case IM_ONE_COIL:
		return engineConfiguration->specs.cylindersCount;
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
percent_t getCoilDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	floatms_t totalPerCycle = 1/**getInjectionDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX)*/ * getNumberOfSparks(getCurrentIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX);
	floatms_t engineCycleDuration = getCrankshaftRevolutionTimeMs(rpm) * (engineConfiguration->operationMode == TWO_STROKE ? 1 : 2);
	return 100 * totalPerCycle / engineCycleDuration;
}
