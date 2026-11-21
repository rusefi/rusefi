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

	// Baro-normalized MAP for table lookups ('Compensated MAP' feature); returns map unchanged when disabled
	float getCompensatedMap(float map) const;

private:
	// Load/VE lookups use map; the ideal-gas air mass uses airmassMap.
	AirmassResult getAirmass(float rpm, float map, float airmassMap, bool postState);
	// [tag:ae_temperature_mult] MAP for the physical air mass while prediction is active.
	float getPredictionAirmassMap(float effectiveMap);
	float getPredictiveMap(float rpm, bool postState, float mapSensor);
	float logAndGetFallback(float rpm, bool postState) const;
	const ValueProvider3D* const m_mapEstimationTable;

	// State for predictive MAP blending
	bool m_isMapPredictionActive = false;
	bool m_awaitingThrottleRelease = false;
	// Whether the latest getMap() returned an active prediction.
	bool m_lastMapPredicted = false;
	Timer m_predictionTimer;
	// Starts with a new event; rising-TPS blend restarts leave this timer running.
	Timer m_sessionTimer;
	float m_initialPredictedMap = 0;
	// Measured MAP from the latest predictive call, the base of the predicted excess.
	float m_predictionMeasuredMap = 0;
	float m_tpsPeak = 0;
};
