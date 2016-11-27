/*
 * @file spark_logic.cpp
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "engine_math.h"
#include "utlist.h"
#include "event_queue.h"
#include "efilib2.h"

EXTERN_ENGINE;

static cyclic_buffer<int> ignitionErrorDetection;
static Logging *logger;

static const char *prevSparkName = NULL;

int isInjectionEnabled(engine_configuration_s *engineConfiguration) {
	// todo: is this worth a method? should this be inlined?
	return engineConfiguration->isInjectionEnabled;
}

int isIgnitionTimingError(void) {
	return ignitionErrorDetection.sum(6) > 4;
}

void turnSparkPinLow2(IgnitionEvent *event, IgnitionOutputPin *output) {
#if SPARK_EXTREME_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "spark goes low  %d %s %d current=%d cnt=%d id=%d", getRevolutionCounter(), output->name, (int)getTimeNowUs(),
			output->currentLogicValue, output->outOfOrder, event->sparkId);
#endif /* FUEL_MATH_EXTREME_LOGGING */

	/**
	 * there are two kinds of 'out-of-order'
	 * 1) low goes before high, everything is fine after words
	 *
	 * 2) we have an un-matched low followed by legit pairs
	 *
	 */

	output->signalFallSparkId = event->sparkId;

	if (!output->currentLogicValue) {
		warning(CUSTOM_OUT_OF_ORDER_COIL, "out-of-order coil off %s", output->name);
		output->outOfOrder = true;
	}

	turnPinLow(output);
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (CONFIG(dizzySparkOutputPin) != GPIO_UNASSIGNED) {
		turnPinLow(&enginePins.dizzyOutput);
	}
#endif /* EFI_PROD_CODE */
}

void turnSparkPinLow(IgnitionEvent *event) {
	IgnitionOutputPin *output = event->outputs[0];
	turnSparkPinLow2(event, output);
}

void turnSparkPinHigh2(IgnitionEvent *event, IgnitionOutputPin *output) {

#if ! EFI_UNIT_TEST
	if (engine->rpmCalculator.rpmValue > 2 * engineConfiguration->cranking.rpm) {
		const char *outputName = output->name;
		if (prevSparkName == outputName) {
			warning(CUSTOM_OBD_SKIPPED_SPARK, "looks like skipped spark event %d %s", getRevolutionCounter(), outputName);
		}
		prevSparkName = outputName;
	}
#endif /* EFI_UNIT_TEST */


#if SPARK_EXTREME_LOGGING || defined(__DOXYGEN__)
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

	turnPinHigh(output);
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (CONFIG(dizzySparkOutputPin) != GPIO_UNASSIGNED) {
		turnPinHigh(&enginePins.dizzyOutput);
	}
#endif /* EFI_PROD_CODE */
}

void turnSparkPinHigh(IgnitionEvent *event) {
	IgnitionOutputPin *output = event->outputs[0];
	turnSparkPinHigh2(event, output);
}

static int globalSparkIdCoutner = 0;

static ALWAYS_INLINE void handleSparkEvent(bool limitedSpark, uint32_t trgEventIndex, IgnitionEvent *iEvent,
		int rpm DECLARE_ENGINE_PARAMETER_S) {

	const floatms_t dwellMs = ENGINE(engineState.sparkDwell);
	if (cisnan(dwellMs) || dwellMs <= 0) {
		warning(CUSTOM_DWELL, "invalid dwell: %f at %d", dwellMs, rpm);
		return;
	}

	floatus_t chargeDelayUs = ENGINE(rpmCalculator.oneDegreeUs) * iEvent->dwellPosition.angleOffset;
	int isIgnitionError = chargeDelayUs < 0;
	ignitionErrorDetection.add(isIgnitionError);
	if (isIgnitionError) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
		scheduleMsg(logger, "Negative spark delay=%f", chargeDelayUs);
#endif
		chargeDelayUs = 0;
		return;
	}

	iEvent->sparkId = globalSparkIdCoutner++;

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
#if SPARK_EXTREME_LOGGING || defined(__DOXYGEN__)
		scheduleMsg(logger, "scheduling sparkUp ind=%d %d %s now=%d %d later id=%d", trgEventIndex, getRevolutionCounter(), iEvent->outputs[0]->name, (int)getTimeNowUs(), (int)chargeDelayUs,
				iEvent->sparkId);
#endif /* FUEL_MATH_EXTREME_LOGGING */


	/**
		 * Note how we do not check if spark is limited or not while scheduling 'spark down'
		 * This way we make sure that coil dwell started while spark was enabled would fire and not burn
		 * the coil.
		 */
		scheduleTask(true, "spark up", sUp, chargeDelayUs, (schfunc_t) &turnSparkPinHigh, iEvent);
	}
	/**
	 * Spark event is often happening during a later trigger event timeframe
	 * TODO: improve precision
	 */
	findTriggerPosition(&iEvent->sparkPosition, iEvent->advance PASS_ENGINE_PARAMETER);

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	printf("spark dwell@ %d/%d spark@ %d/%d id=%d\r\n", iEvent->dwellPosition.eventIndex, (int)iEvent->dwellPosition.angleOffset,
			iEvent->sparkPosition.eventIndex, (int)iEvent->sparkPosition.angleOffset,
			iEvent->sparkId);
#endif

	if (iEvent->sparkPosition.eventIndex == trgEventIndex) {
		/**
		 * Spark should be fired before the next trigger event - time-based delay is best precision possible
		 */
		float timeTillIgnitionUs = ENGINE(rpmCalculator.oneDegreeUs) * iEvent->sparkPosition.angleOffset;

#if SPARK_EXTREME_LOGGING || defined(__DOXYGEN__)
		scheduleMsg(logger, "scheduling sparkDown ind=%d %d %s now=%d %d later id=%d", trgEventIndex, getRevolutionCounter(), iEvent->outputs[0]->name, (int)getTimeNowUs(), (int)timeTillIgnitionUs, iEvent->sparkId);
#endif /* FUEL_MATH_EXTREME_LOGGING */

		scheduleTask(true, "spark1 down", sDown, (int) timeTillIgnitionUs, (schfunc_t) &turnSparkPinLow, iEvent);
	} else {
#if SPARK_EXTREME_LOGGING || defined(__DOXYGEN__)
		scheduleMsg(logger, "to queue sparkDown ind=%d %d %s %d for %d", trgEventIndex, getRevolutionCounter(), iEvent->outputs[0]->name, (int)getTimeNowUs(), iEvent->sparkPosition.eventIndex);
#endif /* FUEL_MATH_EXTREME_LOGGING */
		/**
		 * Spark should be scheduled in relation to some future trigger event, this way we get better firing precision
		 */
		bool isPending = assertNotInList<IgnitionEvent>(ENGINE(iHead), iEvent);
		if (isPending) {
#if SPARK_EXTREME_LOGGING || defined(__DOXYGEN__)
			scheduleMsg(logger, "not adding to queue sparkDown ind=%d %d %s %d", trgEventIndex, getRevolutionCounter(), iEvent->outputs[0]->name, (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */
			return;
		}

		LL_APPEND(ENGINE(iHead), iEvent);
	}
}

static void addIgnitionEvent(angle_t localAdvance, angle_t dwellAngle, IgnitionEventList *list, IgnitionOutputPin *output DECLARE_ENGINE_PARAMETER_S) {
	IgnitionEvent *event = list->add();

	if (!isPinAssigned(output)) {
		// todo: extract method for this index math
		warning(CUSTOM_OBD_PIN_NOT_ASSIGNED, "no_pin_cl #%s", output->name);
	}
	event->outputs[0] = output;
	event->advance = localAdvance;

	findTriggerPosition(&event->dwellPosition, localAdvance - dwellAngle PASS_ENGINE_PARAMETER);

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	printf("addIgnitionEvent %s ind=%d\n", output->name, event->dwellPosition.eventIndex);
	//	scheduleMsg(logger, "addIgnitionEvent %s ind=%d", output->name, event->dwellPosition->eventIndex);
#endif /* FUEL_MATH_EXTREME_LOGGING */

}

static void initializeIgnitionActions(angle_t advance, angle_t dwellAngle,
		IgnitionEventList *list DECLARE_ENGINE_PARAMETER_S) {
	efiAssertVoid(engineConfiguration->specs.cylindersCount > 0, "cylindersCount");

	list->reset();

	for (int i = 0; i < CONFIG(specs.cylindersCount); i++) {
		// todo: clean up this implementation? does not look too nice as is.

		// change of sign here from 'before TDC' to 'after TDC'
		angle_t localAdvance = -advance + ENGINE(angleExtra[i]);
		int index = ENGINE(ignitionPin[i]);
		int cylinderIndex = ID2INDEX(getCylinderId(CONFIG(specs.firingOrder), index));
		IgnitionOutputPin *output = &enginePins.coils[cylinderIndex];

		addIgnitionEvent(localAdvance, dwellAngle, list, output PASS_ENGINE_PARAMETER);

		if (CONFIG(ignitionMode) == IM_WASTED_SPARK && CONFIG(twoWireBatchIgnition)) {
			index += CONFIG(specs.cylindersCount) / 2;
			cylinderIndex = ID2INDEX(getCylinderId(CONFIG(specs.firingOrder), index));
			output = &enginePins.coils[cylinderIndex];

			addIgnitionEvent(localAdvance, dwellAngle, list, output PASS_ENGINE_PARAMETER);
		}

	}
}

static ALWAYS_INLINE void prepareIgnitionSchedule(int rpm, int revolutionIndex DECLARE_ENGINE_PARAMETER_S) {

	engine->m.beforeIgnitionSch = GET_TIMESTAMP();
	/**
	 * TODO: warning. there is a bit of a hack here, todo: improve.
	 * currently output signals/times signalTimerUp from the previous revolutions could be
	 * still used because they have crossed the revolution boundary
	 * but we are already re-purposing the output signals, but everything works because we
	 * are not affecting that space in memory. todo: use two instances of 'ignitionSignals'
	 */
	float maxAllowedDwellAngle = (int) (getEngineCycle(engineConfiguration->operationMode) / 2); // the cast is about making Coverity happy

	if (engineConfiguration->ignitionMode == IM_ONE_COIL) {
		maxAllowedDwellAngle = getEngineCycle(engineConfiguration->operationMode) / engineConfiguration->specs.cylindersCount / 1.1;
	}

	if (engine->engineState.dwellAngle == 0) {
		warning(CUSTOM_OBD_32, "dwell is zero?");
	}
	if (engine->engineState.dwellAngle > maxAllowedDwellAngle) {
		warning(CUSTOM_OBD_33, "dwell angle too long: %f", engine->engineState.dwellAngle);
	}

	// todo: add some check for dwell overflow? like 4 times 6 ms while engine cycle is less then that

	IgnitionEventList *list = &engine->engineConfiguration2->ignitionEvents[revolutionIndex];

	if (cisnan(ENGINE(engineState.timingAdvance))) {
		// error should already be reported
		list->reset(); // reset is needed to clear previous ignition schedule
		return;
	}
	initializeIgnitionActions(ENGINE(engineState.timingAdvance), ENGINE(engineState.dwellAngle), list PASS_ENGINE_PARAMETER);
	engine->m.ignitionSchTime = GET_TIMESTAMP() - engine->m.beforeIgnitionSch;
}

void handleSpark(int revolutionIndex, bool limitedSpark, uint32_t trgEventIndex, int rpm,
		IgnitionEventList *list DECLARE_ENGINE_PARAMETER_S) {
	if (trgEventIndex == 0) {
		prepareIgnitionSchedule(rpm, revolutionIndex PASS_ENGINE_PARAMETER);
	}

	if (!isValidRpm(rpm) || !CONFIG(isIgnitionEnabled)) {
		 // this might happen for instance in case of a single trigger event after a pause
		return;
	}
	/**
	 * Ignition schedule is defined once per revolution
	 * See initializeIgnitionActions()
	 */

	IgnitionEvent *current, *tmp;

	LL_FOREACH_SAFE(ENGINE(iHead), current, tmp)
	{
		if (current->sparkPosition.eventIndex == trgEventIndex) {
			// time to fire a spark which was scheduled previously
			LL_DELETE(ENGINE(iHead), current);

			scheduling_s * sDown = &current->signalTimerDown;

#if SPARK_EXTREME_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "time to sparkDown ind=%d %d %s %d", trgEventIndex, getRevolutionCounter(), current->outputs[0]->name, (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */


			float timeTillIgnitionUs = ENGINE(rpmCalculator.oneDegreeUs) * current->sparkPosition.angleOffset;
			scheduleTask(true, "spark 2down", sDown, (int) timeTillIgnitionUs, (schfunc_t) &turnSparkPinLow, current);
		}
	}

//	scheduleSimpleMsg(&logger, "eventId spark ", eventIndex);
	for (int i = 0; i < list->size; i++) {
		IgnitionEvent *event = &list->elements[i];
		if (event->dwellPosition.eventIndex != trgEventIndex)
			continue;
		handleSparkEvent(limitedSpark, trgEventIndex, event, rpm PASS_ENGINE_PARAMETER);
	}
}

void initSparkLogic(Logging *sharedLogger) {
	logger = sharedLogger;
}
