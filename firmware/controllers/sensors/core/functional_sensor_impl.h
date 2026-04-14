//
// Created by kifir on 12/20/24.
//

#pragma once

#include "functional_sensor_base.h"

/**
 * @brief Template class for sensors that use a converter function.
 *
 * This class handles the storage of the raw value and the execution of the conversion
 * function (e.g., `LinearFunc`, `TableFunc`) to produce the physical sensor reading.
 *
 * @tparam ConverterType The type of the conversion function class.
 */
template<class ConverterType>
class FunctionalSensorImpl : public FunctionalSensorBase {
public:
	FunctionalSensorImpl(SensorType type, efidur_t timeoutPeriod);

	/**
	 * @brief Receives a raw value, applies the converter, and stores the result.
	 *
	 * @param inputValue The raw value (e.g., ADC voltage).
	 * @param timestamp The time the reading was taken.
	 */
	void postRawValue(float inputValue, efitick_t timestamp) override;

	/**
	 * @brief Set the conversion function.
	 *
	 * @param func The conversion function to use.
	 */
	void setFunction(ConverterType& func);

	/**
	 * @return A pointer to the current conversion function.
	 */
	ConverterType* getFunction() const;

	/**
	 * @brief Retrieves the last raw value posted to the sensor.
	 *
	 * Note: `get()` returns the converted physical value.
	 *
	 * @return The raw input value.
	 */
	float getRaw() const override final;
private:
	// Conversion function for this sensor
	ConverterType* m_function = nullptr;

	// The raw value before conversion (e.g., ADC voltage)
	float m_rawValue = 0;
};

#include "functional_sensor_impl.hpp"
