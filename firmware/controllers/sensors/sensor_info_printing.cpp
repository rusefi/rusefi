#include "pch.h"
#include "proxy_sensor.h"
#include "functional_sensor.h"
#include "redundant_sensor.h"
#include "redundant_ford_tps.h"
#include "fallback_sensor.h"
#include "Lps25Sensor.h"
#include "linear_func.h"
#include "resistance_func.h"
#include "thermistor_func.h"

void ProxySensor::showInfo(const char* sensorName) const {
	efiPrintf("Sensor \"%s\" proxied from sensor \"%s\"", sensorName, getSensorName(m_proxiedSensor));
}

void FunctionalSensor::showInfo(const char* sensorName) const {
	const auto [valid, value] = get();
	efiPrintf("Sensor \"%s\": Raw value: %.2f Valid: %s Converted value %.2f", sensorName, m_rawValue, boolToString(valid), value);

	// now print out the underlying function's info
	if (auto func = m_function) {
		func->showInfo(m_rawValue);
	}
}

#if EFI_CAN_SUPPORT
#include "can_sensor.h"

void CanSensorBase::showInfo(const char* sensorName) const {
	const auto [valid, value] = get();
	efiPrintf("CAN Sensor \"%s\": valid: %s value: %.2f", sensorName, boolToString(valid), value);
}
#endif // EFI_CAN_SUPPORT

void RedundantSensor::showInfo(const char* sensorName) const {
	efiPrintf("Sensor \"%s\" is redundant combining \"%s\" and \"%s\"", sensorName, getSensorName(m_first), getSensorName(m_second));
}

void RedundantFordTps::showInfo(const char* sensorName) const {
	efiPrintf("Sensor \"%s\" is Ford-type redundant TPS combining \"%s\" and \"%s\"", sensorName, getSensorName(m_first), getSensorName(m_second));
}

void FallbackSensor::showInfo(const char* sensorName) const {
	efiPrintf("Sensor \"%s\" is fallback sensor with primary \"%s\" and fallback \"%s\"", sensorName, getSensorName(m_primary), getSensorName(m_fallback));
}

void RpmCalculator::showInfo(const char* /*sensorName*/) const {
	efiPrintf("RPM sensor: stopped: %d spinning up: %d cranking: %d running: %d rpm: %f", 
		isStopped(),
		isSpinningUp(),
		isCranking(),
		isRunning(),
		get().value_or(0)
	);
}

void Lps25Sensor::showInfo(const char* sensorName) const {
	efiPrintf("%s: LPS25 baro %.2f kPa", sensorName, get().Value);
}

void LinearFunc::showInfo(float testRawValue) const {
	efiPrintf("    Linear function slope: %.2f offset: %.2f min: %.1f max: %.1f", m_a, m_b, m_minOutput, m_maxOutput);
	const auto [valid, value] = convert(testRawValue);
	efiPrintf("      raw value %.2f converts to %.2f valid: %s", testRawValue, value, boolToString(valid));
}

void ResistanceFunc::showInfo(float testInputValue) const {
	const auto result = convert(testInputValue);
	efiPrintf("    %.2f volts -> %.1f ohms with supply voltage %.2f and pullup %.1f.", testInputValue, result.Value, m_supplyVoltage, m_pullupResistor);
}

void ThermistorFunc::showInfo(float testInputValue) const {
	const auto [valid, value] = convert(testInputValue);
	efiPrintf("    %.1f ohms -> valid: %s. %.1f deg C", testInputValue, boolToString(valid), value);
}
