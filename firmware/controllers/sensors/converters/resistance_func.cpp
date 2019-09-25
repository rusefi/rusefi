#include "resistance_func.h"

void ResistanceFunc::configure(float supplyVoltage, float pullupResistor) {
	m_pullupResistor = pullupResistor;
	m_supplyVoltage = supplyVoltage;
}

SensorResult ResistanceFunc::convert(float raw) const {
	// If the voltage is very low, the sensor is a dead short.
	if (raw < 0.1) {
		return {false, 0.0f};
	}
	
	// If the voltage is very high (95% VCC), the sensor is open circuit.
	if (raw > (m_supplyVoltage * 0.95f)) {
		return {false, 1e6};
	}

	// Voltage is in a sensible range - convert
	float resistance = m_pullupResistor / (m_supplyVoltage / raw - 1);

	return {true, resistance};
}
