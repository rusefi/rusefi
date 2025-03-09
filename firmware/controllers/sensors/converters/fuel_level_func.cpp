//
// Created by kifir on 12/19/24.
//

#include "pch.h"

#include "fuel_level_func.h"

SensorResult FuelLevelFunc::convert(const float inputValue) {
	if (std::isnan(inputValue)) {
		criticalError("temp error FuelLevelFunc NaN input");
		m_filteredValue.reset();
		return UnexpectedCode::Unknown;
	} else if (inputValue < engineConfiguration->fuelLevelLowThresholdVoltage) {
		m_filteredValue.reset();
		return UnexpectedCode::Low;
	} else if (engineConfiguration->fuelLevelHighThresholdVoltage < inputValue) {
		m_filteredValue.reset();
		return UnexpectedCode::High;
	} else {
		const float filteredValue = filterFuelValue(inputValue);
		const float fuelLevel = interpolate2d(filteredValue, config->fuelLevelBins, config->fuelLevelValues);
		return fuelLevel;
	}
}

float FuelLevelFunc::getFuelLevelAlpha() const {
	const float configuredAlpha = engineConfiguration->fuelLevelAveragingAlpha;
	return (0.0f < configuredAlpha ? configuredAlpha : engine_configuration_defaults::FUEL_LEVEL_AVERAGING_ALPHA);
}

float FuelLevelFunc::filterFuelValue(const float value) {
	if (m_filteredValue.has_value()) {
		if (m_fuelLevelTimer.hasElapsedSec(maxF(
				engineConfiguration->fuelLevelUpdatePeriodSec,
				MIN_FUEL_LEVEL_UPDATE_PERIOD_SEC
		))) {
			const float prevFilteredValue = m_filteredValue.value();
			const float diff = value - prevFilteredValue;
			updateFilteredValue(prevFilteredValue + getFuelLevelAlpha() * diff);
		}
	} else {
		updateFilteredValue(value);
	}
	return m_filteredValue.value();
}

void FuelLevelFunc::updateFilteredValue(const float value) {
	m_filteredValue = value;
	m_fuelLevelTimer.reset();
}
