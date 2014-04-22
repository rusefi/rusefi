/**
 * @file thermistors.c
 *
 * @date Feb 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

/**
 * http://en.wikipedia.org/wiki/Thermistor
 * http://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
 */

#include "main.h"
#include "thermistors.h"
#include "adc_inputs.h"
#include "engine_configuration.h"
#include "engine_math.h"

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

static bool_t initialized = FALSE;

/**
 * http://en.wikipedia.org/wiki/Voltage_divider
 */
float getR1InVoltageDividor(float Vout, float Vin, float r2) {
	return r2 * Vin / Vout - r2;
}

float getR2InVoltageDividor(float Vout, float Vin, float r1) {
	if (Vout == 0)
		return NAN;
	return r1 / (Vin / Vout - 1);
}

float getVoutInVoltageDividor(float Vin, float r1, float r2) {
	return r2 * Vin / (r1 + r2);
}

float convertResistanceToKelvinTemperature(float resistance, ThermistorConf *thermistor) {
	if (resistance <= 0) {
		//warning("Invalid resistance in convertResistanceToKelvinTemperature=", resistance);
		return 0;
	}
	float logR = log(resistance);
	return 1 / (thermistor->s_h_a + thermistor->s_h_b * logR + thermistor->s_h_c * logR * logR * logR);
}

float convertKelvinToC(float tempK) {
	return tempK - KELV;
}

float convertCelciustoKelvin(float tempC) {
	return tempC + KELV;
}

float convertCelciustoF(float tempC) {
	return tempC * 9 / 5 + 32;
}

float convertFtoCelcius(float tempF) {
	return (tempF - 32) / 9 * 5;
}

float convertKelvinToFahrenheit(float kelvin) {
	float tempC = convertKelvinToC(kelvin);
	return convertCelciustoF(tempC);
}

float getKelvinTemperature(float resistance, ThermistorConf *thermistor) {
	chDbgCheck(thermistor!=NULL, "thermistor pointer is NULL");

	float kelvinTemperature = convertResistanceToKelvinTemperature(resistance, thermistor);
	return kelvinTemperature;
}

float getResistance(Thermistor *thermistor) {
	float voltage = getVoltageDivided(thermistor->channel);
	chDbgCheck(thermistor->config != NULL, "config is null");
	float resistance = getR2InVoltageDividor(voltage, _5_VOLTS, thermistor->config->bias_resistor);
	return resistance;
}

float getTemperatureC(Thermistor *thermistor) {
	chDbgCheck(initialized, "initialized");
	float resistance = getResistance(thermistor);

	float kelvinTemperature = getKelvinTemperature(resistance, thermistor->config);
	return convertKelvinToC(kelvinTemperature);
}

int isValidCoolantTemperature(float temperature) {
	// I hope magic constants are appropriate here
	return !cisnan(temperature) && temperature > -50 && temperature < 250;
}

int isValidIntakeAirTemperature(float temperature) {
	// I hope magic constants are appropriate here
	return !cisnan(temperature) && temperature > -50 && temperature < 100;
}

float getCoolantTemperature(void) {
	float temperature = getTemperatureC(&engineConfiguration2->clt);
	if (!isValidCoolantTemperature(temperature))
		return NAN;
	return temperature;
}

void setThermistorConfiguration(ThermistorConf * tc, float tempC1, float r1, float tempC2, float r2, float tempC3,
		float r3) {
	tc->tempC_1 = tempC1;
	tc->resistance_1 = r1;

	tc->tempC_2 = tempC2;
	tc->resistance_2 = r2;

	tc->tempC_3 = tempC3;
	tc->resistance_3 = r3;
}

void prepareThermistorCurve(ThermistorConf * config) {
	float T1 = config->tempC_1 + KELV;
	float T2 = config->tempC_2 + KELV;
	float T3 = config->tempC_3 + KELV;

	float L1 = log(config->resistance_1);
	float L2 = log(config->resistance_2);
	float L3 = log(config->resistance_3);

	float Y1 = 1 / T1;
	float Y2 = 1 / T2;
	float Y3 = 1 / T3;

	float U2 = (Y2 - Y1) / (L2 - L1);
	float U3 = (Y3 - Y1) / (L3 - L1);

	config->s_h_c = (U3 - U2) / (L3 - L2) * pow(L1 + L2 + L3, -1);
	config->s_h_b = U2 - config->s_h_c * (L1 * L1 + L1 * L2 + L2 * L2);
	config->s_h_a = Y1 - (config->s_h_b + L1 * L1 * config->s_h_c) * L1;
}

float getIntakeAirTemperature(void) {
	float temperature = getTemperatureC(&engineConfiguration2->iat);
	if (!isValidIntakeAirTemperature(temperature))
		return NAN;
	return temperature;
}

static void initThermistorCurve(Thermistor * t, ThermistorConf *config, int channel) {
	prepareThermistorCurve(config);
	t->config = config;
	t->channel = channel;
}

void initThermistors(void) {
	initThermistorCurve(&engineConfiguration2->clt, &engineConfiguration->cltThermistorConf,
			engineConfiguration->cltAdcChannel);
	initThermistorCurve(&engineConfiguration2->iat, &engineConfiguration->iatThermistorConf,
			engineConfiguration->iatAdcChannel);
	initialized = TRUE;
}
