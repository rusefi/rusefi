#include "global.h"
#include "proxy_sensor.h"
#include "functional_sensor.h"
#include "redundant_sensor.h"
#include "efilib.h"
#include "loggingcentral.h"

void ProxySensor::showInfo(Logging* logger, const char* sensorName) const {
	scheduleMsg(logger, "Sensor \"%s\" proxied from sensor \"%s\"", sensorName, getSensorName(m_proxiedSensor));
}

void FunctionalSensor::showInfo(Logging* logger, const char* sensorName) const {
	const auto [valid, value] = get();
	scheduleMsg(logger, "Sensor \"%s\": Raw value: %.2f Valid: %d Converted value %.2f", sensorName, m_rawValue, valid, value);

	// now print out the underlying function's info
	if (auto func = m_function) {
		func->showInfo(logger, m_rawValue);
	}
}

#if EFI_CAN_SUPPORT
#include "can_sensor.h"

void CanSensorBase::showInfo(Logging* logger, const char* sensorName) const {
	const auto [valid, value] = get();
	scheduleMsg(logger, "CAN Sensor \"%s\": valid: %d value: %.2f", sensorName, valid, value);
}
#endif // EFI_CAN_SUPPORT

void RedundantSensor::showInfo(Logging* logger, const char* sensorName) const {
	scheduleMsg(logger, "Redundant sensor \"%s\" combining \"%s\" and \"%s\"", sensorName, getSensorName(m_first), getSensorName(m_second));
}
