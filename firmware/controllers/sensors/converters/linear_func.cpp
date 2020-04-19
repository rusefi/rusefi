#include "linear_func.h"

#include "interpolation.h"

void LinearFunc::configure(float in1, float out1, float in2, float out2, float minOutput, float maxOutput) {
	m_minOutput = minOutput;
	m_maxOutput = maxOutput;

	in1 = in1 / m_divideInput;
	in2 = in2 / m_divideInput;

	m_a = INTERPOLATION_A(in1, out1, in2, out2);
	m_b = out1 - m_a * in1;
}

SensorResult LinearFunc::convert(float inputValue) const {
	float result = m_a * inputValue + m_b;

	// Bounds check
	bool isValid = result <= m_maxOutput && result >= m_minOutput;

	if (!isValid) {
		return unexpected;
	}

	return result;
}

void LinearFunc::showInfo(Logging* logger, float testRawValue) const {
	scheduleMsg(logger, "    Linear function slope: %.2f offset: %.2f min: %.1f max: %.1f", m_a, m_b, m_minOutput, m_maxOutput);
	const auto [valid, value] = convert(testRawValue);
	scheduleMsg(logger, "      raw value %.2f converts to %.2f valid: %d", testRawValue, value, valid);
}
