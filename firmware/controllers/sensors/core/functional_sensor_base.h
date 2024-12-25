//
// Created by kifir on 12/20/24.
//

#pragma once

#include "stored_value_sensor.h"

class FunctionalSensorBase : public StoredValueSensor {
protected:
	FunctionalSensorBase(SensorType type, efidur_t timeoutPeriod);
public:
	virtual void postRawValue(float inputValue, efitick_t timestamp) = 0;
};