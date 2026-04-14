/**
 * @file    converter_sensor.h
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#pragma once

#include "converters/sensor_converter_func.h"
#include "functional_sensor_impl.h"

/**
 * @brief Concrete sensor class that converts raw inputs into physical readings.
 *
 * This class uses a `SensorConverter` to map raw values (e.g., voltage from ADC)
 * to physical measurements (e.g., pressure, percentage, temperature).
 *
 * Use `postRawValue()` to update the sensor with a raw reading.
 * Use `get()` (inherited from `StoredValueSensor`) to retrieve the converted result.
 * Use `getRaw()` (inherited from `FunctionalSensorImpl`) to retrieve the last raw value.
 *
 * To use this class, provide a converter that inherits from `SensorConverter`.
 */
class FunctionalSensor : public FunctionalSensorImpl<SensorConverter> {
public:
	FunctionalSensor(SensorType type, efidur_t timeoutPeriod);

	void showInfo(const char* sensorName) const override;
};
