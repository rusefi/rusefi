/**
 * @file thermistors.cpp
 *
 * @date Feb 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

/**
 * http://en.wikipedia.org/wiki/Thermistor
 * http://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
 */

#include "pch.h"

void setDodgeSensor(ThermistorConf *thermistorConf, float pullup) {
	thermistorConf->config = {-40, 30, 120, 336660, 7550, 390, pullup};
}

void setAtSensor(ThermistorConf *thermistorConf,
    float tempLow, float rLow,
    float tempMid, float rMid,
    float tempHigh, float rHigh) {
    thermistorConf->config.tempC_1 = tempLow;
    thermistorConf->config.resistance_1 = rLow;

    thermistorConf->config.tempC_2 = tempMid;
    thermistorConf->config.resistance_2 = rMid;

    thermistorConf->config.tempC_3 = tempHigh;
    thermistorConf->config.resistance_3 = rHigh;
}

void setCommonNTCSensorParameters(ThermistorConf *thermistorConf) {
	/**
	 * 18K Ohm @ -20C
	 * 2.1K Ohm @ 24C
	 * 294 Ohm @ 80C
	 * http://www.rexbo.eu/hella/coolant-temperature-sensor-6pt009107121?c=100334&at=3130
	 */
    thermistorConf->config.tempC_1 = -20;
    thermistorConf->config.tempC_2 = 23.8889;
    thermistorConf->config.tempC_3 = 120;
    thermistorConf->config.resistance_1 = 18000;
    thermistorConf->config.resistance_2 = 2100;
    thermistorConf->config.resistance_3 = 100;
}


// todo: better method name?
// todo: poor API mixes SensorParameters with board bias resistor
void setCommonNTCSensor(ThermistorConf *thermistorConf, float pullup) {
  setCommonNTCSensorParameters(thermistorConf);
  thermistorConf->config.bias_resistor = pullup;
}

void setGmCltSensor(ThermistorConf *thermistorConf) {
    thermistorConf->config.tempC_1 = -40;
    thermistorConf->config.tempC_2 = 40;
    thermistorConf->config.tempC_3 = 130;
    thermistorConf->config.resistance_1 = 100'000;
    thermistorConf->config.resistance_2 = 1459;
    thermistorConf->config.resistance_3 = 70;
}
