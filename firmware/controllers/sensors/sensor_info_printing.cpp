#include "global.h"
#include "proxy_sensor.h"
#include "functional_sensor.h"
#include "redundant_sensor.h"
#include "linear_func.h"
#include "resistance_func.h"
#include "thermistor_func.h"
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
	scheduleMsg(logger, "Sensor \"%s\" is redundant combining \"%s\" and \"%s\"", sensorName, getSensorName(m_first), getSensorName(m_second));
}

void LinearFunc::showInfo(Logging* logger, float testRawValue) const {
	scheduleMsg(logger, "    Linear function slope: %.2f offset: %.2f min: %.1f max: %.1f", m_a, m_b, m_minOutput, m_maxOutput);
	const auto [valid, value] = convert(testRawValue);
	scheduleMsg(logger, "      raw value %.2f converts to %.2f valid: %d", testRawValue, value, valid);
}

void ResistanceFunc::showInfo(Logging* logger, float testInputValue) const {
	const auto result = convert(testInputValue);
	scheduleMsg(logger, "    %.2f volts -> %.1f ohms with supply voltage %.2f and pullup %.1f.", testInputValue, result.Value, m_supplyVoltage, m_pullupResistor);
}

void ThermistorFunc::showInfo(Logging* logger, float testInputValue) const {
	const auto [valid, value] = convert(testInputValue);
	scheduleMsg(logger, "    %.1f ohms -> valid: %d. %.1f deg C", testInputValue, valid, value);
}
