#pragma once

#include "rusefi_types.h"
class ValueProvider3D;

struct AirmassResult {
	mass_t CylinderAirmass = 0;
	percent_t EngineLoadPercent = 100;
};

struct AirmassModelBase {
	virtual AirmassResult getAirmass(int rpm, bool postState) = 0;
};

class AirmassVeModelBase : public AirmassModelBase {
public:
	explicit AirmassVeModelBase(const ValueProvider3D& veTable);

	// Retrieve the user-calibrated volumetric efficiency from the table
	float getVe(int rpm, percent_t load, bool postState) const;

private:
	const ValueProvider3D* const m_veTable;
};
