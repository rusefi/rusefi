#pragma once

class ValueProvider3D;

struct AirmassResult {
	float CylinderAirmass = 0;
	float EngineLoadPercent = 100;
};

struct AirmassModelBase {
	virtual AirmassResult getAirmass(int rpm) = 0;
};

class AirmassVeModelBase : public AirmassModelBase {
public:
	DECLARE_ENGINE_PTR;

	explicit AirmassVeModelBase(const ValueProvider3D& veTable);

protected:
	// Retrieve the user-calibrated volumetric efficiency from the table
	float getVe(int rpm, percent_t load) const;

private:
	const ValueProvider3D* const m_veTable;

	float getVeLoadAxis(float passedLoad) const;
};
