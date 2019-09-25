/**
 * @file thermistors.h
 *
 * @date Feb 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef THERMISTORS_H_
#define THERMISTORS_H_

#include "engine_configuration_generated_structures.h"

#define KELV 273.15f

/**
 * converts Kelvin temperature into Celsius temperature
 */
constexpr float convertKelvinToCelcius(float kelvin) { return kelvin - KELV; }
constexpr float convertCelsiusToKelvin(float celsius) { return celsius + KELV; }

temperature_t getCoolantTemperature();
temperature_t getIntakeAirTemperature();
bool hasIatSensor();
bool hasCltSensor();

void setCommonNTCSensor(ThermistorConf *thermistorConf, float pullup);
void setDodgeSensor(ThermistorConf *thermistorConf, float pullup);
void set10K_4050K(ThermistorConf *thermistorConf, float pullup);

#endif /* THERMISTORS_H_ */
