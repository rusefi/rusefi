/**
 * @author Matthew Kennedy, (c) 2019
 */

#include "resistance_func.h"

void ResistanceFunc::configure(float supplyVoltage, float pullupResistor, bool isPulldown) {
	m_pullupResistor = pullupResistor;
	m_supplyVoltage = supplyVoltage;
	m_isPulldown = isPulldown;
	m_trackingRatio = 0;
}

void ResistanceFunc::configureTrackingBias(float ratio, float pullupResistor, bool isPulldown) {
	// The fixed 5.0V supply stays as the boot-time fallback (the battery
	// sensor is invalid for the first ~100 ms).
	m_pullupResistor = pullupResistor;
	m_supplyVoltage = 5.0f;
	m_isPulldown = isPulldown;
	m_trackingRatio = ratio;
}

SensorResult ResistanceFunc::convert(float raw) const {
	// The bias rail is normally a fixed voltage. Boards with a battery-
	// TRACKING bias (m74_9 VTRK1/2 = VBATT/2.5, battery-proportional by
	// design) configure a ratio > 0: the supply then follows the measured
	// battery voltage. Without this the resistance (and so the temperature)
	// is a function of the battery voltage - at 14.4 V the rail is 5.76 V
	// while the math assumed 5.0, which read +5C as -30C on the car.
	float supply = m_supplyVoltage;
	if (m_trackingRatio > 0) {
		float vbat = Sensor::getOrZero(SensorType::BatteryVoltage);
		if (vbat > 6.0f) {
			supply = vbat * m_trackingRatio;
		}
	}

	// If the voltage is very low, the sensor is a dead short.
	if (raw < 0.05f) {
		m_lastResistance = 0;
		return UnexpectedCode::Low;
	}

	// If the voltage is very high (98% VCC), the sensor is open circuit.
	if (raw > (supply * 0.98f)) {
		m_lastResistance = 0;
		return UnexpectedCode::High;
	}

	if (m_isPulldown) {
		// If the sensor is on the high side (fixed resistor is pulldown),
		// invert the voltage so the math comes out correctly
		raw = supply - raw;
	}

	// Voltage is in a sensible range - convert
	float resistance = m_pullupResistor / (supply / raw - 1);

	// Cache for live monitoring (thermistor calibration gauge)
	m_lastResistance = resistance;

	return resistance;
}
