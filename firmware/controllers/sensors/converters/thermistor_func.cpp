/**
 * @author Matthew Kennedy, (c) 2019
 */

#include "pch.h"

#include "thermistor_func.h"

#include <cmath>

SensorResult ThermistorFunc::convert(float ohms) const {
	// This resistance should have already been validated - only
	// thing we can check is that it's non-negative
	if (ohms <= 0) {
		return UnexpectedCode::Low;
	}

	float lnR = logf(ohms);

	float lnR3 = lnR * lnR * lnR;

	float recip = m_a + m_b * lnR + m_c * lnR3;

	float kelvin = 1 / recip;

	float celsius = convertKelvinToCelcius(kelvin);

	// bounds check result - please don't try to run rusEFI when colder than -50C
	// high end limit is required as this could be an oil temp sensor on an
	// air cooled engine
	if (celsius < -50) {
		return UnexpectedCode::Low;
	}

	if (celsius > CLT_UPPER_VALIDATION_LIMIT) {
		return UnexpectedCode::High;
	}

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

	float resistance10percent = cfg.resistance_1 + 0.1 * (cfg.resistance_2 - cfg.resistance_1);
	float tempAt10percentPoint = convert(resistance10percent).Value;

	if (tempAt10percentPoint < cfg.tempC_1) {
#if EFI_UNIT_TEST
  throw std::logic_error("Bad thermistor configuration at the left");
#endif
  	  criticalError("Thermistor configuration has failed 10% test");
	}

	float resistance90percent = cfg.resistance_2 + 0.9 * (cfg.resistance_3 - cfg.resistance_2);
	float tempAt90percentPoint = convert(resistance90percent).Value;
	if (tempAt90percentPoint > cfg.tempC_3) {
#if EFI_UNIT_TEST
  throw std::logic_error("Bad thermistor configuration at the right");
#endif
  	  criticalError("Thermistor configuration has failed 90% test");
	}
}
