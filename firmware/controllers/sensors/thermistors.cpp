/**
 * @file thermistors.cpp
 *
 * @date Feb 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
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

// Celsius
#define LIMPING_MODE_IAT_TEMPERATURE 30.0f
#define LIMPING_MODE_CLT_TEMPERATURE 70.0f

EXTERN_ENGINE
;

static bool initialized = false;

/**
 * http://en.wikipedia.org/wiki/Voltage_divider
 */
float getR1InVoltageDividor(float Vout, float Vin, float r2) {
	return r2 * Vin / Vout - r2;
}

float getR2InVoltageDividor(float Vout, float Vin, float r1) {
	if (Vout == 0) {
		return NAN;
	}
	return r1 / (Vin / Vout - 1);
}

float getVoutInVoltageDividor(float Vin, float r1, float r2) {
	return r2 * Vin / (r1 + r2);
}

float convertResistanceToKelvinTemperature(float resistance, ThermistorConf *thermistor) {
	if (resistance <= 0) {
		//warning("Invalid resistance in convertResistanceToKelvinTemperature=", resistance);
		return 0.0f;
	}
	float logR = logf(resistance);
	return 1 / (thermistor->s_h_a + thermistor->s_h_b * logR + thermistor->s_h_c * logR * logR * logR);
}

float convertCelsiustoF(float tempC) {
	return tempC * 9 / 5 + 32;
}

float convertFtoCelsius(float tempF) {
	return (tempF - 32) / 9 * 5;
}

float convertKelvinToFahrenheit(float kelvin) {
	float tempC = convertKelvinToCelcius(kelvin);
	return convertCelsiustoF(tempC);
}

float getKelvinTemperature(float resistance, ThermistorConf *thermistor) {
	if (thermistor == NULL) {
		firmwareError("thermistor pointer is NULL");
		return NAN;
	}

	float kelvinTemperature = convertResistanceToKelvinTemperature(resistance, thermistor);
	return kelvinTemperature;
}

float getResistance(Thermistor *thermistor) {
	float voltage = getVoltageDivided(thermistor->channel);
	efiAssert(thermistor->config != NULL, "thermistor config is null", NAN);
	float resistance = getR2InVoltageDividor(voltage, _5_VOLTS, thermistor->config->bias_resistor);
	return resistance;
}

float getTemperatureC(Thermistor *thermistor) {
	if (!initialized) {
		firmwareError("thermstr not initialized");
		return NAN;
	}
	float resistance = getResistance(thermistor);

	float kelvinTemperature = getKelvinTemperature(resistance, thermistor->config);
	return convertKelvinToCelcius(kelvinTemperature);
}

bool isValidCoolantTemperature(float temperature) {
	// I hope magic constants are appropriate here
	return !cisnan(temperature) && temperature > -50 && temperature < 250;
}

bool isValidIntakeAirTemperature(float temperature) {
	// I hope magic constants are appropriate here
	return !cisnan(temperature) && temperature > -50 && temperature < 100;
}

/**
 * @return coolant temperature, in Celsius
 */
float getCoolantTemperature(Engine * engine) {
	float temperature = getTemperatureC(&engine->clt);
	if (!isValidCoolantTemperature(temperature)) {
		efiAssert(engine->engineConfiguration!=NULL, "NULL engineConfiguration", NAN);
		if (engine->engineConfiguration->hasCltSensor) {
			warning(OBD_PCM_Processor_Fault, "unrealistic CLT %f", temperature);
		}
		return LIMPING_MODE_CLT_TEMPERATURE;
	}
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

	float L1 = logf(config->resistance_1);
	float L2 = logf(config->resistance_2);
	float L3 = logf(config->resistance_3);

	float Y1 = 1 / T1;
	float Y2 = 1 / T2;
	float Y3 = 1 / T3;

	float U2 = (Y2 - Y1) / (L2 - L1);
	float U3 = (Y3 - Y1) / (L3 - L1);

	config->s_h_c = (U3 - U2) / (L3 - L2) * pow(L1 + L2 + L3, -1);
	config->s_h_b = U2 - config->s_h_c * (L1 * L1 + L1 * L2 + L2 * L2);
	config->s_h_a = Y1 - (config->s_h_b + L1 * L1 * config->s_h_c) * L1;
}

/**
 * @return Celsius value
 */
float getIntakeAirTemperature(Engine * engine) {
	float temperature = getTemperatureC(&engine->iat);
	if (!isValidIntakeAirTemperature(temperature)) {
		efiAssert(engine->engineConfiguration!=NULL, "NULL engineConfiguration", NAN);
		if (engine->engineConfiguration->hasIatSensor) {
			warning(OBD_PCM_Processor_Fault, "unrealistic IAT %f", temperature);
		}
		return LIMPING_MODE_IAT_TEMPERATURE;
	}
	return temperature;
}

static void initThermistorCurve(Thermistor * t, ThermistorConf *config, adc_channel_e channel) {
	prepareThermistorCurve(config);
	t->config = config;
	t->channel = channel;
}

// todo: better method name?
void setCommonNTCSensor(ThermistorConf *thermistorConf) {
	/**
	 * 18K Ohm @ -20C
	 * 2.1K Ohm @ 24C
	 * 294 Ohm @ 80C
	 * http://www.rexbo.eu/hella/coolant-temperature-sensor-6pt009107121?c=100334&at=3130
	 */
	setThermistorConfiguration(thermistorConf, -20, 18000, 23.8889, 2100, 120.0, 100.0);
}

static Logging *logger;

#if EFI_PROD_CODE
static void testCltByR(float resistance) {
	Thermistor *thermistor = &engine->clt;
	float kTemp = getKelvinTemperature(resistance, thermistor->config);
	scheduleMsg(logger, "for R=%f we have %f", resistance, (kTemp - KELV));
}
#endif

void initThermistors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
	efiAssertVoid(engine!=NULL, "e NULL initThermistors");
	efiAssertVoid(engine->engineConfiguration2!=NULL, "e2 NULL initThermistors");
	initThermistorCurve(&engine->clt, &engine->engineConfiguration->clt, engine->engineConfiguration->cltAdcChannel);
	initThermistorCurve(&engine->iat, &engine->engineConfiguration->iat, engine->engineConfiguration->iatAdcChannel);

#if EFI_PROD_CODE
	addConsoleActionF("test_clt_by_r", testCltByR);
#endif

	initialized = true;
}
