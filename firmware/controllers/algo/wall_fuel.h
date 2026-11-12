/*
 * @file wall_fuel.h
 *
 */

#pragma once

#include "wall_fuel_state_generated.h"
#include "engine_module.h"

/**
 * Wall wetting, also known as fuel film
 * See https://github.com/rusefi/rusefi/issues/151 for the theory
 */
class WallFuel : public wall_fuel_state_s {
public:
	/**
	 * @param desiredMassGrams desired fuel quantity, in grams
	 * @return total adjusted fuel squirt mass in grams once wall wetting is taken into effect
	 */
	float adjust(float desiredMassGrams);
	/**
	 * [tag:fuel_top_up] Extra fuel injected after the main pulse in the same cycle.
	 * Only the deposition applies: evaporation was accounted for by adjust() for this
	 * cycle, so running adjust() again would advance the film twice.
	 * @return commanded mass so that desiredMassGrams reaches the cylinder
	 */
	float getTopUpCommandedMass(float desiredMassGrams) const;
	// Adds the deposited part of a top-up that was actually scheduled.
	void commitTopUp(float commandedMassGrams);
	// Fuel-film mass in grams.
	float getWallFuel() const;
	void resetWF();
	int invocationCounter = 0;
};

struct IWallFuelController {
	virtual bool getEnable() const = 0;
	virtual float getAlpha() const = 0;
	virtual float getBeta() const = 0;
};

class WallFuelController : public IWallFuelController, public EngineModule {
public:
	using interface_t = IWallFuelController;

	void onFastCallback() override;

	bool getEnable() const override {
		return m_enable;
	}

	float getAlpha() const override {
		return m_alpha;
	}

	float getBeta() const override {
		return m_beta;
	}

protected:
	float computeTau() const;
	float computeBeta() const;

private:
	bool m_enable = false;
	float m_alpha = 0;
	float m_beta = 0;
};
