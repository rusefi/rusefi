#pragma once

#include "airmass.h"

class MafAirmass final : public AirmassModelBase {
public:
	explicit MafAirmass(const ValueProvider3D& veTable) : AirmassModelBase(veTable) {}

	AirmassResult getAirmass(int rpm) override;

	// Compute airmass based on flow & engine speed
	AirmassResult getAirmassImpl(float massAirFlow, int rpm) const;
};
