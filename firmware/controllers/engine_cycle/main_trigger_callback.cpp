/**
 * @file    main_trigger_callback.cpp
 * @brief   Main logic is here!
 *
 * See http://rusefi.com/docs/html/
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
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

#if EFI_PRINTF_FUEL_DETAILS
	bool printFuelDebug = false;
#endif // EFI_PRINTF_FUEL_DETAILS

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT

#include "main_trigger_callback.h"
#include "trigger_central.h"
#include "spark_logic.h"
#include "advance_map.h"
#include "cyclic_buffer.h"
#include "fuel_math.h"
#include "cdm_ion_sense.h"
#include "tooth_logger.h"
#include "os_util.h"
#include "local_version_holder.h"
#include "event_queue.h"
#include "injector_model.h"
#if EFI_LAUNCH_CONTROL
#include "launch_control.h"
#endif

#include "backup_ram.h"

// todo: figure out if this even helps?
//#if defined __GNUC__
//#define RAM_METHOD_PREFIX __attribute__((section(".ram")))
//#else
//#define RAM_METHOD_PREFIX
//#endif

void startSimultaniousInjection(Engine *engine) {
#if EFI_UNIT_TEST
	EXPAND_Engine;
#endif // EFI_UNIT_TEST
	efitick_t nowNt = getTimeNowNt();
	for (size_t i = 0; i < CONFIG(specs.cylindersCount); i++) {
		enginePins.injectors[i].open(nowNt PASS_ENGINE_PARAMETER_SUFFIX);
	}
}

static void endSimultaniousInjectionOnlyTogglePins(Engine *engine) {
#if EFI_UNIT_TEST
	EXPAND_Engine;
#endif
	efitick_t nowNt = getTimeNowNt();
	for (size_t i = 0; i < CONFIG(specs.cylindersCount); i++) {
		enginePins.injectors[i].close(nowNt PASS_ENGINE_PARAMETER_SUFFIX);
	}
}

void endSimultaniousInjection(InjectionEvent *event) {
#if EFI_UNIT_TEST
	Engine *engine = event->engine;
	EXPAND_Engine;
#endif
	event->isScheduled = false;
	endSimultaniousInjectionOnlyTogglePins(engine);
	engine->injectionEvents.addFuelEventsForCylinder(event->ownIndex PASS_ENGINE_PARAMETER_SUFFIX);
}

void InjectorOutputPin::open(efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	overlappingCounter++;

#if FUEL_MATH_EXTREME_LOGGING
	if (printFuelDebug) {
		printf("InjectorOutputPin::open %s %d now=%0.1fms\r\n", name, overlappingCounter, (int)getTimeNowUs() / 1000.0);
	}
#endif /* FUEL_MATH_EXTREME_LOGGING */

	if (overlappingCounter > 1) {
//		/**
//		 * #299
//		 * this is another kind of overlap which happens in case of a small duty cycle after a large duty cycle
//		 */
#if FUEL_MATH_EXTREME_LOGGING
		if (printFuelDebug) {
			printf("overlapping, no need to touch pin %s %d\r\n", name, (int)getTimeNowUs());
		}
#endif /* FUEL_MATH_EXTREME_LOGGING */
	} else {
#if EFI_TOOTH_LOGGER
	LogTriggerInjectorState(nowNt, true PASS_ENGINE_PARAMETER_SUFFIX);
#endif // EFI_TOOTH_LOGGER
		this->inject(PASS_ENGINE_PARAMETER_SIGNATURE);
		setHigh();
	}
}

void turnInjectionPinHigh(InjectionEvent *event) {
#if EFI_UNIT_TEST
	Engine *engine = event->engine;
	EXPAND_Engine;
#endif // EFI_UNIT_TEST
	efitick_t nowNt = getTimeNowNt();
	for (int i = 0;i < MAX_WIRES_COUNT;i++) {
		InjectorOutputPin *output = event->outputs[i];

		if (output) {
			output->open(nowNt PASS_ENGINE_PARAMETER_SUFFIX);
		}
	}
}

void InjectorOutputPin::close(efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if FUEL_MATH_EXTREME_LOGGING
	if (printFuelDebug) {
		printf("InjectorOutputPin::close %s %d %d\r\n", name, overlappingCounter, (int)getTimeNowUs());
	}
#endif /* FUEL_MATH_EXTREME_LOGGING */

	overlappingCounter--;
	if (overlappingCounter > 0) {
#if FUEL_MATH_EXTREME_LOGGING
		if (printFuelDebug) {
			printf("was overlapping, no need to touch pin %s %d\r\n", name, (int)getTimeNowUs());
		}
#endif /* FUEL_MATH_EXTREME_LOGGING */
	} else {
#if EFI_TOOTH_LOGGER
	LogTriggerInjectorState(nowNt, false PASS_ENGINE_PARAMETER_SUFFIX);
#endif // EFI_TOOTH_LOGGER
		setLow();
	}

	// Don't allow negative overlap count
	if (overlappingCounter < 0) {
		overlappingCounter = 0;
	}
}

void turnInjectionPinLow(InjectionEvent *event) {
	efitick_t nowNt = getTimeNowNt();

#if EFI_UNIT_TEST
	Engine *engine = event->engine;
	EXPAND_Engine;
#endif

	engine->mostRecentTimeBetweenIgnitionEvents = nowNt - engine->mostRecentIgnitionEvent;
	engine->mostRecentIgnitionEvent = nowNt;

	event->isScheduled = false;
	for (int i = 0;i<MAX_WIRES_COUNT;i++) {
		InjectorOutputPin *output = event->outputs[i];
		if (output) {
			output->close(nowNt PASS_ENGINE_PARAMETER_SUFFIX);
		}
	}
	ENGINE(injectionEvents.addFuelEventsForCylinder(event->ownIndex PASS_ENGINE_PARAMETER_SUFFIX));
}

void InjectionEvent::onTriggerTooth(size_t trgEventIndex, int rpm, efitick_t nowNt) {
	uint32_t eventIndex = injectionStart.triggerEventIndex;
// right after trigger change we are still using old & invalid fuel schedule. good news is we do not change trigger on the fly in real life
//		efiAssertVoid(CUSTOM_ERR_ASSERT_VOID, eventIndex < ENGINE(triggerShape.getLength()), "handleFuel/event sch index");
	if (eventIndex != trgEventIndex) {
		return;
	}

	// Select fuel mass from the correct bank
	uint8_t bankIndex = CONFIG(cylinderBankSelect[this->cylinderNumber]);
	float injectionMassGrams = ENGINE(injectionMass)[bankIndex];

	// Perform wall wetting adjustment on fuel mass, not duration, so that
	// it's correct during fuel pressure (injector flow) or battery voltage (deadtime) transients
	injectionMassGrams = wallFuel.adjust(injectionMassGrams PASS_ENGINE_PARAMETER_SUFFIX);
	const floatms_t injectionDuration = ENGINE(injectorModel)->getInjectionDuration(injectionMassGrams);

#if EFI_PRINTF_FUEL_DETAILS
	if (printFuelDebug) {
		printf("fuel index=%d injectionDuration=%.2fms adjusted=%.2fms\n",
		  eventIndex,
		  ENGINE(injectionDuration),
		  injectionDuration);
	}
#endif /*EFI_PRINTF_FUEL_DETAILS */

	bool isCranking = ENGINE(rpmCalculator).isCranking();
	/**
	 * todo: pre-calculate 'numberOfInjections'
	 * see also injectorDutyCycle
	 */
	int numberOfInjections = isCranking ? getNumberOfInjections(engineConfiguration->crankingInjectionMode PASS_ENGINE_PARAMETER_SUFFIX) : getNumberOfInjections(engineConfiguration->injectionMode PASS_ENGINE_PARAMETER_SUFFIX);
	if (injectionDuration * numberOfInjections > getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX)) {
		warning(CUSTOM_TOO_LONG_FUEL_INJECTION, "Too long fuel injection %.2fms", injectionDuration);
	}

	ENGINE(engineState.fuelConsumption).consumeFuel(injectionMassGrams * numberOfInjections, nowNt);

	ENGINE(actualLastInjection)[bankIndex] = injectionDuration;

	if (cisnan(injectionDuration)) {
		warning(CUSTOM_OBD_NAN_INJECTION, "NaN injection pulse");
		return;
	}
	if (injectionDuration < 0) {
		warning(CUSTOM_OBD_NEG_INJECTION, "Negative injection pulse %.2f", injectionDuration);
		return;
	}

	// If somebody commanded an impossibly short injection, do nothing.
	// Durations under 50us-ish aren't safe for the scheduler
	// as their order may be swapped, resulting in a stuck open injector
	// see https://github.com/rusefi/rusefi/pull/596 for more details
	if (injectionDuration < 0.050f)
	{
		return;
	}

	floatus_t durationUs = MS2US(injectionDuration);


	// we are ignoring low RPM in order not to handle "engine was stopped to engine now running" transition
/*
 * Wall Wetting would totally skip fuel on sudden deceleration a
	if (rpm > 2 * engineConfiguration->cranking.rpm) {
		const char *outputName = outputs[0]->name;
		if (engine->prevOutputName == outputName
				&& engineConfiguration->injectionMode != IM_SIMULTANEOUS
				&& engineConfiguration->injectionMode != IM_SINGLE_POINT) {
			warning(CUSTOM_OBD_SKIPPED_FUEL, "looks like skipped fuel event revCounter=%d %s", getRevolutionCounter(), outputName);
		}
		engine->prevOutputName = outputName;
	}
*/

#if EFI_PRINTF_FUEL_DETAILS
	if (printFuelDebug) {
		InjectorOutputPin *output = outputs[0];
		printf("handleFuelInjectionEvent fuelout %s injection_duration %dus engineCycleDuration=%.1fms\t\n", output->name, (int)durationUs,
				(int)MS2US(getCrankshaftRevolutionTimeMs(GET_RPM())) / 1000.0);
	}
#endif /*EFI_PRINTF_FUEL_DETAILS */

	if (isScheduled) {
#if EFI_PRINTF_FUEL_DETAILS
		if (printFuelDebug) {
			InjectorOutputPin *output = outputs[0];
			printf("handleFuelInjectionEvent still used %s now=%.1fms\r\n", output->name, (int)getTimeNowUs() / 1000.0);
		}
#endif /*EFI_PRINTF_FUEL_DETAILS */
		return; // this InjectionEvent is still needed for an extremely long injection scheduled previously
	}

	isScheduled = true;

	action_s startAction, endAction;
	// We use different callbacks based on whether we're running sequential mode or not - everything else is the same
	if (isSimultanious) {
		startAction = { &startSimultaniousInjection, engine };
		endAction = { &endSimultaniousInjection, this };
	} else {
		// sequential or batch
		startAction = { &turnInjectionPinHigh, this };
		endAction = { &turnInjectionPinLow, this };
	}

	efitick_t startTime = scheduleByAngle(&signalTimerUp, nowNt, injectionStart.angleOffsetFromTriggerEvent, startAction PASS_ENGINE_PARAMETER_SUFFIX);
	efitick_t turnOffTime = startTime + US2NT((int)durationUs);
	engine->executor.scheduleByTimestampNt("inj", &endOfInjectionEvent, turnOffTime, endAction);

#if EFI_UNIT_TEST
		printf("scheduling injection angle=%.2f/delay=%.2f injectionDuration=%.2f\r\n", injectionStart.angleOffsetFromTriggerEvent, NT2US(startTime - nowNt), injectionDuration);
#endif
#if EFI_DEFAILED_LOGGING
	efiPrintf("handleFuel pin=%s eventIndex %d duration=%.2fms %d", outputs[0]->name,
			injEventIndex,
			injectionDuration,
			getRevolutionCounter());
	efiPrintf("handleFuel pin=%s delay=%.2f %d", outputs[0]->name, NT2US(startTime - nowNt),
			getRevolutionCounter());
#endif /* EFI_DEFAILED_LOGGING */
}

static void handleFuel(const bool limitedFuel, uint32_t trgEventIndex, int rpm, efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	ScopePerf perf(PE::HandleFuel);
	
	efiAssertVoid(CUSTOM_STACK_6627, getCurrentRemainingStack() > 128, "lowstck#3");
	efiAssertVoid(CUSTOM_ERR_6628, trgEventIndex < engine->engineCycleEventCount, "handleFuel/event index");

	ENGINE(tpsAccelEnrichment.onNewValue(Sensor::getOrZero(SensorType::Tps1) PASS_ENGINE_PARAMETER_SUFFIX));
	if (trgEventIndex == 0) {
		ENGINE(tpsAccelEnrichment.onEngineCycleTps(PASS_ENGINE_PARAMETER_SIGNATURE));
	}

	if (limitedFuel) {
		return;
	}
	if (ENGINE(isCylinderCleanupMode)) {
		return;
	}

	// If duty cycle is high, impose a fuel cut rev limiter.
	// This is safer than attempting to limp along with injectors or a pump that are out of flow.
	if (getInjectorDutyCycle(rpm PASS_ENGINE_PARAMETER_SUFFIX) > 96.0f) {
		warning(CUSTOM_OBD_63, "Injector Duty cycle cut");
		return;
	}

	/**
	 * Injection events are defined by addFuelEvents() according to selected
	 * fueling strategy
	 */
	FuelSchedule *fs = &ENGINE(injectionEvents);
	if (!fs->isReady) {
		fs->addFuelEvents(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

#if FUEL_MATH_EXTREME_LOGGING
	if (printFuelDebug) {
		efiPrintf("handleFuel ind=%d %d", trgEventIndex, getRevolutionCounter());
	}
#endif /* FUEL_MATH_EXTREME_LOGGING */

	fs->onTriggerTooth(trgEventIndex, rpm, nowNt PASS_ENGINE_PARAMETER_SUFFIX);
}

#if EFI_PROD_CODE
/**
 * this field is used as an Expression in IAR debugger
 */
uint32_t *cyccnt = (uint32_t*) &DWT->CYCCNT;
#endif

/**
 * This is the main trigger event handler.
 * Both injection and ignition are controlled from this method.
 */
void mainTriggerCallback(uint32_t trgEventIndex, efitick_t edgeTimestamp DECLARE_ENGINE_PARAMETER_SUFFIX) {
	ScopePerf perf(PE::MainTriggerCallback);

	if (CONFIG(vvtMode[0]) == VVT_MIATA_NB2 && ENGINE(triggerCentral.vvtSyncTimeNt) == 0) {
		// this is a bit spaghetti code for sure
		// do not spark & do not fuel until we have VVT sync. NB2 is a special case
		// due to symmetrical crank wheel and we need to make sure no spark happens out of sync
		return;
	}

#if ! HW_CHECK_MODE
	if (hasFirmwareError()) {
		/**
		 * In case on a major error we should not process any more events.
		 * TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		 */
		return;
	}
#endif // HW_CHECK_MODE

#if EFI_CDM_INTEGRATION
	if (trgEventIndex == 0 && isBrainPinValid(CONFIG(cdmInputPin))) {
		int cdmKnockValue = getCurrentCdmValue(engine->triggerCentral.triggerState.getTotalRevolutionCounter());
		engine->knockLogic(cdmKnockValue);
	}
#endif /* EFI_CDM_INTEGRATION */

	if (trgEventIndex >= ENGINE(engineCycleEventCount)) {
		/**
		 * this could happen in case of a trigger error, just exit silently since the trigger error is supposed to be handled already
		 * todo: should this check be somewhere higher so that no trigger listeners are invoked with noise?
		 */
		return;
	}

	int rpm = GET_RPM();
	if (rpm == 0) {
		// this happens while we just start cranking
		/**
		 * While we have start-up priming pulse, we also want to start pumping fuel again as soon as possible.
		 * Hopefully battery would handle both cranking and fuel pump simultaneously?
		 */
		enginePins.fuelPumpRelay.setValue(true); // quickly set pin right from the callback here! todo: would it work OK for smart SPI pin?!
#if EFI_PROD_CODE
		engine->triggerActivityMs = currentTimeMillis();
#endif
		// todo: check for 'trigger->is_synchnonized?'
		// TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		return;
	}
	if (rpm == NOISY_RPM) {
		warning(OBD_Crankshaft_Position_Sensor_A_Circuit_Malfunction, "noisy trigger");
		// TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		return;
	}

	bool limitedSpark = !ENGINE(limpManager).allowIgnition();
	bool limitedFuel = !ENGINE(limpManager).allowInjection();

#if EFI_LAUNCH_CONTROL
	if (engine->launchController.isLaunchCondition && !limitedSpark && !limitedFuel) {
		/* in case we are not already on a limited conditions, check launch as well */

		limitedSpark &= engine->launchController.isLaunchSparkRpmRetardCondition();
		limitedFuel &= engine->launchController.isLaunchFuelRpmRetardCondition();
	}
#endif
	if (trgEventIndex == 0) {
		if (HAVE_CAM_INPUT()) {
			engine->triggerCentral.validateCamVvtCounters();
		}

		if (engine->triggerCentral.checkIfTriggerConfigChanged(PASS_ENGINE_PARAMETER_SIGNATURE)) {
			engine->ignitionEvents.isReady = false; // we need to rebuild complete ignition schedule
			engine->injectionEvents.isReady = false;
			// moved 'triggerIndexByAngle' into trigger initialization (why was it invoked from here if it's only about trigger shape & optimization?)
			// see initializeTriggerWaveform() -> prepareOutputSignals(PASS_ENGINE_PARAMETER_SIGNATURE)

			// we need this to apply new 'triggerIndexByAngle' values
			engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
		}
	}

	/**
	 * For fuel we schedule start of injection based on trigger angle, and then inject for
	 * specified duration of time
	 */
	handleFuel(limitedFuel, trgEventIndex, rpm, edgeTimestamp PASS_ENGINE_PARAMETER_SUFFIX);
	/**
	 * For spark we schedule both start of coil charge and actual spark based on trigger angle
	 */
	onTriggerEventSparkLogic(limitedSpark, trgEventIndex, rpm, edgeTimestamp PASS_ENGINE_PARAMETER_SUFFIX);
}

// Check if the engine is not stopped or cylinder cleanup is activated
static bool isPrimeInjectionPulseSkipped(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!engine->rpmCalculator.isStopped())
		return true;
	return CONFIG(isCylinderCleanupEnabled) && (Sensor::getOrZero(SensorType::Tps1) > CLEANUP_MODE_TPS);
}

/**
 * Prime injection pulse, mainly needed for mono-injectors or long intake manifolds.
 * See testStartOfCrankingPrimingPulse()
 */
void startPrimeInjectionPulse(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engine->primeInjEvent.inject(PASS_ENGINE_PARAMETER_SIGNATURE);

	// First, we need a protection against 'fake' ignition switch on and off (i.e. no engine started), to avoid repeated prime pulses.
	// So we check and update the ignition switch counter in non-volatile backup-RAM
#if EFI_PROD_CODE
	uint32_t ignSwitchCounter = backupRamLoad(BACKUP_IGNITION_SWITCH_COUNTER);
#else /* EFI_PROD_CODE */
	uint32_t ignSwitchCounter = 0;
#endif /* EFI_PROD_CODE */

	// if we're just toying with the ignition switch, give it another chance eventually...
	if (ignSwitchCounter > 10)
		ignSwitchCounter = 0;
	// If we're going to skip this pulse, then save the counter as 0.
	// That's because we'll definitely need the prime pulse next time (either due to the cylinder cleanup or the engine spinning)
	if (isPrimeInjectionPulseSkipped(PASS_ENGINE_PARAMETER_SIGNATURE))
		ignSwitchCounter = -1;
	// start prime injection if this is a 'fresh start'
	if (ignSwitchCounter == 0) {
		engine->primeInjEvent.ownIndex = 0;
		engine->primeInjEvent.isSimultanious = true;

		scheduling_s *sDown = &ENGINE(injectionEvents.elements[0]).endOfInjectionEvent;
		// When the engine is hot, basically we don't need prime inj.pulse, so we use an interpolation over temperature (falloff).
		// If 'primeInjFalloffTemperature' is not specified (by default), we have a prime pulse deactivation at zero celsius degrees, which is okay.
		const float maxPrimeInjAtTemperature = -40.0f;	// at this temperature the pulse is maximal.
		floatms_t pulseLength = interpolateClamped(maxPrimeInjAtTemperature, CONFIG(startOfCrankingPrimingPulse),
			CONFIG(primeInjFalloffTemperature), 0.0f, Sensor::get(SensorType::Clt).value_or(70));
		if (pulseLength > 0) {
			startSimultaniousInjection(engine);
			int turnOffDelayUs = efiRound(MS2US(pulseLength), 1.0f);
			engine->executor.scheduleForLater(sDown, turnOffDelayUs, { &endSimultaniousInjectionOnlyTogglePins, engine });
		}
	}
#if EFI_PROD_CODE
	// we'll reset it later when the engine starts
	backupRamSave(BACKUP_IGNITION_SWITCH_COUNTER, ignSwitchCounter + 1);
#endif /* EFI_PROD_CODE */
}

void updatePrimeInjectionPulseState(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_PROD_CODE
	static bool counterWasReset = false;
	if (counterWasReset)
		return;

	if (!engine->rpmCalculator.isStopped()) {
		backupRamSave(BACKUP_IGNITION_SWITCH_COUNTER, 0);
		counterWasReset = true;
	}
#endif /* EFI_PROD_CODE */
}

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
#endif

static void showMainInfo(Engine *engine) {
#if EFI_PROD_CODE
	int rpm = GET_RPM();
	float el = getFuelingLoad(PASS_ENGINE_PARAMETER_SIGNATURE);
	efiPrintf("rpm %d engine_load %.2f", rpm, el);
	efiPrintf("fuel %.2fms timing %.2f", ENGINE(injectionDuration), engine->engineState.timingAdvance);
#endif /* EFI_PROD_CODE */
}

void initMainEventListener(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	efiAssertVoid(CUSTOM_ERR_6631, engine!=NULL, "null engine");

#if EFI_PROD_CODE
	addConsoleActionP("maininfo", (VoidPtr) showMainInfo, engine);
#endif

    // We start prime injection pulse at the early init stage - don't wait for the engine to start spinning!
    if (CONFIG(startOfCrankingPrimingPulse) > 0)
    	startPrimeInjectionPulse(PASS_ENGINE_PARAMETER_SIGNATURE);

}

#endif /* EFI_ENGINE_CONTROL */
