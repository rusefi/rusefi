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
	 * @param target desired squirt duration
	 * @return total adjusted fuel squirt duration once wall wetting is taken into effect
	 */
	floatms_t adjust(floatms_t target DECLARE_ENGINE_PARAMETER_SUFFIX);
	floatms_t getWallFuel() const;
	void resetWF();
	int invocationCounter = 0;
};
