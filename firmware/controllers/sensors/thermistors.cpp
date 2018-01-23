/**
 * @file thermistors.cpp
 *
 * @date Feb 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

/**
 * http://en.wikipedia.org/wiki/Thermistor
 * http://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
 */

#include "main.h"
#include "thermistors.h"
#include "analog_input.h"
#include "engine_configuration.h"
#include "engine_math.h"

#define _5_VOLTS 5.0

// Celsius
#define NO_IAT_SENSOR_TEMPERATURE 32.0f
#define LIMPING_MODE_IAT_TEMPERATURE 30.0f
#define LIMPING_MODE_CLT_TEMPERATURE 70.0f
#define NO_CLT_SENSOR_TEMPERATURE 72.0f

EXTERN_ENGINE
;

static Logging *logger = NULL;

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

float ThermistorMath::getKelvinTemperatureByResistance(float resistance) {
	if (resistance <= 0) {
		//warning("Invalid resistance in getKelvinTemperature=", resistance);
		return 0.0f;
	}
	float logR = logf(resistance);
	return 1 / (s_h_a + s_h_b * logR + s_h_c * logR * logR * logR);
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

float getResistance(ThermistorConf *config, float voltage) {
	efiAssert(config != NULL, "thermistor config is null", NAN);
	thermistor_conf_s *tc = &config->config;

	float resistance = getR2InVoltageDividor(voltage, _5_VOLTS, tc->bias_resistor);
	return resistance;
}

float getTemperatureC(ThermistorConf *config, ThermistorMath *tm, bool useLinear) {
	tm->setConfig(&config->config); // implementation checks if configuration has changed or not

	float voltage = getVoltageDivided("term", config->adcChannel);
	if (useLinear) {
			// todo: fix this horrible code!
			// should work as a short term fix.
			// todo: move 'useLinearXXXSensor' into termistor configuration record
		// yes, we use 'resistance' setting for 'voltage' here
		return interpolate(config->config.resistance_1, config->config.tempC_1,
				config->config.resistance_2, config->config.tempC_2,
						voltage);

	}
	float resistance = getResistance(config, voltage);

	float kelvinTemperature = tm->getKelvinTemperatureByResistance(resistance);
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

bool hasCltSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engineConfiguration->clt.adcChannel != EFI_ADC_NONE;
}

/**
 * @return coolant temperature, in Celsius
 */
float getCoolantTemperature(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!hasCltSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		engine->isCltBroken = false;
		return NO_CLT_SENSOR_TEMPERATURE;
	}
 	float temperature = getTemperatureC(&engineConfiguration->clt, &engine->engineState.cltCurve,
 			engineConfiguration->useLinearCltSensor);
	if (!isValidCoolantTemperature(temperature)) {
		efiAssert(engineConfiguration!=NULL, "NULL engineConfiguration", NAN);
		warning(OBD_Engine_Coolant_Temperature_Circuit_Malfunction, "unrealistic CLT %.2f", temperature);
		engine->isCltBroken = true;
		return LIMPING_MODE_CLT_TEMPERATURE;
	}
	engine->isCltBroken = false;
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

void ThermistorMath::prepareThermistorCurve(thermistor_conf_s *tc) {
	float T1 = tc->tempC_1 + KELV;
	float T2 = tc->tempC_2 + KELV;
	float T3 = tc->tempC_3 + KELV;
#if EXTREME_TERM_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "T1=%.5f/T2=%.5f/T3=%.5f", T1, T2, T3);
#endif

	float L1 = logf(tc->resistance_1);
	if (L1 == tc->resistance_1) {
		/**
		 * See https://github.com/rusefi/rusefi/issues/375
		 * See https://sourceforge.net/p/rusefi/tickets/149/
		 */
		firmwareError(CUSTOM_ERR_NATURAL_LOGARITHM_ERROR, "Natural logarithm logf() is broken: %.2f", tc->resistance_1);
	}
	float L2 = logf(tc->resistance_2);
	float L3 = logf(tc->resistance_3);
#if EXTREME_TERM_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "R1=%.5f/R2=%.5f/R3=%.5f", tc->resistance_1, tc->resistance_2,
			tc->resistance_3);
	scheduleMsg(logger, "L1=%.5f/L2=%.5f/L3=%.5f", L1, L2, L3);
#endif

	float Y1 = 1 / T1;
	float Y2 = 1 / T2;
	float Y3 = 1 / T3;


	float U2 = (Y2 - Y1) / (L2 - L1);
	float U3 = (Y3 - Y1) / (L3 - L1);


	s_h_c = (U3 - U2) / (L3 - L2) * pow(L1 + L2 + L3, -1);
	s_h_b = U2 - s_h_c * (L1 * L1 + L1 * L2 + L2 * L2);
	s_h_a = Y1 - (s_h_b + L1 * L1 * s_h_c) * L1;

#if EXTREME_TERM_LOGGING || defined(__DOXYGEN__)
	scheduleMsg(logger, "Y1=%.5f/Y2=%.5f/Y3=%.5f", Y1, Y2, Y3);
	scheduleMsg(logger, "U2=%.5f/U3=%.5f", U2, U3);
	scheduleMsg(logger, "s_h_c=%.5f/s_h_b=%.5f/s_h_a=%.5f", curve->s_h_c, curve->s_h_b,
			curve->s_h_a);
#endif
}

bool hasIatSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engineConfiguration->iat.adcChannel != EFI_ADC_NONE;
}

/**
 * @return Celsius value
 */
float getIntakeAirTemperature(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!hasIatSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		return NO_IAT_SENSOR_TEMPERATURE;
	}
	float temperature = getTemperatureC(&engineConfiguration->iat, &engine->engineState.iatCurve,
			engineConfiguration->useLinearIatSensor);
	if (!isValidIntakeAirTemperature(temperature)) {
		efiAssert(engineConfiguration!=NULL, "NULL engineConfiguration", NAN);
#if EFI_PROD_CODE || EFI_UNIT_TEST || defined(__DOXYGEN__)
		warning(OBD_Intake_Air_Temperature_Circuit_Malfunction, "unrealistic IAT %.2f", temperature);
#endif /* EFI_PROD_CODE */
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

#if EFI_PROD_CODE || defined(__DOXYGEN__)
static void testCltByR(float resistance) {
	if (logger == NULL) {
		firmwareError(CUSTOM_ERR_THERM, "thermstr not initialized");
		return;
	}

	// we expect periodicSlowCallback to already update configuration in the curve helper class see setConfig
	float kTemp = engine->engineState.cltCurve.getKelvinTemperatureByResistance(resistance);
	scheduleMsg(logger, "for R=%.2f we have %.2f", resistance, (kTemp - KELV));
}
#endif

void initThermistors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	logger = sharedLogger;
	efiAssertVoid(engine!=NULL, "e NULL initThermistors");

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	addConsoleActionF("test_clt_by_r", testCltByR);
#endif
}

ThermistorMath::ThermistorMath() {
	memset(&currentConfig, 0, sizeof(currentConfig));
	s_h_a = s_h_b = s_h_c = 0;
}

void ThermistorMath::setConfig(thermistor_conf_s *config) {
	bool isSameConfig = memcmp(config, &currentConfig, sizeof(currentConfig)) == 0;
	if (isSameConfig) {
		return;
	}
	memcpy(&currentConfig, config, sizeof(currentConfig));
	prepareThermistorCurve(config);
}
