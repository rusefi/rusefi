/**
 * @file thermistors.h
 *
 * @date Feb 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef THERMISTORS_H_
#define THERMISTORS_H_

#include "global.h"

#define KELV 273.15f

#include "engine.h"

/**
 * converts Kelvin temperature into Celsius temperature
 */
#define convertKelvinToCelcius(tempK) ((tempK) - KELV)
#define convertCelsiusToKelvin(tempC) ((tempC) + KELV)

temperature_t getCoolantTemperature(DECLARE_ENGINE_PARAMETER_SIGNATURE);
temperature_t getIntakeAirTemperature(DECLARE_ENGINE_PARAMETER_SIGNATURE);
bool hasIatSensor();
bool hasCltSensor();

void setCommonNTCSensor(ThermistorConf *thermistorConf);
void setDodgeSensor(ThermistorConf *thermistorConf);
void set10K_4050K(ThermistorConf *thermistorConf);

#endif /* THERMISTORS_H_ */
