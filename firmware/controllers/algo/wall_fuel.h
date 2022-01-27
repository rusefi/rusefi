/*
 * @file wall_fuel.h
 *
 */

#pragma once

/**
 * Wall wetting, also known as fuel film
 * See https://github.com/rusefi/rusefi/issues/151 for the theory
 */
class WallFuel : public wall_fuel_state {
public:
	/**
	 * @param desiredMassGrams desired fuel quantity, in grams
	 * @return total adjusted fuel squirt mass in grams once wall wetting is taken into effect
	 */
	float adjust(float desiredMassGrams);
	float getWallFuel() const;
	void resetWF();
	int invocationCounter = 0;
};

class WallFuelController : public EngineModule {
public:
	void onFastCallback() override;

	// allow WallFuel to peek inside
	friend class WallFuel;

private:
	bool m_enable = false;
	float m_alpha = 0;
	float m_beta = 0;
};
