#include "pch.h"

#include "fuel_computer.h"

mass_t FuelComputerBase::getCycleFuel(mass_t airmass, int rpm, float load) const {
	load = getTargetLambdaLoadAxis(load);
	
	float stoich = getStoichiometricRatio();
	float lambda = getTargetLambda(rpm, load);
	float afr = stoich * lambda;

	ENGINE(engineState.currentAfrLoad) = load;
	ENGINE(engineState.targetLambda) = lambda;
	ENGINE(engineState.targetAFR) = afr;
	ENGINE(engineState.stoichiometricRatio) = stoich;

	return airmass / afr;
}

FuelComputer::FuelComputer(const ValueProvider3D& lambdaTable) : m_lambdaTable(&lambdaTable) {}

float FuelComputer::getStoichiometricRatio() const {
	float primary = CONFIG(stoichRatioPrimary);

	// Config compatibility: this field may be zero on ECUs with old defaults
	if (primary < 5) {
		// 14.7 = E0 gasoline AFR
		primary = STOICH_RATIO;
	}

	// Without an ethanol/flex sensor, return primary configured stoich ratio
	if (!Sensor::hasSensor(SensorType::FuelEthanolPercent)) {
		return primary;
	}

	float secondary = CONFIG(stoichRatioSecondary);

	// Config compatibility: this field may be zero on ECUs with old defaults
	if (secondary < 5) {
		// 9.0 = E100 ethanol AFR
		secondary = 9.0f;
	}

	auto flex = Sensor::get(SensorType::FuelEthanolPercent);

	// TODO: what do do if flex sensor fails?

	// Linear interpolate between primary and secondary stoich ratios
	return interpolateClamped(0, primary, 100, secondary, flex.Value);
}

float FuelComputer::getTargetLambda(int rpm, float load) const {
	efiAssert(OBD_PCM_Processor_Fault, m_lambdaTable != nullptr, "AFR table null", 0);

	return m_lambdaTable->getValue(rpm, load);
}

float FuelComputer::getTargetLambdaLoadAxis(float defaultLoad) const {
	return getLoadOverride(defaultLoad, CONFIG(afrOverrideMode) PASS_ENGINE_PARAMETER_SUFFIX);
}

float getLoadOverride(float defaultLoad, afr_override_e overrideMode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch(overrideMode) {
		case AFR_None: return defaultLoad;
		// MAP default to 200kpa - failed MAP goes rich
		case AFR_MAP: return Sensor::get(SensorType::Map).value_or(200);
		// TPS/pedal default to 100% - failed TPS goes rich
		case AFR_Tps: return Sensor::get(SensorType::Tps1).value_or(100);
		case AFR_AccPedal: return Sensor::get(SensorType::AcceleratorPedal).value_or(100);
		case AFR_CylFilling: return 100 * ENGINE(engineState.sd.airMassInOneCylinder) / ENGINE(standardAirCharge);
		default: return 0;
	}
}
