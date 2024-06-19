/**
 * @author Matthew Kennedy, (c) 2019
 *
 * A function to convert resistance to thermistor temperature (NTC). Uses the
 * Steinhart-Hart equation to avoid having to compute many logarithms at runtime.
 */

#pragma once

#include "generated_lookup_engine_configuration.h"
#include "sensor_converter_func.h"
#include "resistance_func.h"
#include "func_chain.h"

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

using resist = ResistanceFunc;
using therm = ThermistorFunc;

typedef FuncChain<resist, therm> thermistor_t;
