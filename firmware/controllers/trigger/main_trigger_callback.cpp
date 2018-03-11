/**
 * @file    main_trigger_callback.cpp
 * @brief   Main logic is here!
 *
 * See http://rusefi.com/docs/html/
 *
 * @date Feb 7, 2013
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
#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include <nvic.h>
#endif

#if (!EFI_PROD_CODE && !EFI_SIMULATOR) || defined(__DOXYGEN__)

#define chThdGetSelfX() 0
#define getRemainingStack(x) (999999)

#endif

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

#include "main_trigger_callback.h"
#include "efiGpio.h"
#include "engine_math.h"
#include "trigger_central.h"
#include "spark_logic.h"
#include "rpm_calculator.h"
#include "signal_executor.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "advance_map.h"
#include "allsensors.h"
#include "cyclic_buffer.h"
#include "histogram.h"
#include "fuel_math.h"
#include "histogram.h"
#include "engine_controller.h"
#include "efiGpio.h"
#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "rfiutil.h"
#endif /* EFI_HISTOGRAMS */
#include "LocalVersionHolder.h"
#include "event_queue.h"
#include "engine.h"
#include "efilib2.h"
#include "aux_valves.h"
#include "backup_ram.h"

EXTERN_ENGINE
;
extern bool hasFirmwareErrorFlag;

static const char *prevOutputName = NULL;

static InjectionEvent primeInjEvent;

static Logging *logger;
#if ! EFI_UNIT_TEST
static pid_s *fuelPidS = &persistentState.persistentConfiguration.engineConfiguration.fuelClosedLoopPid;
static Pid fuelPid(fuelPidS);
extern TunerStudioOutputChannels tsOutputChannels;
#endif

// todo: figure out if this even helps?
//#if defined __GNUC__
//#define RAM_METHOD_PREFIX __attribute__((section(".ram")))
//#else
//#define RAM_METHOD_PREFIX
//#endif

void startSimultaniousInjection(Engine *engine) {
	for (int i = 0; i < engine->engineConfiguration->specs.cylindersCount; i++) {
		enginePins.injectors[i].setHigh();
	}
}

static void endSimultaniousInjectionOnlyTogglePins(Engine *engine) {
	for (int i = 0; i < engine->engineConfiguration->specs.cylindersCount; i++) {
		enginePins.injectors[i].setLow();
	}
}

void endSimultaniousInjection(InjectionEvent *event) {
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	Engine *engine = event->engine;
	EXPAND_Engine;
#endif
	endSimultaniousInjectionOnlyTogglePins(engine);
	engine->injectionEvents.addFuelEventsForCylinder(event->ownIndex PASS_ENGINE_PARAMETER_SUFFIX);
}

static inline void tempTurnPinHigh(InjectorOutputPin *output) {
	output->overlappingCounter++;

#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	printf("seTurnPinHigh %s %d %d\r\n", output->name, output->overlappingCounter, (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

	if (output->overlappingCounter > 1) {
//		/**
//		 * #299
//		 * this is another kind of overlap which happens in case of a small duty cycle after a large duty cycle
//		 */
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
		printf("overlapping, no need to touch pin %s %d\r\n", output->name, (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */
	} else {
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
		const char * w = output->currentLogicValue == true ? "err" : "";
//	scheduleMsg(&sharedLogger, "^ %spin=%s eventIndex %d %d", w, output->name,
//			getRevolutionCounter(), getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

		output->setHigh();
	}
}

// todo: make these macro? kind of a penny optimization if compiler is not smart to inline
void seTurnPinHigh(InjectionSignalPair *pair) {
	for (int i = 0;i < MAX_WIRES_COUNT;i++) {
		InjectorOutputPin *output = pair->outputs[i];
		if (output != NULL) {
			tempTurnPinHigh(output);
		}
	}
}

static inline void tempTurnPinLow(InjectorOutputPin *output) {
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	printf("seTurnPinLow %s %d %d\r\n", output->name, output->overlappingCounter, (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

	if (output->cancelNextTurningInjectorOff) {
		/**
		 * in case of fuel schedule overlap between engine cycles,
		 * and if engine cycle is above say 75% for batch mode on 4 cylinders,
		 * we will get a secondary overlap between the special injection and a normal injection on the same injector.
		 * In such a case want to combine these two injection into one continues injection.
		 * Unneeded turn of injector on is handle while scheduling that second injection, but cancellation
		 * of special injection end has to be taken care of dynamically
		 *
		 */
		output->cancelNextTurningInjectorOff = false;
#if EFI_SIMULATOR || defined(__DOXYGEN__)
		printf("was cancelled %s %d\r\n", output->name, (int)getTimeNowUs());
#endif /* EFI_SIMULATOR */
	} else {

#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
		const char * w = output->currentLogicValue == false ? "err" : "";

//	scheduleMsg(&sharedLogger, "- %spin=%s eventIndex %d %d", w, output->name,
//			getRevolutionCounter(), getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

		output->overlappingCounter--;
		if (output->overlappingCounter > 0) {
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
			printf("was overlapping, no need to touch pin %s %d\r\n", output->name, (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */
		} else {
			output->setLow();
		}
	}
}

void seTurnPinLow(InjectionSignalPair *pair) {
	pair->isScheduled = false;
	for (int i = 0;i<MAX_WIRES_COUNT;i++) {
		InjectorOutputPin *output = pair->outputs[i];
		if (output != NULL) {
			tempTurnPinLow(output);
		}
	}
	efiAssertVoid(pair->event != NULL, "pair event");
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	Engine *engine = pair->event->engine;
	EXPAND_Engine;
#endif
	ENGINE(injectionEvents.addFuelEventsForCylinder(pair->event->ownIndex PASS_ENGINE_PARAMETER_SUFFIX));
}

static void sescheduleByTimestamp(scheduling_s *scheduling, efitimeus_t time, schfunc_t callback, InjectionSignalPair *pair) {
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	InjectorOutputPin *param = pair->outputs[0];
//	scheduleMsg(&sharedLogger, "schX %s %x %d", prefix, scheduling,	time);
//	scheduleMsg(&sharedLogger, "schX %s", param->name);

	const char *direction = callback == (schfunc_t) &seTurnPinHigh ? "up" : "down";
	printf("seScheduleByTime %s %s %d sch=%d\r\n", direction, param->name, (int)time, (int)scheduling);
#endif /* FUEL_MATH_EXTREME_LOGGING || EFI_UNIT_TEST */

	scheduleByTimestamp(scheduling, time, callback, pair);
}

static ALWAYS_INLINE void handleFuelInjectionEvent(int injEventIndex, InjectionEvent *event,
		int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {

	/**
	 * todo: this is a bit tricky with batched injection. is it? Does the same
	 * wetting coefficient works the same way for any injection mode, or is something
	 * x2 or /2?
	 */
	const floatms_t injectionDuration = ENGINE(wallFuel).adjust(event->outputs[0]->injectorIndex, ENGINE(injectionDuration) PASS_ENGINE_PARAMETER_SUFFIX);
#if EFI_PRINTF_FUEL_DETAILS || defined(__DOXYGEN__)
	printf("fuel injectionDuration=%.2f adjusted=%.2f\t\n", ENGINE(injectionDuration), injectionDuration);
#endif /*EFI_PRINTF_FUEL_DETAILS */

	bool isCranking = ENGINE(rpmCalculator).isCranking(PASS_ENGINE_PARAMETER_SIGNATURE);
	/**
	 * todo: pre-calculate 'numberOfInjections'
	 * see also injectorDutyCycle
	 */
	if (!isCranking && injectionDuration * getNumberOfInjections(engineConfiguration->injectionMode PASS_ENGINE_PARAMETER_SUFFIX) > getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX)) {
		warning(CUSTOM_TOO_LONG_FUEL_INJECTION, "Too long fuel injection %.2fms", injectionDuration);
	} else if (isCranking && injectionDuration * getNumberOfInjections(engineConfiguration->crankingInjectionMode PASS_ENGINE_PARAMETER_SUFFIX) > getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX)) {
		warning(CUSTOM_TOO_LONG_CRANKING_FUEL_INJECTION, "Too long cranking fuel injection %.2fms", injectionDuration);
	}

	// Store 'pure' injection duration (w/o injector lag) for fuel rate calc.
	engine->engineState.fuelConsumption.addData(injectionDuration - ENGINE(engineState.injectorLag));
	
	ENGINE(actualLastInjection) = injectionDuration;
	if (cisnan(injectionDuration)) {
		warning(CUSTOM_OBD_NAN_INJECTION, "NaN injection pulse");
		return;
	}
	if (injectionDuration < 0) {
		warning(CUSTOM_OBD_NEG_INJECTION, "Negative injection pulse %.2f", injectionDuration);
		return;
	}
	floatus_t durationUs = MS2US(injectionDuration);


#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
//	scheduleMsg(logger, "handleFuel totalPerCycle=%.2f", totalPerCycle);
//	scheduleMsg(logger, "handleFuel engineCycleDuration=%.2f", engineCycleDuration);
#endif /* FUEL_MATH_EXTREME_LOGGING */

	floatus_t injectionStartDelayUs = ENGINE(rpmCalculator.oneDegreeUs) * event->injectionStart.angleOffset;

#if EFI_DEFAILED_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "handleFuel pin=%s eventIndex %d duration=%.2fms %d", event->outputs[0]->name,
			injEventIndex,
			injectionDuration,
			getRevolutionCounter());
	scheduleMsg(logger, "handleFuel pin=%s delay=%.2f %d", event->outputs[0]->name, injectionStartDelayUs,
			getRevolutionCounter());
#endif /* EFI_DEFAILED_LOGGING */

	InjectionSignalPair *pair = &ENGINE(fuelActuators[injEventIndex]);

	if (event->isSimultanious) {
		/**
		 * this is pretty much copy-paste of 'scheduleOutput'
		 * 'scheduleOutput' is currently only used for injection, so maybe it should be
		 * changed into 'scheduleInjection' and unified? todo: think about it.
		 */

		scheduling_s * sUp = &pair->signalTimerUp;
// todo: sequential need this logic as well, just do not forget to clear flag		pair->isScheduled = true;
		scheduling_s * sDown = &pair->signalTimerDown;

		scheduleForLater(sUp, (int) injectionStartDelayUs, (schfunc_t) &startSimultaniousInjection, engine);
		scheduleForLater(sDown, (int) injectionStartDelayUs + durationUs,
					(schfunc_t) &endSimultaniousInjection, event);

	} else {
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		printf("scheduling injection angle=%.2f/delay=%.2f injectionDuration=%.2f\r\n", event->injectionStart.angleOffset, injectionStartDelayUs, injectionDuration);
#endif

		// we are in this branch of code only in case of NOT IM_SIMULTANEOUS/IM_SINGLE_POINT injection
		// we are ignoring low RPM in order not to handle "engine was stopped to engine now running" transition
		if (rpm > 2 * engineConfiguration->cranking.rpm) {
			const char *outputName = event->outputs[0]->name;
			if (prevOutputName == outputName
					&& engineConfiguration->injectionMode != IM_SIMULTANEOUS
					&& engineConfiguration->injectionMode != IM_SINGLE_POINT) {
				warning(CUSTOM_OBD_SKIPPED_FUEL, "looks like skipped fuel event %d %s", getRevolutionCounter(), outputName);
			}
			prevOutputName = outputName;
		}

		efitimeus_t nowUs = getTimeNowUs();

		InjectorOutputPin *output = event->outputs[0];
	#if EFI_PRINTF_FUEL_DETAILS || defined(__DOXYGEN__)
		printf("fuelout %s duration %d total=%d\t\n", output->name, (int)durationUs,
				(int)MS2US(getCrankshaftRevolutionTimeMs(ENGINE(rpmCalculator.rpmValue))));
	#endif /*EFI_PRINTF_FUEL_DETAILS */


		if (pair->isScheduled) {
	#if EFI_UNIT_TEST || EFI_SIMULATOR || defined(__DOXYGEN__)
		printf("still used1 %s %d\r\n", output->name, (int)getTimeNowUs());
	#endif /* EFI_UNIT_TEST || EFI_SIMULATOR */
			return; // this InjectionSignalPair is still needed for an extremely long injection scheduled previously
		}
		pair->outputs[0] = output;
		pair->outputs[1] = event->outputs[1];
		scheduling_s * sUp = &pair->signalTimerUp;
		scheduling_s * sDown = &pair->signalTimerDown;

		pair->isScheduled = true;
		pair->event = event;
		efitimeus_t turnOnTime = nowUs + (int) injectionStartDelayUs;
		bool isSecondaryOverlapping = turnOnTime < output->overlappingScheduleOffTime;

		if (isSecondaryOverlapping) {
			output->cancelNextTurningInjectorOff = true;
	#if EFI_UNIT_TEST || EFI_SIMULATOR || defined(__DOXYGEN__)
		printf("please cancel %s %d %d\r\n", output->name, (int)getTimeNowUs(), output->overlappingCounter);
	#endif /* EFI_UNIT_TEST || EFI_SIMULATOR */
		} else {
			sescheduleByTimestamp(sUp, turnOnTime, (schfunc_t) &seTurnPinHigh, pair);
		}
		efitimeus_t turnOffTime = nowUs + (int) (injectionStartDelayUs + durationUs);
		sescheduleByTimestamp(sDown, turnOffTime, (schfunc_t) &seTurnPinLow, pair);
	}
}

static void fuelClosedLoopCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if ! EFI_UNIT_TEST
	if (GET_RPM() < CONFIG(fuelClosedLoopRpmThreshold) ||
			ENGINE(sensors.clt) < CONFIG(fuelClosedLoopCltThreshold) ||
			getTPS(PASS_ENGINE_PARAMETER_SIGNATURE) > CONFIG(fuelClosedLoopTpsThreshold) ||
			ENGINE(sensors.currentAfr) < boardConfiguration->fuelClosedLoopAfrLowThreshold ||
			ENGINE(sensors.currentAfr) > engineConfiguration->fuelClosedLoopAfrHighThreshold) {
		engine->engineState.fuelPidCorrection = 0;
		fuelPid.reset();
		return;
	}

	engine->engineState.fuelPidCorrection = fuelPid.getValue(ENGINE(engineState.targetAFR), ENGINE(sensors.currentAfr), 1);
	if (engineConfiguration->debugMode == DBG_FUEL_PID_CORRECTION) {
		tsOutputChannels.debugFloatField1 = engine->engineState.fuelPidCorrection;
		fuelPid.postState(&tsOutputChannels);
	}

#endif
}


static ALWAYS_INLINE void handleFuel(const bool limitedFuel, uint32_t trgEventIndex, int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 128, "lowstck#3");
	efiAssertVoid(trgEventIndex < engine->engineCycleEventCount, "handleFuel/event index");

	if (!isInjectionEnabled(PASS_ENGINE_PARAMETER_SIGNATURE) || limitedFuel) {
		return;
	}
	if (ENGINE(isCylinderCleanupMode)) {
		return;
	}


	/**
	 * Ignition events are defined by addFuelEvents() according to selected
	 * fueling strategy
	 */
	FuelSchedule *fs = &ENGINE(injectionEvents);
	if (!fs->isReady) {
		fs->addFuelEvents(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "handleFuel ind=%d %d", trgEventIndex, getRevolutionCounter());
#endif /* FUEL_MATH_EXTREME_LOGGING */

	ENGINE(tpsAccelEnrichment.onNewValue(getTPS(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX));
	ENGINE(engineLoadAccelEnrichment.onEngineCycle(PASS_ENGINE_PARAMETER_SIGNATURE));

	ENGINE(injectionDuration) = getInjectionDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX);

	for (int injEventIndex = 0; injEventIndex < CONFIG(specs.cylindersCount); injEventIndex++) {
		InjectionEvent *event = &fs->elements[injEventIndex];
		uint32_t eventIndex = event->injectionStart.eventIndex;
// right after trigger change we are still using old & invalid fuel schedule. good news is we do not change trigger on the fly in real life
//		efiAssertVoid(eventIndex < ENGINE(triggerShape.getLength()), "handleFuel/event sch index");
		if (eventIndex != trgEventIndex) {
			continue;
		}
		handleFuelInjectionEvent(injEventIndex, event, rpm PASS_ENGINE_PARAMETER_SUFFIX);
	}
}

#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
static histogram_s mainLoopHistogram;
#endif /* EFI_HISTOGRAMS */

void showMainHistogram(void) {
#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
	printHistogram(logger, &mainLoopHistogram);
#endif /* EFI_HISTOGRAMS */
}

#if EFI_PROD_CODE || defined(__DOXYGEN__)
/**
 * this field is used as an Expression in IAR debugger
 */
uint32_t *cyccnt = (uint32_t*) &DWT->CYCCNT;
#endif

/**
 * This is the main trigger event handler.
 * Both injection and ignition are controlled from this method.
 */
void mainTriggerCallback(trigger_event_e ckpSignalType, uint32_t trgEventIndex DECLARE_ENGINE_PARAMETER_SUFFIX) {
	(void) ckpSignalType;

	ENGINE(m.beforeMainTrigger) = GET_TIMESTAMP();
	if (hasFirmwareError()) {
		/**
		 * In case on a major error we should not process any more events.
		 * TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		 */
		return;
	}
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 128, "lowstck#2");

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
		// todo: check for 'trigger->is_synchnonized?'
		// TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		return;
	}
	if (rpm == NOISY_RPM) {
		warning(OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, "noisy trigger");
		// TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		return;
	}
	bool limitedSpark = rpm > CONFIG(rpmHardLimit);
	bool limitedFuel = rpm > CONFIG(rpmHardLimit);

	if (CONFIG(boostCutPressure) !=0) {
		if (getMap() > CONFIG(boostCutPressure)) {
			limitedSpark = true;
			limitedFuel = true;
		}
	}

	if (limitedSpark || limitedFuel) {
		// todo: this is not really a warning
		warning(CUSTOM_SKIPPING_STROKE, "skipping stroke due to rpm=%d", rpm);
	}

#if (EFI_HISTOGRAMS && EFI_PROD_CODE) || defined(__DOXYGEN__)
	int beforeCallback = hal_lld_get_counter_value();
#endif

	if (trgEventIndex == 0) {

		if (checkIfTriggerConfigChanged()) {
			engine->ignitionEvents.isReady = false; // we need to rebuild ignition schedule
			engine->injectionEvents.isReady = false;
			// moved 'triggerIndexByAngle' into trigger initialization (why was it invoked from here if it's only about trigger shape & optimization?)
			// see initializeTriggerShape() -> prepareOutputSignals(PASS_ENGINE_PARAMETER_SIGNATURE)

			// we need this to apply new 'triggerIndexByAngle' values
			engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
		}

		if (CONFIG(fuelClosedLoopCorrectionEnabled)) {
			fuelClosedLoopCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);
		}
	}

	efiAssertVoid(!CONFIG(useOnlyRisingEdgeForTrigger) || CONFIG(ignMathCalculateAtIndex) % 2 == 0, "invalid ignMathCalculateAtIndex");

	if (trgEventIndex == CONFIG(ignMathCalculateAtIndex)) {
		if (CONFIG(externalKnockSenseAdc) != EFI_ADC_NONE) {
			float externalKnockValue = getVoltageDivided("knock", engineConfiguration->externalKnockSenseAdc);
			engine->knockLogic(externalKnockValue);
		}
	}


	/**
	 * For fuel we schedule start of injection based on trigger angle, and then inject for
	 * specified duration of time
	 */
	handleFuel(limitedFuel, trgEventIndex, rpm PASS_ENGINE_PARAMETER_SUFFIX);
	/**
	 * For spark we schedule both start of coil charge and actual spark based on trigger angle
	 */
	handleSpark(limitedSpark, trgEventIndex, rpm PASS_ENGINE_PARAMETER_SUFFIX);
#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
	int diff = hal_lld_get_counter_value() - beforeCallback;
	if (diff > 0)
	hsAdd(&mainLoopHistogram, diff);
#endif /* EFI_HISTOGRAMS */

	if (trgEventIndex == 0) {
		ENGINE(m.mainTriggerCallbackTime) = GET_TIMESTAMP() - ENGINE(m.beforeMainTrigger);
	}
}

// Check if the engine is not stopped or cylinder cleanup is activated
static bool isPrimeInjectionPulseSkipped(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!engine->rpmCalculator.isStopped(PASS_ENGINE_PARAMETER_SIGNATURE))
		return true;
	return CONFIG(isCylinderCleanupEnabled) && (getTPS(PASS_ENGINE_PARAMETER_SIGNATURE) > CLEANUP_MODE_TPS);
}

/**
 * Prime injection pulse, mainly needed for mono-injectors or long intake manifolds.
 * See testStartOfCrankingPrimingPulse()
 */
void startPrimeInjectionPulse(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// First, we need a protection against 'fake' ignition switch on and off (i.e. no engine started), to avoid repeated prime pulses.
	// So we check and update the ignition switch counter in non-volatile backup-RAM
#if EFI_PROD_CODE || defined(__DOXYGEN__)
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
		// fill-in the prime event struct
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		primeInjEvent.engine = engine;
#endif /* EFI_UNIT_TEST */
		primeInjEvent.ownIndex = 0;
		primeInjEvent.isSimultanious = true;

		scheduling_s *sDown = &ENGINE(fuelActuators[0]).signalTimerDown;
		// When the engine is hot, basically we don't need prime inj.pulse, so we use an interpolation over temperature (falloff).
		// If 'primeInjFalloffTemperature' is not specified (by default), we have a prime pulse deactivation at zero celsius degrees, which is okay.
		const float maxPrimeInjAtTemperature = -40.0f;	// at this temperature the pulse is maximal.
		floatms_t pulseLength = interpolateClamped(maxPrimeInjAtTemperature, CONFIG(startOfCrankingPrimingPulse),
			CONFIG(primeInjFalloffTemperature), 0.0f, ENGINE(sensors.clt));
		if (pulseLength > 0) {
			startSimultaniousInjection(engine);
			efitimeus_t turnOffDelayUs = (efitimeus_t)efiRound(MS2US(pulseLength), 1.0f);
			scheduleForLater(sDown, turnOffDelayUs, (schfunc_t) &endSimultaniousInjectionOnlyTogglePins, engine);
		}
	}
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	// we'll reset it later when the engine starts
	backupRamSave(BACKUP_IGNITION_SWITCH_COUNTER, ignSwitchCounter + 1);
#endif /* EFI_PROD_CODE */
}

void updatePrimeInjectionPulseState(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	static bool counterWasReset = false;
	if (counterWasReset)
		return;

	if (!engine->rpmCalculator.isStopped(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		backupRamSave(BACKUP_IGNITION_SWITCH_COUNTER, 0);
		counterWasReset = true;
	}
#endif /* EFI_PROD_CODE */
}

#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
#include "engine_sniffer.h"
#endif

static void showTriggerHistogram(void) {
	printAllCallbacksHistogram();
	showMainHistogram();
#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
	showWaveChartHistogram();
#endif
}

static void showMainInfo(Engine *engine) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	int rpm = engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE);
	float el = getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE);
	scheduleMsg(logger, "rpm %d engine_load %.2f", rpm, el);
	scheduleMsg(logger, "fuel %.2fms timing %.2f", getInjectionDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX), engine->engineState.timingAdvance);
#endif
}

void initMainEventListener(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	logger = sharedLogger;
	efiAssertVoid(engine!=NULL, "null engine");
	initSparkLogic(logger);

	initAuxValves(logger);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	addConsoleAction("performanceinfo", showTriggerHistogram);
	addConsoleActionP("maininfo", (VoidPtr) showMainInfo, engine);

	printMsg(logger, "initMainLoop: %d", currentTimeMillis());
	if (!isInjectionEnabled(PASS_ENGINE_PARAMETER_SIGNATURE))
		printMsg(logger, "!!!!!!!!!!!!!!!!!!! injection disabled");
#endif

#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
	initHistogram(&mainLoopHistogram, "main callback");
#endif /* EFI_HISTOGRAMS */

	addTriggerEventListener(mainTriggerCallback, "main loop", engine);

    // We start prime injection pulse at the early init stage - don't wait for the engine to start spinning!
    if (CONFIG(startOfCrankingPrimingPulse) > 0)
    	startPrimeInjectionPulse(PASS_ENGINE_PARAMETER_SIGNATURE);

}

#endif /* EFI_ENGINE_CONTROL */
