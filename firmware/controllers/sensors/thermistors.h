/**
 * @file thermistors.h
 *
 * @date Feb 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#define KELV 273.15f

/**
 * converts Kelvin temperature into Celsius temperature
 */
#define convertKelvinToCelcius(tempK) ((tempK) - KELV)
#define convertCelsiusToKelvin(tempC) ((tempC) + KELV)

void setAtSensor(ThermistorConf *thermistorConf,
    float tempLow, float rLow,
    float tempMid, float rMid,
    float tempHigh, float rHigh);

void setCommonNTCSensorParameters(ThermistorConf *thermistorConf);
void setCommonNTCSensor(ThermistorConf *thermistorConf, float pullup);
void setGmCltSensor(ThermistorConf *thermistorConf);
void setDodgeSensor(ThermistorConf *thermistorConf, float pullup);

