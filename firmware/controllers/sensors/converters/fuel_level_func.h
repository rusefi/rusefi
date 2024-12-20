//
// Created by kifir on 12/19/24.
//

#pragma once

#include "sensor_converter_func.h"
#include "engine_configuration_defaults.h"

class FuelLevelFunc final : public SensorConverter {
public:
	SensorResult convert(float inputValue) const override;
private:
	float getFuelLevelAlpha() const;
	float filterFuelValue(float value) const;

	static constexpr float MIN_FUEL_LEVEL_UPDATE_PERIOD_SEC =
		engine_configuration_defaults::FUEL_LEVEL_UPDATE_PERIOD_SEC;

	// TODO: I really dislike the following mutable field, but I failed to find a more pleasant way to make
	//  `convert` method constant :(((
	// TODO2: shall we just not reuse whatever we are reusing here without enjoying it?!
	mutable std::optional<float> m_filteredValue;
	mutable Timer m_fuelLevelTimer;
};
