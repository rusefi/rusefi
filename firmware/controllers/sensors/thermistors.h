/**
 * @file thermistors.h
 *
 * @date Feb 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef THERMISTORS_H_
#define THERMISTORS_H_

#include "main.h"

#define _5_VOLTS 5.0
#define KELV 273.15

#include "sensor_types.h"
#include "engine.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
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

float getKelvinTemperature(float resistance, ThermistorConf *thermistor);
float getResistance(Thermistor *thermistor);
float getTemperatureC(Thermistor *thermistor);
float getCoolantTemperature(Engine * engine);
bool isValidCoolantTemperature(float temperature);
float getIntakeAirTemperature(Engine * engine);
bool isValidIntakeAirTemperature(float temperature);

float convertResistanceToKelvinTemperature(float resistance,
		ThermistorConf *thermistor);
void setThermistorConfiguration(ThermistorConf * tc, float temp1, float r1, float temp2, float r2, float temp3,
		float r3);
void prepareThermistorCurve(ThermistorConf * config);

class Engine;

void initThermistors(Engine *engine);

void setCommonNTCSensor(ThermistorConf *thermistorConf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* THERMISTORS_H_ */
