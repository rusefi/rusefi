/*
 * @file accelerometer.h
 *
 * @date May 19, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef HW_LAYER_ACCELEROMETER_H_
#define HW_LAYER_ACCELEROMETER_H_

#include "global.h"
#include "engine.h"

void configureAccelerometerPins(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void initAccelerometer(DECLARE_ENGINE_PARAMETER_SIGNATURE);

/**
 * Acceleration/braking
 */
float getLongitudinalAcceleration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
/**
 * Turns
 */
float getTransverseAcceleration(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#endif /* HW_LAYER_ACCELEROMETER_H_ */
