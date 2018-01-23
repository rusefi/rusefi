/**
 * @file    allsensors.h
 * @brief
 *
 * This file should have been called 'sensors.h' but there is some conflict
 * with standard win32 header :(
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include "tps.h"
#include "map.h"
#include "maf.h"
#include "ego.h"
#include "voltage.h"
#include "thermistors.h"
#include "oil_pressure.h"
#include "flex_fuel.h"
#include "adc_inputs.h"

#include "analog_input.h"

void initSensors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);

bool getAcToggle(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#endif /*SENSORS_H_*/
