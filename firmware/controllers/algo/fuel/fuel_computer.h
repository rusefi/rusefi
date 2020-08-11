#pragma once

#include "engine.h"

class ValueProvider3D;

using mass_t = float;

struct IFuelComputer {
	virtual mass_t getCycleFuel(mass_t airmass, int rpm, float load) const = 0;
};

// This contains the math of the fuel model, but doesn't actually read any configuration
class FuelComputerBase : public IFuelComputer {
public:
	DECLARE_ENGINE_PTR;

	mass_t getCycleFuel(mass_t airmass, int rpm, float load) const override;

protected:
	virtual float getStoichiometricRatio() const = 0;
	virtual float getTargetLambda(int rpm, float load) const = 0;
};

// This class is a usable implemenation of a fuel model that reads real configuration
class FuelComputer final : public FuelComputerBase {
public:
	FuelComputer(const ValueProvider3D& afrTable);

protected:
	float getStoichiometricRatio() const override;
	float getTargetLambda(int rpm, float load) const override;

private:
	const ValueProvider3D* const m_afrTable;
};
