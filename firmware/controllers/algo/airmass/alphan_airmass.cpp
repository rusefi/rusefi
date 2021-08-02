#include "pch.h"

#include "alphan_airmass.h"

AirmassResult AlphaNAirmass::getAirmass(int rpm) {
	auto tps = Sensor::get(SensorType::Tps1);

	if (!tps.Valid) {
		// We are fully reliant on TPS - if the TPS fails, stop the engine.
		return {};
	}

	// In this case, VE directly describes the cylinder filling relative to the ideal
	float ve = getVe(rpm, tps.Value);

	// TODO: should this be barometric pressure and/or temperature compensated?
	float airmass = getAirmassImpl(
		ve,
		101.325f,		// std atmosphere pressure
		273.0f + 20.0f	// std atmosphere pressure
		PASS_ENGINE_PARAMETER_SUFFIX
	);

	return {
		airmass,
		tps.Value
	};
}
