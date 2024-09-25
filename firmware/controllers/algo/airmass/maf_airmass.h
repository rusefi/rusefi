#pragma once

#include "airmass.h"

class MafAirmass final : public AirmassVeModelBase {
public:
	explicit MafAirmass(const ValueProvider3D& veTable) : AirmassVeModelBase(veTable) {}

	AirmassResult getAirmass(float rpm, bool postState) override;

	// Compute airmass based on flow & engine speed
	AirmassResult getAirmassImpl(float massAirFlow, float rpm, bool postState) const;

private:
	float getMaf() const;
};
