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

#include "sensor_types.h"
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
float getTemperatureC(ThermistorConf *cfg, ThermistorMath *tm, bool useLinear DECLARE_ENGINE_PARAMETER_SUFFIX);
float getCoolantTemperature(DECLARE_ENGINE_PARAMETER_SIGNATURE);
bool isValidCoolantTemperature(float temperature);
float getIntakeAirTemperature(DECLARE_ENGINE_PARAMETER_SIGNATURE);
bool isValidIntakeAirTemperature(float temperature);
bool hasIatSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE);
bool hasCltSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE);

void setThermistorConfiguration(ThermistorConf * tc, float temp1, float r1, float temp2, float r2, float temp3,
		float r3);

void initThermistors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);

void setCommonNTCSensor(ThermistorConf *thermistorConf);
void setDodgeSensor(ThermistorConf *thermistorConf);
void set10K_4050K(ThermistorConf *thermistorConf);

#endif /* THERMISTORS_H_ */
