/**
 * @author Matthew Kennedy, (c) 2019
 */

#include "resistance_func.h"

void ResistanceFunc::configure(float supplyVoltage, float pullupResistor, bool isPulldown) {
	m_pullupResistor = pullupResistor;
	m_supplyVoltage = supplyVoltage;
	m_isPulldown = isPulldown;
}

SensorResult ResistanceFunc::convert(float raw) const {
	// If the voltage is very low, the sensor is a dead short.
	if (raw < 0.05f) {
		return UnexpectedCode::Low;
	}

	// If the voltage is very high (98% VCC), the sensor is open circuit.
	if (raw > (m_supplyVoltage * 0.98f)) {
		return UnexpectedCode::High;
	}

	if (m_isPulldown) {
		// If the sensor is on the high side (fixed resistor is pulldown),
		// invert the voltage so the math comes out correctly
		raw = m_supplyVoltage - raw;
	}

	// Voltage is in a sensible range - convert
	float resistance = m_pullupResistor / (m_supplyVoltage / raw - 1);

	return resistance;
}
