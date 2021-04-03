#include "redundant_ford_tps.h"

#include "efilib.h"

RedundantFordTps::RedundantFordTps(SensorType outputType, SensorType first, SensorType second)
	: Sensor(outputType)
	, m_first(first)
	, m_second(second)
{
}

void RedundantFordTps::configure(float maxDifference) {
	m_maxDifference = maxDifference;
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

	// Case 1: both sensors show low position, average result
	if (tps1.Value < 45 && tps2.Value < 90) {
		return (tps1.Value + tps2.Value / 2) / 2;
	}

	// Case 2: both sensors show high position, return "full scale" sensor
	if (tps1.Value > 45 && tps2.Value > 85) {
		return tps1;
	}

	// Any other condition indicates an mismatch, and therefore an error
	return unexpected;
}
