//
// Created by kifir on 12/20/24.
//

#pragma once

#include "functional_sensor_impl.h"
#include "fuel_level_func.h"

class FuelLevelSensor : public FunctionalSensorImpl<FuelLevelFunc> {
public:
	FuelLevelSensor(SensorType type, efidur_t timeoutPeriod);

	void showInfo(const char* sensorName) const override;
private:
	FuelLevelFunc m_FuelLevelFunc;
};