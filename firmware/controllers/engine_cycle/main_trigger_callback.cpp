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
	endSimultaneousInjectionOnlyTogglePins();
	event->update();
}

void turnInjectionPinLow(InjectionEvent *event) {
	efitick_t nowNt = getTimeNowNt();

	for (size_t i = 0; i < efi::size(event->outputs); i++) {
		InjectorOutputPin *output = event->outputs[i];
		if (output) {
			output->close(nowNt);
		}
	}
	event->update();
}

void turnInjectionPinLowStage2(InjectionEvent* event) {
	efitick_t nowNt = getTimeNowNt();

	for (size_t i = 0; i < efi::size(event->outputsStage2); i++) {
		InjectorOutputPin *output = event->outputsStage2[i];
		if (output) {
			output->close(nowNt);
		}
	}
}

void InjectionEvent::onTriggerTooth(efitick_t nowNt, float currentPhase, float nextPhase) {
	auto eventAngle = injectionStartAngle;

	// Determine whether our angle is going to happen before (or near) the next tooth
	if (!isPhaseInRange(eventAngle, currentPhase, nextPhase)) {
		return;
	}

	// Select fuel mass from the correct cylinder
	auto injectionMassGrams = getEngineState()->injectionMass[this->cylinderNumber];

	// Perform wall wetting adjustment on fuel mass, not duration, so that
	// it's correct during fuel pressure (injector flow) or battery voltage (deadtime) transients
	// TODO: is it correct to wall wet on both pulses?
	injectionMassGrams = wallFuel.adjust(injectionMassGrams);

	// Disable staging in simultaneous mode
	float stage2Fraction = isSimultaneous ? 0 : getEngineState()->injectionStage2Fraction;

	// Compute fraction of fuel on stage 2, remainder goes on stage 1
	const float injectionMassStage2 = stage2Fraction * injectionMassGrams;
	float injectionMassStage1 = injectionMassGrams - injectionMassStage2;

	{
		// Log this fuel as consumed

		bool isCranking = getEngineRotationState()->isCranking();
		int numberOfInjections = isCranking ? getNumberOfInjections(engineConfiguration->crankingInjectionMode) : getNumberOfInjections(engineConfiguration->injectionMode);

		float actualInjectedMass = numberOfInjections * (injectionMassStage1 + injectionMassStage2);

		engine->module<TripOdometer>()->consumeFuel(actualInjectedMass, nowNt);
	}

	const floatms_t injectionDurationStage1 = engine->module<InjectorModelPrimary>()->getInjectionDuration(injectionMassStage1);
	const floatms_t injectionDurationStage2 = injectionMassStage2 > 0 ? engine->module<InjectorModelSecondary>()->getInjectionDuration(injectionMassStage2) : 0;

#if EFI_PRINTF_FUEL_DETAILS
	if (printFuelDebug) {
		printf("fuel injectionDuration=%.2fms adjusted=%.2fms\n",
				getEngineState()->injectionDuration,
		  injectionDurationStage1);
	}
#endif /*EFI_PRINTF_FUEL_DETAILS */

	if (this->cylinderNumber == 0) {
		engine->outputChannels.actualLastInjection = injectionDurationStage1;
		engine->outputChannels.actualLastInjectionStage2 = injectionDurationStage2;
	}

	if (cisnan(injectionDurationStage1) || cisnan(injectionDurationStage2)) {
		warning(ObdCode::CUSTOM_OBD_NAN_INJECTION, "NaN injection pulse");
		return;
	}
	if (injectionDurationStage1 < 0) {
		warning(ObdCode::CUSTOM_OBD_NEG_INJECTION, "Negative injection pulse %.2f", injectionDurationStage1);
		return;
	}

	// If somebody commanded an impossibly short injection, do nothing.
	// Durations under 50us-ish aren't safe for the scheduler
	// as their order may be swapped, resulting in a stuck open injector
	// see https://github.com/rusefi/rusefi/pull/596 for more details
	if (injectionDurationStage1 < 0.050f)
	{
		return;
	}

	floatus_t durationUsStage1 = MS2US(injectionDurationStage1);
	floatus_t durationUsStage2 = MS2US(injectionDurationStage2);

	// Only bother with the second stage if it's long enough to be relevant
	bool hasStage2Injection = durationUsStage2 > 50;

#if EFI_PRINTF_FUEL_DETAILS
	if (printFuelDebug) {
		InjectorOutputPin *output = outputs[0];
		printf("handleFuelInjectionEvent fuelout %s injection_duration %dus engineCycleDuration=%.1fms\t\n", output->getName(), (int)durationUsStage1,
				(int)MS2US(getCrankshaftRevolutionTimeMs(Sensor::getOrZero(SensorType::Rpm))) / 1000.0);
	}
#endif /*EFI_PRINTF_FUEL_DETAILS */

	action_s startAction, endActionStage1, endActionStage2;
	// We use different callbacks based on whether we're running sequential mode or not - everything else is the same
	if (isSimultaneous) {
		startAction = startSimultaneousInjection;
		endActionStage1 = { &endSimultaneousInjection, this };
	} else {
		uintptr_t startActionPtr = reinterpret_cast<uintptr_t>(this);

		if (hasStage2Injection) {
			// Set the low bit in the arg if there's a secondary injection to start too
			startActionPtr |= 1;
		}

		// sequential or batch
		startAction = { &turnInjectionPinHigh, startActionPtr };
		endActionStage1 = { &turnInjectionPinLow, this };
		endActionStage2 = { &turnInjectionPinLowStage2, this };
	}

	// Correctly wrap injection start angle
	float angleFromNow = eventAngle - currentPhase;
	if (angleFromNow < 0) {
		angleFromNow += getEngineState()->engineCycle;
	}

	// Schedule opening (stage 1 + stage 2 open together)
	efitick_t startTime = scheduleByAngle(nullptr, nowNt, angleFromNow, startAction);

	// Schedule closing stage 1
	efitick_t turnOffTimeStage1 = startTime + US2NT((int)durationUsStage1);
	getExecutorInterface()->scheduleByTimestampNt("inj", nullptr, turnOffTimeStage1, endActionStage1);

	// Schedule closing stage 2 (if applicable)
	if (hasStage2Injection && endActionStage2) {
		efitick_t turnOffTimeStage2 = startTime + US2NT((int)durationUsStage2);
		getExecutorInterface()->scheduleByTimestampNt("inj stage 2", nullptr, turnOffTimeStage2, endActionStage2);
	}

#if EFI_UNIT_TEST
	printf("scheduling injection angle=%.2f/delay=%d injectionDuration=%d %d\r\n", angleFromNow, (int)NT2US(startTime - nowNt), (int)durationUsStage1, (int)durationUsStage2);
#endif
#if EFI_DEFAILED_LOGGING
	efiPrintf("handleFuel pin=%s eventIndex %d duration=%.2fms %d", outputs[0]->name,
			injEventIndex,
			injectionDurationStage1,
			getRevolutionCounter());
	efiPrintf("handleFuel pin=%s delay=%.2f %d", outputs[0]->name, NT2US(startTime - nowNt),
			getRevolutionCounter());
#endif /* EFI_DEFAILED_LOGGING */
}

static void handleFuel(efitick_t nowNt, float currentPhase, float nextPhase) {
	ScopePerf perf(PE::HandleFuel);
	
	efiAssertVoid(ObdCode::CUSTOM_STACK_6627, getCurrentRemainingStack() > 128, "lowstck#3");

	LimpState limitedFuelState = getLimpManager()->allowInjection();

	// todo: eliminate state copy logic by giving limpManager it's owm limp_manager.txt and leveraging LiveData
	engine->outputChannels.fuelCutReason = (int8_t)limitedFuelState.reason;
	bool limitedFuel = !limitedFuelState.value;
	if (limitedFuel) {
		return;
	}

	// This is called in the fast callback already, but since we may have just achieved engine sync (and RPM)
	// for the first time, force update the schedule so that we can inject immediately if necessary
	FuelSchedule *fs = getFuelSchedule();
	if (!fs->isReady) {
		fs->addFuelEvents();
	}

#if FUEL_MATH_EXTREME_LOGGING
	if (printFuelDebug) {
		efiPrintf("handleFuel [%.1f, %.1f) %d", currentPhase, nextPhase, getRevolutionCounter());
	}
#endif /* FUEL_MATH_EXTREME_LOGGING */

	fs->onTriggerTooth(nowNt, currentPhase, nextPhase);
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
		 */
		return;
	}

	int rpm = engine->rpmCalculator.getCachedRpm();
	if (rpm == 0) {
		// this happens while we just start cranking

		// todo: check for 'trigger->is_synchnonized?'
		return;
	}
	if (rpm == NOISY_RPM) {
		warning(ObdCode::OBD_Crankshaft_Position_Sensor_A_Circuit_Malfunction, "noisy trigger");
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
	handleFuel(edgeTimestamp, currentPhase, nextPhase);

	engine->module<TriggerScheduler>()->scheduleEventsUntilNextTriggerTooth(
		rpm, edgeTimestamp, currentPhase, nextPhase);

	/**
	 * For spark we schedule both start of coil charge and actual spark based on trigger angle
	 */
	onTriggerEventSparkLogic(rpm, edgeTimestamp, currentPhase, nextPhase);
}

#endif /* EFI_ENGINE_CONTROL */
