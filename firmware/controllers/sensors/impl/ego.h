/**
 * @file    ego.h
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "stored_value_sensor.h"
#include "engine_configuration.h"
#include "exp_average.h"

float getAfr(SensorType type);
bool hasAfrSensor();
void setEgoSensor(ego_sensor_e type);

extern StoredValueSensor smoothedLambda1Sensor;
extern StoredValueSensor smoothedLambda2Sensor;

extern ExpAverage expAverageLambda1;
extern ExpAverage expAverageLambda2;