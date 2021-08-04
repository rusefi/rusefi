#include "pch.h"

#include "redundant_ford_tps.h"

RedundantFordTps::RedundantFordTps(SensorType outputType, SensorType first, SensorType second)
	: Sensor(outputType)
	, m_first(first)
	, m_second(second)
{
}

void RedundantFordTps::configure(float maxDifference, float secondaryMaximum) {
	m_maxDifference = maxDifference;
	m_secondaryMaximum = secondaryMaximum;
}

SensorResult RedundantFordTps::get() const {
	// Sensor 1 is "plain linear" and has the full range
	auto tps1 = Sensor::get(m_first);

	// Sensor 2 is compressed in to 0 -> 50%
	auto tps2 = Sensor::get(m_second);

	// If either result is invalid, return invalid.
	if (!tps1 || !tps2) {
		return unexpected;
	}

	// The "actual" position resolved by the second throttle - this tops out at m_secondaryMaximum instead of 100%
	float tps2Actual = tps2.Value * m_secondaryMaximum / 100;

	// Switch modes slightly below the maximum of the secondary, to avoid misbehavior near 100%
	float avgThreshold = m_secondaryMaximum * 0.9f;

	// Case 1: both sensors show low position, average result
	if (tps1.Value < avgThreshold && tps2Actual < avgThreshold) {
		// Check that the resolved positions are close
		float delta = absF(tps1.Value - tps2Actual);
		if (delta > m_maxDifference) {
			return unexpected;
		}

		return (tps1.Value + tps2Actual) / 2;
	}

	// Case 2: both sensors show high position, return "full scale" sensor's position
	if (tps1.Value > avgThreshold && tps2Actual > (avgThreshold - 3)) {
		return tps1;
	}

	// Any other condition indicates an mismatch, and therefore an error
	return unexpected;
}
