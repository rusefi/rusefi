/**
 * @file vehicle_speed.h
 *
 *@date Dec 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

/**
 * @return vehicle speed, in kilometers per hour
 */
float getVehicleSpeed(void);
void initVehicleSpeed(Logging *logger);
void setMockVehicleSpeed(float speedKPH);
bool hasVehicleSpeedSensor();
void stopVSSPins(void);
void startVSSPins(void);
