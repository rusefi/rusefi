#include "pch.h"
#include "speed_density_airmass.h"

AirmassResult SpeedDensityAirmass::getAirmass(float rpm, bool postState) {
	ScopePerf perf(PE::GetSpeedDensityFuel);

	auto map = getMap(rpm, postState);

	return getAirmass(rpm, map, postState);
}

AirmassResult SpeedDensityAirmass::getAirmass(float rpm, float map, bool postState) {
	/**
	 * most of the values are pre-calculated for performance reasons
	 */
	float tChargeK = engine->engineState.sd.tChargeK;
	if (std::isnan(tChargeK)) {
		warning(ObdCode::CUSTOM_ERR_TCHARGE_NOT_READY2, "tChargeK not ready"); // this would happen before we have CLT reading for example
		return {};
	}

	float ve = getVe(rpm, map, postState);

	float airMass = getAirmassImpl(ve, map, tChargeK);
	if (std::isnan(airMass)) {
		warning(ObdCode::CUSTOM_ERR_6685, "NaN airMass");
		return {};
	}
#if EFI_PRINTF_FUEL_DETAILS
	printf("getSpeedDensityAirmass map=%.2f\n", map);
#endif /*EFI_PRINTF_FUEL_DETAILS */

	return {
		airMass,
		map,	// AFR/VE table Y axis
	};
}

float SpeedDensityAirmass::getAirflow(float rpm, float map, bool postState) {
	auto airmassResult = getAirmass(rpm, map, postState);

	float massPerCycle = airmassResult.CylinderAirmass * engineConfiguration->cylindersCount;

	if (!engineConfiguration->twoStroke) {
		// 4 stroke engines only do a half cycle per rev
		massPerCycle = massPerCycle / 2;
	}

	// g/s
	return massPerCycle * rpm / 60;
}

float SpeedDensityAirmass::getMap(float rpm, bool postState) const {
	auto wotTps = interpolate2d(Sensor::getOrZero(SensorType::Rpm), config->wotTpsRpmBins, config->wotTpsValues);
	auto tps =  Sensor::getOrZero(SensorType::Tps1);
	// if wotTps is out of range, it's 100.
	if (wotTps <= 0 || wotTps > 100 || tps > wotTps) {
		wotTps = 100;
	}

	float fallbackMap = m_mapEstimationTable->getValue(rpm, tps);

#if EFI_TUNER_STUDIO
	if (postState) {
		engine->outputChannels.fallbackMap = fallbackMap;
	}
#endif // EFI_TUNER_STUDIO

	auto map = Sensor::get(SensorType::Map);
	if (!map) {
		// MAP sensor is dead, nothing we can do
		return fallbackMap;
	} else if (engineConfiguration->useMapEstimateDuringTransient && engine->module<TpsAccelEnrichment>()->isAboveAccelThreshold) {
	// TODO: add taper here
		// Take the greater of real or estimated map based on scaled up TPS from wotTps so we don't under-fuel on a transient
		fallbackMap = m_mapEstimationTable->getValue(rpm, (100/wotTps) * tps);
		engine->outputChannels.fallbackMap = fallbackMap;
		return std::max(map.Value,  fallbackMap);

	} else {
		// Normal operation,
		return map.Value;
	}
}
