#include "pch.h"
#include "speed_density_airmass.h"

AirmassResult SpeedDensityAirmass::getAirmass(int rpm) {
	ScopePerf perf(PE::GetSpeedDensityFuel);

	/**
	 * most of the values are pre-calculated for performance reasons
	 */
	float tChargeK = engine->engineState.sd.tChargeK;
	if (cisnan(tChargeK)) {
		warning(CUSTOM_ERR_TCHARGE_NOT_READY2, "tChargeK not ready"); // this would happen before we have CLT reading for example
		return {};
	}

	auto map = getMap(rpm);

	float ve = getVe(rpm, map);

	float airMass = getAirmassImpl(ve, map, tChargeK);
	if (cisnan(airMass)) {
		warning(CUSTOM_ERR_6685, "NaN airMass");
		return {};
	}
#if EFI_PRINTF_FUEL_DETAILS
	printf("getSpeedDensityAirmass map=%.2f\n",
			map);
#endif /*EFI_PRINTF_FUEL_DETAILS */

	return {
		airMass,
		map,	// AFR/VE table Y axis
	};
}

float SpeedDensityAirmass::getMap(int rpm) const {
	SensorResult map = Sensor::get(SensorType::Map);
	if (map) {
		return map.Value;
	} else {
		float fallbackMap;
		if (engineConfiguration->enableMapEstimationTableFallback) {
		// if the map estimation table is enabled, estimate map based on the TPS and RPM
			fallbackMap = m_mapEstimationTable->getValue(rpm, Sensor::getOrZero(SensorType::Tps1));
		} else {
			fallbackMap = engineConfiguration->failedMapFallback;
		}

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_MAP) {
		tsOutputChannels.debugFloatField4 = fallbackMap;
	}
#endif // EFI_TUNER_STUDIO

		return fallbackMap;
	}
}
