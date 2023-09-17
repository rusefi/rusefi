#include "pch.h"

#include "engine_configuration.h"
#include "sensor.h"
#include "error_handling.h"
#include "efi_interpolation.h"
#include "table_helper.h"
#include "fuel_math.h"
#include "fuel_computer.h"

mass_t FuelComputerBase::getCycleFuel(mass_t airmass, int rpm, float load) {
	load = getTargetLambdaLoadAxis(load);
	
	float stoich = getStoichiometricRatio();
	float lambda = getTargetLambda(rpm, load);
	float afr = stoich * lambda;

	afrTableYAxis = load;
	targetLambda = lambda;
	targetAFR = afr;
	stoichiometricRatio = stoich;

	return airmass / afr;
}

float FuelComputer::getStoichiometricRatio() const {
	float primary = engineConfiguration->stoichRatioPrimary;

	// Config compatibility: this field may be zero on ECUs with old defaults
	if (primary < 5) {
		// todo: fatal in July of 2023
		// 14.7 = E0 gasoline AFR
		engineConfiguration->stoichRatioPrimary = primary = STOICH_RATIO;
	}

	// Without an ethanol/flex sensor, return primary configured stoich ratio
	if (!Sensor::hasSensor(SensorType::FuelEthanolPercent)) {
		return primary;
	}

	float secondary = engineConfiguration->stoichRatioSecondary;

	// Config compatibility: this field may be zero on ECUs with old defaults
	if (secondary < 5) {
		// 9.0 = E100 ethanol AFR
		// todo: fatal in July of 2023
		engineConfiguration->stoichRatioSecondary = secondary = 9.0f;
	}

	auto flex = Sensor::get(SensorType::FuelEthanolPercent);

	// TODO: what do do if flex sensor fails?

	// Linear interpolate between primary and secondary stoich ratios
	return interpolateClamped(0, primary, 100, secondary, flex.Value);
}


float FuelComputer::getTargetLambda(int rpm, float load) const {
	return interpolate3d(
		config->lambdaTable,
		config->lambdaLoadBins, load,
		config->lambdaRpmBins, rpm
	);
}

float FuelComputer::getTargetLambdaLoadAxis(float defaultLoad) const {
	return getLoadOverride(defaultLoad, engineConfiguration->afrOverrideMode);
}

float IFuelComputer::getLoadOverride(float defaultLoad, load_override_e overrideMode) const {
	switch(overrideMode) {
		case AFR_None: return defaultLoad;
		// MAP default to 200kpa - failed MAP goes rich
		case AFR_MAP: return Sensor::get(SensorType::Map).value_or(200);
		// TPS/pedal default to 100% - failed TPS goes rich
		case AFR_Tps: return Sensor::get(SensorType::Tps1).value_or(100);
		case AFR_AccPedal: return Sensor::get(SensorType::AcceleratorPedal).value_or(100);
		case AFR_CylFilling: return normalizedCylinderFilling;
		default: return 0;
	}
}
