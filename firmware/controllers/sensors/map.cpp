#include "main.h"
#include "boards.h"
#include "engine_configuration.h"
#include "engine_math.h"
#include "adc_inputs.h"
#include "interpolation.h"
#include "error_handling.h"
#include "map.h"

extern engine_configuration_s * engineConfiguration;

/**
 * @brief	MAP value decoded for a 1.83 Honda sensor
 * -6.64kPa at zero volts
 * 182.78kPa at 5 volts
 *
 * @returns kPa value
 */
static FastInterpolation denso183(0, -6.64, 5, 182.78);

// todo: figure out real values
static FastInterpolation honda3bar(0.32, -95.8371264, 4.84, 300);

static FastInterpolation mpx4250(0, 8, 5, 260);

float decodePressure(float voltage, air_pressure_sensor_config_s * config) {
	switch (config->sensorType) {
	case MT_CUSTOM:
		// todo: introduce 'FastInterpolation customMap'
		return interpolate(0, config->Min, 5, config->Max, voltage);
	case MT_DENSO183:
		return denso183.getValue(voltage);
	case MT_MPX4250:
		return mpx4250.getValue(voltage);
	case MT_HONDA3BAR:
		return honda3bar.getValue(voltage);
	default:
		firmwareError("Unknown MAP type: %d", config->sensorType);
		return NAN;
	}
}

/**
 * @brief	MAP value decoded according to current settings
 * @returns kPa value
 */
float getMapByVoltage(float voltage) {
	air_pressure_sensor_config_s * config = &engineConfiguration->map.sensor;
	return decodePressure(voltage, config);
}

/**
 * @return Manifold Absolute Pressure, in kPa
 */
float getRawMap(void) {
	float voltage = getVoltageDivided(engineConfiguration->map.sensor.hwChannel);
	return getMapByVoltage(voltage);
}

float getBaroPressure(void) {
	float voltage = getVoltageDivided(engineConfiguration->baroSensor.hwChannel);
	return decodePressure(voltage, &engineConfiguration->baroSensor);
}
