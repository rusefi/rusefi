#pragma once

#include "sensor_converter_func.h"

class ThermistorFunc final : public SensorConverter {
public:
	SensorResult convert(float ohms) const override;
};
