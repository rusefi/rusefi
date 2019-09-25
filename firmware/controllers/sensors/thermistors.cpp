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

#include "global.h"
#include "sensor_reader.h"
#include "thermistors.h"
#include "engine_configuration.h"
#include "engine_math.h"

#define _5_VOLTS 5.0

EXTERN_ENGINE;

bool hasCltSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engineConfiguration->clt.adcChannel != EFI_ADC_NONE;
}

/**
 * @return coolant temperature, in Celsius
 */
temperature_t getCoolantTemperature(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	static SensorReader<SensorType::Clt> reader(273.15 + 70.0f);

	SensorResult result = reader.get();

	if (!result.Valid) {
		warning(OBD_Engine_Coolant_Temperature_Circuit_Malfunction, "unrealistic CLT");
	}

	engine->isCltBroken = !result.Valid;

	return reader - 273.15f;
}


bool hasIatSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engineConfiguration->iat.adcChannel != EFI_ADC_NONE;
}

/**
 * @return Celsius value
 */
temperature_t getIntakeAirTemperature(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	static SensorReader<SensorType::Iat> reader(273.15f + 20.0f);

	auto result = reader.get();

	if (!result.Valid) {
		warning(OBD_Intake_Air_Temperature_Circuit_Malfunction, "unrealistic IAT");
	}

	return reader - 273.15f;
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

void set10K_4050K(ThermistorConf *thermistorConf) {
	// see https://www.taydaelectronics.com/datasheets/A-409.pdf
	setThermistorConfiguration(thermistorConf, -30, 108000, 25.0, 10000, 130.0, 225);
}
