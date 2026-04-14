//
// Created by kifir on 12/20/24.
//

#pragma once

#include "stored_value_sensor.h"

/**
 * @brief FunctionalSensorBase provides the interface for sensors that receive raw inputs.
 *
 * A "raw" value in this context is the input before any conversion function is applied,
 * such as an ADC voltage, frequency, or pulse width.
 */
class FunctionalSensorBase : public StoredValueSensor {
protected:
	FunctionalSensorBase(SensorType type, efidur_t timeoutPeriod);
public:
	/**
	 * @brief Post a new raw value to the sensor to be converted and stored.
	 *
	 * @param inputValue The raw value (e.g., ADC voltage).
	 * @param timestamp The time the reading was taken.
	 */
	virtual void postRawValue(float inputValue, efitick_t timestamp) = 0;
};
