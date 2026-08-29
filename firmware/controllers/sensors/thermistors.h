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

/**
 * Boards whose thermistor pull-up rail is a battery-TRACKING supply (m74_9
 * VTRK1/2 = VBATT/2.5) return the tracking ratio (0.4) for the sensors wired
 * to it; the resistance math then uses the measured battery voltage instead
 * of a fixed 5.0 V. Return 0 for a fixed 5.0 V supply (the default).
 *
 * Overridden by boards; declared here because both init_thermistors.cpp and
 * the board configuration include this header.
 */
float getThermistorBiasTrackingRatio(const char* msg);

