/**
 * @file vehicle_speed.h
 *
 *@date Dec 26, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef HW_LAYER_VEHICLE_SPEED_H_
#define HW_LAYER_VEHICLE_SPEED_H_

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

#endif /* HW_LAYER_VEHICLE_SPEED_H_ */
