#include "linear_func.h"

#include "interpolation.h"

void LinearFunc::configure(float in1, float out1, float in2, float out2, float minOutput, float maxOutput) {
	m_minOutput = minOutput;
	m_maxOutput = maxOutput;

	m_a = INTERPOLATION_A(in1, out1, in2, out2);
	m_b = out1 - m_a * in1;
}

SensorResult LinearFunc::convert(float inputValue) const {
	float result = m_a * inputValue + m_b;

	// Bounds check
	bool isValid = result <= m_maxOutput && result >= m_minOutput;

	return {isValid, result};
}
