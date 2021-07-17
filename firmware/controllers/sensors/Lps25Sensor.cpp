#include "Lps25Sensor.h"

Lps25Sensor::Lps25Sensor(Lps25& sensor)
	: StoredValueSensor(SensorType::BarometricPressure, MS2NT(1000))
	, m_sensor(&sensor)
{
}

void Lps25Sensor::update() {
	auto result = m_sensor->readPressureKpa();

	if (result) {
		setValidValue(result.Value, getTimeNowNt());
	} else {
		invalidate();
	}
}
