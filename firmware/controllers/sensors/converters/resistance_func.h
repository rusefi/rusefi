/**
 * @author Matthew Kennedy, (c) 2019
 * 
 * A function to convert input voltage to resistance in a voltage divider.
 * Configured with the value of the pullup resistor, and the voltage to which
 * it's connected.
 */

#pragma once

#include "sensor_converter_func.h"

class ResistanceFunc final : public SensorConverter {
public:
	void configure(float supplyVoltage, float pullupResistor, bool isPulldown);

	SensorResult convert(float inputValue) const override;

	void showInfo(float testInputValue) const override;

	// Last resistance computed by convert(), in ohms. Zero if the last conversion failed
	// (dead short / open circuit) or no conversion happened yet. Exposed so the measured
	// resistance can be shown live during thermistor calibration.
	float getLastResistance() const {
		return m_lastResistance;
	}

private:
	float m_supplyVoltage = 5.0f;
	float m_pullupResistor = 1000.0f;
	bool m_isPulldown = false;

	// convert() is const as it's logically a pure conversion; caching the last result for
	// monitoring purposes doesn't change that, hence mutable.
	mutable float m_lastResistance = 0;
};
