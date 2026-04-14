#pragma once

#include "sensor_converter_func.h"

/**
 * @brief LinearFunc maps an input value (like raw voltage) to a physical unit using a linear equation.
 *
 * This class implements a sensor converter that uses a linear mapping (y = ax + b).
 * It includes an input divisor for handling different scaling factors and performs output bounds checking.
 * If the calculated result is outside the specified range, it returns appropriate high/low error codes.
 */
class LinearFunc final : public SensorConverter {
public:
	/**
	 * @param divideInput Constant divisor applied to calibration input points during configuration.
	 */
	LinearFunc(float divideInput = 1.0f) : m_divideInput(divideInput) {}

	/**
	 * @brief Configures the linear equation parameters.
	 *
	 * Parameters in1 and in2 are divided by `divideInput` before calculating the linear coefficients.
	 *
	 * @param in1 First calibration input point (e.g., voltage).
	 * @param out1 First calibration output point (e.g., physical value at in1).
	 * @param in2 Second calibration input point.
	 * @param out2 Second calibration output point.
	 * @param minOutput Lower bound for the output value.
	 * @param maxOutput Upper bound for the output value.
	 */
	void configure(float in1, float out1, float in2, float out2, float minOutput, float maxOutput);

	/**
	 * @brief Converts an input value to a physical unit using the linear equation.
	 *
	 * @param inputValue Raw input value to convert.
	 * @return SensorResult containing either the physical value or an error code if out of bounds.
	 */
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
