#pragma once

#include "sensor_converter_func.h"

class ResistanceFunc final : public SensorConverter {
public:
	void configure(float supplyVoltage, float pullupResistor);

	SensorResult convert(float inputValue) const override;

private:
	float m_supplyVoltage = 5.0f;
	float m_pullupResistor = 1000.0f;
};
