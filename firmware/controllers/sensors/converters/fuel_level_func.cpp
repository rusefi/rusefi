// file fuel_level_func.cpp
//
// Created by kifir on 12/19/24.
//

#include "pch.h"

#include "fuel_level_func.h"

#if EFI_PROD_CODE && HW_HELLEN
#include "hellen_meta.h"
extern Timer hellenEnPinStateChange;
#endif // HW_HELLEN

SensorResult FuelLevelFunc::convert(const float inputVoltage) {
	if (std::isnan(inputVoltage)) {
		criticalError("temp error FuelLevelFunc NaN input");
		m_filteredValue.reset();
		return UnexpectedCode::Unknown;
#if EFI_PROD_CODE && HW_HELLEN
  } else if (isBoardWithPowerManagement() && !hellenEnPinStateChange.hasElapsedMs(200)) {
    // todo: can we accomplish same reset by AdcSubscription::ResetFilters?
		m_filteredValue.reset();
		return UnexpectedCode::Unknown;
#endif // HW_HELLEN
	} else if (inputVoltage < engineConfiguration->fuelLevelLowThresholdVoltage) {
		m_filteredValue.reset();
		return UnexpectedCode::Low;
	} else if (engineConfiguration->fuelLevelHighThresholdVoltage < inputVoltage) {
		m_filteredValue.reset();
		return UnexpectedCode::High;
	} else {
		const float filteredVoltage = filterFuelValue(inputVoltage);
		const float fuelLevel = interpolate2d(filteredVoltage, config->fuelLevelBins, config->fuelLevelValues);
		return fuelLevel;
	}
}

float FuelLevelFunc::getFuelLevelAlpha() const {
	const float configuredAlpha = engineConfiguration->fuelLevelAveragingAlpha;
	return (0.0f < configuredAlpha ? configuredAlpha : engine_configuration_defaults::FUEL_LEVEL_AVERAGING_ALPHA);
}

float FuelLevelFunc::filterFuelValue(const float inputVoltage) {
	if (m_filteredValue.has_value()) {
		if (m_fuelLevelTimer.hasElapsedSec(maxF(
				engineConfiguration->fuelLevelUpdatePeriodSec,
				MIN_FUEL_LEVEL_UPDATE_PERIOD_SEC
		))) {
			const float prevFilteredValue = m_filteredValue.value();
			const float diff = inputVoltage - prevFilteredValue;
			updateFilteredValue(prevFilteredValue + getFuelLevelAlpha() * diff);
		}
	} else {
		updateFilteredValue(inputVoltage);
	}
static bool isFirst = true;
  if (isFirst) {
    isFirst = false;
	  efiPrintf("[temp] first %f", m_filteredValue.value());
  }
	return m_filteredValue.value();
}

void FuelLevelFunc::updateFilteredValue(const float filteredVoltage) {
	m_filteredValue = filteredVoltage;
	m_fuelLevelTimer.reset();
}
