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

#include "spark_logic.h"

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

	engine->engineModules.apply_all([=](auto & m) {
		m.onEnginePhase(rpm, edgeTimestamp, currentPhase, nextPhase);
	});

	/**
	 * For fuel we schedule start of injection based on trigger angle, and then inject for
	 * specified duration of time
	 */
	handleFuel(edgeTimestamp, currentPhase, nextPhase);

	/**
	 * For spark we schedule both start of coil charge and actual spark based on trigger angle
	 */
	onTriggerEventSparkLogic(rpm, edgeTimestamp, currentPhase, nextPhase);

#if !EFI_UNIT_TEST
#if EFI_MAP_AVERAGING
	mapAveragingTriggerCallback(edgeTimestamp, currentPhase, nextPhase);
#endif /* EFI_MAP_AVERAGING */
#endif /* EFI_UNIT_TEST */
}

#endif /* EFI_ENGINE_CONTROL */
