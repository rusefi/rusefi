//
// Created by kifir on 12/19/24.
//

#pragma once

#include "engine_configuration_defaults.h"

class FuelLevelFunc {
public:
    FuelLevelFunc() : m_filteredValue() {
    }
	SensorResult convert(float inputValue);
private:
	float getFuelLevelAlpha() const;
	float filterFuelValue(float value);
	void updateFilteredValue(float value);

	static constexpr float MIN_FUEL_LEVEL_UPDATE_PERIOD_SEC =
		engine_configuration_defaults::FUEL_LEVEL_UPDATE_PERIOD_SEC;

	std::optional<float> m_filteredValue;
	Timer m_fuelLevelTimer;
};
