/**
 * @file fuel_computer.cpp
 * @brief Fuel computer.
 *
 * Converts the air mass produced by the air-mass model into a required fuel mass
 * using the target lambda/AFR (looked up from the target tables and corrected for
 * stoichiometric ratio of the configured fuel). The result feeds the injector model.
 */

#include "pch.h"

#include "engine_configuration.h"
#include "sensor.h"
#include "error_handling.h"
#include "efi_interpolation.h"
#include "table_helper.h"
#include "fuel_math.h"
#include "fuel_computer.h"

#if EFI_ENGINE_CONTROL

mass_t FuelComputerBase::getCycleFuel(mass_t airmass, float rpm, float load) {
	load = getTargetLambdaLoadAxis(load);

	float stoich = getStoichiometricRatio();
	float lambda = getTargetLambda(rpm, load);
	float afr = stoich * lambda;
	// Time at WOT changes the AFR target, including the published lambda used by STFT.
	float adjustedAfr = afr + engine->module<WotEnrichment>().unmock().getAfrAdder();
	if (std::isfinite(adjustedAfr) && adjustedAfr > 0) {
		afr = adjustedAfr;
		lambda = afr / stoich;
	}

	afrTableYAxis = load;
	targetLambda = lambda;
	targetAFR = afr;
	stoichiometricRatio = stoich;

	return airmass / afr;
}

float FuelComputer::getStoichiometricRatio() const {
	float primary = engineConfiguration->stoichRatioPrimary;


	if (primary < 5) {
   	// Config compatibility: this field may be zero on ECUs with very old defaults
		// [tag:user_facing_ini_labels] Match the fuel setting label in tunerstudio.template.ini.
		criticalError("Please set Stoichiometric ratio");
		return 0;
	}

	// A missing or invalid ethanol reading cannot select the secondary fuel.
	auto flex = Sensor::get(SensorType::FuelEthanolPercent);
	if (!flex) {
		return primary;
	}

	float secondary = engineConfiguration->stoichRatioSecondary;

	if (secondary < 5) {
	  // Config compatibility: this field may be zero on ECUs with very old defaults
		// [tag:user_facing_ini_labels] Match the flex-fuel setting label in tunerstudio.template.ini.
		criticalError("Please set E100 stoichiometric ratio");
		return 0;
	}

	// Linear interpolate between primary and secondary stoich ratios
	return interpolateClamped(0, primary, 100, secondary, flex.Value);
}


float FuelComputer::getTargetLambda(float rpm, float load) const {
	float target = interpolate3d(
		config->lambdaTable,
		config->lambdaLoadBins, load,
		config->lambdaRpmBins, rpm
	);

	// Add any blends if configured
	for (size_t i = 0; i < efi::size(config->targetAfrBlends); i++) {
		auto result = calculateBlend(config->targetAfrBlends[i], rpm, load);

		engine->outputChannels.targetAfrBlendParameter[i] = result.BlendParameter;
		engine->outputChannels.targetAfrBlendBias[i] = result.Bias;
		engine->outputChannels.targetAfrBlendOutput[i] = result.Value;
		engine->outputChannels.targetAfrBlendYAxis[i] = result.TableYAxis;

		target += result.Value;
	}

    return target;
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

#endif // EFI_ENGINE_CONTROL
