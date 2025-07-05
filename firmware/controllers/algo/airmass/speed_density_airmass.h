#pragma once

#include "speed_density_base.h"
#include "efitime.h"

class SpeedDensityAirmass : public SpeedDensityBase {
public:
	explicit SpeedDensityAirmass(const ValueProvider3D& veTable, const ValueProvider3D& mapEstimationTable)
		: SpeedDensityBase(veTable)
		, m_mapEstimationTable(&mapEstimationTable)
	{}

	AirmassResult getAirmass(float rpm, bool postState) override;
	AirmassResult getAirmass(float rpm, float map, bool postState);
	float getAirflow(float rpm, float map, bool postState);

	float getMap(float rpm, bool postState);

private:
	float getPredictiveMap(float rpm, bool postState, float mapSensor);
	float logAndGetFallback(float rpm, bool postState) const;
	const ValueProvider3D* const m_mapEstimationTable;

	// State for predictive MAP blending
	bool m_isMapPredictionActive = false;
	Timer m_predictionTimer;
	float m_initialPredictedMap = 0;
	float m_initialRealMap = 0;
};
