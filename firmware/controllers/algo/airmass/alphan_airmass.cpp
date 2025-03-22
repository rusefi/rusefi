#include "pch.h"

#include "alphan_airmass.h"
#include "fuel_math.h"

AirmassResult AlphaNAirmass::getAirmass(float rpm, bool postState) {
	auto tps = Sensor::get(SensorType::Tps1);

	if (!tps.Valid) {
		// We are fully reliant on TPS - if the TPS fails, stop the engine.
		return {};
	}

	// In this case, VE directly describes the cylinder filling relative to the ideal
	float ve = getVe(rpm, tps.Value, postState);

	// optionally use real IAT instead of fixed air temperature
	constexpr float standardIat = STD_IAT;	// std atmosphere temperature
	float iat = engineConfiguration->alphaNUseIat
		? Sensor::get(SensorType::Iat).value_or(standardIat)
		: standardIat;

	float iatK = iat + 273/* todo reuse C_K_OFFSET which would require adjusting unit tests*/;

	// TODO: should this be barometric pressure and/or temperature compensated?
	mass_t airmass = getAirmassImpl(
		ve,
		STD_ATMOSPHERE,
		iatK
	);

	return {
		airmass,
		tps.Value
	};
}
