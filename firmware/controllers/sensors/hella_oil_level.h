#pragma once

#include "stored_value_sensor.h"

class HellaOilLevelSensor : public StoredValueSensor {
public:
	HellaOilLevelSensor(SensorType type) : StoredValueSensor(type, MS2NT(2000)) {}

	void init(brain_pin_e pin);

	void onEdge(efitick_t nowNt);
	void onEdge(efitick_t nowNt, bool value);

private:
	brain_pin_e m_pin = Gpio::Unassigned;

	// Measures the width of positive pulses (rising -> falling)
	Timer m_pulseTimer;

	// Measures the time between pulses (rising -> rising)
	Timer m_betweenPulseTimer;

	enum class NextPulse { None, Temp, Level, Diag };
	NextPulse m_nextPulse = NextPulse::None;
};
