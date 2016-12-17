/**
 * @file thermistors.cpp
 *
 * @date Feb 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
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
#define NO_IAT_SENSOR_TEMPERATURE 32.0f
#define LIMPING_MODE_IAT_TEMPERATURE 30.0f
#define LIMPING_MODE_CLT_TEMPERATURE 70.0f
#define NO_CLT_SENSOR_TEMPERATURE 72.0f

EXTERN_ENGINE
;

static Logging *logger;

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

float getKelvinTemperature(ThermistorConf *config, float resistance, ThermistorMath *tm) {
	tm->init(&config->config); // implementation checks if config has changed or not
	thermistor_curve_s * curve = &tm->curve;
	efiAssert(curve != NULL, "thermistor pointer is NULL", NAN);

	if (resistance <= 0) {
		//warning("Invalid resistance in getKelvinTemperature=", resistance);
		return 0.0f;
	}
	float logR = logf(resistance);
	return 1 / (curve->s_h_a + curve->s_h_b * logR + curve->s_h_c * logR * logR * logR);
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

float getResistance(ThermistorConf *config) {
	float voltage = getVoltageDivided("term", config->adcChannel);
	efiAssert(config != NULL, "thermistor config is null", NAN);
	thermistor_conf_s *tc = &config->config;

	float resistance = getR2InVoltageDividor(voltage, _5_VOLTS, tc->bias_resistor);
	return resistance;
}

float getTemperatureC(ThermistorConf *config, ThermistorMath *tm) {
	if (!initialized) {
		firmwareError(CUSTOM_ERR_THERM, "thermstr not initialized");
		return NAN;
	}
	float resistance = getResistance(config);

	float kelvinTemperature = getKelvinTemperature(config, resistance, tm);
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
float getCoolantTemperature(DECLARE_ENGINE_PARAMETER_F) {
	if (engineConfiguration->clt.adcChannel == EFI_ADC_NONE) {
		return NO_CLT_SENSOR_TEMPERATURE;
	}
 	float temperature = getTemperatureC(&engineConfiguration->clt, &engine->engineState.cltCurve);
	if (!isValidCoolantTemperature(temperature)) {
		efiAssert(engineConfiguration!=NULL, "NULL engineConfiguration", NAN);
		warning(OBD_Engine_Coolant_Temperature_Circuit_Malfunction, "unrealistic CLT %f", temperature);
		return LIMPING_MODE_CLT_TEMPERATURE;
	}
	return temperature;
}

void setThermistorConfiguration(ThermistorConf * thermistor, float tempC1, float r1, float tempC2, float r2, float tempC3,
		float r3) {
	thermistor_conf_s *tc = &thermistor->config;
	tc->tempC_1 = tempC1;
	tc->resistance_1 = r1;

	tc->tempC_2 = tempC2;
	tc->resistance_2 = r2;

	tc->tempC_3 = tempC3;
	tc->resistance_3 = r3;
}

static void prepareThermistorCurve(thermistor_conf_s *tc, thermistor_curve_s * curve) {
	float T1 = tc->tempC_1 + KELV;
	float T2 = tc->tempC_2 + KELV;
	float T3 = tc->tempC_3 + KELV;
#if EXTREME_TERM_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "T1=%..100000f/T2=%..100000f/T3=%..100000f", T1, T2, T3);
#endif

	float L1 = logf(tc->resistance_1);
	if (L1 == tc->resistance_1) {
		firmwareError(CUSTOM_ERR_LOG_ERROR, "log is broken?");
	}
	float L2 = logf(tc->resistance_2);
	float L3 = logf(tc->resistance_3);
#if EXTREME_TERM_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "R1=%..100000f/R2=%..100000f/R3=%..100000f", tc->resistance_1, tc->resistance_2,
			tc->resistance_3);
	scheduleMsg(logger, "L1=%..100000f/L2=%..100000f/L3=%..100000f", L1, L2, L3);
#endif

	float Y1 = 1 / T1;
	float Y2 = 1 / T2;
	float Y3 = 1 / T3;


	float U2 = (Y2 - Y1) / (L2 - L1);
	float U3 = (Y3 - Y1) / (L3 - L1);


	curve->s_h_c = (U3 - U2) / (L3 - L2) * pow(L1 + L2 + L3, -1);
	curve->s_h_b = U2 - curve->s_h_c * (L1 * L1 + L1 * L2 + L2 * L2);
	curve->s_h_a = Y1 - (curve->s_h_b + L1 * L1 * curve->s_h_c) * L1;

#if EXTREME_TERM_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "Y1=%..100000f/Y2=%..100000f/Y3=%..100000f", Y1, Y2, Y3);
	scheduleMsg(logger, "U2=%..100000f/U3=%..100000f", U2, U3);
	scheduleMsg(logger, "s_h_c=%..100000f/s_h_b=%..100000f/s_h_a=%..100000f", curve->s_h_c, curve->s_h_b,
			curve->s_h_a);
#endif
}

/**
 * @return Celsius value
 */
float getIntakeAirTemperature(DECLARE_ENGINE_PARAMETER_F) {
	if (engineConfiguration->iat.adcChannel == EFI_ADC_NONE) {
		return NO_IAT_SENSOR_TEMPERATURE;
	}
	float temperature = getTemperatureC(&engineConfiguration->iat, &engine->engineState.iatCurve);
	if (!isValidIntakeAirTemperature(temperature)) {
		efiAssert(engineConfiguration!=NULL, "NULL engineConfiguration", NAN);
		warning(OBD_Intake_Air_Temperature_Circuit_Malfunction, "unrealistic IAT %f", temperature);
		return LIMPING_MODE_IAT_TEMPERATURE;
	}
	return temperature;
}

void setDodgeSensor(ThermistorConf *thermistorConf) {
	setThermistorConfiguration(thermistorConf, -40, 336660, 30, 7550, 120, 390);
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

#if EFI_PROD_CODE
static void testCltByR(float resistance) {
	float kTemp = getKelvinTemperature(&engineConfiguration->clt, resistance, &engine->engineState.cltCurve);
	scheduleMsg(logger, "for R=%f we have %f", resistance, (kTemp - KELV));
}
#endif

void initThermistors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_S) {
	logger = sharedLogger;
	efiAssertVoid(engine!=NULL, "e NULL initThermistors");
	efiAssertVoid(engine->engineConfiguration2!=NULL, "e2 NULL initThermistors");

#if EFI_PROD_CODE
	addConsoleActionF("test_clt_by_r", testCltByR);
#endif

	initialized = true;
}

ThermistorMath::ThermistorMath() {
	memset(&currentConfig, 0, sizeof(currentConfig));
	memset(&curve, 0, sizeof(curve));
}

void ThermistorMath::init(thermistor_conf_s *config) {
	bool isSameConfig = memcmp(config, &currentConfig, sizeof(currentConfig)) == 0;
	if (isSameConfig) {
		return;
	}
	memcpy(&currentConfig, config, sizeof(currentConfig));
	prepareThermistorCurve(config, &curve);
}
