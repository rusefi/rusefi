#include "pch.h"

#include "linear_func.h"

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

	// Bounds checks
	// Flipped error codes in case of m_a < 0 so that they indicate whether the input
	// voltage is high/low, instead of the output high/low
	if (result > m_maxOutput) {
		return m_a > 0 ? UnexpectedCode::High : UnexpectedCode::Low;
	}

	if (result < m_minOutput) {
		return m_a > 0 ? UnexpectedCode::Low : UnexpectedCode::High;
	}

	return result;
}
