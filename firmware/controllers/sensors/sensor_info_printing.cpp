#include "proxy_sensor.h"
#include "functional_sensor.h"
#include "efilib.h"
#include "loggingcentral.h"

void ProxySensor::showInfo(Logging* logger, const char* sensorName) const {
	scheduleMsg(logger, "Sensor \"%s\" proxied from sensor \"%s\"", sensorName, getSensorName(m_proxiedSensor));
}

void FunctionalSensor::showInfo(Logging* logger, const char* sensorName) const {
	const auto [valid, value] = get();
	scheduleMsg(logger, "Sensor \"%s\": Raw value: %f Valid: %d Converted value %f", sensorName, m_rawValue, valid, value);
}
