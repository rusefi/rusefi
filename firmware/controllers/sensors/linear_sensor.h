#pragma once

#include "converter_sensor.h"

class LinearSensor final : public ConvertedSensor {
  public:
	LinearSensor(SensorType type)
		: ConvertedSensor(type) {}

	void configure(float in1, float out1, float in2, float out2, float minOutput, float maxOutput);

  protected:
	SensorResult convertFromInputValue(float inputValue) override;

  private:
	// Linear equation parameters for equation of form
	// y = ax + b
	float m_a = 1;
	float m_b = 0;

	float m_minOutput = 0;
	float m_maxOutput = 0;
};
