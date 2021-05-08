/**
 * @author Matthew Kennedy, (c) 2019
 * 
 * A function to convert resistance to thermistor temperature (NTC). Uses the
 * Steinhart-Hart equation to avoid having to compute many logarithms at runtime.
 */

#pragma once

#include "engine_configuration_generated_structures.h"
#include "sensor_converter_func.h"

class ThermistorFunc final : public SensorConverter {
public:
	SensorResult convert(float ohms) const override;

	void configure(thermistor_conf_s &cfg);

	void showInfo(float testRawValue) const override;

	// Steinhart-Hart coefficients
	float m_a = 0;
	float m_b = 0;
	float m_c = 0;
};
