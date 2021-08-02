/**
 * @file functional_sensor.cpp
 */

#include "functional_sensor.h"

void FunctionalSensor::postRawValue(float inputValue, efitick_t timestamp) {
	// If no function is set, this sensor isn't valid.
	if (!m_function) {
		invalidate();
		return;
	}

	m_rawValue = inputValue;

	auto r = m_function->convert(inputValue);

	// This has to happen so that we set the valid bit after
	// the value is stored, to prevent the data race of reading
	// an old invalid value
	if (r.Valid) {
		setValidValue(r.Value, timestamp);
	} else {
		invalidate();
	}
}
