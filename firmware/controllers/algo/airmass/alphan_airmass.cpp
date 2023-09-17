#include "pch.h"

#include "alphan_airmass.h"

AirmassResult AlphaNAirmass::getAirmass(int rpm, bool postState) {
	auto tps = Sensor::get(SensorType::Tps1);

	if (!tps.Valid) {
		// We are fully reliant on TPS - if the TPS fails, stop the engine.
		return {};
	}

	// In this case, VE directly describes the cylinder filling relative to the ideal
	float ve = getVe(rpm, tps.Value, postState);

	// optionally use real IAT instead of fixed air temperature
	constexpr float standardIat = 273.0f + 20.0f;	// std atmosphere temperature
	float iat = engineConfiguration->alphaNUseIat
		? Sensor::get(SensorType::Iat).value_or(standardIat)
		: standardIat;

	// TODO: should this be barometric pressure and/or temperature compensated?
	mass_t airmass = getAirmassImpl(
		ve,
		101.325f,		// std atmosphere pressure
		iat
	);

	return {
		airmass,
		tps.Value
	};
}
