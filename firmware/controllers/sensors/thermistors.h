/**
 * @file thermistors.h
 *
 * @date Feb 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

#define KELV 273.15f

#include "engine.h"

/**
 * Vout=r2/(r1+r2)*Vin
 */
float getR1InVoltageDividor(float Vout, float Vin, float r2);
float getR2InVoltageDividor(float Vout, float Vin, float r1);
float getTempK(float resistance);
/**
 * converts Kelvin temperature into Celsius temperature
 */
#define convertKelvinToCelcius(tempK) ((tempK) - KELV)
#define convertCelsiusToKelvin(tempC) ((tempC) + KELV)

float convertCelciustoF(float tempC);
float convertFtoCelcius(float tempF);

float getKelvinTemperature(float resistance, ThermistorMath *tm);
float getResistance(ThermistorConf *cfg, float voltage);
temperature_t getTemperatureC(ThermistorConf *cfg, ThermistorMath *tm, bool useLinear DECLARE_ENGINE_PARAMETER_SUFFIX);
temperature_t getCoolantTemperatureM(DECLARE_ENGINE_PARAMETER_SIGNATURE);
temperature_t getIntakeAirTemperatureM(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void initThermistors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);

void setCommonNTCSensor(ThermistorConf *thermistorConf, float pullup);
void setDodgeSensor(ThermistorConf *thermistorConf, float pullup);
void set10K_4050K(ThermistorConf *thermistorConf, float pullup);

