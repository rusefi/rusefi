/**
 * @file    main_trigger_callback.cpp
 * @brief   Main logic is here!
 *
 * See http://rusefi.com/docs/html/
 *
 * @date Feb 7, 2013
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
#include "efiGpio.h"
#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "rfiutil.h"
#endif /* EFI_HISTOGRAMS */
#include "LocalVersionHolder.h"
#include "event_queue.h"
#include "engine.h"
#include "efilib2.h"

EXTERN_ENGINE
;
extern bool hasFirmwareErrorFlag;

static LocalVersionHolder triggerVersion;
static const char *prevOutputName = NULL;

static Logging *logger;
#if ! EFI_UNIT_TEST
static pid_s *fuelPidS = &persistentState.persistentConfiguration.engineConfiguration.fuelClosedLoopPid;
static Pid fuelPid(fuelPidS, -100, 100);
extern TunerStudioOutputChannels tsOutputChannels;
#endif

// todo: figure out if this even helps?
//#if defined __GNUC__
//#define RAM_METHOD_PREFIX __attribute__((section(".ram")))
//#else
//#define RAM_METHOD_PREFIX
//#endif

static void startSimultaniousInjection(InjectionEvent *event) {
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	Engine *engine = event->engine;
#endif
	for (int i = 0; i < engine->engineConfiguration->specs.cylindersCount; i++) {
		turnPinHigh(&enginePins.injectors[i]);
	}
}

static void endSimultaniousInjection(InjectionEvent *event) {
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	Engine *engine = event->engine;
	EXPAND_Engine;
#endif
	for (int i = 0; i < engine->engineConfiguration->specs.cylindersCount; i++) {
		turnPinLow(&enginePins.injectors[i]);
	}
	engine->injectionEvents.addFuelEventsForCylinder(event->ownIndex PASS_ENGINE_PARAMETER);
}

static void tempTurnPinHigh(InjectorOutputPin *output) {
	output->overlappingCounter++;

#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	printf("seTurnPinHigh %s %d %d\r\n", output->name, output->overlappingCounter, (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

	if (output->overlappingCounter > 1) {
//		if (output->cancelNextTurningInjectorOff) {
//			// how comes AutoTest.testFordAspire ends up here?
//		} else {
//		/**
//		 * #299
//		 * this is another kind of overlap which happens in case of a small duty cycle after a large duty cycle
//		 */
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
		printf("overlapping, no need to touch pin %s %d\r\n", output->name, (int)getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

//			output->cancelNextTurningInjectorOff = true;
			return;
//		}
	}
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	const char * w = output->currentLogicValue == true ? "err" : "";
//	scheduleMsg(&sharedLogger, "^ %spin=%s eventIndex %d %d", w, output->name,
//			getRevolutionCounter(), getTimeNowUs());
#endif /* FUEL_MATH_EXTREME_LOGGING */

	turnPinHigh(output);
}

// todo: make these macro? kind of a penny optimization if compiler is not smart to inline
void seTurnPinHigh(OutputSignalPair *pair) {
	for (int i = 0;i<MAX_WIRES_COUNT;i++) {
		InjectorOutputPin *output = pair->outputs[i];
		if (output != NULL) {
			tempTurnPinHigh(output);
		}
	}
}

static void tempTurnPinLow(InjectorOutputPin *output) {
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
		return;
	}

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
		return;
	}
	turnPinLow(output);
}

void seTurnPinLow(OutputSignalPair *pair) {
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
	engine->injectionEvents.addFuelEventsForCylinder(pair->event->ownIndex PASS_ENGINE_PARAMETER);
}

static void seScheduleByTime(const char *prefix, scheduling_s *scheduling, efitimeus_t time, schfunc_t callback, OutputSignalPair *pair) {
	InjectorOutputPin *param = pair->outputs[0];
#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
//	scheduleMsg(&sharedLogger, "schX %s %x %d", prefix, scheduling,	time);
//	scheduleMsg(&sharedLogger, "schX %s", param->name);
#endif /* FUEL_MATH_EXTREME_LOGGING */

#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	const char *direction = callback == (schfunc_t) &seTurnPinHigh ? "up" : "down";
	printf("seScheduleByTime %s %s %d sch=%d\r\n", direction, param->name, (int)time, (int)scheduling);
#endif /* FUEL_MATH_EXTREME_LOGGING || EFI_UNIT_TEST */

	scheduleByTime(true, prefix, scheduling, time, callback, pair);
}

static void scheduleFuelInjection(int rpm, OutputSignalPair *pair, efitimeus_t nowUs, floatus_t delayUs, floatus_t durationUs, InjectionEvent *event DECLARE_ENGINE_PARAMETER_S) {
	if (durationUs < 0) {
		warning(CUSTOM_NEGATIVE_DURATION, "duration cannot be negative: %d", durationUs);
		return;
	}
	if (cisnan(durationUs)) {
		warning(CUSTOM_NAN_DURACTION, "NaN in scheduleFuelInjection", durationUs);
		return;
	}
	InjectorOutputPin *output = event->outputs[0];
#if EFI_PRINTF_FUEL_DETAILS || defined(__DOXYGEN__)
	printf("fuelout %s duration %d total=%d\t\n", output->name, (int)durationUs,
			(int)MS2US(getCrankshaftRevolutionTimeMs(rpm)));
#endif /*EFI_PRINTF_FUEL_DETAILS */


	if (pair->isScheduled) {
#if EFI_UNIT_TEST || EFI_SIMULATOR || defined(__DOXYGEN__)
	printf("still used1 %s %d\r\n", output->name, (int)getTimeNowUs());
#endif /* EFI_UNIT_TEST || EFI_SIMULATOR */
		return; // this OutputSignalPair is still needed for an extremely long injection scheduled previously
	}
	pair->outputs[0] = output;
	pair->outputs[1] = event->outputs[1];
	scheduling_s * sUp = &pair->signalTimerUp;
	scheduling_s * sDown = &pair->signalTimerDown;

	pair->isScheduled = true;
	pair->event = event;
	efitimeus_t turnOnTime = nowUs + (int) delayUs;
	bool isSecondaryOverlapping = turnOnTime < output->overlappingScheduleOffTime;

	if (isSecondaryOverlapping) {
		output->cancelNextTurningInjectorOff = true;
#if EFI_UNIT_TEST || EFI_SIMULATOR || defined(__DOXYGEN__)
	printf("please cancel %s %d %d\r\n", output->name, (int)getTimeNowUs(), output->overlappingCounter);
#endif /* EFI_UNIT_TEST || EFI_SIMULATOR */
	} else {
		seScheduleByTime("out up1", sUp, turnOnTime, (schfunc_t) &seTurnPinHigh, pair);
	}
	efitimeus_t turnOffTime = nowUs + (int) (delayUs + durationUs);
	seScheduleByTime("out down", sDown, turnOffTime, (schfunc_t) &seTurnPinLow, pair);
}

static ALWAYS_INLINE void handleFuelInjectionEvent(int injEventIndex, InjectionEvent *event,
		int rpm DECLARE_ENGINE_PARAMETER_S) {

	/**
	 * todo: this is a bit tricky with batched injection. is it? Does the same
	 * wetting coefficient works the same way for any injection mode, or is something
	 * x2 or /2?
	 */
	const floatms_t injectionDuration = ENGINE(wallFuel).adjust(event->outputs[0]->injectorIndex, ENGINE(fuelMs) PASS_ENGINE_PARAMETER);
#if EFI_PRINTF_FUEL_DETAILS || defined(__DOXYGEN__)
	printf("fuel fuelMs=%f adjusted=%f\t\n", ENGINE(fuelMs), injectionDuration);
#endif /*EFI_PRINTF_FUEL_DETAILS */

	/**
	 * todo: pre-calculate 'numberOfInjections'
	 * see also injectorDutyCycle
	 */
	if (!isCrankingR(rpm) && injectionDuration * getNumberOfInjections(engineConfiguration->injectionMode PASS_ENGINE_PARAMETER) > getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER)) {
		warning(CUSTOM_TOO_LONG_FUEL_INJECTION, "Too long fuel injection %fms", injectionDuration);
	} else if (isCrankingR(rpm) && injectionDuration * getNumberOfInjections(engineConfiguration->crankingInjectionMode PASS_ENGINE_PARAMETER) > getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER)) {
		warning(CUSTOM_TOO_LONG_CRANKING_FUEL_INJECTION, "Too long cranking fuel injection %fms", injectionDuration);
	}

	ENGINE(actualLastInjection) = injectionDuration;
	if (cisnan(injectionDuration)) {
		warning(CUSTOM_OBD_NAN_INJECTION, "NaN injection pulse");
		return;
	}
	if (injectionDuration < 0) {
		warning(CUSTOM_OBD_NEG_INJECTION, "Negative injection pulse %f", injectionDuration);
		return;
	}

#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "handleFuel totalPerCycle=%f", totalPerCycle);
	scheduleMsg(logger, "handleFuel engineCycleDuration=%f", engineCycleDuration);
#endif /* FUEL_MATH_EXTREME_LOGGING */

	floatus_t injectionStartDelayUs = ENGINE(rpmCalculator.oneDegreeUs) * event->injectionStart.angleOffset;

#if EFI_DEFAILED_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "handleFuel pin=%s eventIndex %d duration=%fms %d", event->output->name,
			eventIndex,
			injectionDuration,
			getRevolutionCounter());
	scheduleMsg(logger, "handleFuel pin=%s delay=%f %d", event->output->name, injectionStartDelayUs,
			getRevolutionCounter());
#endif /* EFI_DEFAILED_LOGGING */

	OutputSignalPair *pair = &ENGINE(fuelActuators[injEventIndex]);

	if (event->isSimultanious) {
		/**
		 * this is pretty much copy-paste of 'scheduleOutput'
		 * 'scheduleOutput' is currently only used for injection, so maybe it should be
		 * changed into 'scheduleInjection' and unified? todo: think about it.
		 */

		scheduling_s * sUp = &pair->signalTimerUp;
// todo: sequential need this logic as well, just do not forget to clear flag		pair->isScheduled = true;
		scheduling_s * sDown = &pair->signalTimerDown;

		scheduleTask(true, "out up s", sUp, (int) injectionStartDelayUs, (schfunc_t) &startSimultaniousInjection, event);
		scheduleTask(true, "out down", sDown, (int) injectionStartDelayUs + MS2US(injectionDuration),
					(schfunc_t) &endSimultaniousInjection, event);

	} else {
#if EFI_UNIT_TEST || defined(__DOXYGEN__)
		printf("scheduling injection angle=%f/delay=%f injectionDuration=%f\r\n", event->injectionStart.angleOffset, injectionStartDelayUs, injectionDuration);
#endif

		// we are in this branch of code only in case of NOT IM_SIMULTANEOUS injection
		// we are ignoring low RPM in order not to handle "engine was stopped to engine now running" transition
		if (rpm > 2 * engineConfiguration->cranking.rpm) {
			const char *outputName = event->outputs[0]->name;
			if (prevOutputName == outputName && engineConfiguration->injectionMode != IM_SIMULTANEOUS) {
				warning(CUSTOM_OBD_SKIPPED_FUEL, "looks like skipped fuel event %d %s", getRevolutionCounter(), outputName);
			}
			prevOutputName = outputName;
		}

		scheduleFuelInjection(rpm, pair, getTimeNowUs(), injectionStartDelayUs, MS2US(injectionDuration), event PASS_ENGINE_PARAMETER);
	}
}

/**
 *
 * @param	delay	the number of ticks before the output signal
 * 					immediate output if delay is zero
 * @param	dwell	the number of ticks of output duration
 *
 */
static void scheduleOutput2(OutputSignalPair *pair, efitimeus_t nowUs, float delayUs, float durationUs, InjectorOutputPin *output) {
#if EFI_GPIO || defined(__DOXYGEN__)

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	printf("scheduling output %s\r\n", output->name);
#endif /* EFI_UNIT_TEST */

	efitimeus_t turnOnTime = nowUs + (int) delayUs;

	scheduling_s *sUp = &pair->signalTimerUp;
	pair->isScheduled = true;
	scheduling_s *sDown = &pair->signalTimerDown;

	pair->outputs[0] = output;
	seScheduleByTime("out up2", sUp, turnOnTime, (schfunc_t) &seTurnPinHigh, pair);
	efitimeus_t turnOffTime = nowUs + (int) (delayUs + durationUs);

	seScheduleByTime("out down", sDown, turnOffTime, (schfunc_t) &seTurnPinLow, pair);
#endif /* EFI_GPIO */
}

static void fuelClosedLoopCorrection(DECLARE_ENGINE_PARAMETER_F) {
#if ! EFI_UNIT_TEST
	if (ENGINE(rpmCalculator.rpmValue) < CONFIG(fuelClosedLoopRpmThreshold) ||
			ENGINE(sensors.clt) < CONFIG(fuelClosedLoopCltThreshold) ||
			getTPS(PASS_ENGINE_PARAMETER_F) > CONFIG(fuelClosedLoopTpsThreshold) ||
			ENGINE(sensors.currentAfr) < engineConfiguration->fuelClosedLoopAfrLowThreshold ||
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


static ALWAYS_INLINE void handleFuel(const bool limitedFuel, uint32_t trgEventIndex, int rpm DECLARE_ENGINE_PARAMETER_S) {
	efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 128, "lowstck#3");
	efiAssertVoid(trgEventIndex < engine->engineCycleEventCount, "handleFuel/event index");

	if (!isInjectionEnabled(engineConfiguration) || limitedFuel) {
		return;
	}
	if (engine->isCylinderCleanupMode) {
		return;
	}


	/**
	 * Ignition events are defined by addFuelEvents() according to selected
	 * fueling strategy
	 */
	FuelSchedule *fs = &engine->injectionEvents;
	if (!fs->isReady) {
		fs->addFuelEvents(PASS_ENGINE_PARAMETER_F);
	}

#if FUEL_MATH_EXTREME_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "handleFuel ind=%d %d", trgEventIndex, getRevolutionCounter());
#endif /* FUEL_MATH_EXTREME_LOGGING */

	ENGINE(tpsAccelEnrichment.onNewValue(getTPS(PASS_ENGINE_PARAMETER_F) PASS_ENGINE_PARAMETER));
	ENGINE(engineLoadAccelEnrichment.onEngineCycle(PASS_ENGINE_PARAMETER_F));

	ENGINE(fuelMs) = getInjectionDuration(rpm PASS_ENGINE_PARAMETER) * CONFIG(globalFuelCorrection);

	for (int injEventIndex = 0; injEventIndex < CONFIG(specs.cylindersCount); injEventIndex++) {
		InjectionEvent *event = &fs->elements[injEventIndex];
		uint32_t eventIndex = event->injectionStart.eventIndex;
// right after trigger change we are still using old & invalid fuel schedule. good news is we do not change trigger on the fly in real life
//		efiAssertVoid(eventIndex < ENGINE(triggerShape.getLength()), "handleFuel/event sch index");
		if (eventIndex != trgEventIndex) {
			continue;
		}
		handleFuelInjectionEvent(injEventIndex, event, rpm PASS_ENGINE_PARAMETER);
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
void mainTriggerCallback(trigger_event_e ckpSignalType, uint32_t trgEventIndex DECLARE_ENGINE_PARAMETER_S) {
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

	if (trgEventIndex >= engine->engineCycleEventCount) {
		/**
		 * this could happen in case of a trigger error, just exit silently since the trigger error is supposed to be handled already
		 * todo: should this check be somewhere higher so that no trigger listeners are invoked with noise?
		 */
		return;
	}

	int rpm = ENGINE(rpmCalculator.rpmValue);
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
		warning(CUSTOM_OBD_34, "skipping stroke due to rpm=%d", rpm);
	}

#if (EFI_HISTOGRAMS && EFI_PROD_CODE) || defined(__DOXYGEN__)
	int beforeCallback = hal_lld_get_counter_value();
#endif

	if (trgEventIndex == 0) {

		if (triggerVersion.isOld()) {
			engine->ignitionEvents.isReady = false; // we need to rebuild ignition schedule
			engine->injectionEvents.isReady = false;
			// todo: move 'triggerIndexByAngle' change into trigger initialization, why is it invoked from here if it's only about trigger shape & optimization?
			prepareOutputSignals(PASS_ENGINE_PARAMETER_F);
			// we need this to apply new 'triggerIndexByAngle' values
			engine->periodicFastCallback(PASS_ENGINE_PARAMETER_F);
		}

		if (CONFIG(fuelClosedLoopCorrectionEnabled)) {
			fuelClosedLoopCorrection(PASS_ENGINE_PARAMETER_F);
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
	handleFuel(limitedFuel, trgEventIndex, rpm PASS_ENGINE_PARAMETER);
	/**
	 * For spark we schedule both start of coil charge and actual spark based on trigger angle
	 */
	handleSpark(limitedSpark, trgEventIndex, rpm PASS_ENGINE_PARAMETER);
#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
	int diff = hal_lld_get_counter_value() - beforeCallback;
	if (diff > 0)
	hsAdd(&mainLoopHistogram, diff);
#endif /* EFI_HISTOGRAMS */

	if (trgEventIndex == 0) {
		ENGINE(m.mainTriggerCallbackTime) = GET_TIMESTAMP() - ENGINE(m.beforeMainTrigger);
	}
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
	int rpm = engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_F);
	float el = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
	scheduleMsg(logger, "rpm %d engine_load %f", rpm, el);
	scheduleMsg(logger, "fuel %fms timing %f", getInjectionDuration(rpm PASS_ENGINE_PARAMETER), engine->engineState.timingAdvance);
#endif
}

void initMainEventListener(Logging *sharedLogger, Engine *engine) {
	logger = sharedLogger;
	efiAssertVoid(engine!=NULL, "null engine");
	initSparkLogic(logger);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	addConsoleAction("performanceinfo", showTriggerHistogram);
	addConsoleActionP("maininfo", (VoidPtr) showMainInfo, engine);

	printMsg(logger, "initMainLoop: %d", currentTimeMillis());
	if (!isInjectionEnabled(engine->engineConfiguration))
		printMsg(logger, "!!!!!!!!!!!!!!!!!!! injection disabled");
#endif

#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
	initHistogram(&mainLoopHistogram, "main callback");
#endif /* EFI_HISTOGRAMS */

	addTriggerEventListener(mainTriggerCallback, "main loop", engine);
}

#endif /* EFI_ENGINE_CONTROL */
