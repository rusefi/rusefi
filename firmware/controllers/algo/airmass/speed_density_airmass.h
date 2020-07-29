#pragma once

#include "speed_density_base.h"

class SpeedDensityAirmass : public SpeedDensityBase {
public:
	SpeedDensityAirmass(const ValueProvider3D& veTable) : SpeedDensityBase(veTable) {}
	AirmassResult getAirmass(int rpm) override;
};
