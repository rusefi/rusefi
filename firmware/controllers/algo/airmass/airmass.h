#pragma once

#include "engine.h"

class ValueProvider3D;

struct AirmassResult {
	float CylinderAirmass = 0;
	float EngineLoadPercent = 100;
};

class AirmassModelBase {
public:
	DECLARE_ENGINE_PTR;

	explicit AirmassModelBase(const ValueProvider3D& veTable);
	virtual AirmassResult getAirmass(int rpm) = 0;

protected:
	// Retrieve the user-calibrated volumetric efficiency from the table
	float getVe(int rpm, percent_t load) const;

private:
	const ValueProvider3D* const m_veTable;

	float getVeLoadAxis(float passedLoad) const;
};
