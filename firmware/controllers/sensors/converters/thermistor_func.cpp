/**
 * @author Matthew Kennedy, (c) 2019
 */

#include "thermistor_func.h"

#include "thermistors.h"

#include "loggingcentral.h"
#include <math.h>

SensorResult ThermistorFunc::convert(float ohms) const {
	// This resistance should have already been validated - only
	// thing we can check is that it's non-negative
	if (ohms <= 0) {
		return unexpected;
	}

	float lnR = logf(ohms);

	float lnR3 = lnR * lnR * lnR;

	float recip = m_a + m_b * lnR + m_c * lnR3;

	float kelvin = 1 / recip;

	float celsius = convertKelvinToCelcius(kelvin);

	return celsius;
}

void ThermistorFunc::configure(thermistor_conf_s &cfg) {
	// https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
	float l1 = logf(cfg.resistance_1);
	float l2 = logf(cfg.resistance_2);
	float l3 = logf(cfg.resistance_3);

	float y1 = 1 / convertCelsiusToKelvin(cfg.tempC_1);
	float y2 = 1 / convertCelsiusToKelvin(cfg.tempC_2);
	float y3 = 1 / convertCelsiusToKelvin(cfg.tempC_3);

	float u2 = (y2 - y1) / (l2 - l1);
	float u3 = (y3 - y1) / (l3 - l1);

	m_c = ((u3 - u2) / (l3 - l2)) / (l1 + l2 + l3);
	m_b = u2 - m_c * (l1 * l1 + l1 * l2 + l2 * l2);
	m_a = y1 - (m_b + l1 * l1 * m_c) * l1;
}

void ThermistorFunc::showInfo(Logging* logger, float testInputValue) const {
	const auto [valid, value] = convert(testInputValue);
	scheduleMsg(logger, "    %.1f ohms -> valid: %d. %.1f deg C", testInputValue, valid, value);
}
