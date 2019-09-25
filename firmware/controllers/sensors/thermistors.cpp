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

#include "thermistors.h"

#include "error_handling.h"
#include "sensor_reader.h"

static SensorReader<SensorType::Clt> cltReader(70.0f);

bool hasCltSensor() {
	return cltReader.get().Valid;
}

/**
 * @return coolant temperature, in Celsius
 */
temperature_t getCoolantTemperature() {
	SensorResult result = cltReader.get();

	if (!result.Valid) {
		warning(OBD_Engine_Coolant_Temperature_Circuit_Malfunction, "unrealistic CLT");
	}

	return cltReader.getOrDefault();
}


static SensorReader<SensorType::Iat> iatReader(20.0f);

bool hasIatSensor() {
	return iatReader.get().Valid;
}

/**
 * @return Celsius value
 */
temperature_t getIntakeAirTemperature() {
	auto result = iatReader.get();

	if (!result.Valid) {
		warning(OBD_Intake_Air_Temperature_Circuit_Malfunction, "unrealistic IAT");
	}

	return iatReader.getOrDefault();
}


void setDodgeSensor(ThermistorConf *thermistorConf, float pullup) {
	thermistorConf->config = {-40, 30, 120, 336660, 7550, 390, pullup};
}

// todo: better method name?
void setCommonNTCSensor(ThermistorConf *thermistorConf, float pullup) {
	/**
	 * 18K Ohm @ -20C
	 * 2.1K Ohm @ 24C
	 * 294 Ohm @ 80C
	 * http://www.rexbo.eu/hella/coolant-temperature-sensor-6pt009107121?c=100334&at=3130
	 */
	thermistorConf->config = {-20, 23.8889, 120, 18000, 2100, 100, pullup};
}

void set10K_4050K(ThermistorConf *thermistorConf, float pullup) {
	// see https://www.taydaelectronics.com/datasheets/A-409.pdf
	thermistorConf->config = {-30, 25, 130, 108000, 10000, 225, pullup};
}
