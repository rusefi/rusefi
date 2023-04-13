#include "pch.h"
#include "speed_density_airmass.h"

AirmassResult SpeedDensityAirmass::getAirmass(int rpm) {
	ScopePerf perf(PE::GetSpeedDensityFuel);

	auto map = getMap(rpm);

	return getAirmass(rpm, map);
}

AirmassResult SpeedDensityAirmass::getAirmass(float rpm, float map) {
	/**
	 * most of the values are pre-calculated for performance reasons
	 */
	float tChargeK = engine->engineState.sd.tChargeK;
	if (cisnan(tChargeK)) {
		warning(ObdCode::CUSTOM_ERR_TCHARGE_NOT_READY2, "tChargeK not ready"); // this would happen before we have CLT reading for example
		return {};
	}

	float ve = getVe(rpm, map);

	float airMass = getAirmassImpl(ve, map, tChargeK);
	if (cisnan(airMass)) {
		warning(ObdCode::CUSTOM_ERR_6685, "NaN airMass");
		return {};
	}

	return {
		airMass,
		map,	// AFR/VE table Y axis
	};
}

float SpeedDensityAirmass::getAirflow(float rpm, float map) {
	auto airmassResult = getAirmass(rpm, map);

	float massPerCycle = airmassResult.CylinderAirmass * engineConfiguration->cylindersCount;

	if (!engineConfiguration->twoStroke) {
		// 4 stroke engines only do a half cycle per rev
		massPerCycle = massPerCycle / 2;
	}

	// g/s
	return massPerCycle * rpm / 60;
}

float SpeedDensityAirmass::getMap(int rpm) const {
	float fallbackMap = m_mapEstimationTable->getValue(rpm, Sensor::getOrZero(SensorType::Tps1));

#if EFI_TUNER_STUDIO
	engine->outputChannels.fallbackMap = fallbackMap;
#endif // EFI_TUNER_STUDIO

	return Sensor::get(SensorType::Map).value_or(fallbackMap);
}
