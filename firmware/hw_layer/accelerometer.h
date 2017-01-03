/*
 * accelerometer.h
 *
 * @date May 19, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef HW_LAYER_ACCELEROMETER_H_
#define HW_LAYER_ACCELEROMETER_H_


void initAccelerometer(DECLARE_ENGINE_PARAMETER_F);
/**
 * Acceleration/braking
 */
float getLongitudinalAcceleration(DECLARE_ENGINE_PARAMETER_F);
/**
 * Turns
 */
float getTransverseAcceleration(DECLARE_ENGINE_PARAMETER_F);

#endif /* HW_LAYER_ACCELEROMETER_H_ */
