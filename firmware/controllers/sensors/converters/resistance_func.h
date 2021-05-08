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
	void configure(float supplyVoltage, float pullupResistor);

	SensorResult convert(float inputValue) const override;

	void showInfo(float testInputValue) const override;

private:
	float m_supplyVoltage = 5.0f;
	float m_pullupResistor = 1000.0f;
};
