//
// Created by kifir on 12/20/24.
//

#include "pch.h"

#include "functional_sensor_base.h"

FunctionalSensorBase::FunctionalSensorBase(const SensorType type, const efidur_t timeoutPeriod)
	: StoredValueSensor(type, timeoutPeriod) {
}
