/**
 * @file	engine_math.cpp
 * @brief
 *
 * @date Jul 13, 2013
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

#include "event_registry.h"
#include "fuel_math.h"
#include "advance_map.h"
#include "gppwm_channel_reader.h"

#if EFI_UNIT_TEST
extern bool verboseMode;
#endif /* EFI_UNIT_TEST */

floatms_t getEngineCycleDuration(float rpm) {
	return getCrankshaftRevolutionTimeMs(rpm) * (getEngineRotationState()->getOperationMode() == TWO_STROKE ? 1 : 2);
}

/**
 * @return number of milliseconds in one crank shaft revolution
 */
floatms_t getCrankshaftRevolutionTimeMs(float rpm) {
	if (rpm == 0) {
		return NAN;
	}
	return 360 * getOneDegreeTimeMs(rpm);
}

float getFuelingLoad() {
	return getEngineState()->fuelingLoad;
}

float getIgnitionLoad() {
	return getEngineState()->ignitionLoad;
}

/**
 * see also setConstantDwell
 */
void setSingleCoilDwell() {
	for (int i = 0; i < DWELL_CURVE_SIZE; i++) {
		config->sparkDwellRpmBins[i] = (i + 1) * 50;
		config->sparkDwellValues[i] = 4;
	}

	config->sparkDwellRpmBins[5] = 500;
	config->sparkDwellValues[5] = 4;

	config->sparkDwellRpmBins[6] = 4500;
	config->sparkDwellValues[6] = 4;

	config->sparkDwellRpmBins[7] = 12500;
	config->sparkDwellValues[7] = 0;
}

/**
 * @return IM_WASTED_SPARK if in SPINNING mode and IM_INDIVIDUAL_COILS setting
 * @return engineConfiguration->ignitionMode otherwise
 */
ignition_mode_e getCurrentIgnitionMode() {
	ignition_mode_e ignitionMode = engineConfiguration->ignitionMode;
#if EFI_SHAFT_POSITION_INPUT
	// In spin-up cranking mode we don't have full phase sync info yet, so wasted spark mode is better
	if (ignitionMode == IM_INDIVIDUAL_COILS) {
		bool missingPhaseInfoForSequential =
			!engine->triggerCentral.triggerState.hasSynchronizedPhase();

		if (engine->rpmCalculator.isSpinningUp() || missingPhaseInfoForSequential) {
			ignitionMode = IM_WASTED_SPARK;
		}
	}
#endif /* EFI_SHAFT_POSITION_INPUT */
	return ignitionMode;
}

#if EFI_ENGINE_CONTROL

/**
 * This heavy method is only invoked in case of a configuration change or initialization.
 */
void prepareOutputSignals() {
	auto operationMode = getEngineRotationState()->getOperationMode();
	getEngineState()->engineCycle = getEngineCycle(operationMode);

	bool isOddFire = false;
	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		if (engineConfiguration->timing_offset_cylinder[i] != 0) {
			isOddFire = true;
			break;
		}
	}

	EngineCylinders::updateCylinders();

	// Use odd fire wasted spark logic if not two stroke, and an odd fire or odd cylinder # engine
	getEngineState()->useOddFireWastedSpark = operationMode != TWO_STROKE
								&& (isOddFire | (engineConfiguration->cylindersCount % 2 == 1));

#if EFI_UNIT_TEST
	if (verboseMode) {
		printf("prepareOutputSignals %d %s\r\n", engineConfiguration->trigger.type, getIgnition_mode_e(engineConfiguration->ignitionMode));
	}
#endif /* EFI_UNIT_TEST */

#if EFI_SHAFT_POSITION_INPUT
	engine->triggerCentral.prepareTriggerShape();
#endif // EFI_SHAFT_POSITION_INPUT

	// Fuel schedule may now be completely wrong, force a reset
	engine->injectionEvents.invalidate();
}

angle_t getPerCylinderFiringOrderOffset(uint8_t cylinderIndex, uint8_t cylinderNumber) {
	// base = position of this cylinder in the firing order.
	// We get a cylinder every n-th of an engine cycle where N is the number of cylinders
	auto firingOrderOffset = engine->engineState.engineCycle * cylinderIndex / engineConfiguration->cylindersCount;

	// Plus or minus any adjustment if this is an odd-fire engine
	auto adjustment = engineConfiguration->timing_offset_cylinder[cylinderNumber];

	auto result = firingOrderOffset + adjustment;

	assertAngleRange(result, "getCylinderAngle", ObdCode::CUSTOM_ERR_CYL_ANGLE);

	return result;
}

void setTimingRpmBin(float from, float to) {
	setRpmBin(config->ignitionRpmBins, IGN_RPM_COUNT, from, to);
}

/**
 * this method sets algorithm and ignition table scale
 */
void setAlgorithm(engine_load_mode_e algo) {
	engineConfiguration->fuelAlgorithm = algo;
}

void setFlatInjectorLag(float value) {
	setTable(engineConfiguration->injector.battLagCorrTable, value);
}

BlendResult calculateBlend(blend_table_s& cfg, float rpm, float load) {
	// If set to 0, skip the math as its disabled
	if (cfg.blendParameter == GPPWM_Zero) {
		return { 0, 0, 0, 0 };
	}

	auto value = readGppwmChannel(cfg.blendParameter);

	if (!value) {
		return { 0, 0, 0, 0 };
	}

	// Override Y axis value (if necessary)
	if (cfg.yAxisOverride != GPPWM_Zero) {
		// TODO: is this value_or(0) correct or even reasonable?
		load = readGppwmChannel(cfg.yAxisOverride).value_or(0);
	}

	float tableValue = interpolate3d(
		cfg.table,
		cfg.loadBins, load,
		cfg.rpmBins, rpm
	);

	float blendFactor = interpolate2d(value.Value, cfg.blendBins, cfg.blendValues);

	return { value.Value, blendFactor, 0.01f * blendFactor * tableValue, load };
}

// Add torque estimation functions
float getAirmassFromMap(float map_kpa, int rpm, float intake_temp_c) {
    // Calculate theoretical air mass per stroke based on MAP
    float displacement_per_stroke = engineConfiguration->displacement / 
                                   engineConfiguration->cylindersCount / 2.0f; // cc per stroke
    
    // Convert to liters
    displacement_per_stroke *= 0.001f;
    
    // Air density calculation (ideal gas law approximation)
    float air_density_kg_m3 = (map_kpa * 1000.0f) / (287.0f * (intake_temp_c + 273.15f));
    
    // Air mass per stroke in mg
    float air_mass_mg = displacement_per_stroke * air_density_kg_m3 * 1000000.0f;
    
    // Apply volumetric efficiency
    float ve = getVeFromMap(rpm, map_kpa);
    
    return air_mass_mg * ve;
}

float calculateTorqueFromAirmass(float airmass_mg_per_stroke, int rpm) {
    // Simplified torque calculation
    // Energy per stroke = Air mass * Fuel energy density * Combustion efficiency
    
    float fuel_energy_mj_per_kg = 44.0f; // Gasoline energy content
    float combustion_efficiency = 0.35f;  // Typical gasoline engine efficiency
    float mechanical_efficiency = 0.85f;  // Mechanical losses
    
    // Convert air mass to fuel energy (assuming stoichiometric mixture)
    float fuel_mass_mg = airmass_mg_per_stroke / 14.7f; // Stoichiometric ratio
    float fuel_mass_kg = fuel_mass_mg * 0.000001f;
    
    // Energy per stroke in Joules
    float energy_per_stroke = fuel_mass_kg * fuel_energy_mj_per_kg * 1000000.0f * 
                             combustion_efficiency * mechanical_efficiency;
    
    // Convert to torque: Torque = Energy / (2 * pi)
    float torque_nm = energy_per_stroke / (2.0f * 3.14159f);
    
    return torque_nm;
}

#endif /* EFI_ENGINE_CONTROL */
