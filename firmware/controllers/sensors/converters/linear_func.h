#pragma once

#include "sensor_converter_func.h"

class LinearFunc final : public SensorConverter {
public:
	LinearFunc(float divideInput = 1.0f) : m_divideInput(divideInput) {}

	void configure(float in1, float out1, float in2, float out2, float minOutput, float maxOutput);

	SensorResult convert(float inputValue) const override;

	void showInfo(float testRawValue) const override;

	float getDivideInput() const {
		return m_divideInput;
	}

private:
	// Linear equation parameters for equation of form
	// y = ax + b
	float m_a = 1;
	float m_b = 0;

	float m_minOutput = 0;
	float m_maxOutput = 0;

	// Divisor for the input values - some configurations use a ratio'd value for compat
	const float m_divideInput;
};
