//
// Created by kifir on 12/20/24.
//

#pragma once

#include "functional_sensor_base.h"

template<class ConverterType>
class FunctionalSensorImpl : public FunctionalSensorBase {
public:
	FunctionalSensorImpl(SensorType type, efidur_t timeoutPeriod);

	void postRawValue(float inputValue, efitick_t timestamp) override;

	void setFunction(ConverterType& func);

	ConverterType* getFunction() const;

	float getRaw() const override final;
private:
	// Conversion function for this sensor
	ConverterType* m_function = nullptr;

	float m_rawValue = 0;
};

#include "functional_sensor_impl.hpp"