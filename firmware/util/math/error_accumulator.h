#pragma once

class ErrorAccumulator {
public:
	void init(float ignoreError, float dt) {
		m_ignoreError = ignoreError;
		m_dt = dt;
	}

	// Accumulate the current error, returning the total integrated error
	float accumulate(float error);

	// Get the current accumulator value
	float getAccumulator() const {
		return m_errorIntegral;
	}

	// Reset the integrator to 0 error.
	void reset();

private:
	float m_ignoreError = 0;
	float m_errorIntegral = 0;
	float m_dt = 0;
};
