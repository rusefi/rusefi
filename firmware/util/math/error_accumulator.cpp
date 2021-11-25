#include "pch.h"

#include "error_accumulator.h"

float ErrorAccumulator::accumulate(float error) {
	// We only care about the absolute value of the error
	error = absF(error);

	// If m_ignoreError is 5, for example:
	// 0 error		-> bleeds down at 5 per second
	// 5 error		-> integral stays where it is
	// 10 error		-> integral grows at 5 per second
	float accumulationRate = error - m_ignoreError;

	float newIntegral = accumulationRate * m_dt + m_errorIntegral;

	// Don't allow less than 0 error
	if (newIntegral < 0) {
		newIntegral = 0;
	}

	m_errorIntegral = newIntegral;

	return newIntegral;
}

void ErrorAccumulator::reset() {
	m_errorIntegral = 0;
}
