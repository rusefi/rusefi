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
#include "filtered_sensor.h"
#include "engine_configuration.h"

float getAfr(SensorType type);
bool hasAfrSensor();
void setEgoSensor(ego_sensor_e type);
void updateSmoothedLambda();

extern FilteredSensor smoothedLambda1Sensor;
extern FilteredSensor smoothedLambda2Sensor;