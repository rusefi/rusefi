#pragma once

#include "speed_density_base.h"

class AlphaNAirmass : public SpeedDensityBase {
public:
	explicit AlphaNAirmass(const ValueProvider3D& veTable) : SpeedDensityBase(veTable) {}

	AirmassResult getAirmass(int rpm, bool postState) override;
};
