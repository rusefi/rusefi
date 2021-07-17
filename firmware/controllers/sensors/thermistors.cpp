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

#include "thermistors.h"

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
