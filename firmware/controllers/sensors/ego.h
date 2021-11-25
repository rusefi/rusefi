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
#include "engine_configuration.h"

float getAfr();
bool hasAfrSensor();
void setEgoSensor(ego_sensor_e type);
void initEgoAveraging();
