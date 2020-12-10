#include "global.h"
#include "proxy_sensor.h"
#include "functional_sensor.h"
#include "redundant_sensor.h"
#include "rpm_calculator.h"
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
	scheduleMsg(logger, "Sensor \"%s\": Raw value: %.2f Valid: %s Converted value %.2f", sensorName, m_rawValue, boolToString(valid), value);

	// now print out the underlying function's info
	if (auto func = m_function) {
		func->showInfo(logger, m_rawValue);
	}
}

#if EFI_CAN_SUPPORT
#include "can_sensor.h"

void CanSensorBase::showInfo(Logging* logger, const char* sensorName) const {
	const auto [valid, value] = get();
	scheduleMsg(logger, "CAN Sensor \"%s\": valid: %s value: %.2f", sensorName, boolToString(valid), value);
}
#endif // EFI_CAN_SUPPORT

void RedundantSensor::showInfo(Logging* logger, const char* sensorName) const {
	scheduleMsg(logger, "Sensor \"%s\" is redundant combining \"%s\" and \"%s\"", sensorName, getSensorName(m_first), getSensorName(m_second));
}

void RpmCalculator::showInfo(Logging* logger, const char* /*sensorName*/) const {
	scheduleMsg(logger, "RPM sensor: stopped: %d spinning up: %d cranking: %d running: %d rpm: %f", 
		isStopped(),
		isSpinningUp(),
		isCranking(),
		isRunning(),
		get().value_or(0)
	);
}

void LinearFunc::showInfo(Logging* logger, float testRawValue) const {
	scheduleMsg(logger, "    Linear function slope: %.2f offset: %.2f min: %.1f max: %.1f", m_a, m_b, m_minOutput, m_maxOutput);
	const auto [valid, value] = convert(testRawValue);
	scheduleMsg(logger, "      raw value %.2f converts to %.2f valid: %s", testRawValue, value, boolToString(valid));
}

void ResistanceFunc::showInfo(Logging* logger, float testInputValue) const {
	const auto result = convert(testInputValue);
	scheduleMsg(logger, "    %.2f volts -> %.1f ohms with supply voltage %.2f and pullup %.1f.", testInputValue, result.Value, m_supplyVoltage, m_pullupResistor);
}

void ThermistorFunc::showInfo(Logging* logger, float testInputValue) const {
	const auto [valid, value] = convert(testInputValue);
	scheduleMsg(logger, "    %.1f ohms -> valid: %s. %.1f deg C", testInputValue, boolToString(valid), value);
}
