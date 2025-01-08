//
// Created by kifir on 12/19/24.
//

#include "pch.h"

#include "fuel_level_func.h"

SensorResult FuelLevelFunc::convert(const float inputValue) {
	if (std::isnan(inputValue)) {
		criticalError("temp error FuelLevelFunc NaN input");
	}
	const float filteredValue = filterFuelValue(inputValue);
	const float fuelLevel = interpolate2d(filteredValue, config->fuelLevelBins, config->fuelLevelValues);
	return fuelLevel;
}

float FuelLevelFunc::getFuelLevelAlpha() const {
	const float configuredAlpha = engineConfiguration->fuelLevelAveragingAlpha;
	return (0.0f < configuredAlpha ? configuredAlpha : engine_configuration_defaults::FUEL_LEVEL_AVERAGING_ALPHA);
}

float FuelLevelFunc::filterFuelValue(const float value) {
	if (m_fuelLevelTimer.hasElapsedSec(maxF(
		engineConfiguration->fuelLevelUpdatePeriodSec,
		MIN_FUEL_LEVEL_UPDATE_PERIOD_SEC
	))) {
		if (m_filteredValue.has_value()) {
			const float prevFilteredValue = m_filteredValue.value();
			const float diff = value - prevFilteredValue;
			m_filteredValue = prevFilteredValue + getFuelLevelAlpha() * diff;
		} else {
			m_filteredValue = value;
		}
		m_fuelLevelTimer.reset();
	}
	return m_filteredValue.value();
}
