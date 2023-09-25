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
#include "local_version_holder.h"
#include "event_queue.h"
#include "injector_model.h"
#include "injection_gpio.h"

#if EFI_LAUNCH_CONTROL
#include "launch_control.h"
#endif // EFI_LAUNCH_CONTROL

#include "backup_ram.h"

void endSimultaneousInjection(InjectionEvent *event) {
	event->isScheduled = false;
	endSimultaneousInjectionOnlyTogglePins();
	getFuelSchedule()->addFuelEventsForCylinder(event->ownIndex);
}

void turnInjectionPinLow(InjectionEvent *event) {
	efitick_t nowNt = getTimeNowNt();

	event->isScheduled = false;
	for (int i = 0;i<MAX_WIRES_COUNT;i++) {
		InjectorOutputPin *output = event->outputs[i];
		if (output) {
			output->close(nowNt);
		}
	}
	getFuelSchedule()->addFuelEventsForCylinder(event->ownIndex);
}

void InjectionEvent::onTriggerTooth(int rpm, efitick_t nowNt, float currentPhase, float nextPhase) {
	auto eventAngle = injectionStartAngle;

	// Determine whether our angle is going to happen before (or near) the next tooth
	if (!isPhaseInRange(eventAngle, currentPhase, nextPhase)) {
		return;
	}

	// Select fuel mass from the correct cylinder
	auto injectionMassGrams = getEngineState()->injectionMass[this->cylinderNumber];

	// Perform wall wetting adjustment on fuel mass, not duration, so that
	// it's correct during fuel pressure (injector flow) or battery voltage (deadtime) transients
	injectionMassGrams = wallFuel.adjust(injectionMassGrams);
	const floatms_t injectionDuration = engine->module<InjectorModel>()->getInjectionDuration(injectionMassGrams);

#if EFI_PRINTF_FUEL_DETAILS
	if (printFuelDebug) {
		printf("fuel injectionDuration=%.2fms adjusted=%.2fms\n",
				getEngineState()->injectionDuration,
		  injectionDuration);
	}
#endif /*EFI_PRINTF_FUEL_DETAILS */

	bool isCranking = getEngineRotationState()->isCranking();
	/**
	 * todo: pre-calculate 'numberOfInjections'
	 * see also injectorDutyCycle
	 */
	int numberOfInjections = isCranking ? getNumberOfInjections(engineConfiguration->crankingInjectionMode) : getNumberOfInjections(engineConfiguration->injectionMode);
	if (injectionDuration * numberOfInjections > getEngineCycleDuration(rpm)) {
		warning(ObdCode::CUSTOM_TOO_LONG_FUEL_INJECTION, "Too long fuel injection %.2fms", injectionDuration);
	}

	engine->module<TripOdometer>()->consumeFuel(injectionMassGrams * numberOfInjections, nowNt);

	if (this->cylinderNumber == 0) {
		engine->outputChannels.actualLastInjection = injectionDuration;
	}

	if (cisnan(injectionDuration)) {
		warning(ObdCode::CUSTOM_OBD_NAN_INJECTION, "NaN injection pulse");
		return;
	}
	if (injectionDuration < 0) {
		warning(ObdCode::CUSTOM_OBD_NEG_INJECTION, "Negative injection pulse %.2f", injectionDuration);
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
			warning(ObdCode::CUSTOM_OBD_SKIPPED_FUEL, "looks like skipped fuel event revCounter=%d %s", getRevolutionCounter(), outputName);
		}
		engine->prevOutputName = outputName;
	}
*/

#if EFI_PRINTF_FUEL_DETAILS
	if (printFuelDebug) {
		InjectorOutputPin *output = outputs[0];
		printf("handleFuelInjectionEvent fuelout %s injection_duration %dus engineCycleDuration=%.1fms\t\n", output->name, (int)durationUs,
				(int)MS2US(getCrankshaftRevolutionTimeMs(Sensor::getOrZero(SensorType::Rpm))) / 1000.0);
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
	if (isSimultaneous) {
		startAction = startSimultaneousInjection;
		endAction = { &endSimultaneousInjection, this };
	} else {
		// sequential or batch
		startAction = { &turnInjectionPinHigh, this };
		endAction = { &turnInjectionPinLow, this };
	}

	float angleFromNow = eventAngle - currentPhase;
	if (angleFromNow < 0) {
		angleFromNow += getEngineState()->engineCycle;
	}

	efitick_t startTime = scheduleByAngle(nullptr, nowNt, angleFromNow, startAction);
	efitick_t turnOffTime = startTime + US2NT((int)durationUs);
	getExecutorInterface()->scheduleByTimestampNt("inj", nullptr, turnOffTime, endAction);

#if EFI_UNIT_TEST
		printf("scheduling injection angle=%.2f/delay=%.2f injectionDuration=%.2f\r\n", angleFromNow, NT2US(startTime - nowNt), injectionDuration);
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

static void handleFuel(int rpm, efitick_t nowNt, float currentPhase, float nextPhase) {
	ScopePerf perf(PE::HandleFuel);
	
	efiAssertVoid(ObdCode::CUSTOM_STACK_6627, getCurrentRemainingStack() > 128, "lowstck#3");

	LimpState limitedFuelState = getLimpManager()->allowInjection();

	// todo: eliminate state copy logic by giving limpManager it's owm limp_manager.txt and leveraging LiveData
	engine->outputChannels.fuelCutReason = (int8_t)limitedFuelState.reason;
	bool limitedFuel = !limitedFuelState.value;
	if (limitedFuel) {
		return;
	}

	/**
	 * Injection events are defined by addFuelEvents() according to selected
	 * fueling strategy
	 */
	FuelSchedule *fs = getFuelSchedule();
	if (!fs->isReady) {
		fs->addFuelEvents();
	}

#if FUEL_MATH_EXTREME_LOGGING
	if (printFuelDebug) {
		efiPrintf("handleFuel [%.1f, %.1f) %d", currentPhase, nextPhase, getRevolutionCounter());
	}
#endif /* FUEL_MATH_EXTREME_LOGGING */

	fs->onTriggerTooth(rpm, nowNt, currentPhase, nextPhase);
}

/**
 * This is the main trigger event handler.
 * Both injection and ignition are controlled from this method.
 */
void mainTriggerCallback(uint32_t trgEventIndex, efitick_t edgeTimestamp, angle_t currentPhase, angle_t nextPhase) {
	ScopePerf perf(PE::MainTriggerCallback);

	if (hasFirmwareError()) {
		/**
		 * In case on a major error we should not process any more events.
		 * TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		 */
		return;
	}

	int rpm = engine->rpmCalculator.getCachedRpm();
	if (rpm == 0) {
		// this happens while we just start cranking

		// todo: check for 'trigger->is_synchnonized?'
		// TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		return;
	}
	if (rpm == NOISY_RPM) {
		warning(ObdCode::OBD_Crankshaft_Position_Sensor_A_Circuit_Malfunction, "noisy trigger");
		// TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		return;
	}

	
	if (trgEventIndex == 0) {

		if (getTriggerCentral()->checkIfTriggerConfigChanged()) {
			getIgnitionEvents()->isReady = false; // we need to rebuild complete ignition schedule
			getFuelSchedule()->isReady = false;
			// moved 'triggerIndexByAngle' into trigger initialization (why was it invoked from here if it's only about trigger shape & optimization?)
			// see updateTriggerWaveform() -> prepareOutputSignals()

			// we need this to apply new 'triggerIndexByAngle' values
			engine->periodicFastCallback();
		}
	}

	/**
	 * For fuel we schedule start of injection based on trigger angle, and then inject for
	 * specified duration of time
	 */
	handleFuel(rpm, edgeTimestamp, currentPhase, nextPhase);

	engine->module<TriggerScheduler>()->scheduleEventsUntilNextTriggerTooth(
		rpm, edgeTimestamp, currentPhase, nextPhase);

	/**
	 * For spark we schedule both start of coil charge and actual spark based on trigger angle
	 */
	onTriggerEventSparkLogic(rpm, edgeTimestamp, currentPhase, nextPhase);
}

#endif /* EFI_ENGINE_CONTROL */
