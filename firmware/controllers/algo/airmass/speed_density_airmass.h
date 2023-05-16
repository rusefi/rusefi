#pragma once

#include "speed_density_base.h"

class SpeedDensityAirmass : public SpeedDensityBase {
public:
	explicit SpeedDensityAirmass(const ValueProvider3D& veTable, const ValueProvider3D& mapEstimationTable)
		: SpeedDensityBase(veTable)
		, m_mapEstimationTable(&mapEstimationTable)
	{}

	AirmassResult getAirmass(int rpm, bool postState) override;
	AirmassResult getAirmass(float rpm, float map, bool postState);
	float getAirflow(float rpm, float map, bool postState);

	float getMap(int rpm, bool postState) const;

private:
	const ValueProvider3D* const m_mapEstimationTable;
};
