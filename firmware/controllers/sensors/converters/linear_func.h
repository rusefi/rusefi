#pragma once

#include "sensor_converter_func.h"

class LinearFunc final : public SensorConverter {
public:
	LinearFunc() = default;

	void configure(float in1, float out1, float in2, float out2, float minOutput, float maxOutput);

	SensorResult convert(float inputValue) const override;

private:
	// Linear equation parameters for equation of form
	// y = ax + b
	float m_a = 1;
	float m_b = 0;

	float m_minOutput = 0;
	float m_maxOutput = 0;
};
