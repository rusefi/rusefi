/*
 * @file accelerometer.h
 *
 * @date May 19, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"

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

