//
// Created by kifir on 12/20/24.
//

#include "pch.h"

#include "fuel_level_sensor.h"

FuelLevelSensor::FuelLevelSensor(const SensorType type, const efidur_t timeoutPeriod)
	: FunctionalSensorImpl(type, timeoutPeriod) {
	setFunction(m_FuelLevelFunc);
}