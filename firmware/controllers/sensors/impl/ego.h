/**
 * @file    ego.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "pch.h"
#include "functional_sensor.h"
#include "engine_configuration.h"

float getAfr(SensorType type);
bool hasAfrSensor();
void setEgoSensor(ego_sensor_e type);
float getAfrInterpolation(float lambdaSensorVolts, afr_sensor_s * sensor);
extern StoredValueSensor smoothedLambda1Sensor;
extern StoredValueSensor smoothedLambda2Sensor;
