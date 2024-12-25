/**
 * @file functional_sensor.cpp
 */

#include "pch.h"

#include "functional_sensor.h"

FunctionalSensor::FunctionalSensor(const SensorType type, const efidur_t timeoutPeriod)
	: FunctionalSensorImpl(type, timeoutPeriod) {
}