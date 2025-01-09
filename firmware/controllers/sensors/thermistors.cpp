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

// todo: better method name?
void setCommonNTCSensor(ThermistorConf *thermistorConf, float pullup) {
	/**
	 * 18K Ohm @ -20C
	 * 2.1K Ohm @ 24C
	 * 294 Ohm @ 80C
	 * http://www.rexbo.eu/hella/coolant-temperature-sensor-6pt009107121?c=100334&at=3130
	 */
	thermistorConf->config = {/*temp*/-20, /*temp*/23.8889, /*temp*/120,
	/*resistance*/18000, /*resistance*/2100, /*resistance*/100, pullup};
}

void setGmCltSensor(ThermistorConf *thermistorConf, float pullup) {
	thermistorConf->config = {-40, 40, 130, 100'000, 1459, 70, pullup};
}
