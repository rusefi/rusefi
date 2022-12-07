/*
 * @file accelerometer.h
 *
 * @date May 19, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

void configureAccelerometerPins();

void initAccelerometer();

/**
 * Acceleration/braking
 */
float getLongitudinalAcceleration();
/**
 * Turns
 */
float getTransverseAcceleration();

